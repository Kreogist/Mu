/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QFileInfo>
#include <QMouseEvent>

#include "knglobal.h"

#include "knfilepathlabel.h"

KNFilePathLabel::KNFilePathLabel(QWidget *parent) :
    QLabel(parent)
{
}

void KNFilePathLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    //Set pressed flag.
    m_pressed=true;
}

void KNFilePathLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    //Check flag.
    if(m_pressed)
    {
        //Release flag.
        m_pressed=false;
        //Check position.
        if(rect().contains(event->pos()))
        {
            //Check the path you set first.
            QFileInfo contentCheck(m_filePath);
            //Check if it's exist not.
            if(contentCheck.exists())
            {
                switch(event->button())
                {
                case Qt::LeftButton:
                    //Emit showing signal.
                    emit aboutToShowInGraphicalShell();
                    //Show the file in the graphics shell.
                    KNGlobal::showInGraphicalShell(contentCheck.absoluteFilePath());
                    break;
                case Qt::RightButton:
                    break;
                default:
                    break;
                }
            }
        }
    }
}

QString KNFilePathLabel::filePath() const
{
    return m_filePath;
}

void KNFilePathLabel::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}
