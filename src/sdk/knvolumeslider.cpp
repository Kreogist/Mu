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
#include <QPainter>
#include <QTimeLine>
#include <QMouseEvent>

#include "kndpimanager.h"

#include "knvolumeslider.h"

#define MinimalAlpha    80.0
#define MaximumAlpha    240.0
#define GapAlpha        175.0
#define SliderHeight    5

KNVolumeSlider::KNVolumeSlider(QWidget *parent) :
    KNAbstractSlider(parent),
    m_mouseInOut(new QTimeLine(200, this)),
    m_rectColor(QColor(255, 255, 255, MinimalAlpha)),
    m_pressed(false)
{
    //Set properties.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::Slider));
    //Configure the time line.
    m_mouseInOut->setUpdateInterval(16);
    m_mouseInOut->setEasingCurve(QEasingCurve::OutCubic);
    //Link the time line and the mouse in out slot.
    connect(m_mouseInOut, &QTimeLine::frameChanged,
                this, &KNVolumeSlider::onMouseInOut);
}

void KNVolumeSlider::paintEvent(QPaintEvent *event)
{
    //Ignore the paint event.
    Q_UNUSED(event);
    //Initial painter.
    QPainter painter(this);
    //Set antialiasing.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Set pen and draw central rects.
    painter.setPen(m_rectColor);
    int scaledSliderHeight=knDpi->width(SliderHeight);
    painter.drawRoundedRect(QRect(scaledSliderHeight,
                                  (height()-scaledSliderHeight)>>1,
                                  width()-(scaledSliderHeight<<1),
                                  scaledSliderHeight),
                            scaledSliderHeight>>1,
                            scaledSliderHeight>>1,
                            Qt::AbsoluteSize);
    //Set no pen, and set brush to the filling color.
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, percentage()*GapAlpha+MinimalAlpha));
    //Get the current position, paint the content rect.
    painter.drawRoundedRect(
                scaledSliderHeight,
                (height()-scaledSliderHeight)>>1,
                percentage()*(qreal)(width()-(scaledSliderHeight<<1)),
                scaledSliderHeight,
                scaledSliderHeight>>1,
                scaledSliderHeight>>1,
                Qt::AbsoluteSize);
}

void KNVolumeSlider::enterEvent(QEvent *event)
{
    //Start the enter anime.
    startAnime(MaximumAlpha);
    //Do original enter event.
    KNAbstractSlider::enterEvent(event);
}

void KNVolumeSlider::leaveEvent(QEvent *event)
{
    //Start the leave anime.
    startAnime(MinimalAlpha);
    //Do original leave event.
    KNAbstractSlider::leaveEvent(event);
}

void KNVolumeSlider::mousePressEvent(QMouseEvent *event)
{
    //Set pressed flag.
    m_pressed=true;
    //Update the value.
    setValue(posToValue(event->pos().x()));
    //Emit pressed signal.
    emit sliderPressed();
    //Do original pressed event.
    KNAbstractSlider::mousePressEvent(event);
}

void KNVolumeSlider::mouseMoveEvent(QMouseEvent *event)
{
    //Update the value when the mouse pressed on the slider.
    if(m_pressed)
    {
        setValue(posToValue(event->pos().x()));
    }
}

void KNVolumeSlider::mouseReleaseEvent(QMouseEvent *event)
{
    //Release the pressed flag.
    m_pressed=false;
    //Emit slider release signal.
    emit sliderReleased();
    //Update the value.
    setValue(posToValue(event->pos().x()));
    //Emit slider moved signal.
    emit sliderMoved(value());
    //Do original release event.
    KNAbstractSlider::mouseReleaseEvent(event);
}

void KNVolumeSlider::onMouseInOut(int frame)
{
    //Update the rect color.
    m_rectColor.setAlpha(frame);
    //Update the slider.
    update();
}

qint64 KNVolumeSlider::posToValue(int position)
{
    //Check whether is the current position is out of range.
    if(position<SliderHeight)
    {
        return minimal();
    }
    if(position>width()-SliderHeight)
    {
        return maximum();
    }
    //Calculate the value according to the range percentage.
    return minimal()+
                (qreal)range()/(qreal)(width()-SliderHeight)*(qreal)position;
}

inline void KNVolumeSlider::startAnime(int endFrame)
{
    //Stop all the time line.
    m_mouseInOut->stop();
    //Set parameter to the time line.
    m_mouseInOut->setFrameRange(m_rectColor.alpha(), endFrame);
    //Start animation.
    m_mouseInOut->start();
}
