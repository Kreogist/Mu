/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmousedetectlabel.h"

KNMouseDetectLabel::KNMouseDetectLabel(QWidget *parent) :
    QLabel(parent)
{
    setFocusPolicy(Qt::WheelFocus);
}

KNMouseDetectLabel::~KNMouseDetectLabel()
{

}

void KNMouseDetectLabel::enterEvent(QEvent *event)
{
    //Do the enter event.
    QLabel::enterEvent(event);
    emit mouseEntered();
}

void KNMouseDetectLabel::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    emit mouseLeaved();
}

