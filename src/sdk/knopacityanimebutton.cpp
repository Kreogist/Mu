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

#include "knopacityanimebutton.h"

#define BaseOpacity 400
#define HoverOpacity 800

KNOpacityAnimeButton::KNOpacityAnimeButton(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseIn(generateTimeLine(HoverOpacity)),
    m_mouseOut(generateTimeLine(BaseOpacity)),
    m_mouseDown(generateTimeLine(1000)),
    m_mouseUp(generateTimeLine(HoverOpacity)),
    m_imageOpacity(BaseOpacity)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
}

void KNOpacityAnimeButton::paintEvent(QPaintEvent *event)
{
    //Ignore original painting.
    Q_UNUSED(event)
    //Check the icon first.
    if(!icon().isNull())
    {
        //Initial painter.
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::SmoothPixmapTransform, true);
        //Set the opacity.
        painter.setOpacity(imageOpacity());
        //Calcualte the icon size.
        int iconSize=qMin(width(), height());
        //Draw the icon.
        painter.drawPixmap((width()-iconSize)>>1,
                           (height()-iconSize)>>1,
                           iconSize,
                           iconSize,
                           icon().pixmap(iconSize, iconSize));
    }
}

qreal KNOpacityAnimeButton::imageOpacity()
{
    return m_imageOpacity/1000.0;
}

void KNOpacityAnimeButton::enterEvent(QEvent *event)
{
    //Start the original enter event.
    QAbstractButton::enterEvent(event);
    //Start the mouse in animation.
    startAnime(m_mouseIn);
}

void KNOpacityAnimeButton::leaveEvent(QEvent *event)
{
    //Start the original leave event.
    QAbstractButton::leaveEvent(event);
    //Start the mouse out animation.
    startAnime(m_mouseOut);
}

void KNOpacityAnimeButton::focusInEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusInEvent(event);
    //Start the mouse in animation.
    startAnime(m_mouseIn);
}

void KNOpacityAnimeButton::focusOutEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusOutEvent(event);
    //Start the mouse in animation.
    startAnime(m_mouseOut);
}

void KNOpacityAnimeButton::mousePressEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mousePressEvent(event);
    //Start the mouse down animation.
    startAnime(m_mouseDown);
}

void KNOpacityAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mouseReleaseEvent(event);
    //Start the mouse down animation.
    startAnime(m_mouseUp);
}

void KNOpacityAnimeButton::onActionOpacityChanged(const int &opacity)
{
    //Save the opacity.
    m_imageOpacity=opacity;
    //Update the widget.
    update();
}

void KNOpacityAnimeButton::startAnime(QTimeLine *timeLine)
{
    //Stop all time line.
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseDown->stop();
    m_mouseUp->stop();
    //Set the parameter of the time line.
    timeLine->setStartFrame(m_imageOpacity);
    //Start the time line.
    timeLine->start();
}

QTimeLine *KNOpacityAnimeButton::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setEndFrame(endFrame);
    timeLine->setUpdateInterval(10);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNOpacityAnimeButton::onActionOpacityChanged);
    return timeLine;
}

