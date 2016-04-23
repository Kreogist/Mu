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
#include <QMouseEvent>

#include "knlabelbutton.h"

KNLabelButton::KNLabelButton(QWidget *parent) :
    QLabel(parent),
    m_pressed(false)
{
}

void KNLabelButton::mousePressEvent(QMouseEvent *event)
{
    //Do the press event.
    QLabel::mousePressEvent(event);
    //Set the pressed flag.
    m_pressed=true;
    //Emit pressed signal.
    emit pressed();
}

void KNLabelButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    QLabel::mouseDoubleClickEvent(event);
    //Emit the double clicked signal.
    emit doubleClicked();
}

void KNLabelButton::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    //Emit the release signal.
    emit released();
    //Check the pressed flag.
    if(m_pressed)
    {
        //Reset the pressed flag.
        m_pressed=false;
        //Check whether the position of the event is valid.
        if(rect().contains(event->pos()))
        {
            //Emit clicked signal.
            emit clicked();
        }
    }
}
