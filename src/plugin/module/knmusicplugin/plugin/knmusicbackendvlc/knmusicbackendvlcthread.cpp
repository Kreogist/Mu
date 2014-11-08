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
#include <QDir>

#include "knmusicbackendvlcthread.h"

#include <QDebug>

KNMusicBackendVLCThread::KNMusicBackendVLCThread(QObject *parent) :
    KNMusicBackendThread(parent)
{
    //Get the VLC global.
    m_vlcGlobal=KNMusicVLCGlobal::instance();
    //Initial the player.
    m_player=libvlc_media_player_new(m_vlcGlobal->vlcInstance());
    //Generate the event manager for the player.
    m_vlcEventManager=libvlc_media_player_event_manager(m_player);
    //Disable mouse and keyboard events.
    libvlc_video_set_key_input(m_player, false);
    libvlc_video_set_mouse_input(m_player, false);
    //Initial the event list.
    m_vlcEventList << libvlc_MediaPlayerPositionChanged
                   << libvlc_MediaPlayerEndReached;
    //Established event attach.
    establishEventAttach();
}

KNMusicBackendVLCThread::~KNMusicBackendVLCThread()
{
    //Release the event attach.
    releaseEventAttach();
    //Clear the thread.
    clear();
    //Release the player.
    libvlc_media_player_release(m_player);
}

void KNMusicBackendVLCThread::loadFromFile(const QString &filePath)
{
    //Stop the thread first.
    stop();
    //Load the file to thread.
    //Check is the file the current file.
    if(filePath==m_filePath)
    {
        resetState();
        return;
    }
    //Backup the file path.
    m_filePath=filePath;
    //Emit file path changed signal.
    emit filePathChanged(m_filePath);
    //Clear the media data first.
    clearMedia();
    //Load the file.
    m_media=libvlc_media_new_path(m_vlcGlobal->vlcInstance(),
                                  QDir::toNativeSeparators(m_filePath).toStdString().data());
    if(m_media==nullptr)
    {
        emit cannotLoadFile();
        return;
    }
    //Parse the media immedately.
    libvlc_media_parse(m_media);
    //Set the media to the player.
    libvlc_media_player_set_media(m_player, m_media);
    //Get the total duration.
    m_totalDuration=libvlc_media_get_duration(m_media);
    //Reset the thread.
    resetState();
}

void KNMusicBackendVLCThread::clear()
{
    //Clear media data.
    clearMedia();
    //Reset the thread.
    m_filePath.clear();
    //Reset the durations.
    m_totalDuration=0;
    //Reset thread data.
    resetState();
}

void KNMusicBackendVLCThread::resetState()
{
    //Get the duration.
    m_duration=m_totalDuration;
    //Set the start position at the very beginning.
    m_startPosition=0;
    //Set the default end position as the whole file.
    m_endPosition=m_duration;
}

void KNMusicBackendVLCThread::stop()
{
    if(m_playingState!=StoppedState)
    {
        //Stop the player.
        libvlc_media_player_stop(m_player);
        //Reset the state.
        setState(StoppedState);
        emit stopped();
    }
}

void KNMusicBackendVLCThread::pause()
{
    if(m_playingState!=PausedState &&
            libvlc_media_player_can_pause(m_player))
    {
        //Pause that thread.
        libvlc_media_player_set_pause(m_player, true);
        //Reset the state.
        setState(PausedState);
    }
}

void KNMusicBackendVLCThread::play()
{
    if(m_playingState!=PlayingState)
    {
        //Check whether is now is playing or not.
        if(m_playingState==StoppedState)
        {
            //Reset the position to fit track playing.
            setPosition(0);
            //Play the thread.
            libvlc_media_player_play(m_player);
        }
        else
        {
            //Release the pause state.
            libvlc_media_player_set_pause(m_player, false);
        }
        //Reset the state.
        setState(PlayingState);
    }
}

int KNMusicBackendVLCThread::volume()
{
    return libvlc_audio_get_volume(m_player);
}

qint64 KNMusicBackendVLCThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendVLCThread::position()
{
    return libvlc_media_player_get_time(m_player)-m_startPosition;
}

void KNMusicBackendVLCThread::setPlaySection(const qint64 &sectionStart, const qint64 &sectionDuration)
{
    //Check the start position and duration is still in the duration.
    //If it's available, set the start position.
    if(sectionStart!=-1 && sectionStart<m_duration)
    {
        m_startPosition=sectionStart;
        //Update the duration.
        if(m_startPosition!=-1 && m_startPosition+sectionDuration<m_duration)
        {
            m_duration=sectionDuration;
        }
        else
        {
            m_duration=m_duration-m_startPosition;
        }
        //Update the end position.
        m_endPosition=m_startPosition+m_duration;
    }
    //Update the duration like playing file.
    emit durationChanged(duration());
}

void KNMusicBackendVLCThread::playSection(const qint64 &sectionStart,
                                          const qint64 &sectionDuration)
{
    //Set the section.
    setPlaySection(sectionStart, sectionDuration);
    //Play the main thread.
    play();
}

void KNMusicBackendVLCThread::positionCheck()
{
    qint64 currentPosition=position();
    emit positionChanged(currentPosition);
    /*
     * - Q: Why we still need to do this?
     * - A: When cue is playing, it may not stopped at the end of the file.
     *      The callback is only used to solve the position won't reach the end
     *   bug, the track duration stopped will still process here.
     */
    if(currentPosition>=m_duration)
    {
//        stop();
        emit finished();
    }
}

void KNMusicBackendVLCThread::setVolume(const int &volumeSize)
{
    libvlc_audio_set_volume(m_player, volumeSize);
}

void KNMusicBackendVLCThread::setPosition(const qint64 &position)
{
    //If no media, ignore.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Change the position into second.
    libvlc_media_player_set_time(m_player, position+m_startPosition);
}

void KNMusicBackendVLCThread::libvlcCallBack(const libvlc_event_t *event,
                                             void *data)
{
    //Transform the data pointer to vlc thread pointer.
    KNMusicBackendVLCThread *vlcThread=(KNMusicBackendVLCThread *)data;

    switch(event->type)
    {
    case libvlc_MediaPlayerEndReached:
        //Emit finished signal, simply calling that function is enough.
        vlcThread->finished();
        break;
    case libvlc_MediaPlayerPositionChanged:
        //Do position check.
        vlcThread->positionCheck();
        break;
    }
}

void KNMusicBackendVLCThread::clearMedia()
{
    if(m_media!=nullptr)
    {
        libvlc_media_release(m_media);
        m_media=nullptr;
    }
}

void KNMusicBackendVLCThread::setState(const int &state)
{
    //If the state is really different, we are going to emit playing state
    //changed signal.
    if(state!=m_playingState)
    {
        m_playingState=state;
        emit stateChanged(m_playingState);
    }
}

void KNMusicBackendVLCThread::establishEventAttach()
{
    //Use libvlc_event_attach to established the attach.
    foreach (const libvlc_event_e &event, m_vlcEventList) {
        libvlc_event_attach(m_vlcEventManager, event, libvlcCallBack, this);
    }
}

void KNMusicBackendVLCThread::releaseEventAttach()
{
    //Use libvlc_event_detach to remove the attach.
    foreach (const libvlc_event_e &event, m_vlcEventList) {
        libvlc_event_detach(m_vlcEventManager, event, libvlcCallBack, this);
    }
}
