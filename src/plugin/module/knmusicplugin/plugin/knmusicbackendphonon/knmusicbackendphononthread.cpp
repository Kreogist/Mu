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
#include "knmusicbackendphononthread.h"

KNMusicBackendPhononThread::KNMusicBackendPhononThread(QObject *parent) :
    KNMusicBackendThread(parent)
{
    //Initial the media object and audio output.
    m_mediaObject=new MediaObject(this);
    m_audioOutput=new AudioOutput(MusicCategory,
                                  this);

    //Linke the media object and audio output.
    createPath(m_mediaObject, m_audioOutput);
}

KNMusicBackendPhononThread::~KNMusicBackendPhononThread()
{
}

void KNMusicBackendPhononThread::loadFromFile(const QString &filePath)
{
    m_mediaSource=MediaSource(filePath);
    //Load the file to media source.
    m_mediaObject->setCurrentSource(m_mediaSource);
}

void KNMusicBackendPhononThread::clear()
{
    m_mediaObject->clear();
}

void KNMusicBackendPhononThread::resetState()
{
    ;
}

void KNMusicBackendPhononThread::stop()
{
    m_mediaObject->stop();
}

void KNMusicBackendPhononThread::pause()
{
    m_mediaObject->pause();
}

void KNMusicBackendPhononThread::play()
{
    m_mediaObject->play();
}

int KNMusicBackendPhononThread::volume()
{
    return m_audioOutput->volume()*10000;
}

qint64 KNMusicBackendPhononThread::duration()
{
    return m_mediaObject->totalTime();
}

qint64 KNMusicBackendPhononThread::position()
{
    return m_mediaObject->currentTime();
}

void KNMusicBackendPhononThread::setPlaySection(const qint64 &sectionStart,
                                                const qint64 &sectionDuration)
{
    ;
}

void KNMusicBackendPhononThread::playSection(const qint64 &sectionStart,
                                             const qint64 &sectionDuration)
{
    //Set the section.
    setPlaySection(sectionStart, sectionDuration);
    //Play the main thread.
    play();
}

void KNMusicBackendPhononThread::setVolume(const int &volumeSize)
{
    m_audioOutput->setVolume((qreal)volumeSize/10000.0);
}

void KNMusicBackendPhononThread::setPosition(const qint64 &position)
{
    ;
}
