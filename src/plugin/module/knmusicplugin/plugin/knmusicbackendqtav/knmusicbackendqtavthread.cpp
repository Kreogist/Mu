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
#include <QEventLoop>

#include "knmusicbackendqtavthread.h"

#include <QDebug>

KNMusicBackendQtAVThread::KNMusicBackendQtAVThread(QObject *parent) :
    KNMusicBackendThread(parent)
{
    //Initial the player and enable audio.
    m_player=new AVPlayer;
    m_player->enableAudio(true);
    //Set the default audio size to maximum.
    setVolume(10000);
    //Linked the duration changed signal.
    connect(m_player, &AVPlayer::loaded,
            this, &KNMusicBackendQtAVThread::onActionLoaded);
    connect(m_player, &AVPlayer::positionChanged,
            this, &KNMusicBackendQtAVThread::onActionPositionChanged);
    connect(m_player, &AVPlayer::stopped,
            this, &KNMusicBackendQtAVThread::onActionStopped);
    //Started hack, when started, set the volume value to the player.
    connect(m_player, &AVPlayer::started,
            this, &KNMusicBackendQtAVThread::setPlayerVolume);
}

KNMusicBackendQtAVThread::~KNMusicBackendQtAVThread()
{
    m_player->deleteLater();
}

void KNMusicBackendQtAVThread::loadFromFile(const QString &filePath)
{
    //Block the signals.
    m_player->blockSignals(true);
    //Stop playing.
    m_player->stop();
    //Release the signal.
    m_player->blockSignals(false);
//    //Check the file path.
//    if(m_player->file()==filePath)
//    {
//        //Reset playing parameter state.
//        resetState();
//    }
    //Emit the file changed signal.
    emit filePathChanged(filePath);
    //Don't use load! Use setFile!
    m_player->setFile(filePath);
    //Reset playing parameter state.
    resetState();
}

void KNMusicBackendQtAVThread::clear()
{
    ;
}

void KNMusicBackendQtAVThread::resetState()
{
    //Get the duration.
    m_duration=0;
    //Set the start position at the very beginning.
    m_startPosition=0;
    //Set the default end position as the whole file.
    m_endPosition=0;
    m_hasSection=false;
}

void KNMusicBackendQtAVThread::stop()
{
    //Stop player and emit signal.
    m_player->stop();
    emit stateChanged(StoppedState);
}

void KNMusicBackendQtAVThread::pause()
{
    //Process nothing is the player has been paused.
    if(m_player->isPaused())
    {
        return;
    }
    //Set the pause.
    m_player->pause(true);
    //Emit pause state changed signal.
    emit stateChanged(PausedState);
}

void KNMusicBackendQtAVThread::play()
{
    //Check if now is paused.
    if(m_player->isPaused())
    {
        m_player->pause(false);
    }
    else
    {
        //Play the music, and set the volume to the volume.
        m_player->play();
        //Set the player volume.
        setPlayerVolume();
    }
    //Emit played state changed signal.
    emit stateChanged(PlayingState);
}

int KNMusicBackendQtAVThread::volume()
{
    return m_threadVolumeLevel;
}

qint64 KNMusicBackendQtAVThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendQtAVThread::position()
{
    return m_player->position()-m_startPosition;
}

void KNMusicBackendQtAVThread::setPlaySection(const qint64 &sectionStart,
                                              const qint64 &sectionDuration)
{
    //Save the start position.
    m_hasSection=(sectionStart!=-1);
    //Check if the start position is
    if(m_hasSection)
    {
        m_startPosition=sectionStart;
        //Save the section and duration.
        m_endPosition=sectionStart+sectionDuration;
    }
}

void KNMusicBackendQtAVThread::playSection(const qint64 &sectionStart,
                                           const qint64 &sectionDuration)
{
    //Set the section.
    setPlaySection(sectionStart, sectionDuration);
    //Play the main thread.
    play();
}

void KNMusicBackendQtAVThread::setVolume(const int &volumeSize)
{
    //Save the thread volume value.
    m_threadVolumeLevel=(qreal)volumeSize/10000.0;
    //Set the volume.
    setPlayerVolume();
}

void KNMusicBackendQtAVThread::setPosition(const qint64 &position)
{
    if(m_player->file().isEmpty())
    {
        return;
    }
    //Change the position into second.
    m_player->setPosition(position+m_startPosition);
}

void KNMusicBackendQtAVThread::onActionLoaded()
{
    //Update duration data.
    if(m_hasSection)
    {
        //Update the duration.
        m_duration=m_endPosition-m_startPosition;
        //Set start and stop postion.
        m_player->setStartPosition(m_startPosition);
        m_player->setStopPosition(m_endPosition);
    }
    else
    {
        //Update the duration and end position.
        m_duration=m_player->duration();
        m_endPosition=m_duration;
    }
    //Emit duration changed signal.
    emit durationChanged(m_duration);
}

void KNMusicBackendQtAVThread::onActionStopped()
{
    //Emit the final position reach signal.
    emit durationChanged(m_duration);
    //Emit finished signal.
    emit finished();
}

void KNMusicBackendQtAVThread::onActionPositionChanged(const qint64 &position)
{
    //Emit the reduced position.
    emit positionChanged(position-m_startPosition);
}

void KNMusicBackendQtAVThread::setPlayerVolume()
{
    //Check is audio null, set the volume.
    if(m_player->audio()!=nullptr)
    {
        m_player->audio()->setVolume(m_threadVolumeLevel);
    }
}
