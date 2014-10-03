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

signals:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(int state);
    void finished();
    void stopped();

public slots:

};

#endif // KNMUSICBACKENDTHREAD_H
