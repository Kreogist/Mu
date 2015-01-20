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
#ifndef KNMUSICBACKENDPHONONTHREAD_H
#define KNMUSICBACKENDPHONONTHREAD_H

#include <phonon4qt5/phonon/MediaObject>
#include <phonon4qt5/phonon/AudioOutput>
#include <phonon4qt5/phonon/MediaSource>

#include "knmusicglobal.h"

#include "knmusicbackendthread.h"

using namespace Phonon;

class KNMusicBackendPhononThread : public KNMusicBackendThread
{
    Q_OBJECT
public:
    explicit KNMusicBackendPhononThread(QObject *parent = 0);
    ~KNMusicBackendPhononThread();
    bool loadFromFile(const QString &filePath);
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
    void loaded();

public slots:
    void setVolume(const int &volumeSize);
    void setPosition(const qint64 &position);

private slots:
    void onActionStateChanged(const State &newstate,
                              const State &oldstate);
    void onActionDurationChanged(const qint64 &newTotalTime);
    void onActionPositionChanged(const qint64 &time);

private:
    MediaSource m_mediaSource;
    MediaObject *m_mediaObject;
    AudioOutput *m_audioOutput;

    int m_state;
    bool m_ticking=false;
    qreal m_userSetVolume=1.0;
};

#endif // KNMUSICBACKENDPHONONTHREAD_H
