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
#include <QTimeLine>
#include <QGraphicsOpacityEffect>

#include "knglassbutton.h"

#include <QDebug>

#define BaseOpacity 500
#define HoverOpacity 800

KNGlassButton::KNGlassButton(QWidget *parent) :
    QAbstractButton(parent),
    m_contentAlign(Qt::AlignLeft | Qt::AlignVCenter),
    m_spacing(10),
    m_imageOpacity(BaseOpacity),
    m_mouseIn(generateTimeLine(HoverOpacity)),
    m_mouseOut(generateTimeLine(BaseOpacity)),
    m_mouseDown(generateTimeLine(1000)),
    m_mouseUp(generateTimeLine(HoverOpacity)),
    m_highLight(QLinearGradient(0.0, 0.0, 0.0, 0.0)),
    m_highLightMask(QLinearGradient(0.0, 0.0, 0.0, 0.0)),
    m_maskImage(QPixmap()),
    m_highLightOpacity(0.0)
{
    //Set properties.
    setContentsMargins(20, 5, 20, 5);
    //Update the high light gradient.
    m_highLight.setColorAt(0, QColor(255,255,255));
    m_highLight.setColorAt(0.7, QColor(255,255,255,0));
    //Update the high light mask gradient.
    m_highLightMask.setColorAt(0, QColor(0,0,0));
    m_highLightMask.setColorAt(1.0, QColor(255,255,255,0));
}

void KNGlassButton::paintEvent(QPaintEvent *event)
{
    //Initial the painter.
    QPainter painter(this);
    //Set rendering hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Calculate the basic opacity.
    qreal basePaintOpacity=(qreal)m_imageOpacity/1000.0;
    //Set the painter opacity.
    painter.setOpacity(basePaintOpacity/2.0);
    //Draw the border.
    QPen pen(QColor(255, 255, 255));
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect(), 8, 6);

    //Set the opacity.
    painter.setOpacity(basePaintOpacity);
    //Draw the icon.
    const int iconSize=contentsRect().height();
    painter.drawPixmap(contentsRect().x(),
                       contentsRect().y(),
                       iconSize,
                       iconSize,
                       icon().pixmap(iconSize, iconSize));
    //Draw the text.
    painter.drawText(contentsRect().x()+iconSize+m_spacing,
                     contentsRect().y(),
                     contentsRect().width()-iconSize-m_spacing,
                     contentsRect().height(),
                     m_contentAlign,
                     text());

    //Set the painter opacity.
    painter.setOpacity(basePaintOpacity-((qreal)BaseOpacity/1000.0));
    //Draw the high light image.
    painter.drawPixmap(0, 0, m_maskImage);
}

void KNGlassButton::enterEvent(QEvent *event)
{
    //Start the original enter event.
    QAbstractButton::enterEvent(event);
    //Start the mouse in animation.
    startAnime(m_mouseIn);
}

void KNGlassButton::leaveEvent(QEvent *event)
{
    //Start the original leave event.
    QAbstractButton::leaveEvent(event);
    //Start the mouse out animation.
    startAnime(m_mouseOut);
}

void KNGlassButton::focusInEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusInEvent(event);
    //Start the mouse in animation.
    startAnime(m_mouseIn);
}

void KNGlassButton::focusOutEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusOutEvent(event);
    //Start the mouse in animation.
    startAnime(m_mouseOut);
}

void KNGlassButton::mousePressEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mousePressEvent(event);
    //Start the mouse down animation.
    startAnime(m_mouseDown);
}

void KNGlassButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mouseReleaseEvent(event);
    //Start the mouse down animation.
    startAnime(m_mouseUp);
}

void KNGlassButton::resizeEvent(QResizeEvent *event)
{
    //Resize the button.
    QAbstractButton::resizeEvent(event);
    //Update the high light gradient.
    m_highLight.setFinalStop(QPoint(0, height()));
    //Update the high light mask gradient.
    m_highLightMask.setFinalStop(QPoint(width(), 0));
    //Generate a QImage.
    QPixmap maskPixmap(size());
    maskPixmap.fill(QColor(0,0,0,0));
    //Draw things on the mask image.
    QPainter maskPainter(&maskPixmap);
    maskPainter.setRenderHints(QPainter::Antialiasing |
                               QPainter::TextAntialiasing |
                               QPainter::SmoothPixmapTransform, true);
    maskPainter.setPen(Qt::NoPen);
    maskPainter.setBrush(m_highLight);
    QRect roundRect=QRect(rect().x()+2,
                          rect().y()+2,
                          rect().width()-4,
                          rect().height()-4);
    maskPainter.drawRoundedRect(roundRect, 4, 4);
    //Change the composition mode.
    maskPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    //Draw the mask.
    maskPainter.setBrush(m_highLightMask);
    maskPainter.drawRoundedRect(roundRect, 4, 4);
    //Save the mask pixmap.
    m_maskImage=maskPixmap;
}

void KNGlassButton::onActionOpacityChanged(const int &opacity)
{
    //Save the opacity.
    m_imageOpacity=opacity;
    //Update the widget.
    update();
}

void KNGlassButton::startAnime(QTimeLine *timeLine)
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

QTimeLine *KNGlassButton::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setEndFrame(endFrame);
    timeLine->setUpdateInterval(10);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNGlassButton::onActionOpacityChanged);
    return timeLine;
}

int KNGlassButton::spacing() const
{
    return m_spacing;
}

void KNGlassButton::setSpacing(int spacing)
{
    m_spacing = spacing;
}
