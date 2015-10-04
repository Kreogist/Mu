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

#ifndef KNLABELBUTTON_H
#define KNLABELBUTTON_H

#include <QLabel>

class KNLabelButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNLabelButton(QWidget *parent = 0);

signals:
    void clicked();
    void pressed();
    void released();
    void dblClicked();

public slots:

protected:
    /*!
     * \brief mousePressEvent
     * \param event
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief mouseDoubleClickEvent
     * \param event
     */
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief mouseReleaseEvent
     * \param event
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_pressed;
};

#endif // KNLABELBUTTON_H
