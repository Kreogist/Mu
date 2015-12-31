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

#include "knmusicbackendbassthread.h"

#include <QDebug>

KNMusicBackendBassThread::KNMusicBackendBassThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_filePath(QString()),
    m_channel(0),
    m_channelFlags(0),
    m_totalDuration(-1),
    m_duration(-1),
    m_startPosition(-1),
    m_endPosition(-1),
    m_savedPosition(-1),
    m_volume(1.0),
    m_state(Stopped),
    m_positionUpdater(new QTimer(this)),
    m_syncHandlers(QList<HSYNC>())
{
    //Configure the position updater.
    m_positionUpdater->setInterval(17);
    connect(m_positionUpdater, &QTimer::timeout,
            this, &KNMusicBackendBassThread::checkPosition);
    //Link the reachesFinished() signal.
    connect(this, &KNMusicBackendBassThread::reachesFinished,
            this, &KNMusicBackendBassThread::finishPlaying);
}

KNMusicBackendBassThread::~KNMusicBackendBassThread()
{
    //Stop the position updater.
    m_positionUpdater->stop();
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Free the channel.
    freeChannel();
}

bool KNMusicBackendBassThread::loadFile(const QString &filePath)
{
    //Stop the current thread first.
    stop();
    //Remove all the previous sync handlers.
    removeChannelSyncs();
    //Check is the file the current file.
    if(filePath==m_filePath)
    {
        //Reset the current state.
        resetChannelInformation();
        //Update the duration.
        emit durationChanged(m_duration);
        //Emit the load success signal.
        emit loadSuccess();
        //Mission complete.
        return true;
    }
    //Load the file path.
    if(!loadBassThread(filePath))
    {
        //Load the file failed.
        return false;
    }
    //When loading the file complete, load the channel info to the thread.
    //Get the duration of the whole file.
    m_totalDuration=
            BASS_ChannelBytes2Seconds(m_channel,
                                      BASS_ChannelGetLength(m_channel,
                                                            BASS_POS_BYTE))
            *1000.0;
    //Emit the duration changed signal.
    emit durationChanged(m_totalDuration);
    //Reset the thread information.
    resetChannelInformation();
    //Load complete.
    return true;
}

void KNMusicBackendBassThread::reset()
{
    //Stop the position updater.
    m_positionUpdater->stop();
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Check if the channel is not null.
    freeChannel();
    //Reset the channel data.
    m_filePath.clear();
    //Reset the total duration.
    m_totalDuration=-1;
    //Reset the stream status data.
    resetChannelInformation();
    //Check the current state is stopped or not.
    setPlayingState(Stopped);
}

void KNMusicBackendBassThread::stop()
{
    //Check:
    // 1. The state is already stopped.
    // 2. The channel is null.
    if(m_state==Stopped || (!m_channel))
    {
        return;
    }
    //Stop the channel.
    BASS_ChannelStop(m_channel);
    //Stop the position updater.
    m_positionUpdater->stop();
    //Reset the position to the start position.
    setPosition(0);
    //Update the state.
    setPlayingState(Stopped);
    //Emit stopped signal.
    emit stopped();
}

void KNMusicBackendBassThread::play()
{
    //Check:
    // 1. The state is already playing.
    // 2. The channel is null.
    if(m_state==Playing || (!m_channel))
    {
        return;
    }
    //Start the position updater.
    m_positionUpdater->start();
    //Check the playing state before.
    if(m_state==Stopped)
    {
        //Reset the position to fit track playing.
        setPosition(0);
        //Set the volume to the last volume, because of the reset, the
        //volume is back to 1.0.
        BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
    }
    //Play the thread.
    BASS_ChannelPlay(m_channel, FALSE);
    //Update the state.
    setPlayingState(Playing);
}

void KNMusicBackendBassThread::pause()
{
    //Check:
    // 1. The state is already paused.
    // 2. The channel is null.
    if(m_state==Paused || (!m_channel))
    {
        return;
    }
    //Pause the thread.
    BASS_ChannelPause(m_channel);
    //Stop the updater.
    m_positionUpdater->stop();
    //Reset the state.
    setPlayingState(Paused);
}

int KNMusicBackendBassThread::volume()
{
    //Scale the float number.
    return (int)(getChannelVolume()*100.0);
}

qint64 KNMusicBackendBassThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendBassThread::position()
{
    return getChannelPosition();
}

int KNMusicBackendBassThread::state() const
{
    return m_state;
}

void KNMusicBackendBassThread::setPlaySection(const qint64 &start,
                                              const qint64 &duration)
{
    //Check the validation of the start position.
    // 1. Start is not null(-1).
    // 2. Start is lesser than duration.
    if(start!=-1 && start<m_duration)
    {
        //Save the start position.
        m_startPosition=start;
        //Check the validation of the duration.
        if(duration==-1 || start+duration>m_duration)
        {
            //We will treat the invalid duration when the start is valid to
            //play to the end of the file.
            m_duration-=m_startPosition;
        }
        else
        {
            //Or else, save the new duration.
            m_duration=duration;
        }
        //Update the end position.
        m_endPosition=m_startPosition+m_duration;
        //Emit the new duration.
        emit durationChanged(m_duration);
    }
}

void KNMusicBackendBassThread::save()
{
    //Pause the thread first.
    BASS_ChannelPause(m_channel);
    //Stop the updater.
    m_positionUpdater->stop();
    //Save the position of the current thread.
    m_savedPosition=position();
    //Reset the current playing thread, but saved all the other parameter.
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Check if the channel is not null.
    freeChannel();
}

void KNMusicBackendBassThread::restore(const QString &updatedFilePath)
{
    //Check out the saved position, if it's -1, means it never saved before.
    //Ignore the invalid call.
    if(m_savedPosition==-1)
    {
        return;
    }
    //Check out the updated file path.
    QString restoreFilePath=
            updatedFilePath.isEmpty()?m_filePath:updatedFilePath;
    //Reload the bass thread.
    loadBassThread(restoreFilePath);
    //Reset the postion.
    setPosition(m_savedPosition);
    //Set the volume to the last volume, because of the reset, the
    //volume is back to 1.0.
    BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
    //Check out the state.
    if(m_state==Playing)
    {
        //Start the updater.
        m_positionUpdater->start();
        //Play the thread.
        BASS_ChannelPlay(m_channel, FALSE);
    }
    //Reset the saved position.
    m_savedPosition=-1;
}

void KNMusicBackendBassThread::setVolume(const int &volume)
{
    //Check the channel is null.
    if(!m_channel)
    {
        return;
    }
    //Set the volume to channel.
    BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, ((float)volume)/100.0);
    //Save the latest volume size.
    m_volume=getChannelVolume();
}

void KNMusicBackendBassThread::setPosition(const qint64 &position)
{
    //Check the channel is null.
    if(!m_channel)
    {
        return;
    }
    //If the state is stopped and the position is not 0, then changed the state
    //to pause.
    if(m_state==Stopped && position!=0)
    {
        //Change to paused.
        setPlayingState(Paused);
    }
    //Change the position, the unit of the position should be translate into
    //second.
    BASS_ChannelSetPosition(
                m_channel,
                BASS_ChannelSeconds2Bytes(m_channel,
                                          //The position here should be the
                                          //'absolute' position.
                                          //That means it should be the position
                                          //plus the start position.
                                          (double)(m_startPosition+position)
                                          /1000.0),
                BASS_POS_BYTE);
    //Check the position.
    checkPosition();
}

void KNMusicBackendBassThread::setCreateFlags(const DWORD &channelFlags)
{
    //Save the channel flags.
    m_channelFlags=channelFlags;
}

void KNMusicBackendBassThread::checkPosition()
{
    //Get the current position.
    qint64 currentPosition=getChannelPosition();
    //Emit position changed signal.
    emit positionChanged(currentPosition);
    //Check the position is longer than the duration.
    /*
     * Q: Why we still need to do this?
     * A: When cue is playing, it may not stopped at the end of the file.
     *    The callback is only used to solve the position won't reach the end
     *    bug, the track duration stopped will still process here.
     */
    if(currentPosition>=m_duration)
    {
        //Finished the playing.
        finishPlaying();
    }
}

void KNMusicBackendBassThread::threadReachesEnd(HSYNC handle,
                                                DWORD channel,
                                                DWORD data,
                                                void *user)
{
    Q_UNUSED(handle)
    Q_UNUSED(channel)
    Q_UNUSED(data)
    //Finished the playing of the thread.
    //Call the signal function to emit this signal.
    static_cast<KNMusicBackendBassThread *>(user)->reachesFinished();
}

inline void KNMusicBackendBassThread::finishPlaying()
{
    //Stop playing.
    stop();
    //Emit finished signal.
    emit finished();
}

inline void KNMusicBackendBassThread::resetChannelInformation()
{
    //Set the duration to the total duration.
    m_duration=m_totalDuration;
    //Set the start position at the very beginning.
    m_startPosition=0;
    //Set the default end position as the whole file.
    m_endPosition=m_duration;
}

inline void KNMusicBackendBassThread::setPlayingState(const int &state)
{
    //Only process the calling when the state is different.
    if(state!=m_state)
    {
        //Save the new state.
        m_state=state;
        //Emit state changed signal.
        emit stateChanged(m_state);
    }
}

inline void KNMusicBackendBassThread::removeChannelSyncs()
{
    //Get all the handlers.
    for(auto i=m_syncHandlers.begin(); i!=m_syncHandlers.end(); ++i)
    {
        //Remove all the sync from the list.
        BASS_ChannelRemoveSync(m_channel, *i);
    }
    //Clear the hanlder list.
    m_syncHandlers.clear();
}
