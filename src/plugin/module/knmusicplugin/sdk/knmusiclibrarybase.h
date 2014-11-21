/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICLIBRARYBASE_H
#define KNMUSICLIBRARYBASE_H

#include <QObject>

class KNMusicTab;
class KNMusicLibraryBase : public QObject
{
    Q_OBJECT
public:
    KNMusicLibraryBase(QObject *parent = 0):QObject(parent){}
    virtual KNMusicTab *songTab()=0;
    virtual KNMusicTab *artistTab()=0;
    virtual KNMusicTab *albumTab()=0;
    virtual KNMusicTab *genreTab()=0;

signals:
    void requireShowTab();

public slots:

};

#endif // KNMUSICLIBRARYBASE_H
