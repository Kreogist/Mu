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

#include "knmusicbackendqtavthread.h"

#include <QDebug>

KNMusicBackendQtAVThread::KNMusicBackendQtAVThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_player(new QtAV::AVPlayer(this)),
    m_startPosition(-1),
    m_endPosition(-1),
    m_volumeSize(1.0),
    m_status(MusicUtil::Stopped)
{
    //Change the notify interval.
    m_player->setNotifyInterval(16);
    //Link the player to the signals.
    connect(m_player, &QtAV::AVPlayer::positionChanged,
            this, &KNMusicBackendQtAVThread::onActionPositionChanged);
    connect(m_player, &QtAV::AVPlayer::mediaStatusChanged,
            this, &KNMusicBackendQtAVThread::onActionMediaStateChanged);
    connect(m_player, SIGNAL(loaded()), this, SLOT(onActionLoaded()));
}

bool KNMusicBackendQtAVThread::loadFile(const QString &filePath)
{
    //Stop playing.
    stop();
    //Check if the file path is just the current file.
    if(filePath==m_player->file())
    {
        //Mission complete, we won't need anything new.
        return true;
    }
    //Set the file to the AVPlayer.
    m_player->setFile(filePath);
    //Give back true.
    return true;
}

void KNMusicBackendQtAVThread::reset()
{
    //Stop the player.
    m_player->stop();
    //Unload the data.
    m_player->unload();
    //Reset the position value.
    m_startPosition=-1;
    m_endPosition=-1;
}

void KNMusicBackendQtAVThread::stop()
{
    //Set the player to stop state.
    m_player->stop();
    //Reset the state data.
    m_status=MusicUtil::Stopped;
    //Update the position as soon as possible.
    emit positionChanged(m_startPosition);
    //Emit the state changed signal.
    emit stateChanged(m_status);
}

void KNMusicBackendQtAVThread::play()
{
    //Check out the playing states.
    if(m_status==MusicUtil::Playing)
    {
        //Ignore the operation if the player is already playing.
        return;
    }
    //Check out previous status.
    if(m_player->isPaused())
    {
        //Disabled the pause state.
        m_player->pause(false);
    }
    else
    {
        //Make the player to play from the start.
        m_player->play();
    }
    //Set the state data to playing.
    m_status=MusicUtil::Playing;
    //Emit the state changed signal.
    emit stateChanged(m_status);
}

void KNMusicBackendQtAVThread::pause()
{
    //Check out whether the player is playing.
    if(m_player->isPlaying())
    {
        //You can only pause the player when it's playing.
        m_player->pause();
        //Set the state data to playing.
        m_status=MusicUtil::Paused;
        //Emit the state changed signal.
        emit stateChanged(m_status);
    }
}

int KNMusicBackendQtAVThread::volume()
{
    //Check audio from the avplayer is null or not.
    return m_volumeSize*10000;
}

qint64 KNMusicBackendQtAVThread::duration()
{
    return m_player->duration();
}

qint64 KNMusicBackendQtAVThread::position()
{
    return m_player->position()-m_startPosition;
}

int KNMusicBackendQtAVThread::state() const
{
    //Give back the state information.
    return m_status;
}

void KNMusicBackendQtAVThread::setPlaySection(const qint64 &start,
                                              const qint64 &duration)
{
    //Update the start position to the start.
    m_startPosition=(start==-1)?0:start;
    //Check out the duration, that should be the position.
    m_endPosition=(duration==-1)?0:m_startPosition+duration;
    //Check out the media loading state.
    if(m_player->isLoaded())
    {
        //Set the start and end position.
        m_player->setStartPosition(m_startPosition);
        //Set the end position.
        m_player->setStopPosition(m_endPosition);
    }
}

void KNMusicBackendQtAVThread::setVolume(const int &volume)
{
    //Save the volumn size.
    m_volumeSize=(qreal)volume/10000.0;
    //Check out the audio pointer.
    if(m_player->audio())
    {
        //Set the volumn to the player.
        m_player->audio()->setVolume(m_volumeSize);
    }
}

void KNMusicBackendQtAVThread::setPosition(const qint64 &position)
{
    m_player->seek(m_startPosition+position);
}

void KNMusicBackendQtAVThread::onActionPositionChanged(const qint64 &position)
{
    //Emit the position changed signal.
    emit positionChanged(position-m_startPosition);
}

void KNMusicBackendQtAVThread::onActionMediaStateChanged(
        const QtAV::MediaStatus &status)
{
    //Check out the status.
    switch(status)
    {
    case QtAV::InvalidMedia:
        //Because the media is invalid, loaded should be failed.
        emit loadFailed();
        break;
    case QtAV::EndOfMedia:
        //This should means we have played to the end of the file.
        emit finished();
        break;
    }
}

void KNMusicBackendQtAVThread::onActionLoaded()
{
    //Update the volume size.
    if(m_player->audio())
    {
        //Set the volume size.
        m_player->audio()->setVolume(m_volumeSize);
    }
    //Set the start and end position.
    m_player->setStartPosition(m_startPosition);
    //Set the end position.
    m_player->setStopPosition(m_endPosition);
    //Get the duration of the player.
    emit durationChanged(((m_endPosition==0)?
                             m_player->mediaStopPosition():m_endPosition) -
                         m_startPosition);
    //Emit the loaded success signal.
    emit loadSuccess();
}
