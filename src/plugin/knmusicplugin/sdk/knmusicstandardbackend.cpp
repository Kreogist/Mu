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
#include "knmusicstandardbackendthread.h"

#include "knmusicstandardbackend.h"

KNMusicStandardBackend::KNMusicStandardBackend(QObject *parent) :
    KNMusicBackend(parent),
    m_main(nullptr),
    m_preview(nullptr)
{
}

qint64 KNMusicStandardBackend::duration() const
{
    return threadDuration(m_main);
}

qint64 KNMusicStandardBackend::position() const
{
    return threadPosition(m_main);
}

bool KNMusicStandardBackend::loadMusic(const QString &filePath,
                                       const qint64 &start,
                                       const qint64 &duration)
{
    return threadLoadMusic(m_main, filePath, start, duration);
}

void KNMusicStandardBackend::play()
{
    threadPlay(m_main);
}

void KNMusicStandardBackend::pause()
{
    threadPause(m_main);
}

void KNMusicStandardBackend::stop()
{
    threadStop(m_main);
}

void KNMusicStandardBackend::reset()
{
    threadReset(m_main);
}

qint64 KNMusicStandardBackend::previewDuration() const
{
    return threadDuration(m_preview);
}

qint64 KNMusicStandardBackend::previewPosition() const
{
    return threadPosition(m_preview);
}

bool KNMusicStandardBackend::previewLoadMusic(const QString &filePath,
                                              const qint64 &start,
                                              const qint64 &duration)
{
    return threadLoadMusic(m_preview, filePath, start, duration);
}

void KNMusicStandardBackend::previewPlay()
{
    threadPlay(m_preview);
}

void KNMusicStandardBackend::previewPause()
{
    threadPause(m_preview);
}

void KNMusicStandardBackend::previewStop()
{
    threadStop(m_preview);
}

void KNMusicStandardBackend::previewReset()
{
    threadReset(m_preview);
}

void KNMusicStandardBackend::setMainThread(KNMusicStandardBackendThread *thread)
{
    //If there's already a main thread, ignore the later sets.
    if(m_main)
    {
        return;
    }
    //Save the main thread.
    m_main=thread;
    //Link the main thread to the standard backend.
    connect(m_main, &KNMusicStandardBackendThread::durationChanged,
            this, &KNMusicStandardBackend::durationChanged);
    connect(m_main, &KNMusicStandardBackendThread::positionChanged,
            this, &KNMusicStandardBackend::positionChanged);
    connect(m_main, &KNMusicStandardBackendThread::loadFailed,
            this, &KNMusicStandardBackend::loadFailed);
    connect(m_main, &KNMusicStandardBackendThread::loadSuccess,
            this, &KNMusicStandardBackend::loadSuccess);
    connect(m_main, &KNMusicStandardBackendThread::finished,
            this, &KNMusicStandardBackend::finished);
    connect(m_main, &KNMusicStandardBackendThread::stopped,
            this, &KNMusicStandardBackend::stopped);
    connect(m_main, &KNMusicStandardBackendThread::stateChanged,
            this, &KNMusicStandardBackend::playingStateChanged);
}

void KNMusicStandardBackend::setPreviewThread(
        KNMusicStandardBackendThread *thread)
{
    //If there's already a preview thread, ignore the later sets.
    if(m_preview)
    {
        return;
    }
    //Save the preview thread.
    m_preview=thread;
    //Link the preview thread to the standard backend.
    connect(m_preview, &KNMusicStandardBackendThread::durationChanged,
            this, &KNMusicStandardBackend::previewDurationChanged);
    connect(m_preview, &KNMusicStandardBackendThread::positionChanged,
            this, &KNMusicStandardBackend::previewPositionChanged);
    connect(m_preview, &KNMusicStandardBackendThread::loadFailed,
            this, &KNMusicStandardBackend::previewLoadFailed);
    connect(m_preview, &KNMusicStandardBackendThread::loadSuccess,
            this, &KNMusicStandardBackend::previewLoadSuccess);
    connect(m_preview, &KNMusicStandardBackendThread::finished,
            this, &KNMusicStandardBackend::previewFinished);
    connect(m_preview, &KNMusicStandardBackendThread::stopped,
            this, &KNMusicStandardBackend::previewStopped);
    connect(m_preview, &KNMusicStandardBackendThread::stateChanged,
            this, &KNMusicStandardBackend::previewPlayingStateChanged);
}

void KNMusicStandardBackend::setPosition(const qint64 &position)
{
    threadSetPosition(m_main, position);
}

void KNMusicStandardBackend::setPreviewPosition(const qint64 &position)
{
    threadSetPosition(m_preview, position);
}

inline qint64 KNMusicStandardBackend::threadDuration(
        KNMusicStandardBackendThread *thread) const
{
    //The thread cannot be null.
    return thread?thread->duration():-1;
}

inline qint64 KNMusicStandardBackend::threadPosition(
        KNMusicStandardBackendThread *thread) const
{
    //The thread cannot be null.
    return thread?thread->position():-1;
}

inline bool KNMusicStandardBackend::threadLoadMusic(
        KNMusicStandardBackendThread *thread,
        const QString &filePath,
        const qint64 &start,
        const qint64 &duration) const
{
    //Check thread first, if thread exist, load the music.
    if(thread && thread->loadFile(filePath))
    {
        //Set the section for thread.
        thread->setPlaySection(start, duration);
        //Load the music compelte.
        return true;
    }
    //If there's no thread, load music will always be false.
    return false;
}

inline void KNMusicStandardBackend::threadPlay(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Play the thread.
        thread->play();
    }
}

inline void KNMusicStandardBackend::threadPause(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Pause the thread.
        thread->pause();
    }
}

inline void KNMusicStandardBackend::threadStop(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Stop the thread.
        thread->stop();
    }
}

inline void KNMusicStandardBackend::threadReset(
        KNMusicStandardBackendThread *thread)
{
    //The thread cannot be null.
    if(thread)
    {
        //Reset the thread.
        thread->reset();
    }
}

void KNMusicStandardBackend::threadSetPosition(
        KNMusicStandardBackendThread *thread,
        const qint64 &position)
{
    //The thread cannot be null.
    if(thread)
    {
        //Set the position of the thread.
        thread->setPosition(position);
    }
}
