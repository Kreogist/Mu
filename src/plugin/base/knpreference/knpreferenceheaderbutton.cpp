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
#include <QTimeLine>
#include <QPainter>
#include <QPaintEvent>

#include "knpreferenceheaderbutton.h"

KNPreferenceHeaderButton::KNPreferenceHeaderButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set properties.
    setFixedHeight(64);

    //Initial the timelines.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(100);
    initialTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(0);
    initialTimeLine(m_mouseOut);

    //Initial the border.
    m_border.moveTo(0,0);
    m_border.lineTo(0,64);
    m_border.lineTo(47,64);
    m_border.cubicTo(75,64,75,0,106,0);
    m_border.lineTo(0,0);
    //Initial the close button.
    m_closeIcon=QPixmap("://public/items/Close.png");
}

QPixmap KNPreferenceHeaderButton::icon() const
{
    return m_icon;
}

QString KNPreferenceHeaderButton::text() const
{
    return QString();
}

void KNPreferenceHeaderButton::setIcon(const QPixmap &icon)
{
    m_icon=icon;
}

void KNPreferenceHeaderButton::setText(const QString &text)
{
    Q_UNUSED(text)
}

void KNPreferenceHeaderButton::enterEvent(QEvent *event)
{
    KNAbstractButton::enterEvent(event);
    //Stop animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set params.
    m_mouseIn->setStartFrame(m_closeIconOpacity*100);
    //Start anime.
    m_mouseIn->start();
}

void KNPreferenceHeaderButton::leaveEvent(QEvent *event)
{
    KNAbstractButton::leaveEvent(event);
    //Stop animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set params.
    m_mouseOut->setStartFrame(m_closeIconOpacity*100);
    //Start anime.
    m_mouseOut->start();
}

void KNPreferenceHeaderButton::mousePressEvent(QMouseEvent *event)
{
    if(!m_pressed)
    {
        m_pressed=true;
    }
    KNAbstractButton::mousePressEvent(event);
}

void KNPreferenceHeaderButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_pressed && rect().contains(event->pos()))
    {
        m_pressed=false;
        emit clicked();
    }
    KNAbstractButton::mousePressEvent(event);
}

void KNPreferenceHeaderButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the antialiasing painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Draw the background.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().window());
    painter.drawPath(m_border);
    //Draw the close icon.
    painter.setOpacity(m_closeIconOpacity);
    painter.drawPixmap(m_iconPosition,m_closeIcon);
    //Draw the content.
    painter.setOpacity(1-m_closeIconOpacity);
    painter.drawPixmap(m_iconPosition,m_icon);
}

void KNPreferenceHeaderButton::onActionMouseInOut(const int &frame)
{
    m_closeIconOpacity=(qreal)frame/100;
    update();
}

void KNPreferenceHeaderButton::initialTimeLine(QTimeLine *timeLine)
{
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNPreferenceHeaderButton::onActionMouseInOut);
}
