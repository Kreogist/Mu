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
#include <QCursor>
#include <QTimeLine>
#include <QGraphicsOpacityEffect>

#include "knglassbutton.h"

#include <QDebug>

#define BaseOpacity 500
#define PressedOpacity 800
#define HoverOpacity 1000
#define IndicatorLength 16

KNGlassButton::KNGlassButton(QWidget *parent) :
    QAbstractButton(parent),
    m_maskImage(QPixmap()),
    m_highLight(QLinearGradient(0.0, 0.0, 0.0, 0.0)),
    m_highLightMask(QLinearGradient(0.0, 0.0, 0.0, 0.0)),
    m_contentAlign(Qt::AlignLeft | Qt::AlignVCenter),
    m_spacing(10),
    m_imageOpacity(BaseOpacity),
    m_mouseAnime(new QTimeLine(200, this))
{
    //Set properties.
    setContentsMargins(20, 5, 20, 5);
    //Configure the time line.
    m_mouseAnime->setUpdateInterval(10);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNGlassButton::onActionOpacityChanged);
    //Update the high light gradient.
    m_highLight.setColorAt(0, QColor(255,255,255));
    m_highLight.setColorAt(0.7, QColor(255,255,255,0));
    //Update the high light mask gradient.
    m_highLightMask.setColorAt(0, QColor(0,0,0));
    m_highLightMask.setColorAt(1.0, QColor(255,255,255,0));
}

void KNGlassButton::paintEvent(QPaintEvent *event)
{
    //Ignore the original event.
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set rendering hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Calculate the basic opacity.
    qreal basePaintOpacity=(qreal)m_imageOpacity/1000.0;
    //Set the painter opacity.
    painter.setOpacity((basePaintOpacity-0.5)*2);
    //Draw the border.
    painter.setPen(QColor(255, 255, 255));
    painter.setBrush(Qt::NoBrush);
    //Paint the indicator.
    qreal rightPosition=m_halfWidth+m_indicatorX;
    painter.drawLine(QPointF(rightPosition, height()),
                     QPointF(rightPosition, height()-IndicatorLength));
    painter.drawLine(QPointF(rightPosition-1, height()),
                     QPointF(rightPosition-IndicatorLength, height()));
    qreal leftPosition=m_halfWidth-m_indicatorX;
    painter.drawLine(QPointF(leftPosition, 0),
                     QPointF(leftPosition, IndicatorLength));
    painter.drawLine(QPointF(leftPosition+1, 0),
                     QPointF(leftPosition+IndicatorLength, 0));

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
    painter.setOpacity((basePaintOpacity-((qreal)BaseOpacity/1000.0))/4.0);
    //Draw the high light image.
    painter.drawPixmap(0, 0, m_maskImage);
}

void KNGlassButton::hideEvent(QHideEvent *event)
{
    //Hide the widget.
    QAbstractButton::hideEvent(event);
    //Stop all the animation.
    m_mouseAnime->stop();
    //Reset the opacity.
    onActionOpacityChanged(BaseOpacity);
}

void KNGlassButton::enterEvent(QEvent *event)
{
    //Start the original enter event.
    QAbstractButton::enterEvent(event);
    //Start the mouse in animation.
    startAnime(HoverOpacity);
}

void KNGlassButton::leaveEvent(QEvent *event)
{
    //Start the original leave event.
    QAbstractButton::leaveEvent(event);
    //Start the mouse out animation.
    startAnime(BaseOpacity);
}

void KNGlassButton::focusInEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusInEvent(event);
    //Start the mouse in animation.
    startAnime(HoverOpacity);
}

void KNGlassButton::focusOutEvent(QFocusEvent *event)
{
    //Start the original enter event.
    QAbstractButton::focusOutEvent(event);
    //Start the mouse in animation.
    startAnime(BaseOpacity);
}

void KNGlassButton::mousePressEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mousePressEvent(event);
    //Start the mouse down animation.
    startAnime(PressedOpacity);
}

void KNGlassButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Start the original enter event.
    QAbstractButton::mouseReleaseEvent(event);
    //Start the mouse down animation.
    startAnime(HoverOpacity);
}

void KNGlassButton::resizeEvent(QResizeEvent *event)
{
    //Resize the button.
    QAbstractButton::resizeEvent(event);
    //Update the half width.
    m_halfWidth=(qreal)(width()>>1);
    //Update the high light gradient.
    m_highLight.setFinalStop(QPoint(0, height()));
    //Update the high light mask gradient.
    m_highLightMask.setFinalStop(QPoint(width(), 0));
    //Generate a QImage.
    m_maskImage=QPixmap(size());
    m_maskImage.fill(QColor(0,0,0,0));
    //Draw things on the mask image.
    QPainter maskPainter(&m_maskImage);
    maskPainter.setRenderHints(QPainter::Antialiasing |
                               QPainter::TextAntialiasing |
                               QPainter::SmoothPixmapTransform, true);
    //Draw the high light.
    maskPainter.fillRect(rect(), m_highLight);
    //Change the composition mode.
    maskPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    //Draw the mask.
    maskPainter.fillRect(rect(), m_highLightMask);
}

void KNGlassButton::onActionOpacityChanged(const int &opacity)
{
    //Save the opacity.
    m_imageOpacity=opacity;
    //Change the indicator size.
    m_indicatorX=(qreal)m_imageOpacity/1000.0*m_halfWidth;
    //Check mouse position.
    if(!rect().contains(mapFromGlobal(QCursor::pos())))
    {
        //Restart the animation to base opacity.
        startAnime(BaseOpacity);
    }
    //Update the widget.
    update();
}

inline void KNGlassButton::startAnime(const int &endFrame)
{
    //Stop the time line.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(m_imageOpacity, endFrame);
    //Start the time line.
    m_mouseAnime->start();
}

int KNGlassButton::spacing() const
{
    return m_spacing;
}

void KNGlassButton::setSpacing(int spacing)
{
    m_spacing = spacing;
}
