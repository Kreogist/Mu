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

#include "knnotificationbutton.h"

KNNotificationButton::KNNotificationButton(QWidget *parent) :
    KNImageLabel(parent),
    m_anonymousPixmap(QPixmap("://public/anonymous.png")),
    m_pressed(false),
    m_isLogin(false)
{
    //Set properties.
    setFixedSize(110, 110);
    setPixmap(m_anonymousPixmap);
}

void KNNotificationButton::setAccountAvatar(const QPixmap &avatar)
{
    //Check avatar.
    //Set the pixmap.
    setPixmap(avatar.isNull()?
                  m_anonymousPixmap:
                  avatar.scaled(size(),
                                Qt::IgnoreAspectRatio,
                                Qt::SmoothTransformation));
    //Change the login state.
    m_isLogin=true;
}

void KNNotificationButton::resetAccountAvatar()
{
    //Reset the pixmap to anonymous pixmap.
    setPixmap(m_anonymousPixmap);
    //Reset the login state.
    m_isLogin=false;
}

void KNNotificationButton::mousePressEvent(QMouseEvent *event)
{
    //Do original event.
    KNImageLabel::mousePressEvent(event);
    //Set the pressed flag.
    m_pressed=true;
}

void KNNotificationButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the release event.
    KNImageLabel::mouseReleaseEvent(event);
    //Check the released flag.
    if(m_pressed)
    {
        //Reset the pressed flag.
        m_pressed=false;
        //Check event position.
        if(rect().contains(event->pos()))
        {
            //Emit the show widget signal.
            emit requireShowNotificationCenter();
        }
    }
}

bool KNNotificationButton::isLogin() const
{
    return m_isLogin;
}

