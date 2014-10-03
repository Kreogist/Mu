/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QWheelEvent>

#include "knabstractslider.h"

#include <QDebug>

qint64 KNAbstractSlider::maximum() const
{
    return m_maximum;
}

qint64 KNAbstractSlider::minimal() const
{
    return m_minimal;
}

qint64 KNAbstractSlider::value() const
{
    return m_value;
}

qint64 KNAbstractSlider::range() const
{
    return m_range;
}

qreal KNAbstractSlider::percentage() const
{
    return m_percentage;
}

void KNAbstractSlider::setMaximum(qint64 maximum)
{
    qint64 preferMaximum=maximum;
    //Check the low bound.
    if(preferMaximum<m_minimal)
    {
        preferMaximum=m_minimal;
    }
    //Check is the prefer value still the same or not.
    if(preferMaximum==m_maximum)
    {
        return;
    }
    //Set maximum.
    m_maximum=preferMaximum;
    //Update slider range.
    updateRange();
    //Reset the value to make it in the range.
    setValue(m_value);
}

void KNAbstractSlider::setMinimal(qint64 minimal)
{
    qint64 preferMinimal=minimal;
    //Check the up bound.
    if(preferMinimal>m_maximum)
    {
        preferMinimal=m_maximum;
    }
    //Check is the prefer value still the same or not.
    if(preferMinimal==m_minimal)
    {
        return;
    }
    //Set minimal.
    m_minimal=preferMinimal;
    //Update range.
    updateRange();
    //Reset the value to make it in the range.
    setValue(m_value);
}

void KNAbstractSlider::setRange(qint64 min, qint64 max)
{
    //Check is the value avaliable.
    if(min>max || (min==m_minimal && max==m_maximum))
    {
        return;
    }
    //Set value.
    m_minimal=min;
    m_maximum=max;
    //Update range.
    updateRange();
    //Reset the current value to change the data.
    setValue(m_value);
}

void KNAbstractSlider::setValue(qint64 value)
{
    qint64 preferValue=value;
    //Check up and down bound.
    if(preferValue>m_maximum)
    {
        preferValue=m_maximum;
    }
    if(preferValue<m_minimal)
    {
        preferValue=m_minimal;
    }
    //Check now the prefer value is still the same as the current value.
    if(preferValue==m_value)
    {
        return;
    }
    //Set value.
    m_value=preferValue;
    //Update percentage.
    m_percentage=m_range==0?-1:(qreal)(m_value-m_minimal)/(qreal)m_range;
    //Update widget.
    update();
    emit valueChanged(m_value);
}

void KNAbstractSlider::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);
    //Use pixel data as first choice, if no pixel data.
    if(event->pixelDelta()==QPoint(0,0))
    {
        //Using angle data.
        QPoint dataPoint=event->angleDelta()/40;
        setValue(dataPoint.x()==0?
                     value()+(dataPoint.y())*m_wheelStep:
                     value()+(dataPoint.x())*m_wheelStep);
    }
    else
    {
        setValue(event->pixelDelta().x()==0?
                     (value()+event->pixelDelta().y()*m_wheelStep):
                     (value()+event->pixelDelta().x()*m_wheelStep));
    }
    emit sliderMoved(value());
}

void KNAbstractSlider::updateRange()
{
    m_range=m_maximum-m_minimal;
    emit rangeChanged(m_minimal, m_maximum);
}

qint64 KNAbstractSlider::wheelStep() const
{
    return m_wheelStep;
}

void KNAbstractSlider::setWheelStep(qint64 wheelStep)
{
    m_wheelStep=wheelStep;
}
