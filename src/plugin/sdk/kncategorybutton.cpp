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
#include <QSizePolicy>
#include <QPainter>

#include "kncategorybutton.h"

#include <QDebug>

int KNCategoryButton::m_fixedHeight=30;

KNCategoryButton::KNCategoryButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(m_fixedHeight);
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::ToolButton));

    //Update the position parameters.
    updatePositionParameter();

    //Prepare the gradient background.
    m_background=QLinearGradient(0,0,0,height());
    m_background.setColorAt(0, m_gradientColor);
    m_background.setColorAt(1, m_gradientColor);

    //Initial the mouse animation.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(m_maxMouseInAlpha);
    configureInOutTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(0);
    configureInOutTimeLine(m_mouseOut);
    m_mouseUp=new QTimeLine(200, this);
    m_mouseUp->setEndFrame(m_maxMouseDownAlpha);
    configureMouseUpDownTimeLine(m_mouseUp);
    m_mouseDown=new QTimeLine(200, this);
    m_mouseDown->setEndFrame(200);
    configureMouseUpDownTimeLine(m_mouseDown);
}

QPixmap KNCategoryButton::icon() const
{
    return m_icon;
}

void KNCategoryButton::setIcon(const QPixmap &icon)
{
    m_icon = icon;
}

QString KNCategoryButton::text() const
{
    return m_text;
}

void KNCategoryButton::setText(const QString &text)
{
    m_text=text;
    m_textFullWidth=fontMetrics().width(m_text);
    update();
}

bool KNCategoryButton::checked() const
{
    return m_checked;
}

void KNCategoryButton::setChecked(bool checked)
{
    m_checked=checked;
    emit toggled(checked);
    onCheckedChanged();
}

int KNCategoryButton::buttonHeight()
{
    return m_fixedHeight;
}

void KNCategoryButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //Set antialiasing hints.
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Set pen.
    painter.setPen(Qt::NoPen);
    //Draw background.
    painter.setBrush(m_background);
    painter.drawRect(rect());
    //Draw top line
    painter.setBrush(m_highLightColor);
    painter.drawRect(0,0,width(),2);
    //Draw icon
    int iconX=(width()-m_iconSize-m_textWidth)/2;
    painter.drawPixmap(iconX,
                       m_iconY,
                       m_iconSize,
                       m_iconSize,
                       m_icon);
    //Draw text
    painter.setPen(QColor(255,255,255));
    painter.setOpacity(m_textAlpha);
    painter.drawText(iconX+m_iconSize+m_spacing,
                     m_textY,
                     m_textWidth+m_spacing,
                     m_iconSize,
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_text);
}

void KNCategoryButton::enterEvent(QEvent *event)
{
    //Ensure this button is not selected, execute enter animation.
    if(!m_checked)
    {
        startMouseInAnime();
    }
    KNAbstractButton::enterEvent(event);
}

void KNCategoryButton::leaveEvent(QEvent *event)
{
    //Ensure this button is not selected, execute leave animation.
    if(!m_checked)
    {
        startMouseOutAnime();
    }
    KNAbstractButton::leaveEvent(event);
}

void KNCategoryButton::mousePressEvent(QMouseEvent *event)
{
    //Check the button.
    if(event->button()==Qt::LeftButton)
    {
        //Ensure that mouse is not selected,
        //Set button selected, execute animation.
        if(!m_checked)
        {
            //This button cannot be pressed before, set the mouse down to true.
            m_pressed=true;
            startMouseDownAnime();
        }
    }
    KNAbstractButton::mousePressEvent(event);
}

void KNCategoryButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_checked)
    {
        m_checked=true;
        startMouseDownAnime();
    }
    KNAbstractButton::mouseReleaseEvent(event);
}

void KNCategoryButton::onCheckedChanged()
{
    if(m_checked)
    {
        //Set the parameter to the end frame of mouse in.
        mouseInOutAnimeFrame(m_maxMouseInAlpha);
        //Start mouse down animation.
        startMouseDownAnime();
    }
    else
    {
        //Set the parameter to the start frame of mouse out.
        mouseInOutAnimeFrame(m_maxMouseInAlpha);
        //Start mouse out animation.
        startMouseOutAnime();
    }
}

void KNCategoryButton::mouseInOutAnimeFrame(const int &frame)
{
    Q_ASSERT(frame>-1 && frame<=m_maxMouseInAlpha);
    //Change the light color.
    m_highLightColor.setAlpha(frame);
    //Change the background gradient color.
    m_gradientColor.setAlpha(frame>>1);
    m_background.setColorAt(0, m_gradientColor);

    //Change the text alpha.
    m_textAlpha=(qreal)frame/(qreal)m_maxMouseInAlpha;
    //And the text display width.
    m_textWidth=m_textAlpha*(qreal)(m_textFullWidth+m_spacing);
    //Paint.
    update();
}

void KNCategoryButton::mouseUpDownAnimeFrame(const int &frame)
{
    //Change the light color.
    m_highLightColor.setAlpha(frame);
    //Change the background gradient color.
    m_gradientColor.setAlpha(frame>>1);
    m_background.setColorAt(0, m_gradientColor);
    //Paint.
    update();
}

void KNCategoryButton::startMouseInAnime()
{
    //Stop all IO animation.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameters.
    if(m_mouseIn->state()!=QTimeLine::Running)
    {
        m_mouseIn->setStartFrame(m_highLightColor.alpha());
        m_mouseIn->start();
    }
}

void KNCategoryButton::startMouseOutAnime()
{
    //Stop all IO animation.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameters.
    if(m_mouseOut->state()!=QTimeLine::Running)
    {
        m_mouseOut->setStartFrame(m_highLightColor.alpha());
        m_mouseOut->start();
    }
}

void KNCategoryButton::startMouseDownAnime()
{
    //Stop all UD animation.
    m_mouseUp->stop();
    m_mouseDown->stop();
    //Set mouse down parameter.
    m_mouseDown->setStartFrame(m_highLightColor.alpha());

    m_mouseDown->start();
}

void KNCategoryButton::startMouseUpAnime()
{
    //Stop all UD animation.
    m_mouseDown->stop();
    m_mouseUp->stop();
    //Set mouse up parameter.
    m_mouseUp->setStartFrame(m_highLightColor.alpha());

    m_mouseUp->start();
}

void KNCategoryButton::updatePositionParameter()
{
    m_iconCenterX=(width()-m_iconSize)>>1;
}

void KNCategoryButton::configureInOutTimeLine(QTimeLine *timeline)
{
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(5);
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNCategoryButton::mouseInOutAnimeFrame);
}

void KNCategoryButton::configureMouseUpDownTimeLine(QTimeLine *timeline)
{
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(5);
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNCategoryButton::mouseUpDownAnimeFrame);
}
