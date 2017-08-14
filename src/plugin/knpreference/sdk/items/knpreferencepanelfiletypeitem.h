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

#ifndef KNPREFERENCEPANELFILETYPEITEM_H
#define KNPREFERENCEPANELFILETYPEITEM_H

#include "knpreferencepanelitem.h"

class KNRoundSwitchButton;
/*!
 * \brief The KNPreferencePanelFileTypeItem class provides a widget which allows
 * to change the association of a file type.
 */
class KNPreferencePanelFileTypeItem : public KNPreferencePanelItem
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferencePanelFileTypeItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferencePanelFileTypeItem(QWidget *parent = 0);

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

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::getValue().
     */
    QVariant getValue(const QVariant &defaultValue) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPreferencePanelItem::initialValue().
     */
    void initialValue(const QVariant &defaultValue) Q_DECL_OVERRIDE;

private slots:
    void onClicked();

private:
    inline bool isTypeBind() const;
    inline bool setAssociations(bool bind);
    QString m_suffix, m_typeHandler;
    KNRoundSwitchButton *m_button;
    QLabel *m_fullName;
};

#endif // KNPREFERENCEPANELFILETYPEITEM_H
