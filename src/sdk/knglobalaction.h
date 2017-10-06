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

#ifndef KNGLOBALACTION_H
#define KNGLOBALACTION_H

#include <QAbstractNativeEventFilter>
#include <QKeySequence>
#if defined(Q_OS_WIN)
#include <QSet>
#endif

#include <QObject>

/*!
 * \brief The KNGlobalShortcut class provides the ability to trigger an action
 * at the system wide level. Currently, this class supports for Windows,
 * Mac OS X and Linux.
 */
class KNGlobalAction : public QObject
        #ifndef Q_OS_LINUX
        , QAbstractNativeEventFilter
        #endif
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNGlobalShortcut object.
     * \param parent The parent object.
     */
    explicit KNGlobalAction(QObject *parent = 0);
    ~KNGlobalAction();

    /*!
     * \brief Set a key sequence as the current sequence.
     * \param keySequence The key sequence of the action.
     */
    void setKeySequence(const QKeySequence &keySequence);

    /*!
     * \brief Whether the action is enabled or not.
     * \return The enable state of the global short cut.
     */
    bool isEnabled() const;

signals:
    /*!
     * \brief When the global action is activated, this signal will be
     * triggered.
     */
    void activated();

public slots:
    /*!
     * \brief Enable the global shortcut.
     * \param enabled To enable the shortcut, set this to true.
     */
    void setEnabled(bool enabled = true);

protected:

private:
    void updateEnableState();
#ifdef  Q_OS_WIN
    virtual bool nativeEventFilter(const QByteArray &eventType,
                                   void *message, long *result) Q_DECL_OVERRIDE;
    void registerCurrent();
    void unregisterCurrent();

    static QHash<uint, KNGlobalAction *> m_registeredKeys;
    static int m_reference;
    QHash<uint, QPair<quint32, quint32>> m_sequenceIds;
#endif
    bool m_enabled;
};

#endif // KNGLOBALACTION_H
