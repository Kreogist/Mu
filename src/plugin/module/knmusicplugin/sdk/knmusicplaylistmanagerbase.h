/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICPLAYLISTMANAGERBASE_H
#define KNMUSICPLAYLISTMANAGERBASE_H

#include <QObject>

class KNMusicTab;
class KNMusicPlaylistManagerBase : public QObject
{
    Q_OBJECT
public:
    KNMusicPlaylistManagerBase(QObject *parent = 0):QObject(parent){}
    virtual KNMusicTab *categoryTab()=0;

signals:

public slots:

};

#endif // KNMUSICPLAYLISTMANAGERBASE_H
