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
#include <QPainter>
#include <QMouseEvent>

#include "knaccountavatarbutton.h"

KNAccountAvatarButton::KNAccountAvatarButton(QWidget *parent) :
    QWidget(parent),
    m_anonymousPixmap(QPixmap("://public/anonymous.png")),
    m_scaledPixmap(QPixmap()),
    m_graphicsMargin(0),
    m_pressed(false),
    m_isLogin(false)
{
}

void KNAccountAvatarButton::setAccountAvatar(const QPixmap &avatar)
{
    //Check avatar.
    if(avatar.isNull())
    {
        //Set the pixmap to be a null image.
        m_scaledPixmap=QPixmap();
    }
    else
    {
        //Calculate image width and height.
        int imageWidth=width()-(m_graphicsMargin<<1),
            imageHeight=height()-(m_graphicsMargin<<1);
        //Reset the pixmap
        m_scaledPixmap=QPixmap(imageWidth, imageHeight);
        //Clear the pixmap.
        m_scaledPixmap.fill(QColor(0, 0, 0, 0));
        //Initial the painter.
        QPainter painter(&m_scaledPixmap);
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::TextAntialiasing |
                               QPainter::SmoothPixmapTransform, true);
        //Set the color.
        painter.setBrush(QColor(0, 0, 0));
        //Draw rounded rect.
        painter.drawRoundedRect(QRect(0, 0, imageWidth, imageHeight),
                                4.0,
                                4.0);
        //Reset the composition mode.
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        //Draw the image.
        painter.drawPixmap(0, 0, avatar.scaled(QSize(imageWidth, imageHeight),
                                               Qt::KeepAspectRatioByExpanding,
                                               Qt::SmoothTransformation));
        //Painting finsihed.
        painter.end();
    }
    //Change the login state.
    m_isLogin=true;
    //Update the avatar.
    update();
}

void KNAccountAvatarButton::resetAccountAvatar()
{
    //Reset the pixmap to anonymous pixmap.
    m_scaledPixmap=QPixmap();
    //Reset the login state.
    m_isLogin=false;
}

void KNAccountAvatarButton::setButtonSize(int buttonSize)
{
    //Set properties.
    setFixedSize(buttonSize, buttonSize);
    //Scaled the anonymous pixmap.
    m_anonymousScaledPixmap=
            m_anonymousPixmap.scaled(buttonSize-m_graphicsMargin,
                                     buttonSize-m_graphicsMargin,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
}

void KNAccountAvatarButton::mousePressEvent(QMouseEvent *event)
{
    //Do original event.
    QWidget::mousePressEvent(event);
    //Set the pressed flag.
    m_pressed=true;
}

void KNAccountAvatarButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the release event.
    QWidget::mouseReleaseEvent(event);
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

void KNAccountAvatarButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Simply painter the anonymous image.
    painter.drawPixmap(m_graphicsMargin, m_graphicsMargin,
                       m_scaledPixmap.isNull() ?
                           m_anonymousScaledPixmap:
                           m_scaledPixmap);
}

void KNAccountAvatarButton::setGraphicsMargin(int graphicsMargin)
{
    //Save the graphics margin.
    m_graphicsMargin = graphicsMargin;
}

bool KNAccountAvatarButton::isLogin() const
{
    return m_isLogin;
}
