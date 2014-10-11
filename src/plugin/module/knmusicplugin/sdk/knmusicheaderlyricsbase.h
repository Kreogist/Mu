/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICHEADERLYRICSBASE_H
#define KNMUSICHEADERLYRICSBASE_H

#include <QWidget>

class KNMusicHeaderLyricsBase : public QWidget
{
    Q_OBJECT
public:
    KNMusicHeaderLyricsBase(QWidget *parent = 0):QWidget(parent){}

signals:

public slots:
    virtual void loadLyricsForMusic(const QString &filePath)=0;

};

#endif // KNMUSICHEADERLYRICSBASE_H
