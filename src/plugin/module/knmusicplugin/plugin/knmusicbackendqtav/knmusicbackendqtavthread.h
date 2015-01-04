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

#include "knmusicbackendthread.h"

using namespace QtAV;

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
    void onActionPositionChanged(const qint64 &position);

private:
    AVPlayer *m_player;
    qint64 m_startPosition;   //Unit: millisecond
    qint64 m_endPosition;     //Unit: millisecond
    qint64 m_duration;        //Unit: millisecond
    qint64 m_totalDuration;   //Unit: millisecond
};

#endif // KNMUSICBACKENDQTAVTHREAD_H
