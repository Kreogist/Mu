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
#include <QCursor>

#include "knopacityanimebutton.h"

#define DisabledOpacity 0.1
#define BaseOpacity 400
#define HoverOpacity 800

KNOpacityAnimeButton::KNOpacityAnimeButton(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_imageOpacity(BaseOpacity)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    //Configure the time line.
    m_mouseAnime->setUpdateInterval(33);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNOpacityAnimeButton::onActionOpacityChanged);
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
        //Check whether the button is disabled and set the opacity.
        painter.setOpacity(isEnabled()?
                               imageOpacity():
                               DisabledOpacity);
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
    startAnime(HoverOpacity);
}

void KNOpacityAnimeButton::leaveEvent(QEvent *event)
{
    //Start the original leave event.
    QAbstractButton::leaveEvent(event);
    //Start the mouse out animation.
    startAnime(BaseOpacity);
}

void KNOpacityAnimeButton::focusInEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusInEvent(event);
    //Start the mouse in animation.
    startAnime(HoverOpacity);
}

void KNOpacityAnimeButton::focusOutEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusOutEvent(event);
    //Start the mouse in animation.
    startAnime(BaseOpacity);
}

void KNOpacityAnimeButton::mousePressEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mousePressEvent(event);
    //Start the mouse down animation.
    startAnime(1000);
}

void KNOpacityAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mouseReleaseEvent(event);
    //Start the mouse down animation.
    startAnime(HoverOpacity);
}

void KNOpacityAnimeButton::onActionOpacityChanged(const int &opacity)
{
    //Save the opacity.
    m_imageOpacity=opacity;
    //Check the mouse is still in the rect.
    if(!rect().contains(QWidget::mapFromGlobal(QCursor::pos())))
    {
        //Change the target.
        m_mouseAnime->setEndFrame(BaseOpacity);
    }
    //Update the widget.
    update();
}

inline void KNOpacityAnimeButton::startAnime(const int &endFrame)
{
    //Stop the time line.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(m_imageOpacity, endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
