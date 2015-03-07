/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICMAINPLAYERBASE_H
#define KNMUSICMAINPLAYERBASE_H

#include "knmusicglobal.h"

#include "kndropproxycontainer.h"

class KNMusicBackend;
class KNMusicHeaderPlayerBase;
class KNMusicMainPlayerBase : public KNDropProxyContainer
{
    Q_OBJECT
public:
    KNMusicMainPlayerBase(QWidget *parent = 0):KNDropProxyContainer(parent){}
    virtual QWidget *banner()=0;
    virtual void setBackend(KNMusicBackend *backend)=0;
    virtual void setHeaderPlayer(KNMusicHeaderPlayerBase *headerPlayer)=0;

signals:
    void requireHideMainPlayer();

public slots:
    virtual void onActionAnalysisItemUpdate()=0;
};

#endif // KNMUSICMAINPLAYERBASE_H
