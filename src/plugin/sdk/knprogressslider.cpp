/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QTimeLine>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QPainter>
#include <QRadialGradient>

#include "knprogressslider.h"

#include <QDebug>

KNProgressSlider::KNProgressSlider(QWidget *parent) :
    KNAbstractSlider(parent)
{
    //Set the focus.
    setFocusPolicy(Qt::WheelFocus);
    //Initial the mouse sense timeline.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(100);
    configureTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(m_mouseOutOpacity*100);
    configureTimeLine(m_mouseOut);
    //Set properties.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::Slider));
    updateButtonSize();
}

void KNProgressSlider::enterEvent(QEvent *event)
{
    //Stop all the animation.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set mouse in start frame.
    m_mouseIn->setStartFrame(m_backOpacity*100);
    //Start animation
    m_mouseIn->start();
    KNAbstractSlider::enterEvent(event);
}

void KNProgressSlider::leaveEvent(QEvent *event)
{
    //Stop all the animation.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set mouse in start frame.
    m_mouseOut->setStartFrame(m_backOpacity*100);
    //Start animation
    m_mouseOut->start();
    KNAbstractSlider::leaveEvent(event);
}

void KNProgressSlider::paintEvent(QPaintEvent *event)
{
    KNAbstractSlider::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    //Set antialiasing.
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    //Using background opacity to draw the slider background.
    painter.setOpacity(m_backOpacity);

    //Set no pen
    painter.setPen(Qt::NoPen);

    //Draw central rects.
    painter.fillRect(QRect(m_glowWidth,
                           m_glowWidth+m_spacing,
                           width()-(m_glowWidth<<1),
                           m_sliderHeight),
                     m_rectColor);

    //If range is not 0, draw the button.
    if(range()>0)
    {
        //Restore the opacity.
        painter.setOpacity(1.0);

        //Get the current position.
        int positionLeft=percentage()*(qreal)(width()-m_buttonSize);
        if(positionLeft<0)
        {
            positionLeft=0;
        }
        //Paint the rect.
        painter.fillRect(QRect(m_glowWidth,
                               m_glowWidth+m_spacing,
                               positionLeft,
                               m_sliderHeight),
                         m_buttonColor);

        //Draw the circle button.
        painter.save();
        //Calculate position.
        painter.translate(positionLeft,
                          m_spacing);
        //Set the gradient.
        int buttonCenter=m_glowWidth+(m_sliderHeight>>1);
        QRadialGradient buttonGradient=QRadialGradient(QPointF(buttonCenter, buttonCenter),
                                                       buttonCenter,
                                                       QPointF(buttonCenter, buttonCenter));
        buttonGradient.setColorAt(0, QColor(255,255,255,200));
        buttonGradient.setColorAt(1, QColor(255,255,255,0));
        painter.setBrush(buttonGradient);
        painter.drawEllipse(0,0,height(),height());
        //Set the color.
        painter.setBrush(m_backgroundColor);
        painter.drawEllipse(m_glowWidth-1, m_glowWidth-1, m_sliderHeight+2, m_sliderHeight+2);
        //Restore transform.
        painter.restore();
    }
}

void KNProgressSlider::mousePressEvent(QMouseEvent *event)
{
    //Set pressed flag.
    m_pressed=true;
    //Change the current value.
    setValue(posToValue(event->pos().x()));
    //Emit pressed signal.
    emit sliderPressed();
    KNAbstractSlider::mousePressEvent(event);
}

void KNProgressSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(m_pressed)
    {
        setValue(posToValue(event->pos().x()));
    }
}

void KNProgressSlider::mouseReleaseEvent(QMouseEvent *event)
{
    //Release pressed flag.
    m_pressed=false;
    //Emit released value.
    emit sliderReleased();
    //Change the current value.
    setValue(posToValue(event->pos().x()));
    //And emit the slider moved to signal.
    emit sliderMoved(value());
    KNAbstractSlider::mouseReleaseEvent(event);
}

void KNProgressSlider::onActionMouseInOut(const int &frame)
{
    m_backOpacity=(qreal)frame/100;
    update();
}

void KNProgressSlider::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNProgressSlider::onActionMouseInOut);
}

void KNProgressSlider::updateButtonSize()
{
    //Calculate the button size: two light glow width and the slider height.
    m_buttonSize=(m_glowWidth<<1)+m_sliderHeight;
    //Reset the widget height.
    setFixedHeight(m_buttonSize+(m_spacing<<1));
    //Redraw the widget.
    update();
}

qint64 KNProgressSlider::posToValue(int position)
{
    if(position<m_glowWidth)
    {
        return minimal();
    }
    if(position>width()-m_glowWidth)
    {
        return maximum();
    }
    return minimal()+
            (qreal)range()/(qreal)(width()-(m_glowWidth<<1))*(qreal)position;
}
