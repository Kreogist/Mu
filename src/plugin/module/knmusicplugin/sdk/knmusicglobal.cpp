/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicglobal.h"

KNMusicGlobal *KNMusicGlobal::m_instance=nullptr;

KNMusicGlobal *KNMusicGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicGlobal:m_instance;
}

QString KNMusicGlobal::msecondToTime(const qint64 &msecond)
{
    qint64 second=msecond/1000;
    //Calculate the second part.
    QString secondText=QString::number(second%60);
    //If second if 0-9, add a 0 before the number.
    return secondText.length()==1?
                QString::number(second/60)+":0"+secondText:
                QString::number(second/60)+":"+secondText;
}

KNMusicGlobal::KNMusicGlobal(QObject *parent) :
    QObject(parent)
{
}
