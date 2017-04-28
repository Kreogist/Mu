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

#ifndef KNPREFERENCELANGUAGEITEM_H
#define KNPREFERENCELANGUAGEITEM_H

#include "knpreferenceitem.h"

/*!
 * \brief The KNPreferenceLanguageItem class
 */
class KNPreferenceLanguageItem : public KNPreferenceItem
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPreferenceLanguageItem widget.
     * \param parent The parent widget.
     */
    explicit KNPreferenceLanguageItem(QWidget *parent = 0);

    /*!
     * \brief Set the language display name.
     * \param languageName The language display name.
     */
    void setLanguageName(const QString &languageName);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNPreferenceItem::paintContent().
     */
    void paintContent(QPainter *painter) Q_DECL_OVERRIDE;

private:
    QString m_languageName;
};

#endif // KNPREFERENCELANGUAGEITEM_H
