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
#include <QTimer>

#include "knmusicbassglobal.h"

#include "knmusicbackendbassthread.h"

#include <QDebug>

KNMusicBackendBassThread::KNMusicBackendBassThread(QObject *parent) :
    KNMusicBackendThread(parent)
{
    //Initial position updater.
    m_positionUpdater=new QTimer;
    m_positionUpdater->setInterval(5);
    connect(m_positionUpdater, &QTimer::timeout,
            this, &KNMusicBackendBassThread::onActionPositionCheck);

    connect(this, &KNMusicBackendBassThread::requireStopped,
            this, &KNMusicBackendBassThread::stop);
}

KNMusicBackendBassThread::~KNMusicBackendBassThread()
{
    delete m_positionUpdater;
}

bool KNMusicBackendBassThread::loadFromFile(const QString &filePath)
{
    //Stop the thread first.
    stop();
    //Release all the sync handle.
    releaseSyncHandle();
    //Load the file to thread.
    //Check is the file the current file.
    if(filePath==m_filePath)
    {
        resetState();
        //Emit load succeed signal.
        emit loaded();
        return true;
    }
    //Clear the file path.
    m_filePath.clear();
    //Try to load the file.
#ifdef Q_OS_WIN32
    std::wstring uniPath=filePath.toStdWString();
    if(!(m_channel=BASS_StreamCreateFile(FALSE,
                                         uniPath.data(),
                                         0,
                                         0,
                                         BASS_UNICODE |
                                           KNMusicBassGlobal::fdps()))
           && !(m_channel=BASS_MusicLoad(FALSE,
                                         uniPath.data(),
                                         0,
                                         0,
                                         BASS_UNICODE |
                                           BASS_MUSIC_RAMPS |
                                           KNMusicBassGlobal::fdps(),1)))
#endif
#ifdef Q_OS_UNIX
    std::string uniPath=m_filePath.toStdString();
    if(!(m_channel=BASS_StreamCreateFile(FALSE,
                                         uniPath.data(),
                                         0,
                                         0,
                                         KNMusicBassGlobal::fdps()))
            && !(m_channel=BASS_MusicLoad(FALSE,
                                          uniPath.data(),
                                          0,
                                          0,
                                          BASS_MUSIC_RAMPS |
                                          KNMusicBassGlobal::fdps(),1)))
#endif
    {
        //Loaded failed, emit cannot load signal.
        emit cannotLoadFile();
        return false;
    }
    //Backup the file path.
    m_filePath=filePath;
    //Emit load succeed.
    emit loaded();
    //Establish sync handle.
    establishSyncHandle();
    //When loading the file complete, set the channel info to the thread.
    //Get the byte duration.
    m_totalDuration=BASS_ChannelBytes2Seconds(m_channel,
                                              BASS_ChannelGetLength(m_channel, BASS_POS_BYTE))*1000;
    //Change the duration.
    emit durationChanged(m_totalDuration);
    //Reset the thread.
    resetState();
    return true;
}

void KNMusicBackendBassThread::clear()
{
    //Clear channel datas.
    BASS_MusicFree(m_channel);
    BASS_StreamFree(m_channel);
    //Reset thread.
    m_filePath.clear();
    //Reset the durations.
    m_totalDuration=0;
    //Reset thread data.
    resetState();
    //Reset the state to stopped.
    if(m_playingState!=StoppedState)
    {
        m_playingState=StoppedState;
        emit stateChanged(m_playingState);
    }
}

void KNMusicBackendBassThread::resetState()
{
    //Set stop flag.
    m_stoppedState=true;
    //Get the duration
    m_duration=m_totalDuration;
    //Set the start position at the very beginning.
    m_startPosition=0;
    //Set default end position as the whole file.
    m_endPosition=m_duration;
}

void KNMusicBackendBassThread::stop()
{
    //Check if the thread data if empty.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Check the state.
    if(m_playingState!=StoppedState)
    {
        //Stop the channel, here is what the specific thing.
        BASS_ChannelStop(m_channel);
        //Stop position updater.
        m_positionUpdater->stop();
        //Reset position.
        setPosition(0);
        //Set stop flag.
        m_stoppedState=true;
        //Reset the state.
        setState(StoppedState);
        emit stopped();
    }
}

void KNMusicBackendBassThread::pause()
{
    //Check if the thread data if empty.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Check the state.
    if(m_playingState!=PausedState)
    {
        //Pause that thread.
        BASS_ChannelPause(m_channel);
        //Stop the updater.
        m_positionUpdater->stop();
        //Reset the state.
        setState(PausedState);
    }
}

void KNMusicBackendBassThread::play()
{
    //Check if the thread data if empty.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Check the state.
    if(m_playingState!=PlayingState)
    {
        //Start the position updater first.
        m_positionUpdater->start();
        //Check whether is now is playing or not.
        if(m_stoppedState)
        {
            //Reset flag.
            m_stoppedState=false;
            //Reset the position to fit track playing.
            setPosition(0);
            //Set the volume to the last volume, because of the reset, the
            //volume is back to 1.0.
            BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_lastVolume);
        }
        //Play the thread.
        BASS_ChannelPlay(m_channel, FALSE);
        //Reset the state.
        setState(PlayingState);
    }
}

int KNMusicBackendBassThread::volume()
{
    float channelVolume;
    BASS_ChannelGetAttribute(m_channel,
                             BASS_ATTRIB_VOL,
                             &channelVolume);
    return (int)channelVolume*100;
}

qint64 KNMusicBackendBassThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendBassThread::position()
{
    return (qint64)(BASS_ChannelBytes2Seconds(m_channel,
                                             BASS_ChannelGetPosition(m_channel, BASS_POS_BYTE))
                    *1000)-m_startPosition;
}

void KNMusicBackendBassThread::setPlaySection(const qint64 &sectionStart,
                                              const qint64 &sectionDuration)
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

void KNMusicBackendBassThread::playSection(const qint64 &sectionStart,
                                           const qint64 &sectionDuration)
{
    //Set the section.
    setPlaySection(sectionStart, sectionDuration);
    //Play the main thread.
    play();
}

void KNMusicBackendBassThread::setVolume(const int &volumeSize)
{
    float channelVolume=(float)volumeSize/100;
    BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, channelVolume);
    //Backup the volume
    m_lastVolume=channelVolume;
}

void KNMusicBackendBassThread::setPosition(const qint64 &position)
{
    //If no media, ignore.
    if(m_filePath.isEmpty())
    {
        return;
    }
    //Change the position into second.
    BASS_ChannelSetPosition(m_channel,
                            BASS_ChannelSeconds2Bytes(m_channel,
                                                      (double)(m_startPosition+position)/1000.0),
                            BASS_POS_BYTE);
    //Do the position check.
    onActionPositionCheck();
}

void KNMusicBackendBassThread::onActionPositionCheck()
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
        stop();
        m_stoppedState=true;
        emit finished();
    }
}

void KNMusicBackendBassThread::onActionEnd(HSYNC handle,
                                           DWORD channel,
                                           DWORD data,
                                           void *user)
{
    Q_UNUSED(handle)
    Q_UNUSED(channel)
    Q_UNUSED(data)

    //Transform the user pointer to channel pointer.
    KNMusicBackendBassThread *bassThread=(KNMusicBackendBassThread *)user;
    //Stop that thread first.
    bassThread->requireDoStopped();
    //Set the stopped state.
    bassThread->setStoppedState(true);
    //Emit finished signal, simply calling that function is enough.
    bassThread->finished();
}

void KNMusicBackendBassThread::establishSyncHandle()
{
    HSYNC handle=BASS_ChannelSetSync(m_channel,
                                     BASS_SYNC_END,
                                     0,
                                     onActionEnd,
                                     this);
    m_syncHandles.append(handle);
}

void KNMusicBackendBassThread::releaseSyncHandle()
{
    //Remove all the sync in the list.
    while(!m_syncHandles.isEmpty())
    {
        BASS_ChannelRemoveSync(m_channel,
                               m_syncHandles.takeLast());
    }
}

void KNMusicBackendBassThread::setState(const int &state)
{
    //If the state is really different, we are going to emit playing state
    //changed signal.
    if(state!=m_playingState)
    {
        m_playingState=state;
        emit stateChanged(m_playingState);
    }
}
bool KNMusicBackendBassThread::stoppedState() const
{
    return m_stoppedState;
}

void KNMusicBackendBassThread::setStoppedState(bool stoppedState)
{
    m_stoppedState = stoppedState;
}

void KNMusicBackendBassThread::requireDoStopped()
{
    emit requireStopped();
}

