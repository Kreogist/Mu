/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>

#include "knsideshadowwidget.h"

KNSideShadowWidget::KNSideShadowWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    //Initial the shadow gradient.
    m_shadow.setColorAt(0, QColor(0,0,0,150));
    m_shadow.setColorAt(1, QColor(0,0,0,0));
    //Set default direction.
    setDirection(TopShadow);
}

KNSideShadowWidget::KNSideShadowWidget(int direction, QWidget *parent) :
    KNSideShadowWidget(parent)
{
    setDirection(direction);
}

int KNSideShadowWidget::direction() const
{
    return m_direction;
}

void KNSideShadowWidget::setDirection(int shadowDirections)
{
    m_direction = shadowDirections;
    updateGradient();
}

void KNSideShadowWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Set gradient.
    painter.setBrush(m_shadow);
    painter.drawRect(QRect(-1,-1,width()+2,height()+2));
}

void KNSideShadowWidget::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QWidget::resizeEvent(event);
    //Update the gradient.
    updateGradient();
}

void KNSideShadowWidget::updateGradient()
{
    //Generate the start and stop position.
    switch(m_direction)
    {
    case TopShadow:
        m_shadow.setStart(0,0);
        m_shadow.setFinalStop(0,height());
        break;
    case BottomShadow:
        m_shadow.setStart(0,height());
        m_shadow.setFinalStop(0,0);
        break;
    case LeftShadow:
        m_shadow.setStart(0,0);
        m_shadow.setFinalStop(width(),0);
        break;
    case RightShadow:
        m_shadow.setStart(width(),0);
        m_shadow.setFinalStop(0,0);
        break;
    }
}

