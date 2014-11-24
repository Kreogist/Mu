/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICPLAYLISTPARSER_H
#define KNMUSICPLAYLISTPARSER_H

#include <QList>

#include "knmusicglobal.h"

#include <QObject>

class KNMusicPlaylistListItem;
class KNMusicPlaylistParser : public QObject
{
    Q_OBJECT
public:
    KNMusicPlaylistParser(QObject *parent = 0):QObject(parent){}
    virtual QString playlistType() const=0;
    virtual QString playlistSuffix() const=0;
    virtual bool parse(const QString &playlistFilePath,
                       KNMusicPlaylistListItem *playlistItem)=0;

signals:

public slots:

};

#endif // KNMUSICPLAYLISTPARSER_H
