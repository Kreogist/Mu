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

#ifndef KNNOTIFICATIONBUTTON_H
#define KNNOTIFICATIONBUTTON_H

#include "knimagelabel.h"

/*!
 * \brief The KNNotificationButton class provides the notification center
 * customized button.
 */
class KNNotificationButton : public KNImageLabel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNNotificationButton class.
     * \param parent The parent widget.
     */
    explicit KNNotificationButton(QWidget *parent = 0);

signals:
    /*!
     * \brief Ask to show the requireShowNotificationCenter widget.
     */
    void requireShowNotificationCenter();

public slots:

protected:
    /*!
     * \brief Reimplemented from KNImageLabel::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Reimplemented from KNImageLabel::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool m_pressed;
};

#endif // KNNOTIFICATIONBUTTON_H
