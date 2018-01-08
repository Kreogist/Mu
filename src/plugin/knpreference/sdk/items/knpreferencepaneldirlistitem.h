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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KNPREFERENCEPANELDIRLISTITEM_H
#define KNPREFERENCEPANELDIRLISTITEM_H

#include "knpreferencepanelitem.h"

class QBoxLayout;
class KNLabelButton;
class KNPreferenceDirItem;
/*!
 * \brief The KNPreferencePanelDirListItem class provides a widget to edit a
 * list of directory paths.
 */
class KNPreferencePanelDirListItem : public KNPreferencePanelItem
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelDirListItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelDirListItem(QWidget *parent = nullptr);

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::value().
     */
    QVariant value() const Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNPreferencePanelItem::itemHeightMultiple().
     */
    int itemHeightMultiple() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::setWidgetValue().
     */
    void setWidgetValue(const QVariant &value) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::isEqual().
     */
    bool isEqual(const QVariant &currentValue,
                 const QVariant &originalValue) Q_DECL_OVERRIDE;

private:
    inline KNLabelButton *generateButton(const QString &iconPath,
                                                const QString &tooltip);
    QBoxLayout *m_mainLayout;
    KNLabelButton *m_remove;
};

#endif // KNPREFERENCEPANELDIRLISTITEM_H
