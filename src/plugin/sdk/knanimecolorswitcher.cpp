/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QTimeLine>
#include <QMouseEvent>
#include <QPainter>

#include "knanimecolorswitcher.h"

#include <QDebug>

KNAnimeColorSwitcher::KNAnimeColorSwitcher(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(m_switcherWidth, m_switcherHeight);

    //Initial blank gradient.
    m_blankGradient.setStart(0,0);
    m_blankGradient.setFinalStop(0, m_switcherHeight);
    m_blankGradient.setColorAt(0, QColor(0x7d, 0x7d, 0x7d));
    m_blankGradient.setColorAt(0.1, QColor(0xdf, 0xdf, 0xdf));
    m_blankGradient.setColorAt(0.49, QColor(0xef, 0xef, 0xef));
    m_blankGradient.setColorAt(0.5, QColor(0xf9, 0xf9, 0xf9));
    m_blankGradient.setColorAt(1, QColor(0xff, 0xff, 0xff));
    //Initial button gradient.
    m_buttonGradient.setStart(0,0);
    m_buttonGradient.setFinalStop(0, m_switcherHeight);
    setButtonColor(QColor(0xcc, 0xcc, 0xcc));
    //Initial highlight gradient.
    m_highlightGradient.setStart(0,0);
    m_highlightGradient.setFinalStop(0, m_switcherHeight);
    setHighLightColor(QColor(0xf7, 0xcf, 0x3d));

    //Initial the timeline.
    m_moveLeft=new QTimeLine(100, this);
    m_moveLeft->setEndFrame(0);
    configureTimeLine(m_moveLeft);
    m_moveRight=new QTimeLine(100, this);
    m_moveRight->setEndFrame(m_switcherWidth-m_buttonWidth);
    configureTimeLine(m_moveRight);
}

void KNAnimeColorSwitcher::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    //Set mouse down flag.
    m_pressed=true;
}

void KNAnimeColorSwitcher::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    //Check the flag.
    if(m_pressed)
    {
        m_pressed=false;
        if(rect().contains(event->pos()))
        {
            setValue(!value());
        }
    }
}

void KNAnimeColorSwitcher::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);

    QPen borderPen(QColor(0xc0, 0xc0, 0xc0));
    borderPen.setWidthF(0.5);

    //Draw the background.
    painter.setPen(QColor(0xc0, 0xc0, 0xc0));
    painter.setBrush(m_blankGradient);
    painter.drawRoundedRect(rect(), 4, 4);

    //Draw highlight.
    borderPen.setColor(m_buttonBorder);
    painter.setPen(borderPen);
    painter.setBrush(m_highlightGradient);
    painter.drawRoundedRect(0,0,m_buttonX+(m_buttonWidth>>1),m_switcherHeight,3,3);

    //Draw the button.
    painter.setBrush(m_buttonGradient);
    painter.drawRoundedRect(m_buttonX,0,m_buttonWidth,m_switcherHeight,4,4);
}

void KNAnimeColorSwitcher::onActionUpdateParameter(const int &frame)
{
    m_buttonX=frame;
    update();
}

void KNAnimeColorSwitcher::moveButtonLeft()
{
    //Stop all animations.
    m_moveLeft->stop();
    m_moveRight->stop();
    //Set parameters.
    m_moveLeft->setStartFrame(m_buttonX);
    //Start animation.
    m_moveLeft->start();
}

void KNAnimeColorSwitcher::moveButtonRight()
{
    //Stop all animations.
    m_moveLeft->stop();
    m_moveRight->stop();
    //Set parameters.
    m_moveRight->setStartFrame(m_buttonX);
    //Start animation.
    m_moveRight->start();
}

void KNAnimeColorSwitcher::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(2);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNAnimeColorSwitcher::onActionUpdateParameter);
}

void KNAnimeColorSwitcher::setHighLightColor(const QColor &highLightColor)
{
    QColor highlightColorDummy;
    highlightColorDummy.setHsvF(highLightColor.hueF(),
                                0.93,
                                0.54);
    m_highlightGradient.setColorAt(0, highlightColorDummy);
    highlightColorDummy.setHsvF(highLightColor.hueF(),
                               0.76,
                               0.77);
    m_highlightGradient.setColorAt(0.05, highlightColorDummy);
    highlightColorDummy.setHsvF(highLightColor.hueF(),
                               0.73,
                               0.93);
    m_highlightGradient.setColorAt(0.49, highlightColorDummy);
    highlightColorDummy.setHsvF(highLightColor.hueF(),
                               0.68,
                               0.94);
    m_highlightGradient.setColorAt(0.5, highlightColorDummy);
    highlightColorDummy.setHsvF(highLightColor.hueF(),
                               0.53,
                               0.99);
    m_highlightGradient.setColorAt(0.99, highlightColorDummy);
    highlightColorDummy.setHsvF(highLightColor.hueF(),
                               0.62,
                               0.85);
    m_highlightGradient.setColorAt(1, highlightColorDummy);
}

void KNAnimeColorSwitcher::setButtonColor(const QColor &buttonColor)
{
    QColor buttonDummyColor;
    //Set button border.
    buttonDummyColor.setHsvF(buttonColor.hueF(),
                             0,
                             0);
    m_buttonBorder=buttonDummyColor;
    //Set button background gradient.
    buttonDummyColor.setHsvF(buttonColor.hueF(),
                             0.38,
                             buttonColor.valueF());
    m_buttonGradient.setColorAt(0, buttonDummyColor);
    buttonDummyColor.setHsvF(buttonColor.hueF(),
                             0.27,
                             buttonColor.valueF());
    m_buttonGradient.setColorAt(1, buttonDummyColor);
}


bool KNAnimeColorSwitcher::value() const
{
    return m_value;
}

void KNAnimeColorSwitcher::setValue(bool value)
{
    //Check value is the same.
    if(m_value==value)
    {
        return;
    }
    //Save value.
    m_value=value;
    emit valueChanged(m_value);
    //Show animation.
    if(m_value)
    {
        moveButtonRight();
    }
    else
    {
        moveButtonLeft();
    }
}

