/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTANDARDBACKENDTHREAD_H
#define KNMUSICSTANDARDBACKENDTHREAD_H

#include <QObject>

class KNMusicStandardBackendThread : public QObject
{
    Q_OBJECT
public:
    KNMusicStandardBackendThread(QObject *parent = 0):QObject(parent){}

    virtual bool loadFile(const QString &filePath)=0;
    virtual void reset()=0;
    virtual void stop()=0;
    virtual void play()=0;
    virtual void pause()=0;
    virtual int volume()=0;
    virtual qint64 duration()=0;
    virtual qint64 position()=0;
    virtual void setPlaySection(const qint64 &start=-1,
                                const qint64 &duration=-1)=0;

signals:
    void loadFailed();
    void loadSuccess();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void stateChanged(int state);
    void finished();
    void stopped();

public slots:
    virtual void setVolume(const int &volume)=0;
    virtual void setPosition(const qint64 &position)=0;
};

#endif // KNMUSICSTANDARDBACKENDTHREAD_H
