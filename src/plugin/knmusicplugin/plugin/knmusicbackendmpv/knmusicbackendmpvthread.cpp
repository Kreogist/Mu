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
    m_totalDuration(-1),
    m_position(-1),
    m_state(MusicUtil::Stopped)
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
    mpv_observe_property(m_mpvHandle, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpvHandle, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpvHandle, 0, "pause", MPV_FORMAT_FLAG);
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
        qDebug()<<"No mpv handle?!";
        //The mpv handle is null.
        return false;
    }
    //Change the file path to UTF-8 array.
    const QByteArray utfFilePath=filePath.toUtf8();
    //Construct the arguments.
    const char *arguments[]={"loadfile", utfFilePath.data(), NULL};
    //Use async command.
    mpv_command_async(m_mpvHandle, 0, arguments);
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
    mpv_command_async(m_mpvHandle, 0, arguments);
    //Reset all the parameters.
    m_totalDuration=-1;
    m_position=-1;
}

void KNMusicBackendMpvThread::stop()
{
    //Pause and reset to the beginning.
    pause();
    setPosition(0);
}

void KNMusicBackendMpvThread::play()
{
    //Check the mpv handle.
    if(!m_mpvHandle)
    {
        //The mpv handle is null.
        return;
    }
    //Use async command.
    int flag=0;
    mpv_set_property_async(m_mpvHandle, 0, "pause", MPV_FORMAT_FLAG, &flag);
    //Update the volume.
    setVolume(m_volume);
    //Update the state.
    setPlayingState(MusicUtil::Playing);
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
    mpv_set_property_async(m_mpvHandle, 0, "pause", MPV_FORMAT_FLAG, &flag);
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
    ;
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
    mpv_set_property_async(m_mpvHandle,
                           0,
                           "ao-volume",
                           MPV_FORMAT_DOUBLE,
                           &mpvVolume);
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
    mpv_command_async(m_mpvHandle, 0, arguments);
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
        case MPV_EVENT_PROPERTY_CHANGE:
        {
            //Get the property.
            mpv_event_property *property = (mpv_event_property *)event->data;
            //Check the property name.
            if (strcmp(property->name, "time-pos") == 0)
            {
                //The time position changed.
                //The time data should be a double format value.
                if (property->format == MPV_FORMAT_DOUBLE)
                {
                    //Get the time value, unit second..
                    double time = *(double *)property->data;
                    //Save the position.
                    m_position=(qint64)time*1000.0;
                    //Emit the time changed function.
                    emit positionChanged(m_position);
                }
            }
            else if (strcmp(property->name, "duration") == 0)
            {
                //Duration information get there.
                if (property->format == MPV_FORMAT_DOUBLE)
                {
                    //Get the duration value, unit second.
                    double duration = *(double *)property->data;
                    //Save the total duration.
                    m_totalDuration=(qint64)(duration*1000.0);
                    //The duration is changed.
                    emit durationChanged(m_totalDuration);
                }
            }
            else if (strcmp(property->name, "pause") == 0)
            {
                //Check the pause state.
                if (property->format == MPV_FORMAT_FLAG)
                {
                    //Get the paused state.
                    int pauseState = *(int *)property->data;
                    //Check paused state.
                    if(pauseState == 0)
                    {
                        //Update the volume state.
                        setVolume(m_volume);
                    }
                }
            }
            break;
        }
        default:
            break;
        };
        //Fetch next event.
        event=mpv_wait_event(m_mpvHandle, 0);
    }
}

void KNMusicBackendMpvThread::setPlayingState(int state)
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
