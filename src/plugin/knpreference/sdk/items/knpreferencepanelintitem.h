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

#ifndef KNPREFERENCEPANELINTITEM_H
#define KNPREFERENCEPANELINTITEM_H

#include "knpreferencepanelitem.h"

class QSpinBox;
class QSlider;
/*!
 * \brief The KNPreferencePanelIntItem class provides the a widget which allows
 * to display a widget for changing the integer value type. This widget needs to
 * provide the range of the target value.
 */
class KNPreferencePanelIntItem : public KNPreferencePanelItem
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelIntItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelIntItem(QWidget *parent = 0);

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
    void onValueChanged(int value);

private:
    inline void syncValue(int value);
    inline void setSignalBlock(bool block);
    QSpinBox *m_integerViewer;
    QSlider *m_integerSlider;
};

#endif // KNPREFERENCEPANELINTITEM_H
