/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QCursor>

#include "knmousedetectheader.h"

KNMouseDetectHeader::KNMouseDetectHeader(QWidget *parent) :
    QWidget(parent)
{
}

void KNMouseDetectHeader::checkCurrentCursorPos()
{
    //Check the current pos of the cursor.
    if(rect().contains(mapFromGlobal(QCursor::pos())))
    {
        //Emit activate widget signal.
        emit requireActivateWidget();
    }
    else
    {
        //Emit inactivate widget signal.
        emit requireInactivateWidget();
    }
}

void KNMouseDetectHeader::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    //Emit activate signal.
    emit requireActivateWidget();
}

void KNMouseDetectHeader::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    //Emit inactivate signal.
    emit requireInactivateWidget();
}
