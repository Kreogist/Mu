/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICLYRICSBASE_H
#define KNMUSICLYRICSBASE_H

#include "knmusicglobal.h"

#include <QWidget>

using namespace KNMusic;

class KNMusicHeaderPlayerBase;
class KNMusicLyricsBase : public QWidget
{
    Q_OBJECT
public:
    KNMusicLyricsBase(QWidget *parent = 0):QWidget(parent){}

signals:

public slots:
    virtual void onActionPositionChange(const qint64 &position)=0;
    virtual void onActionLyricsReset()=0;
};

#endif // KNMUSICLYRICSBASE_H
