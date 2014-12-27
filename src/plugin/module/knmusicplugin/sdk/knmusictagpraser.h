/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICTAGPRASER_H
#define KNMUSICTAGPRASER_H

#include <QFile>
#include <QDataStream>
#include <QImage>
#include <QHash>
#include <QMap>

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNMusicTagParser : public QObject
{
    Q_OBJECT
public:
    KNMusicTagParser(QObject *parent = 0):QObject(parent){}
    virtual bool praseTag(QFile &musicFile,
                          QDataStream &musicDataStream,
                          KNMusicDetailInfo &detailInfo)=0;
    virtual bool parseAlbumArt(KNMusicDetailInfo &detailInfo)=0;

signals:

public slots:

protected:
    inline void setTextData(QString &destination, const QString &source)
    {
        if(!source.isEmpty())
        {
            destination=source;
        }
    }
};

#endif // KNMUSICTAGPRASER_H
