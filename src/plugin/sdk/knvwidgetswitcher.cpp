/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPropertyAnimation>

#include "knvwidgetswitcher.h"

KNVWidgetSwitcher::KNVWidgetSwitcher(QWidget *parent) :
    KNWidgetSwitcher(parent)
{
    ;
}

void KNVWidgetSwitcher::updateAnimationPosition()
{
    //Get the three position: left, center, right.
    QRect upPosition=QRect(0,
                           -height(),
                           width(),
                           height()),
          centerPosition=QRect(0,
                               0,
                               width(),
                               height()),
          downPosition=QRect(0,
                            height(),
                            width(),
                            height());
    m_inAnimation->setEndValue(centerPosition);
    m_outAnimation->setStartValue(centerPosition);
    //Judge moving position.
    if(m_currentIndex>m_outWidgetIndex)
    {
        //Moving left.
        m_outAnimation->setEndValue(upPosition);
        m_inAnimation->setStartValue(downPosition);
    }
    else
    {
        //Moving right.
        m_outAnimation->setEndValue(downPosition);
        m_inAnimation->setStartValue(upPosition);
    }
}
