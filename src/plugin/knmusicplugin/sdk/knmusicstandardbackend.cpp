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

#include <QDebug>

KNMusicStandardBackend::KNMusicStandardBackend(QObject *parent) :
    KNMusicBackend(parent),
    m_main(nullptr),
    m_preview(nullptr),
    m_originalVolume(-1),
    m_volumeBeforeMute(-1),
    m_mute(false)
{
}

qint64 KNMusicStandardBackend::duration() const
{
    //Get the main thread duration.
    return threadDuration(m_main);
}

qint64 KNMusicStandardBackend::position() const
{
    //Get the main thread position.
    return threadPosition(m_main);
}

bool KNMusicStandardBackend::loadMusic(const QString &filePath,
                                       const qint64 &start,
                                       const qint64 &duration)
{
    //Load the music to the main thread.
    return threadLoadMusic(m_main, filePath, start, duration);
}

int KNMusicStandardBackend::state() const
{
    //Get the main thread playing state.
    return threadState(m_main);
}

void KNMusicStandardBackend::play()
{
    //Play the main thread.
    threadPlay(m_main);
}

void KNMusicStandardBackend::pause()
{
    //Pause the main thread.
    threadPause(m_main);
}

void KNMusicStandardBackend::stop()
{
    //Stop the main thread.
    threadStop(m_main);
}

void KNMusicStandardBackend::reset()
{
    //Reset the main thread.
    threadReset(m_main);
}

qint64 KNMusicStandardBackend::previewDuration() const
{
    //Get the preview thread duration.
    return threadDuration(m_preview);
}

qint64 KNMusicStandardBackend::previewPosition() const
{
    //Get the preview thread position.
    return threadPosition(m_preview);
}

bool KNMusicStandardBackend::previewLoadMusic(const QString &filePath,
                                              const qint64 &start,
                                              const qint64 &duration)
{
    //Load the music to preview thread.
    return threadLoadMusic(m_preview, filePath, start, duration);
}

int KNMusicStandardBackend::previewState() const
{
    //Get the preview thread state.
    return threadState(m_preview);
}

void KNMusicStandardBackend::previewPlay()
{
    //If the origial volume is not -1, means smart volume has been turned on.
    if(m_originalVolume==-1)
    {
        //Turn on the smart volume.
        smartVolumeOn();
    }
    //Play the preview thread.
    threadPlay(m_preview);
}

void KNMusicStandardBackend::previewPause()
{
    //Stop the smart volume.
    smartVolumeOff();
    //Pause the preview thread.
    threadPause(m_preview);
}

void KNMusicStandardBackend::previewStop()
{
    //Stop the smart volume.
    smartVolumeOff();
    //Stop the preview thread.
    threadStop(m_preview);
}

void KNMusicStandardBackend::previewReset()
{
    //Stop the smart volume.
    smartVolumeOff();
    //Reset the thread.
    threadReset(m_preview);
}

bool KNMusicStandardBackend::mute()
{
    //Get the global mute state.
    return m_mute;
}

bool KNMusicStandardBackend::isUsingProxy()
{
    //Get the proxy using state.
    return m_main->isUsingProxy();
}

void KNMusicStandardBackend::save()
{
    //Call the save function of the main thread.
    m_main->save();
}

void KNMusicStandardBackend::restore(const QString &updatedFilePath)
{
    //Call the restore of the main thread.
    m_main->restore(updatedFilePath);
}

void KNMusicStandardBackend::setProxyEnabled(bool enabled)
{
    //Save the enabled states.
    m_main->setProxyEnabled(enabled);
}

void KNMusicStandardBackend::changeMuteState()
{
    //Set the mute state to the different side.
    setMute(!m_mute);
}

void KNMusicStandardBackend::setMute(bool mute)
{
    //Check the state is the same or not, if is the same, do nothing.
    if(m_mute==mute)
    {
        return;
    }
    //Save the new mute state.
    m_mute=mute;
    //Apply the mute state.
    if(m_mute)
    {
        //Backup the original volume.
        m_volumeBeforeMute=volume();
        //Set all the volume to the minimal thread.
        synchronizeThreadVolume(minimalVolume());
    }
    else
    {
        //Set the volume to the backup volume.
        synchronizeThreadVolume(m_volumeBeforeMute);
        //Clear up the backuped volume.
        m_volumeBeforeMute=-1;
    }
    //Emit changed signal.
    emit muteStateChanged(m_mute);
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
    connect(m_main, &KNMusicStandardBackendThread::finished,
            this, &KNMusicStandardBackend::finished);
    connect(m_main, &KNMusicStandardBackendThread::stopped,
            this, &KNMusicStandardBackend::stopped);
    connect(m_main, &KNMusicStandardBackendThread::stateChanged,
            this, &KNMusicStandardBackend::playingStateChanged);
    connect(m_main, &KNMusicStandardBackendThread::urlLoadComplete,
            this, &KNMusicStandardBackend::loadSuccess);
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
    //Set the main thread position to a specific value.
    threadSetPosition(m_main, position);
}

void KNMusicStandardBackend::setPreviewPosition(const qint64 &position)
{
    //Set the preview thread position to a specific value.
    threadSetPosition(m_preview, position);
}

void KNMusicStandardBackend::setVolume(int volumeSize)
{
    //If we want to change the volume, check the mute state first.
    if(mute())
    {
        //Un-mute the backend.
        setMute(false);
    }
    //Check the volume size.
    if(volumeSize<minimalVolume())
    {
        volumeSize=minimalVolume();
    }
    else if(volumeSize>maximumVolume())
    {
        volumeSize=maximumVolume();
    }
    //Sync the thread volume.
    synchronizeThreadVolume(volumeSize);
}

void KNMusicStandardBackend::volumeUp()
{
    //Raise up a volume level of the volume size.
    setVolume(volume()+volumeLevel());
}

void KNMusicStandardBackend::volumeDown()
{
    //Reduce a volume level of the volume size.
    setVolume(volume()-volumeLevel());
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

inline int KNMusicStandardBackend::threadState(
        KNMusicStandardBackendThread *thread) const
{
    //The thread cannot be null.
    return thread?thread->state():-1;
}

inline bool KNMusicStandardBackend::threadLoadMusic(
        KNMusicStandardBackendThread *thread,
        const QString &filePath,
        const qint64 &start,
        const qint64 &duration)
{
    //Check thread first, if thread exist, load the music.
    if(thread && thread->loadFile(filePath))
    {
        //Set the section for thread.
        thread->setPlaySection(start, duration);
        //Emit load success signal.
        emit loadSuccess();
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

inline void KNMusicStandardBackend::synchronizeThreadVolume(const int &volume)
{
    //Check whether the preview smart volume is enabled.
    bool smartVolumeEnabled=(m_originalVolume!=-1);
    //If the smart volume is enabled, disabled it temporarily.
    if(smartVolumeEnabled)
    {
        //Turn off the smart volume.
        smartVolumeOff();
    }
    //Change the global volume size.
    setGlobalVolume(volume);
    //If the smart volume is enabled before, enabled it again.
    if(smartVolumeEnabled)
    {
        //Turn on the smart volume.
        smartVolumeOn();
    }
}

inline void KNMusicStandardBackend::smartVolumeOn()
{
    //Backup the original volume.
    m_originalVolume=m_main->volume()==0?volume():m_main->volume();
    //Set the preview as the full volume.
    m_preview->setVolume(m_originalVolume);
    //Set the main volume as the smartVolumeScale of the full volume.
    m_main->setVolume((int)((qreal)m_originalVolume*smartVolumeScale()));
}

inline void KNMusicStandardBackend::smartVolumeOff()
{
    //If the original volume is -1, means it's not turned on.
    if(m_originalVolume==-1)
    {
        return;
    }
    //Set the main volume back to full volume.
    m_main->setVolume(m_originalVolume);
    //Reset the original volume.
    m_originalVolume=-1;
}
