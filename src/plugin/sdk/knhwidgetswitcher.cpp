/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPropertyAnimation>

#include "knhwidgetswitcher.h"

KNHWidgetSwitcher::KNHWidgetSwitcher(QWidget *parent) :
    KNWidgetSwitcher(parent)
{
    ;
}

void KNHWidgetSwitcher::updateAnimationPosition()
{
    //Get the three position: left, center, right.
    QRect leftPosition=QRect(-width(),
                             0,
                             width(),
                             height()),
          centerPosition=QRect(0,
                               0,
                               width(),
                               height()),
          rightPosition=QRect(0,
                              0,
                              width(),
                              height());
    m_inAnimation->setEndValue(centerPosition);
    m_outAnimation->setStartValue(centerPosition);
    //Judge moving position.
    if(m_currentIndex>m_outWidgetIndex)
    {
        //Moving left.
        m_outAnimation->setEndValue(leftPosition);
        m_inAnimation->setStartValue(rightPosition);
    }
    else
    {
        //Moving right.
        m_outAnimation->setEndValue(rightPosition);
        m_inAnimation->setStartValue(leftPosition);
    }
}
