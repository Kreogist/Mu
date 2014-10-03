/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICLISTPARSER_H
#define KNMUSICLISTPARSER_H

#include <QFile>

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNMusicListParser : public QObject
{
    Q_OBJECT
public:
    KNMusicListParser(QObject *parent = 0):QObject(parent){}
    virtual bool parseList(QFile &listFile,
                           KNMusicListDetailInfo &listDetailInfo)=0;

signals:

public slots:

};

#endif // KNMUSICLISTPARSER_H
