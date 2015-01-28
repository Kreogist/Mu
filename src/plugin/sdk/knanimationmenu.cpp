/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef Q_OS_MACX
#include <QPropertyAnimation>
#endif
#include <QPainter>

#include "knanimationmenu.h"

KNAnimationMenu::KNAnimationMenu(QWidget *parent) :
    QMenu(parent)
{
    //Set properties.
    setWindowOpacity(0.85);
#ifndef Q_OS_MACX
    //Initial the show up animation.
    m_showAnimation=new QPropertyAnimation(this, "geometry", this);
    m_showAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_showAnimation->setDuration(200);
#endif
}

void KNAnimationMenu::showEvent(QShowEvent *event)
{
#ifndef Q_OS_MACX
    //Prepare the position data.
    //Begin the height at 1px, the only reason is I don't want to see the
    //"invalid window content view size" warning under Mac OS X.
    QRect finalPosition=geometry(),
          startPosition=QRect(finalPosition.x(),
                              m_mouseDownPos.y(),
                              finalPosition.width(),
                              1);
    //Set parameters.
    m_showAnimation->setStartValue(startPosition);
    m_showAnimation->setEndValue(finalPosition);
    //Start animation.
    m_showAnimation->start();
#endif
    //Show the widget.
    QMenu::showEvent(event);
}

void KNAnimationMenu::paintEvent(QPaintEvent *event)
{
    //Paint the menu items.
    QMenu::paintEvent(event);
    //Initial painter.
    QPainter painter(this);
    //Draw rects.
    painter.setPen(QColor(255,255,255,50));
    painter.drawRect(QRect(0,0,width()-1,height()-1));
}

void KNAnimationMenu::setMouseDownPos(const QPoint &mouseDownPos)
{
#ifdef Q_OS_MACX
    Q_UNUSED(mouseDownPos)
#endif
#ifndef Q_OS_MACX
    m_mouseDownPos=mouseDownPos;
#endif
}

void KNAnimationMenu::setSeparatorColor(const QColor &backgroundColor)
{
    //Set seperator style sheet, I can't solve this bug in coding way.
    setStyleSheet("QMenu::separator {height:1px;"
                  "background: rgba("+
                  QString::number(backgroundColor.red())+
                  ", "+
                  QString::number(backgroundColor.green())+
                  ", "+
                  QString::number(backgroundColor.blue())+
                  ", "+
                  QString::number(backgroundColor.alpha())+
                  ");"
                  "margin-left: 5px;"
                  "margin-right: 5px;}");
}
