/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICHEADERPLAYERBASE_H
#define KNMUSICHEADERPLAYERBASE_H

#include <QWidget>

class KNMusicBackend;
class KNMusicHeaderPlayerBase : public QWidget
{
    Q_OBJECT
public:
    KNMusicHeaderPlayerBase(QWidget *parent = 0):QWidget(parent){}

signals:
    void requirePlayPrevious();
    void requirePlayNext();
    void requireChangeLoopState();

public slots:
    virtual void setBackend(KNMusicBackend *backend)=0;

};

#endif // KNMUSICHEADERPLAYERBASE_H
