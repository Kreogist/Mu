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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QMouseEvent>
#include <QSizePolicy>
#include <QPainter>
#include <QGraphicsOpacityEffect>

#include "knopacitybutton.h"

KNOpacityButton::KNOpacityButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set toolbutton size policy.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::ToolButton));
    //Initial opacity effect.
    m_opacityEffect=new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(m_originalOpacity);
    setGraphicsEffect(m_opacityEffect);
}

QPixmap KNOpacityButton::icon() const
{
    return m_icon;
}

void KNOpacityButton::setIcon(const QPixmap &icon)
{
    m_icon=icon;
    update();
}

void KNOpacityButton::mousePressEvent(QMouseEvent *event)
{
    KNAbstractButton::mousePressEvent(event);
    //Set opacity.
    m_originalOpacity=m_opacityEffect->opacity();
    qreal preferOpacity=m_originalOpacity-m_opacityGap;
    m_opacityEffect->setOpacity(preferOpacity<0?
                                    m_originalOpacity+m_opacityGap:
                                    preferOpacity);
    //Set flag.
    m_pressed=true;
}

void KNOpacityButton::mouseReleaseEvent(QMouseEvent *event)
{
    KNAbstractButton::mouseReleaseEvent(event);
    //Recover opacity.
    m_opacityEffect->setOpacity(m_originalOpacity);
    //Judge is pressed?
    if(m_pressed)
    {
        m_pressed=false;
        //Judge is mouse in.
        if(rect().contains(event->pos()))
        {
            emit clicked();
        }
    }
}

void KNOpacityButton::paintEvent(QPaintEvent *event)
{
    KNAbstractButton::paintEvent(event);
    //Initial antialiasing painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Paint the contents.
    painter.drawPixmap(0,0,width(),height(),
                       m_icon.scaled(width(),
                                     height(),
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
}

qreal KNOpacityButton::opacity() const
{
    return m_opacityEffect->opacity();
}

void KNOpacityButton::setOpacity(const qreal &opacity)
{
    m_opacityEffect->setOpacity(opacity);
}

qreal KNOpacityButton::opacityGap() const
{
    return m_opacityGap;
}

void KNOpacityButton::setOpacityGap(const qreal &pressedOpacity)
{
    m_opacityGap=pressedOpacity;
}

