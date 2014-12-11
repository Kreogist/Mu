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

void KNMusicStandardBackend::loadMusic(const QString &filePath)
{
    m_main->loadFromFile(filePath);
}

qint64 KNMusicStandardBackend::duration() const
{
    return m_main->duration();
}

qint64 KNMusicStandardBackend::position() const
{
    return m_main->position();;
}

void KNMusicStandardBackend::playFile(const QString &fileName)
{
    //Play the whole file as a section.
    playSection(fileName);
}

void KNMusicStandardBackend::playSection(const QString &fileName,
                                         const qint64 &start,
                                         const qint64 &duration)
{
    //Load the music file first.
    loadMusic(fileName);
    //Play the section
    m_main->playSection(start, duration);
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
    //Turn on the smart volume.
    smartVolumeOn();
    //Play the section
    m_preview->playSection(start, duration);
}

void KNMusicStandardBackend::playPreview()
{
    smartVolumeOn();
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

void KNMusicStandardBackend::changeMuteState()
{
    setMute(!m_mute);
}

void KNMusicStandardBackend::setVolume(const int &volumeSize)
{
    if(m_mute)
    {
        m_mute=false;
        emit muteStateChanged(m_mute);
    }
    changeVolume(volumeSize);
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
        changeVolume(0);
    }
    else
    {
        //Set the volume to the backup volume.
        changeVolume(m_volumeBeforeMute);
    }
    //Emit changed signal.
    emit muteStateChanged(m_mute);

}

void KNMusicStandardBackend::setPosition(const qint64 &position)
{
    m_main->setPosition(position);
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
        connect(m_main, &KNMusicBackendThread::filePathChanged,
                this, &KNMusicStandardBackend::filePathChanged);
        connect(m_main, &KNMusicBackendThread::cannotLoadFile,
                this, &KNMusicStandardBackend::cannotLoadFile);
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
        connect(m_preview, &KNMusicBackendThread::filePathChanged,
                this, &KNMusicStandardBackend::previewFilePathChanged);
        connect(m_preview, &KNMusicBackendThread::cannotLoadFile,
                this, &KNMusicStandardBackend::previewCannotLoadFile);
    }
}

void KNMusicStandardBackend::smartVolumeOn()
{
    //If the origial volume is not -1, means smart volume has been turned on.
    if(m_originalVolume!=-1)
    {
        return;
    }
    //Backup the original volume.
    m_originalVolume=volume();
    //Set the preview as the full volume.
    m_preview->setVolume(m_originalVolume);
    //Set the main volume as the 1/8 of the full volume.
    m_main->setVolume(m_originalVolume>8?m_originalVolume/8:0);
}

void KNMusicStandardBackend::smartVolumeOff()
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
