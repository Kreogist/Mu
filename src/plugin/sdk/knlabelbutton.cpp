/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QMouseEvent>

#include "knlabelbutton.h"

KNLabelButton::KNLabelButton(QWidget *parent) :
    QLabel(parent)
{

}

KNLabelButton::~KNLabelButton()
{

}

void KNLabelButton::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    //Set pressed flag.
    m_pressed=true;
}

void KNLabelButton::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    //Check the flag.
    if(m_pressed)
    {
        //Check the mouse position.
        if(rect().contains(event->pos()))
        {
            emit clicked();
        }
    }
    //Release the flag.
    m_pressed=false;
}

