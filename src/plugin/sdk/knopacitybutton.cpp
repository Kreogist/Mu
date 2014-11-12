/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QMouseEvent>
#include <QSizePolicy>
#include <QPainter>
#include <QGraphicsOpacityEffect>

#include "knopacitybutton.h"

KNOpacityButton::KNOpacityButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set as strong focus.
    setFocusPolicy(Qt::StrongFocus);
    //Set toolbutton size policy.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::ToolButton));
    //Initial opacity effect.
    m_opacityEffect=new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(m_originalOpacity);
    setGraphicsEffect(m_opacityEffect);
}

QPixmap KNOpacityButton::icon() const
{
    return m_icon;
}

void KNOpacityButton::setIcon(const QPixmap &icon)
{
    m_icon=icon;
    update();
}

void KNOpacityButton::mousePressEvent(QMouseEvent *event)
{
    KNAbstractButton::mousePressEvent(event);
    //Set opacity.
    m_originalOpacity=m_opacityEffect->opacity();
    qreal preferOpacity=m_originalOpacity-m_opacityGap;
    m_opacityEffect->setOpacity(preferOpacity<0?
                                    m_originalOpacity+m_opacityGap:
                                    preferOpacity);
    //Set flag.
    m_pressed=true;
}

void KNOpacityButton::mouseReleaseEvent(QMouseEvent *event)
{
    KNAbstractButton::mouseReleaseEvent(event);
    //Recover opacity.
    m_opacityEffect->setOpacity(m_originalOpacity);
    //Judge is pressed?
    if(m_pressed)
    {
        m_pressed=false;
        //Judge is mouse in.
        if(rect().contains(event->pos()))
        {
            emit clicked();
        }
    }
}

void KNOpacityButton::paintEvent(QPaintEvent *event)
{
    KNAbstractButton::paintEvent(event);
    //Initial antialiasing painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Paint the contents.
    painter.drawPixmap(0,0,width(),height(),
                       m_icon.scaled(width(),
                                     height(),
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
}

qreal KNOpacityButton::opacity() const
{
    return m_opacityEffect->opacity();
}

void KNOpacityButton::setOpacity(const qreal &opacity)
{
    m_opacityEffect->setOpacity(opacity);
}

qreal KNOpacityButton::opacityGap() const
{
    return m_opacityGap;
}

void KNOpacityButton::setOpacityGap(const qreal &pressedOpacity)
{
    m_opacityGap=pressedOpacity;
}

