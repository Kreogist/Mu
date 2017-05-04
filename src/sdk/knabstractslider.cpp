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
#include <QWheelEvent>

#include "knabstractslider.h"

KNAbstractSlider::KNAbstractSlider(QWidget *parent) :
    QWidget(parent),
    m_maximum(100),
    m_minimal(0),
    m_value(0),
    m_range(100),
    m_wheelStep(1),
    m_percentage(0)
{
}

void KNAbstractSlider::setMaximum(qint64 maximum)
{
    //Check is the prefer value still the same.
    if(maximum==m_maximum)
    {
        //Do nothing when the maximum is not changed.
        return;
    }
    //Set maximum.
    m_maximum=maximum;
    //Check the low bound.
    if(m_maximum<m_minimal)
    {
        //Change the minimal.
        m_minimal=m_maximum;
    }
    //Update slider range.
    updateRange();
    //Reset the value to make it in the range.
    setValue(m_value);
}

void KNAbstractSlider::setMinimal(qint64 minimal)
{
    //Check is the prefer value still the same.
    if(minimal==m_minimal)
    {
        return;
    }
    //Set minimal.
    m_minimal=minimal;
    //Check the up bound.
    if(m_minimal>m_maximum)
    {
        m_maximum=m_minimal;
    }
    //Update range.
    updateRange();
    //Reset the value to make it in the range.
    setValue(m_value);
}

void KNAbstractSlider::setRange(qint64 min, qint64 max)
{
    //Check is the value available and if all of them are the same as current.
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
    //Check up and down bound.
    if(value>m_maximum)
    {
        value=m_maximum;
    }
    if(value<m_minimal)
    {
        value=m_minimal;
    }
    //Check now the prefer value is still the same as the current value.
    if(value==m_value)
    {
        return;
    }
    //Set value.
    m_value=value;
    //Update percentage.
    m_percentage=m_range==0?-1:(qreal)(m_value-m_minimal)/(qreal)m_range;
    //Update widget.
    update();
    //Emit value changed signal.
    emit valueChanged(m_value);
}

void KNAbstractSlider::wheelEvent(QWheelEvent *event)
{
    //Do original wheel event.
    QWidget::wheelEvent(event);
    //If no pixel data, use pixel data as first choice.
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
        //Use the pixel data.
        setValue(event->pixelDelta().x()==0?
                     (value()+event->pixelDelta().y()*m_wheelStep):
                     (value()+event->pixelDelta().x()*m_wheelStep));
    }
    //Emit slider moved signal.
    emit sliderMoved(value());
}

inline void KNAbstractSlider::updateRange()
{
    //Recalculate the range.
    m_range=m_maximum-m_minimal;
    //Emit the range changed signal.
    emit rangeChanged(m_minimal, m_maximum);
}
