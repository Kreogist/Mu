/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>
#include <QTimeLine>
#include <QMouseEvent>

#include "knroundanimebutton.h"

#include <QDebug>

KNRoundAnimeButton::KNRoundAnimeButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set properties.
    setFixedSize(m_buttonSize, m_buttonSize);
    //Initial the timeline.
    m_toSelect=generateTimeLine(255);
    m_toUnselect=generateTimeLine(m_minimumLightness);
}

KNRoundAnimeButton::~KNRoundAnimeButton()
{

}

void KNRoundAnimeButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set the painter hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the circle.
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(m_lightness,m_lightness,m_lightness));
    painter.drawEllipse(rect());
    //Paint the icon.
    painter.drawPixmap(m_iconPos,m_iconPos,m_iconSize,m_iconSize,m_icon);
}

void KNRoundAnimeButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        //Set mouse down flags.
        m_isPressed=true;
    }
    KNAbstractButton::mousePressEvent(event);
}

void KNRoundAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Ensure this button has been mouse down.
    if(m_isPressed &&
            event->button()==Qt::LeftButton &&
            rect().contains(event->pos()))
    {
        //Accept event.
        event->accept();
        //Check if the button has been checked.
        if(!m_checked)
        {
            //Stop all the animations.
            stopAllAnimations();
            //Start animations to selected.
            startAnimation(m_toSelect);
            //Emit toggled.
            emit toggled(true);
        }
    }
    //Release flag.
    m_isPressed=false;
    KNAbstractButton::mouseReleaseEvent(event);
}

void KNRoundAnimeButton::onActionLightnessChanged(const int &frame)
{
    m_lightness=frame;
    //Update the button.
    update();
}

void KNRoundAnimeButton::stopAllAnimations()
{
    m_toSelect->stop();
    m_toUnselect->stop();
}

inline void KNRoundAnimeButton::startAnimation(QTimeLine *timeLine)
{
    timeLine->setStartFrame(m_lightness);
    timeLine->start();
}

QTimeLine *KNRoundAnimeButton::generateTimeLine(const int &finalFrame)
{
    QTimeLine *timeLine=new QTimeLine(finalFrame, this);
    timeLine->setUpdateInterval(16);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setEndFrame(finalFrame);
    //Linke the time line.
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNRoundAnimeButton::onActionLightnessChanged);
    return timeLine;
}

bool KNRoundAnimeButton::checked() const
{
    return m_checked;
}

void KNRoundAnimeButton::setChecked(bool checked)
{
    //Check checked state first.
    if(checked==m_checked)
    {
        return;
    }
    //Save the checked state.
    m_checked=checked;
    //Launch the animation.
    stopAllAnimations();
    if(m_checked)
    {
        startAnimation(m_toSelect);
        return;
    }
    startAnimation(m_toUnselect);
}
