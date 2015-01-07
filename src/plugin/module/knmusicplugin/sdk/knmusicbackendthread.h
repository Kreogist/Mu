/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICBACKENDTHREAD_H
#define KNMUSICBACKENDTHREAD_H

#include <QObject>

class KNMusicBackendThread : public QObject
{
    Q_OBJECT
public:
    KNMusicBackendThread(QObject *parent = 0):QObject(parent){}
    virtual void loadFromFile(const QString &filePath)=0;
    virtual void clear()=0;
    virtual void resetState()=0;
    virtual void stop()=0;
    virtual void pause()=0;
    virtual void play()=0;
    virtual int volume()=0;
    virtual qint64 duration()=0;
    virtual qint64 position()=0;
    virtual void setPlaySection(const qint64 &sectionStart=-1,
                                const qint64 &sectionDuration=-1)=0;
    virtual void playSection(const qint64 &sectionStart=-1,
                             const qint64 &sectionDuration=-1)=0;

signals:
    void cannotLoadFile();
    void filePathChanged(QString filePath);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(int state);
    void finished();
    void stopped();
    void loaded();

public slots:
    virtual void setVolume(const int &volumeSize)=0;
    virtual void setPosition(const qint64 &position)=0;

};

#endif // KNMUSICBACKENDTHREAD_H
