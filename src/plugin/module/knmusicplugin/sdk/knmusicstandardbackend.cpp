/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicbackendthread.h"

#include "knmusicstandardbackend.h"

#include <QDebug>

KNMusicStandardBackend::KNMusicStandardBackend(QObject *parent) :
    KNMusicBackend(parent)
{
    ;
}

bool KNMusicStandardBackend::loadMusic(const QString &filePath)
{
    return m_main->loadFromFile(filePath);
}

qint64 KNMusicStandardBackend::duration() const
{
    return m_main->duration();
}

qint64 KNMusicStandardBackend::position() const
{
    return m_main->position();
}

bool KNMusicStandardBackend::playSection(const QString &fileName,
                                         const qint64 &start,
                                         const qint64 &duration)
{
    //Load the music file first.
    if(!loadMusic(fileName))
    {
        return false;
    }
    //Play the section
    m_main->playSection(start, duration);
    return true;
}

void KNMusicStandardBackend::play()
{
    m_main->play();
}

void KNMusicStandardBackend::pause()
{
    m_main->pause();
}

void KNMusicStandardBackend::stop()
{
    m_main->stop();
}

void KNMusicStandardBackend::resetMainPlayer()
{
    m_main->clear();
}

void KNMusicStandardBackend::loadPreview(const QString &filePath)
{
    m_preview->loadFromFile(filePath);
}

qint64 KNMusicStandardBackend::previewDuration() const
{
    return m_preview->duration();
}

qint64 KNMusicStandardBackend::previewPosition() const
{
    return m_preview->position();
}

void KNMusicStandardBackend::playPreviewFile(const QString &fileName)
{
    //Preview the whole file as a section.
    playPreviewSection(fileName);
}

void KNMusicStandardBackend::setPreviewSection(const qint64 &start,
                                               const qint64 &duration)
{
    m_preview->setPlaySection(start, duration);
}

void KNMusicStandardBackend::playPreviewSection(const QString &fileName,
                                                const qint64 &start,
                                                const qint64 &duration)
{
    //Load the music file first.
    loadPreview(fileName);
    //If the origial volume is not -1, means smart volume has been turned on.
    if(m_originalVolume==-1)
    {
        //Turn on the smart volume.
        smartVolumeOn();
    }
    //Play the section
    m_preview->playSection(start, duration);
}

void KNMusicStandardBackend::playPreview()
{
    //If the origial volume is not -1, means smart volume has been turned on.
    if(m_originalVolume==-1)
    {
        //Turn on the smart volume.
        smartVolumeOn();
    }
    //Play the preview.
    m_preview->play();
}

void KNMusicStandardBackend::stopPreview()
{
    smartVolumeOff();
    //Stop the preview
    m_preview->stop();
}

void KNMusicStandardBackend::pausePreview()
{
    smartVolumeOff();
    //Stop the preview
    m_preview->pause();
}

void KNMusicStandardBackend::resetPreviewPlayer()
{
    //Set the smart volume off first.
    smartVolumeOff();
    //Clear the thread.
    m_preview->clear();
}

bool KNMusicStandardBackend::mute()
{
    return m_mute;
}

void KNMusicStandardBackend::changeMuteState()
{
    setMute(!m_mute);
}

void KNMusicStandardBackend::setVolume(int volumeSize)
{
    if(m_mute)
    {
        m_mute=false;
        emit muteStateChanged(m_mute);
    }
    //Check the volume size.
    if(volumeSize<volumeMinimal())
    {
        changeAndSyncVolume(volumeMinimal());
        return;
    }
    if(volumeSize>volumeMaximum())
    {
        changeAndSyncVolume(volumeMaximum());
        return;
    }
    changeAndSyncVolume(volumeSize);
}

void KNMusicStandardBackend::setMute(const bool &mute)
{
    //Check the state is the same or not, if is the same, do nothing.
    if(m_mute==mute)
    {
        return;
    }
    //Set state.
    m_mute=mute;
    if(m_mute)
    {
        //Backup the original volume, mute it.
        m_volumeBeforeMute=volume();
        changeAndSyncVolume(0);
    }
    else
    {
        //Set the volume to the backup volume.
        changeAndSyncVolume(m_volumeBeforeMute);
    }
    //Emit changed signal.
    emit muteStateChanged(m_mute);
}

void KNMusicStandardBackend::setPosition(const qint64 &position)
{
    m_main->setPosition(position);
}

void KNMusicStandardBackend::volumeUp()
{
    setVolume(volume()+volumeLevel());
}

void KNMusicStandardBackend::volumeDown()
{
    setVolume(volume()-volumeLevel());
}

void KNMusicStandardBackend::setPreviewPosition(const qint64 &position)
{
    m_preview->setPosition(position);
}

void KNMusicStandardBackend::setMainThread(KNMusicBackendThread *thread)
{
    if(m_main==nullptr)
    {
        m_main=thread;
        connect(m_main, &KNMusicBackendThread::positionChanged,
                this, &KNMusicStandardBackend::positionChanged);
        connect(m_main, &KNMusicBackendThread::durationChanged,
                this, &KNMusicStandardBackend::durationChanged);
        connect(m_main, &KNMusicBackendThread::finished,
                this, &KNMusicStandardBackend::finished);
        connect(m_main, &KNMusicBackendThread::stopped,
                this, &KNMusicStandardBackend::stopped);
        connect(m_main, &KNMusicBackendThread::stateChanged,
                this, &KNMusicStandardBackend::playingStateChanged);
        connect(m_main, &KNMusicBackendThread::loaded,
                this, &KNMusicStandardBackend::loaded);
        connect(m_main, &KNMusicBackendThread::cannotLoadFile,
                this, &KNMusicStandardBackend::cannotLoad);
    }
}

void KNMusicStandardBackend::setPreviewThread(KNMusicBackendThread *thread)
{
    if(m_preview==nullptr)
    {
        m_preview=thread;
        connect(m_preview, &KNMusicBackendThread::positionChanged,
                this, &KNMusicStandardBackend::previewPositionChanged);
        connect(m_preview, &KNMusicBackendThread::durationChanged,
                this, &KNMusicStandardBackend::previewDurationChanged);
        connect(m_preview, &KNMusicBackendThread::finished,
                this, &KNMusicStandardBackend::previewFinished);
        connect(m_preview, &KNMusicBackendThread::stopped,
                this, &KNMusicStandardBackend::previewStopped);
        connect(m_preview, &KNMusicBackendThread::stateChanged,
                this, &KNMusicStandardBackend::previewPlayingStateChanged);
        connect(m_preview, &KNMusicBackendThread::loaded,
                this, &KNMusicStandardBackend::previewLoaded);
        connect(m_preview, &KNMusicBackendThread::cannotLoadFile,
                this, &KNMusicStandardBackend::previewCannotLoad);
    }
}

void KNMusicStandardBackend::changeAndSyncVolume(const int &volumeSize)
{
    bool smartVolume=(m_originalVolume!=-1);
    if(smartVolume)
    {
        smartVolumeOff();
    }
    //Change the main thread volume.
    changeVolume(volumeSize);
    //Check if the smart volume on, if so, reset the smart volume size.
    if(smartVolume)
    {
        //Sync the volume when smart volume is on.
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
    qreal mainVolumeSize=(qreal)m_originalVolume*smartVolumeScale();
    m_main->setVolume((int)mainVolumeSize);
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
