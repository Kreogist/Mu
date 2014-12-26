/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICNOWPLAYINGBASE_H
#define KNMUSICNOWPLAYINGBASE_H

#include "knmusicbackend.h"

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNMusicTab;
class KNMusicModel;
class KNMusicProxyModel;
class KNMusicNowPlayingBase : public QObject
{
    Q_OBJECT
public:
    KNMusicNowPlayingBase(QObject *parent = 0):QObject(parent){}
    virtual void setBackend(KNMusicBackend *backend)=0;
    virtual KNMusicProxyModel *playingModel()=0;
    virtual KNMusicModel *playingMusicModel()=0;
    virtual int loopState()=0;
    virtual QPersistentModelIndex currentPlayingIndex() const=0;

signals:
    void requireResetPlayer();
    void requireUpdatePlayerInfo(KNMusicDetailInfo detailInfo);
    void loopStateChanged(int state);

public slots:
    virtual void showCurrentIndexInOriginalTab()=0;
    virtual void shadowPlayingModel()=0;
    virtual void resetCurrentPlaying()=0;
    virtual void restoreConfigure()=0;
    virtual void playNext()=0;
    virtual void playPrevious()=0;
    virtual void onActionPlayingFinished()=0;
    virtual void onActionCannotPlay()=0;
    virtual void setLoopState(const int &state)=0;
    virtual void changeLoopState()=0;
    virtual void playTemporaryFiles(const QStringList &filePaths)=0;
    virtual void setPlayingModel(KNMusicProxyModel *model,
                                 KNMusicTab *tab=nullptr)=0;
    virtual void playMusic(const int &row)=0;
    virtual void playMusic(const QModelIndex &index)=0;
    virtual void checkRemovedModel(KNMusicModel *model)=0;
};

#endif // KNMUSICNOWPLAYINGBASE_H
