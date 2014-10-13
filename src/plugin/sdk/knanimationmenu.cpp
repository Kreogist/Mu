/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPropertyAnimation>
#include <QShowEvent>

#include "knanimationmenu.h"

KNAnimationMenu::KNAnimationMenu(QWidget *parent) :
    QMenu(parent)
{
    //Initial the show up animation.
    m_showAnimation=new QPropertyAnimation(this, "geometry", this);
    m_showAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_showAnimation->setDuration(200);
}

void KNAnimationMenu::showEvent(QShowEvent *event)
{
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
    //Show the widget.
    QMenu::showEvent(event);
}

void KNAnimationMenu::setMouseDownPos(const QPoint &mouseDownPos)
{
    m_mouseDownPos=mouseDownPos;
}
