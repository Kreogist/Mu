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
#include <QMouseEvent>
#include <QPainter>

#include "kndpimanager.h"

#include "knprogressslider.h"

#define OutOpacity          0.65
#define GlowWidth         5
#define SliderHeight      4
#define Spacing           2
#define RectColor         QColor(255,255,255,70)
#define m_backgroundColor   QColor(255,255,255,80)
#define m_buttonColor       QColor(255,255,255,110)

KNProgressSlider::KNProgressSlider(QWidget *parent) :
    KNAbstractSlider(parent),
    m_mouseInOut(new QTimeLine(200, this)),
    m_buttonGradient(QRadialGradient()),
    m_backOpacity(OutOpacity),
    m_buttonSize(0),
    m_pressed(false)
{
    //Set properties.
    setFocusPolicy(Qt::WheelFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::Slider));
    //Configure the time line.
    m_mouseInOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseInOut->setUpdateInterval(16);
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KNProgressSlider::onMouseInOut);
    //Initial the button radius.
    m_buttonGradient.setColorAt(0, QColor(255,255,255,200));
    m_buttonGradient.setColorAt(1, QColor(255,255,255,0));
    //Update UI elements.
    updateButtonSize();
}

void KNProgressSlider::enterEvent(QEvent *event)
{
    //Start mouse in animation.
    startAnime(100);
    //Do orignal enter event.
    QWidget::enterEvent(event);
}

void KNProgressSlider::leaveEvent(QEvent *event)
{
    //Start mouse in animation.
    startAnime(OutOpacity*100.0);
    //Do orignal enter event.
    QWidget::leaveEvent(event);
}

void KNProgressSlider::paintEvent(QPaintEvent *event)
{
    //Ignore the original painting.
    Q_UNUSED(event);
    //Initial painter.
    QPainter painter(this);
    //Set antialiasing.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Using background opacity to draw the slider background.
    painter.setOpacity(m_backOpacity);
    //Configure the painter.
    painter.setPen(Qt::NoPen);
    painter.setBrush(RectColor);
    //Draw central rects.
    QPoint sliderTopLeft=knDpi->pos(GlowWidth,
                                    GlowWidth+Spacing);
    painter.drawRoundedRect(QRect(sliderTopLeft,
                                  QSize(width()-knDpi->width(GlowWidth<<1),
                                        knDpi->height(SliderHeight))),
                            knDpi->widthF(GlowWidth/2.0),
                            knDpi->heightF(GlowWidth/2.0));

    //Check the range, if the range is 0, ignore the following painting.
    if(range()==0)
    {
        return;
    }

    //Calculat the position left.
    int positionLeft;
    if(range()>0)
    {
        //Get the current position.
        positionLeft=percentage()*(qreal)(width()-m_buttonSize);
        if(positionLeft<0)
        {
            positionLeft=0;
        }
    }
    else
    {
        positionLeft=0;
    }

    //Restore the opacity.
    painter.setOpacity(1.0);
    //Change the brush color to button color.
    painter.setBrush(m_buttonColor);
    //Draw the value parts of the slider.
    painter.drawRoundedRect(QRect(sliderTopLeft,
                                  QSize(positionLeft+knDpi->width(Spacing),
                                        knDpi->height(SliderHeight))),
                            knDpi->widthF(GlowWidth/2.0),
                            knDpi->heightF(GlowWidth/2.0));

    //Translate the painter's coordinate position.
    painter.translate(positionLeft, knDpi->width(Spacing));
    //Calculate the gradient focal center.
    int buttonCenter=knDpi->width(GlowWidth+(SliderHeight>>1));
    QPointF centerFocalPoint=QPointF(buttonCenter, buttonCenter);
    //Configure the gradient.
    m_buttonGradient.setCenter(centerFocalPoint);
    m_buttonGradient.setRadius(buttonCenter);
    m_buttonGradient.setFocalPoint(centerFocalPoint);
    //Draw the button.
    painter.setBrush(m_buttonGradient);
    painter.drawEllipse(QRectF(0,0,height(),height()));
}

void KNProgressSlider::mousePressEvent(QMouseEvent *event)
{
    //Set pressed flag.
    m_pressed=true;
    //Change the current value.
    setValue(posToValue(event->pos().x()));
    //Emit pressed signal.
    emit sliderPressed();
    //Do original mouse press event.
    KNAbstractSlider::mousePressEvent(event);
}

void KNProgressSlider::mouseMoveEvent(QMouseEvent *event)
{
    //If the pressed flag is on, we have to change the value.
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
    //Do original release event.
    KNAbstractSlider::mouseReleaseEvent(event);
}

void KNProgressSlider::onMouseInOut(int frame)
{
    //Update the opacity.
    m_backOpacity=(qreal)frame/100;
    //Redraw the slider.
    update();
}

inline void KNProgressSlider::startAnime(int endFrame)
{
    //Stop all the animation.
    m_mouseInOut->stop();
    //Set the start frame of the time line.
    m_mouseInOut->setFrameRange(m_backOpacity*100, endFrame);
    //Start animation.
    m_mouseInOut->start();
}

inline void KNProgressSlider::updateButtonSize()
{
    //Calculate the button size: two light glow width and the slider height.
    m_buttonSize=knDpi->height((GlowWidth<<1)+SliderHeight);
    //Reset the widget height.
    setFixedHeight(m_buttonSize+knDpi->height(Spacing<<1));
    //Redraw the widget.
    update();
}

inline qint64 KNProgressSlider::posToValue(int position)
{
    //Check whether is the current position is out of range.
    int scaledGlowWidth=knDpi->width(GlowWidth);
    if(position<scaledGlowWidth)
    {
        return minimal();
    }
    if(position>width()-scaledGlowWidth)
    {
        return maximum();
    }
    //Calculate the value according to the range percentage.
    return minimal()+(qreal)range()/
            (qreal)(width()-(scaledGlowWidth<<1))*(qreal)position;
}
