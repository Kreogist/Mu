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
#ifndef KNMUSICBACKENDQTAVTHREAD_H
#define KNMUSICBACKENDQTAVTHREAD_H

#include <QtAV/QtAV.h>

#include "knmusicglobal.h"

#include "knmusicbackendthread.h"

using namespace QtAV;
using namespace KNMusic;

class KNMusicBackendQtAVThread : public KNMusicBackendThread
{
    Q_OBJECT
public:
    explicit KNMusicBackendQtAVThread(QObject *parent = 0);
    ~KNMusicBackendQtAVThread();
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

signals:

public slots:
    void setVolume(const int &volumeSize);
    void setPosition(const qint64 &position);

private slots:
    void onActionLoaded();
    void onActionStopped();
    void onActionPositionChanged(const qint64 &position);
    void setPlayerVolume();

private:
    AVPlayer *m_player;
    bool m_hasSection=false;
    qreal m_threadVolumeLevel=1.0;
    qint64 m_startPosition=0;   //Unit: millisecond
    qint64 m_endPosition=0;     //Unit: millisecond
    qint64 m_duration=0;        //Unit: millisecond
    qint64 m_totalDuration=0;   //Unit: millisecond
};

#endif // KNMUSICBACKENDQTAVTHREAD_H
