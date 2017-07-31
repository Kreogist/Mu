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

#ifndef KNPREFERENCESHORTCUTITEM_H
#define KNPREFERENCESHORTCUTITEM_H

#include "knpreferencepanelitem.h"

class QLabel;
class QPushButton;
class KNShortcutEditor;
/*!
 * \brief The KNPreferenceShortcutItem class provides a preference item to set
 * a shortcut key pair to a specific action.
 */
class KNPreferencePanelShortcutItem : public KNPreferencePanelItem
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferenceShortcutItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelShortcutItem(QWidget *parent = 0);
    ~KNPreferencePanelShortcutItem();

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::value().
     */
    QVariant value() const Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNPreferencePanelItem::setWidgetValue().
     */
    void setWidgetValue(const QVariant &value) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::isEqual().
     */
    bool isEqual(const QVariant &currentValue,
                 const QVariant &originalValue) Q_DECL_OVERRIDE;

private slots:
    void onShortcutUpdate();

private:
    void onStartEdit();
    void onShortcutCancel();
    void onShortcutAccept();
    void onShortcutClear();

private:
    inline void showShortcutText();
    QKeySequence m_sequence;
    QLabel *m_shortcutText;
    QPushButton *m_startEdit;
    KNShortcutEditor *m_shortcutEditor;
};

#endif // KNPREFERENCESHORTCUTITEM_H
