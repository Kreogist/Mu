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
#include <clocale>

#include <QCoreApplication>

#include "knmusicbackendmpvthread.h"

#include <QDebug>

KNMusicBackendMpvThread::KNMusicBackendMpvThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_volumeCurve(QEasingCurve(QEasingCurve::OutQuad)),
    m_filePath(QString()),
    m_totalDuration(-1),
    m_duration(-1),
    m_startPosition(-1),
    m_endPosition(-1),
    m_mpvHandle(nullptr),
    m_state(MusicUtil::Stopped),
    m_volumeSize(100)
{
    //Initial the locale settings.
    setlocale(LC_NUMERIC, "C");
    //Create the mpv handle.
    m_mpvHandle=mpv_create();
    //Check out the mpv handle.
    if(!m_mpvHandle)
    {
        //Cannot create an instance for mpv.
        return;
    }
    //Set mpv options.
    // No mouse handling.
    mpv_set_option_string(m_mpvHandle, "input-cursor", "no");
    // No cursor-autohide.
    mpv_set_option_string(m_mpvHandle, "cursor-autohide", "no");
    mpv_set_option_string(m_mpvHandle, "no-video", NULL);

    // get updates when these properties change
    mpv_observe_property(m_mpvHandle, 0, "time-pos", MPV_FORMAT_DOUBLE);

    // setup callback event handling
    mpv_set_wakeup_callback(m_mpvHandle,
                            KNMusicBackendMpvThread::instanceWakeUp,
                            this);
    //Initialize the mpv handler.
    mpv_initialize(m_mpvHandle);
}

KNMusicBackendMpvThread::~KNMusicBackendMpvThread()
{
    //Destory mpv to recover memory.
    if(m_mpvHandle)
    {
        //Terminate and destory.
        mpv_terminate_destroy(m_mpvHandle);
    }
}

bool KNMusicBackendMpvThread::loadFile(const QString &filePath)
{
    //Check out the file path.
    if(m_filePath==filePath)
    {
        ;
    }
    //Save the load file path.
    m_filePath=filePath;
    //Reset the thread data first.
    m_totalDuration=-1;
    resetParameter();
    //Open file path in the mpv handle.
    //Prepare load file commands.
    const char *args[] = {"loadfile",
                          filePath.toUtf8().constData(),
                          NULL};
    //Launch command.
    exeCommand(args);
    //Give back loaded signal.
    return true;
}

void KNMusicBackendMpvThread::reset()
{
    //Stop the instance.
    //!FIXME: change to real reset.
    pause();
}

void KNMusicBackendMpvThread::stop()
{
    //It seems that I did the same thing as BakaMPlayer.
    //Restart the thread.
    setPosition(0);
    //Pause the thread.
    pause();
}

void KNMusicBackendMpvThread::play()
{
    //Check out the playing state.
    if(m_state!=MusicUtil::Playing && m_mpvHandle)
    {
        //Prepare the property data.
        int f=0;
        //Use async way to play the handle.
        mpv_set_property_async(m_mpvHandle,
                               0,
                               "pause",
                               MPV_FORMAT_FLAG,
                               &f);
    }
}

void KNMusicBackendMpvThread::pause()
{
    //Check out playing state.
    if(m_state==MusicUtil::Playing && m_mpvHandle)
    {
        //Prepare the property data.
        int f=1;
        //Use async way to pause the handle.
        mpv_set_property_async(m_mpvHandle,
                               0,
                               "pause",
                               MPV_FORMAT_FLAG,
                               &f);
    }
}

int KNMusicBackendMpvThread::volume()
{
    return m_volumeSize;
}

qint64 KNMusicBackendMpvThread::duration()
{
    //Give back the duration data.
    return m_duration;
}

qint64 KNMusicBackendMpvThread::position()
{
    ;
}

int KNMusicBackendMpvThread::state() const
{
    //Give back the state.
    return m_state;
}

void KNMusicBackendMpvThread::setPlaySection(const qint64 &start,
                                             const qint64 &duration)
{
    //Save up the start and end duration.
    m_startPosition=start;
    //Check out the duration.
    if(duration!=-1)
    {
        //Save the valid position.
        m_duration=duration;
        //Calculate the end position.
        m_endPosition=m_startPosition+m_duration;
    }
    //Check out the total duration here.
    if(m_totalDuration!=-1)
    {
        //Check and update the positions.
        checkStartAndEndPosition();
    }
}

void KNMusicBackendMpvThread::setVolume(const int &volume)
{
    //Check out the volume size.
    if(volume>100)
    {
        m_volumeSize=100;
    }
    else if(volume<0)
    {
        m_volumeSize=0;
    }
    else
    {
        m_volumeSize=volume;
    }
    //Set the volume size.
    //Generate the volume size data.
    const char *args[]=
    {
        "set",
        "volume",
        QString::number((int)(m_volumeCurve.valueForProgress(
            (qreal)m_volumeSize/100.0)*100.0)).toUtf8().constData(),
        NULL
    };
    //Use async command to set volume.
    exeAsyncCommand(args);
}

void KNMusicBackendMpvThread::setPosition(const qint64 &position)
{
    //Check out the mpv handler.
    if(m_mpvHandle) //!FIXME: Check file as well.
    {
        qDebug()<<"Here?!"<<QString::number((double)(m_startPosition+position)/1000.0,
                                            'g',
                                            6);
        //Use async to seek the file.
        QByteArray seekTarget=
                QString::number((double)(m_startPosition+position)/1000.0,
                                'g',
                                6).toUtf8();
        //Prepare the command.
        const char *args[]=
        {
            "seek",
            seekTarget.constData(),
            "absolute"
        };
        //Execute async command.
        exeAsyncCommand(args);
    }
}

bool KNMusicBackendMpvThread::event(QEvent *event)
{
    //Check out the event type.
    if(event->type()==QEvent::User)
    {
        //Get all the event until there no event.
        while(m_mpvHandle)
        {
            //Get the mpv event.
            mpv_event *mpvEvent=mpv_wait_event(m_mpvHandle, 0);
            //Check out the event pointer.
            if(mpvEvent==nullptr ||
                    MPV_EVENT_NONE == mpvEvent->event_id)
            {
                //All the event has been processed.
                break;
            }
            //Check out whether there's an error occurs.
            if(mpvEvent->error < 0)
            {
                //!FIXME: Do something here.
            }
            switch(mpvEvent->event_id)
            {
            case MPV_EVENT_PROPERTY_CHANGE:
            {
                //Cast the mpv event data to event property.
                mpv_event_property *prop=(mpv_event_property *)mpvEvent->data;
                //Check out the property name.
                // From bakamplayer:
                // playback-time does the same thing as time-pos but works for
                // streaming media
                if(strcmp(prop->name, "time-pos") == 0)
                {
                    //Cehck the format.
                    if(MPV_FORMAT_DOUBLE==prop->format)
                    {
                        //Output the playback time.
                        emit positionChanged((qint64)((*(double *)prop->data)*1000.0)-m_startPosition);
                    }
                }
                //Mission finished.
                break;
            }
                //This event is totally different, though I don't know why.
                //BakaMPlayer says it is.
            case MPV_EVENT_START_FILE:
            {
                break;
            }
            case MPV_EVENT_FILE_LOADED:
            {
                //Pause it right after the file loaded.
                pause();
                //Reset the playing state.
                m_state=MusicUtil::Stopped;
                //Emit the state changed signal.
                emit stateChanged(m_state);
                //We have to update all the file inforamtion here.
                //Get the duration of the file.
                double mediaDuration;
                //Get the property.
                mpv_get_property(m_mpvHandle,
                                 "length",
                                 MPV_FORMAT_DOUBLE,
                                 &mediaDuration);
                //Save the total duration.
                qint64 propertyDuration=(qint64)(mediaDuration*1000.0);
                //Check property duration
                if(propertyDuration>0)
                {
                    //For valid duration.
                    m_totalDuration=propertyDuration;
                    //Update the end position.
                    checkStartAndEndPosition();
                    //Emit the loaded signal.
                    emit loadSuccess();
                }
                //Mission complete.
                break;
            }
            case MPV_EVENT_PAUSE:
            {
                //We should save the state as pause.
                //We may never have stop.
                m_state=MusicUtil::Paused;
                //Emit the state changed signal.
                emit stateChanged(m_state);
                break;
            }
            case MPV_EVENT_UNPAUSE:
            {
                //We should save the state as playing.
                m_state=MusicUtil::Playing;
                //Emit the state changed signal.
                emit stateChanged(m_state);
                //Sync the playing volume.
                setVolume(m_volumeSize);
                break;
            }
            case MPV_EVENT_END_FILE:
            {
                //It should be stopped.
                m_state=MusicUtil::Stopped;
                //Emit the state changed signal.
                emit stateChanged(m_state);
                break;
            }
            }
        }
    }
    //For others, do the defualt event.
    return KNMusicStandardBackendThread::event(event);
}

void KNMusicBackendMpvThread::instanceWakeUp(void *context)
{
    //Recast the context as a mpv thread.
    KNMusicBackendMpvThread *mpvThread=(KNMusicBackendMpvThread *)context;
    //Post up a user event.
    QCoreApplication::postEvent(mpvThread, new QEvent(QEvent::User));
}

inline void KNMusicBackendMpvThread::resetParameter()
{
    //Reset the parameter.
    m_duration=m_totalDuration;
    //Reset the start and end position.
    m_startPosition=-1;
    m_endPosition=-1;
}

inline void KNMusicBackendMpvThread::exeCommand(const char *args[])
{
    //Check mpv pointer first.
    if(m_mpvHandle)
    {
        //Launch mpv command.
        mpv_command(m_mpvHandle, args);
    }
}

inline void KNMusicBackendMpvThread::exeAsyncCommand(const char *args[])
{
    //Check mpv pointer first.
    if(m_mpvHandle)
    {
        //Launch mpv async command.
        mpv_command_async(m_mpvHandle, 0, args);
    }
}

inline void KNMusicBackendMpvThread::checkStartAndEndPosition()
{
    //Check out the start position.
    if(m_startPosition==-1)
    {
        //Start position is invalid, then play the whole file.
        m_startPosition=0;
        //Set the whole file as the end position.
        m_duration=m_totalDuration;
        m_endPosition=m_totalDuration;
    }
    else
    {
        //Check out the end position and the start position.
        if(m_endPosition>m_totalDuration)
        {
            //The end position cannot the greater than the total duration.
            m_endPosition=m_totalDuration;
        }
        //Check out the start position to the end position.
        if(m_startPosition > m_endPosition)
        {
            //Set the start position lesser than the end position.
            m_startPosition=m_endPosition;
        }
        //Calculate the duration.
        m_duration=m_endPosition-m_startPosition;
    }
    //Emit the duration changed signal.
    emit durationChanged(m_duration);
    //Move the player to the start position.
    setPosition(0);
}
