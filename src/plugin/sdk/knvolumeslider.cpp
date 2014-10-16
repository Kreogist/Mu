/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QSizePolicy>
#include <QTimeLine>
#include <QMouseEvent>
#include <QPainter>
#include <QLinearGradient>

#include "knvolumeslider.h"

#include <QDebug>

KNVolumeSlider::KNVolumeSlider(QWidget *parent) :
    KNAbstractSlider(parent)
{
    //Set properties.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::Slider));

    //Initial the mouse in out time line.
    m_mouseIn=new QTimeLine(200, this);
    configureTimeLine(m_mouseIn);
    m_mouseIn->setEndFrame(m_maxAlpha);
    m_mouseOut=new QTimeLine(200, this);
    configureTimeLine(m_mouseOut);
    m_mouseOut->setEndFrame(m_minAlpha);
}

void KNVolumeSlider::paintEvent(QPaintEvent *event)
{
    KNAbstractSlider::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    //Set antialiasing.
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    //Set pen
    painter.setPen(m_rectColor);

    //Draw central rects.
    painter.drawRoundedRect(m_sliderHeight,
                            (height()-m_sliderHeight)>>1,
                            width()-(m_sliderHeight<<1),
                            m_sliderHeight,
                            m_sliderHeight>>1,
                            m_sliderHeight>>1,
                            Qt::AbsoluteSize);

    //Set no pen
    painter.setPen(Qt::NoPen);
    QColor contentsColor=m_rectColor;
    contentsColor.setAlphaF(percentage());
    painter.setBrush(contentsColor);
    //Get the current position.
    int positionLeft=percentage()*(qreal)(width()-(m_sliderHeight<<1));
    //Paint the rect.
    painter.drawRoundedRect(m_sliderHeight,
                            (height()-m_sliderHeight)>>1,
                            positionLeft,
                            m_sliderHeight,
                            m_sliderHeight>>1,
                            m_sliderHeight>>1,
                            Qt::AbsoluteSize);
}

void KNVolumeSlider::enterEvent(QEvent *event)
{
    //Stop all the animation.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameter.
    m_mouseIn->setStartFrame(m_rectColor.alpha());
    //Start animation.
    m_mouseIn->start();
    KNAbstractSlider::enterEvent(event);
}

void KNVolumeSlider::leaveEvent(QEvent *event)
{
    //Stop all the animation.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameter.
    m_mouseOut->setStartFrame(m_rectColor.alpha());
    //Start animation.
    m_mouseOut->start();
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
    KNAbstractSlider::mousePressEvent(event);
}

void KNVolumeSlider::mouseMoveEvent(QMouseEvent *event)
{
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
    KNAbstractSlider::mouseReleaseEvent(event);
}

void KNVolumeSlider::onActionMouseInOut(const int &frame)
{
    m_rectColor.setAlpha(frame);
    update();
}

void KNVolumeSlider::configureTimeLine(QTimeLine *timeline)
{
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(5);
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNVolumeSlider::onActionMouseInOut);
}

qint64 KNVolumeSlider::posToValue(int position)
{
    if(position<m_sliderHeight)
    {
        return minimal();
    }
    if(position>width()-m_sliderHeight)
    {
        return maximum();
    }
    return minimal()+
            (qreal)range()/(qreal)(width()-m_sliderHeight)*(qreal)position;
}
