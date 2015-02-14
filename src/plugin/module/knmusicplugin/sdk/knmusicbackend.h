/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICBACKEND_H
#define KNMUSICBACKEND_H

//Include the music public functions.
#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNMusicBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 previewPosition READ previewPosition WRITE setPreviewPosition NOTIFY previewPositionChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume)
public:
    KNMusicBackend(QObject *parent = 0) : QObject(parent){}
    virtual bool available()=0;
    virtual bool loadMusic(const QString &filePath)=0;
    virtual qint64 duration() const=0;
    virtual qint64 position() const=0;
    virtual bool playFile(const QString &fileName)
    {
        //Play the whole file as a section.
        return playSection(fileName);
    }
    virtual bool playSection(const QString &fileName,
                             const qint64 &start=-1,
                             const qint64 &duration=-1)=0;
    virtual void play()=0;
    virtual void pause()=0;
    virtual void stop()=0;
    virtual void resetMainPlayer()=0;
    virtual int volume() const=0;

    virtual void loadPreview(const QString &filePath)=0;
    virtual qint64 previewDuration() const=0;
    virtual qint64 previewPosition() const=0;
    virtual void playPreviewFile(const QString &fileName)=0;
    virtual void setPreviewSection(const qint64 &start=-1,
                                   const qint64 &duration=-1)=0;
    virtual void playPreviewSection(const QString &fileName,
                                    const qint64 &start=-1,
                                    const qint64 &duration=-1)=0;
    virtual void playPreview()=0;
    virtual void pausePreview()=0;
    virtual void stopPreview()=0;
    virtual void resetPreviewPlayer()=0;

    virtual void loadUrl(const QString &url)=0;

    virtual int volumeMinimal()=0;
    virtual int volumeMaximum()=0;

signals:
    void cannotLoad();
    void loaded();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void finished();
    void stopped();
    void playingStateChanged(int state);

    void previewCannotLoad();
    void previewLoaded();
    void previewPositionChanged(qint64 position);
    void previewDurationChanged(qint64 duration);
    void previewFinished();
    void previewStopped();
    void previewPlayingStateChanged(int state);

    void muteStateChanged(bool mute);

public slots:
    virtual void changeMuteState()=0;
    virtual void setMute(const bool &mute)=0;
    virtual void setVolume(const int &volumeSize)=0;
    virtual void setPosition(const qint64 &position)=0;

    virtual void setPreviewPosition(const qint64 &position)=0;
};

#endif // KNMUSICBACKEND_H
