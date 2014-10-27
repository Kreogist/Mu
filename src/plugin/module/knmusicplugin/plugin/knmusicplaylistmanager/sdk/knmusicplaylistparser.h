/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICPLAYLISTPARSER_H
#define KNMUSICPLAYLISTPARSER_H

#include <QStringList>

#include <QObject>

class KNMusicPlaylistParser : public QObject
{
    Q_OBJECT
public:
    KNMusicPlaylistParser(QObject *parent = 0):QObject(parent){}
    virtual bool parse(const QString &playlistFilePath,
                       QStringList &data)=0;

signals:

public slots:

};

#endif // KNMUSICPLAYLISTPARSER_H
