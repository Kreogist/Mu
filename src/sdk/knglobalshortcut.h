/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNGLOBALSHORTCUT_H
#define KNGLOBALSHORTCUT_H

#include <QHash>
#include <QKeySequence>

#include <QObject>
#include <QAbstractNativeEventFilter>

/*!
 * \brief The KNGlobalShortcut class provides the ability to trigger an action
 * at the system wide level. Currently, this class supports for Windows,
 * Mac OS X and Linux.
 */
class KNGlobalShortcut : public QObject
        #ifndef Q_OS_MACX
        , QAbstractNativeEventFilter
        #endif
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNGlobalShortcut object.
     * \param parent The parent object.
     */
    explicit KNGlobalShortcut(QObject *parent = 0);
    ~KNGlobalShortcut();

    /*!
     * \brief Get the shortcut of the global action.
     * \return The key sequence of the shortcut.
     */
    QKeySequence shortcut() const;

    /*!
     * \brief Set the trigger shortcut key sequence.
     * \param shortcut The key sequence.
     * \return If the shortcut set successful, return true.
     */
    bool setShortcut(const QKeySequence &shortcut);

    /*!
     * \brief Whether the action is enabled or not.
     * \return The enable state of the global short cut.
     */
    bool isEnabled() const;

    /*!
     * \brief Activate a specific global shortcut.
     * \param nativeKey The native key info.
     * \param nativeMods The native modifiers info.
     */
    static void activateShortcut(quint32 nativeKey, quint32 nativeMods);

signals:
    /*!
     * \brief When the global shortcut is triggered, this signal will emitted.
     */
    void activated();

public slots:
    /*!
     * \brief Enable the global shortcut.
     * \param enabled To enable the shortcut, set this to true.
     */
    void setEnabled(bool enabled = true);

    /*!
     * \brief Disable the global shortcut. This works the same as enable.
     * \param disabled To disable the shortcut, set this to true.
     */
    void setDisabled(bool disabled = true);

private:
#ifndef Q_OS_MACX
    virtual bool nativeEventFilter(const QByteArray &eventType,
                                   void *message, long *result);
    static int reference;
#endif
    static quint32 getNativeKeycode(Qt::Key keycode);
    static quint32 getNativeModifiers(Qt::KeyboardModifiers modifiers);

    static bool registerShortcut(quint32 nativeKey, quint32 nativeMods);
    static bool unregisterShortcut(quint32 nativeKey, quint32 nativeMods);

    static QHash<QPair<quint32, quint32>, KNGlobalShortcut*> shortcuts;

    inline void updateEnableState();
    inline bool bindAll();
    inline bool unbindAll();
    inline bool clearAll();

    Qt::KeyboardModifiers m_modifiers[4];
    Qt::Key m_key[4];
    int m_count;
    bool m_enable;
};

#endif // KNGLOBALSHORTCUT_H
