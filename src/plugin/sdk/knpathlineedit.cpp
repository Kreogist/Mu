/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QFileInfo>

#include "knconnectionhandler.h"

#include "knpathlineedit.h"

KNPathLineEdit::KNPathLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    //Initial the check handle.
    m_checkHandle=new KNConnectionHandler(this);
    //Enable changed check handle at default.
    enableChangedCheck();
}

void KNPathLineEdit::focusInEvent(QFocusEvent *event)
{
    //Do line edit's focus out.
    QLineEdit::focusInEvent(event);
    //Disable the check;
    disableChangedCheck();
    //Emit start editing signal.
    emit startEditPath();
}

void KNPathLineEdit::focusOutEvent(QFocusEvent *event)
{
    //Do line edit's focus out.
    QLineEdit::focusOutEvent(event);
    //Check the path.
    checkCurrentPath();
    //Enable the check.
    enableChangedCheck();
}

void KNPathLineEdit::disableChangedCheck()
{
    //This is simple, just disconnect all :)
    m_checkHandle->disConnectAll();
}

void KNPathLineEdit::enableChangedCheck()
{
    //Check is handle empty, if not, means we have connect before,
    //ignore request.
    if(m_checkHandle->isEmpty())
    {
        m_checkHandle->addConnectionHandle(
                    connect(this, SIGNAL(textChanged(QString)),
                            this, SLOT(checkCurrentPath())));
    }
}

void KNPathLineEdit::checkCurrentPath()
{
    QFileInfo pathChecker(text());
    if(pathChecker.exists())
    {
        emit pathExist();
    }
    else
    {
        emit pathNotExist();
    }
}

