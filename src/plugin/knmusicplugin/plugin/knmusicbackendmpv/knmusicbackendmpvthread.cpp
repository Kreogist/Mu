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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <mpv/qthelper.hpp>

#include "knmusicbackendmpvthread.h"

#include <QDebug>

static void mpvWakeUp(void *context)
{
    // This callback is invoked from any mpv thread (but possibly also
    // recursively from a thread that is calling the mpv API). Just notify
    // the Qt GUI thread to wake up (so that it can process events with
    // mpv_wait_event()), and return as quickly as possible.
    emit static_cast<KNMusicBackendMpvThread *>(context)->mpvEvent();
}

KNMusicBackendMpvThread::KNMusicBackendMpvThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_mpvHandle(mpv_create()),
    m_startPosition(-1),
    m_endPosition(-1),
    m_duration(-1),
    m_totalDuration(-1),
    m_position(-1),
    m_state(MusicUtil::Stopped),
    m_mpvState(Idle),
    m_sectionSet(false)
{
    //Check the mpv handle.
    if(!m_mpvHandle)
    {
        //Failed to create the mpv instance.
        return;
    }
    //Initial the properties.
    // Let us receive property change events with MPV_EVENT_PROPERTY_CHANGE if
    // this property changes.
    mpv_observe_property(m_mpvHandle, 0, "playback-time", MPV_FORMAT_DOUBLE);
    // From this point on, the wakeup function will be called. The callback
    // can come from any thread, so we use the QueuedConnection mechanism to
    // relay the wakeup in a thread-safe way.
    connect(this, &KNMusicBackendMpvThread::mpvEvent,
            this, &KNMusicBackendMpvThread::onMpvEvent,
            Qt::QueuedConnection);
    mpv_set_wakeup_callback(m_mpvHandle, mpvWakeUp, this);
    //Initialized the mpv handle.
    mpv_initialize(m_mpvHandle);
}

KNMusicBackendMpvThread::~KNMusicBackendMpvThread()
{
    //Reset the current thread.
    reset();
    //Check the handle is null or not.
    if(m_mpvHandle)
    {
        //Destory the handle.
        mpv_terminate_destroy(m_mpvHandle);
        //Clear the handle.
        m_mpvHandle=nullptr;
    }
}

bool KNMusicBackendMpvThread::loadFile(const QString &filePath)
{
    //Check the mpv handle.
    if(!m_mpvHandle)
    {
        //The mpv handle is null.
        return false;
    }
    //Reset the player.
    reset();
    //Change the file path to UTF-8 array.
    const QByteArray utfFilePath=filePath.toUtf8();
    //Construct the arguments.
    const char *arguments[]={"loadfile", utfFilePath.data(), NULL};
    //Execute the command.
    if(mpv_command(m_mpvHandle, arguments)!=0)
    {
        //Return false.
        return false;
    }
    //Load complete.
    return true;
}

void KNMusicBackendMpvThread::reset()
{
    //Check the mpv handle.
    if(!m_mpvHandle)
    {
        //The mpv handle is null.
        return;
    }
    //The command stop will stop playback and clear playlist.
    //Construct the arguments.
    const char *arguments[]={"stop"};
    //Use async command.
    mpv_command(m_mpvHandle, arguments);
    //Reset all the parameters.
    m_totalDuration=-1;
    m_position=-1;
    //Check the current state is stopped or not.
    setPlayingState(Stopped);
}

void KNMusicBackendMpvThread::stop()
{
    //Check:
    // 1. The state is already stopped.
    // 2. The channel is null.
    if(Stopped==m_state || (!m_mpvHandle))
    {
        return;
    }
    //Reset the position to the start position.
    /********Debug*********/
    setPosition(5000);
    qDebug()<<"Fuck?!";
//    //Pause and reset to the beginning.
//    pause();
    //Update the state.
    setPlayingState(Stopped);
    //Emit stopped signal.
    emit stopped();
}

void KNMusicBackendMpvThread::play()
{
    //Check the mpv handle.
    if(!m_mpvHandle)
    {
        //The mpv handle is null.
        return;
    }
    //Check the playing state before.
    if(Stopped==m_state)
    {
        //Reset the position to fit track playing.
        setPosition(0);
    }
    //Use async command.
    int flag=0;
    mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
}

void KNMusicBackendMpvThread::pause()
{
    //Check the mpv handle.
    if(!m_mpvHandle)
    {
        //The mpv handle is null.
        return;
    }
    //Use async command.
    int flag=1;
    mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
    //Update the state.
    setPlayingState(MusicUtil::Paused);
}

int KNMusicBackendMpvThread::volume()
{
    return m_volume;
}

qint64 KNMusicBackendMpvThread::duration()
{
    return m_totalDuration;
}

qint64 KNMusicBackendMpvThread::position()
{
    //Get the time pos of the mpv handle.
    return m_position;
}

int KNMusicBackendMpvThread::state() const
{
    return m_state;
}

void KNMusicBackendMpvThread::setPlaySection(const qint64 &start,
                                             const qint64 &duration)
{
    //Save the start position and duration.
    m_startPosition=start;
    //Check out the duration.
    if(duration!=-1)
    {
        //Save the valid position, and calculate the new end position.
        m_duration=duration;
        //Calculate the end position of the file.
        m_endPosition=m_startPosition+m_duration;
    }
    //Set the flag.
    m_sectionSet=true;
    //Check out the total duration, it the file is loaded, we have to check the
    //start and end position.
    if(m_totalDuration!=-1)
    {
        //Check and update the positions.
        updateStartAndEndPosition();
    }
}

void KNMusicBackendMpvThread::save()
{
    ;
}

void KNMusicBackendMpvThread::restore(const QString &updatedFilePath)
{
    ;
}

void KNMusicBackendMpvThread::setVolume(const int &volume)
{
    //Save the volume data.
    m_volume=volume;
    //Update the volume data.
    double mpvVolume=(double)volume / 100.0;
    //Set the volume, check the playing state.
    //Set the ao-volume property.
    mpv_set_property(m_mpvHandle, "ao-volume", MPV_FORMAT_DOUBLE, &mpvVolume);
}

void KNMusicBackendMpvThread::setPosition(const qint64 &position)
{
    //Recalculate the position data.
    double mpvPosition=(double)position/1000.0;
    //Set the time pos, using the seek command.
    const QByteArray positionData = QString::number(mpvPosition).toUtf8();
    //Prepare the arguments.
    const char *arguments[] = {"seek",
                               positionData.constData(),
                               "absolute",
                               NULL};
    //Execute the command.
    mpv_command(m_mpvHandle, arguments);
}

void KNMusicBackendMpvThread::onMpvEvent()
{
    //Process all the events.
    mpv_event *event=mpv_wait_event(m_mpvHandle, 0);
    //Check the event_id.
    while(event && event->event_id != MPV_EVENT_NONE)
    {
        //Handle the event.
        switch (event->event_id)
        {
        case MPV_EVENT_FILE_LOADED:
        {
            //Pause the playing.
            pause();
            //Get the duration value, unit second.
            double duration;
            mpv_get_property(m_mpvHandle, "duration", MPV_FORMAT_DOUBLE,
                             &duration);
            //Save the total duration.
            m_totalDuration=(qint64)(duration*1000.0);
            //Check if section has been set then update the position.
            if(m_sectionSet)
            {
                //Check out the start and end position.
                updateStartAndEndPosition();
            }
            //If we comes to here, that means the file is loaded.
            emit loadSuccess();
            break;
        }
        case MPV_EVENT_PROPERTY_CHANGE:
        {
            //Get the property.
            mpv_event_property *property = (mpv_event_property *)event->data;
            //Check the property name.
            if (strcmp(property->name, "playback-time") == 0)
            {
                //The time position changed.
                //The time data should be a double format value.
                if (property->format == MPV_FORMAT_DOUBLE)
                {
                    //Get the time value, unit second..
                    double time=*(double *)property->data;
                    //Save the position.
                    m_position=(qint64)(time*1000.0);
                    //Emit the time changed function.
                    emit positionChanged(m_position);
                    //Check the position.
                    if(m_endPosition>0 && m_position>=m_endPosition)
                    {
                        //Stop the playing.
                        stop();
                        //Emit the finished signal.
                        emit finished();
                    }
                }
            }
            break;
        }
        case MPV_EVENT_UNPAUSE:
        {
            //Update the state.
            setPlayingState(MusicUtil::Playing);
            //Update the volume state.
            setVolume(m_volume);
            //Update the
            break;
        }
        case MPV_EVENT_PAUSE:
        {
            break;
        }
        case MPV_EVENT_END_FILE:
            //Stop the mpv handle.
            stop();
            //Emit finished signal.
            emit finished();
            break;
        default:
            break;
        };
        //Fetch next event.
        event=mpv_wait_event(m_mpvHandle, 0);
    }
}

inline void KNMusicBackendMpvThread::setPlayingState(int state)
{
    //Check state first.
    if(state!=m_state)
    {
        //Save the state.
        m_state=state;
        //Emit the changed signal.
        emit stateChanged(m_state);
    }
}

inline void KNMusicBackendMpvThread::updateStartAndEndPosition()
{
    //Check out the start position, if the start position is -1, then we have to
    //play the whole part of the song.
    if(m_startPosition==-1)
    {
        //Reset the start position to 0.
        m_startPosition=0;
        //Set the whole file as the end position and the duration is the total.
        m_duration=m_totalDuration;
        m_endPosition=-1;
    }
    else
    {
        //Check out the end position and start position.
        if(m_endPosition>m_totalDuration)
        {
            //The end position cannot greater than total duration.
            m_endPosition=m_totalDuration;
        }
        //Check out the start position is greater than end position
        if(m_startPosition > m_endPosition)
        {
            //Set start position lesser than end position.
            m_startPosition=m_endPosition;
        }
        //Calculate the duration.
        m_duration=m_endPosition-m_startPosition;
    }
    //The duration is changed after check.
    emit durationChanged(m_duration);
    //We need to move the playing position to the start position.
    setPosition(0);
}
