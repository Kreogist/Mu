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
#ifndef KNMUSICBACKENDBASSTHREAD_H
#define KNMUSICBACKENDBASSTHREAD_H

#include "bass.h"

#include "knmusicglobal.h"

#include "knmusicbackendthread.h"

using namespace KNMusic;

class KNMusicBackendBassThread : public KNMusicBackendThread
{
    Q_OBJECT
public:
    explicit KNMusicBackendBassThread(QObject *parent = 0);
    ~KNMusicBackendBassThread();
    void loadFromFile(const QString &filePath);
    void clear();
    void resetState();
    void stop();
    void pause();
    void play();
    int volume();
    qint64 duration();
    qint64 position();
    void setPlaySection(const qint64 &sectionStart=-1,
                        const qint64 &sectionDuration=-1);
    void playSection(const qint64 &sectionStart=-1,
                     const qint64 &sectionDuration=-1);

    bool stoppedState() const;
    void setStoppedState(bool stoppedState);
    void requireDoStopped();

signals:
    void requireStopped();

public slots:
    void setVolume(const int &volumeSize);
    void setPosition(const qint64 &position);

private slots:
    void onActionPositionCheck();

private:
    static void CALLBACK onActionEnd(HSYNC handle,
                                     DWORD channel,
                                     DWORD data,
                                     void *user);
    void establishSyncHandle();
    void releaseSyncHandle();
    void setState(const int &state);
    int m_playingState=StoppedState;
    QString m_filePath;
    bool m_stoppedState=true;
    qint64 m_startPosition;   //Unit: millisecond
    qint64 m_endPosition;     //Unit: millisecond
    qint64 m_duration;        //Unit: millisecond
    qint64 m_totalDuration;   //Unit: millisecond
    QTimer *m_positionUpdater=nullptr;
    QList<HSYNC> m_syncHandles;
    DWORD m_channel;
};

#endif // KNMUSICBACKENDBASSTHREAD_H
