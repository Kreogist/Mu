/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPen>
#include <QPainter>
#include <QPainterPath>
#include <QTimeLine>
#include <QMouseEvent>

#include "knanimeroundswitcher.h"

#include <QDebug>

KNAnimeRoundSwitcher::KNAnimeRoundSwitcher(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedSize(m_switcherWidth, m_switcherHeight);
    //Initial the background path.
    m_backgroundRect.moveTo(m_switcherHeight>>1, 0);
    m_backgroundRect.arcTo(0,
                           0,
                           m_switcherHeight,
                           m_switcherHeight,
                           90,
                           180);
    m_backgroundRect.lineTo(m_switcherWidth-m_switcherHeight,
                            m_switcherHeight);
    m_backgroundRect.arcTo(m_switcherWidth-m_switcherHeight,
                           0,
                           m_switcherHeight,
                           m_switcherHeight,
                           270,
                           180);
    m_backgroundRect.closeSubpath();
    //Initial the background brush.
    m_backgroundBrush.setStart(0,0);
    m_backgroundBrush.setFinalStop(0,m_switcherHeight);
    m_backgroundBrush.setColorAt(0, QColor(0x2a, 0x2a, 0x2a));
    m_backgroundBrush.setColorAt(1, QColor(0x67, 0x67, 0x67));

    //Initial the button border.
    m_buttonBorder.setColor(QColor(0,0,0,100));
    m_buttonBorder.setWidth(2);
    //Initial the button brush.
    m_buttonBrush.setStart(0,0);
    m_buttonBrush.setFinalStop(0,m_switcherHeight);
    m_buttonBrush.setColorAt(0, QColor(0xe8,0xe8,0xe8));
    m_buttonBrush.setColorAt(1, QColor(0xcd,0xcd,0xcd));

    //Initial the timeline.
    m_moveToLeft=new QTimeLine(100, this);
    m_moveToLeft->setEndFrame(0);
    configureTimeLine(m_moveToLeft);
    m_moveToRight=new QTimeLine(100, this);
    m_moveToRight->setEndFrame(m_switcherWidth-m_switcherHeight);
    configureTimeLine(m_moveToRight);
}

void KNAnimeRoundSwitcher::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Paint the background.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundBrush);
    painter.drawPath(m_backgroundRect);
    //Paint the button.
    painter.setPen(m_buttonBorder);
    painter.setBrush(m_buttonBrush);
    painter.drawEllipse(m_buttonX+1,1,m_switcherHeight-2,m_switcherHeight-2);
}

void KNAnimeRoundSwitcher::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    //Set flag.
    m_mousePressed=true;
}

void KNAnimeRoundSwitcher::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    //Check flag.
    if(m_mousePressed)
    {
        m_mousePressed=false;
        if(rect().contains(event->pos()))
        {
            setButtonAtLeft(!buttonAtLeft());
        }
    }
}

void KNAnimeRoundSwitcher::onActionMove(const int &frame)
{
    m_buttonX=frame;
    update();
}

void KNAnimeRoundSwitcher::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNAnimeRoundSwitcher::onActionMove);
}

void KNAnimeRoundSwitcher::launchAnime()
{
    //Stop all animation.
    m_moveToLeft->stop();
    m_moveToRight->stop();
    //Judge current status, move the button to the position.
    if(m_buttonAtLeft)
    {
        m_moveToLeft->setStartFrame(m_buttonX);
        m_moveToLeft->start();
    }
    else
    {
        m_moveToRight->setStartFrame(m_buttonX);
        m_moveToRight->start();
    }
}

bool KNAnimeRoundSwitcher::buttonAtLeft() const
{
    return m_buttonAtLeft;
}

void KNAnimeRoundSwitcher::setButtonAtLeft(bool buttonAtLeft)
{
    if(buttonAtLeft!=m_buttonAtLeft)
    {
        m_buttonAtLeft=buttonAtLeft;
        emit buttonPositionChange(m_buttonAtLeft);
        launchAnime();
    }
}
