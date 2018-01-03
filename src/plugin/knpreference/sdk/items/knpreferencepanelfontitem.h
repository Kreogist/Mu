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

#ifndef KNPREFERENCEPANELFONTITEM_H
#define KNPREFERENCEPANELFONTITEM_H

#include "knpreferencepanelitem.h"

class QPushButton;
class QLabel;
class KNMouseDetectLabel;
/*!
 * \brief The KNPreferencePanelFontItem class provides a widget which allows to
 * display a widget for changing the font type value.
 */
class KNPreferencePanelFontItem : public KNPreferencePanelItem
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelFontItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelFontItem(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::value().
     */
    QVariant value() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::setConfig().
     */
    void setConfig(const QVariant &config) Q_DECL_OVERRIDE;

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
    void onActionShowTooltip();

private:
    QMap<QString, bool> m_dialogOption;
    QFont m_font;
    KNMouseDetectLabel *m_previewLabel;
    QLabel *m_informationLabel, *m_previewTooltip;
};

#endif // KNPREFERENCEPANELFONTITEM_H
