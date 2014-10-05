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
    void requireShowMainPlayer();
    void requireShowAppendMenu();
    void finished();

public slots:
    virtual void restoreConfigure()=0;
    virtual void setBackend(KNMusicBackend *backend)=0;
    virtual void onActionLoopStateChanged(const int &state)=0;
    virtual void reset()=0;
    virtual void play()=0;
    virtual void playFile(const QString &filePath)=0;
    virtual void playSection(const QString &filePath,
                             const qint64 &startPosition,
                             const qint64 &duration)=0;

};

#endif // KNMUSICHEADERPLAYERBASE_H
