/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICHEADERPLAYERBASE_H
#define KNMUSICHEADERPLAYERBASE_H

#include "kndropproxycontainer.h"

#include "knmusicglobal.h"

using namespace KNMusic;

class KNMusicBackend;
class KNMusicNowPlayingBase;
class KNMusicHeaderPlayerBase : public KNDropProxyContainer
{
    Q_OBJECT
public:
    KNMusicHeaderPlayerBase(QWidget *parent = 0):KNDropProxyContainer(parent){}
    virtual void setBackend(KNMusicBackend *backend)=0;
    virtual void setNowPlaying(KNMusicNowPlayingBase *nowPlaying)=0;

signals:
    void requirePlayPrevious();
    void requirePlayNext();
    void requireChangeLoopState();
    void requireShowMainPlayer();
    void requireShowAppendMenu();
    void requireLoadLyrics(QString musicFilePath);
    void playerReset();
    void finished();
    void positionChanged(qint64 position);

public slots:
    virtual void restoreConfigure()=0;
    virtual void onActionLoopStateChanged(const int &state)=0;
    virtual void reset()=0;
    virtual void play()=0;

};

#endif // KNMUSICHEADERPLAYERBASE_H
