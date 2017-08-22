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
#include <QApplication>
#include <QEasingCurve>
#include <QWidget>

#include <mpv/qthelper.hpp>

#include "knglobal.h"

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
    m_filePath(QString()),
    m_audioDevice(QString()),
    m_container(new QWidget(knGlobal->mainWindow())),
    m_mpvHandle(nullptr),
    m_startPosition(-1),
    m_endPosition(-1),
    m_duration(-1),
    m_totalDuration(-1),
    m_position(-1),
    m_state(MusicUtil::Stopped),
    m_sectionSet(false),
    m_fileLoaded(false),
    m_restoreFlag(false),
    m_forceStereo(false)
{
    //Configure the container.
    m_container->hide();
    m_container->setAttribute(Qt::WA_DontCreateNativeAncestors);
    m_container->setAttribute(Qt::WA_NativeWindow);
    // From this point on, the wakeup function will be called. The callback
    // can come from any thread, so we use the QueuedConnection mechanism to
    // relay the wakeup in a thread-safe way.
    connect(this, &KNMusicBackendMpvThread::mpvEvent,
            this, &KNMusicBackendMpvThread::onMpvEvent,
            Qt::QueuedConnection);
}

KNMusicBackendMpvThread::~KNMusicBackendMpvThread()
{
    //Reset the current thread.
    reset();
}

bool KNMusicBackendMpvThread::loadFile(const QString &filePath)
{
    //Reset the player.
    reset();
    //Build the MPV player handle.
    //Load the music file.
    return buildMpvHandle() && loadMusicFile(filePath);
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
    //Execute command.
    mpv_command(m_mpvHandle, arguments);
    //Check the current state is stopped or not.
    setPlayingState(Stopped);
    //Clear the file path.
    m_filePath=QString();
    //Reset all the parameters.
    m_startPosition=-1;
    m_endPosition=-1;
    m_duration=-1;
    m_totalDuration=-1;
    m_position=-1;
    //Clear the MPV handle.
    clearMpvHandle();
}

void KNMusicBackendMpvThread::stop()
{
    //Check:
    // 1. The state is already stopped.
    // 2. The channel is null.
    if(!m_mpvHandle)
    {
        return;
    }
    //Use sync command.
    int flag=1;
    mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
    //Actually stop the handle.
    //Construct the arguments.
    const char *arguments[]={"stop"};
    //Use async command.
    mpv_command(m_mpvHandle, arguments);
    //Reset the current position.
    m_position=m_startPosition;
    //Change the file loaded state.
    m_fileLoaded=false;
    //Manually emitted position changed.
    emit positionChanged(0);
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
        /*
         * MPV when play to the end of the file, it will clear the current
         * playing file. So the file loaded flag is needed here.
         */
        //Check the file loaded state.
        if(!m_fileLoaded && !m_filePath.isEmpty())
        {
            //Need to reload the music file.
            loadMusicFile(m_filePath);
        }
        //Reset the position to fit track playing.
        setPosition(m_position==-1?0:m_position);
    }
    //Change the pause property for playing.
    int flag=0;
    mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
    //Update the playing state.
    setPlayingState(Playing);
}

void KNMusicBackendMpvThread::pause()
{
    //Check the mpv handle.
    if(!m_mpvHandle || m_state!=Playing)
    {
        //The mpv handle is null.
        return;
    }
    //Use sync command.
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
    return m_position-m_startPosition;
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

void KNMusicBackendMpvThread::setCreateFlags(const QString &deviceName,
                                             int bufferLength,
                                             int sampleRate,
                                             bool forceStero)
{
    //Save the audio device name.
    m_audioDevice=deviceName;
    //Get the buffer length.
    qreal mpvBufferLength=(qreal)bufferLength/1000.0;
    //Translate the float to string.
    m_bufferLength=QString::number(mpvBufferLength, 'g', 3);
    //Save the device sample rate.
    m_sampleRate=sampleRate;
    //Save the stero settings.
    m_forceStereo=forceStero;
}

void KNMusicBackendMpvThread::save()
{
    //Stop the current playing state.
    //Construct the arguments.
    const char *arguments[]={"stop"};
    //Use async command.
    mpv_command(m_mpvHandle, arguments);
    //Release the MPV handle.
    clearMpvHandle();
}

void KNMusicBackendMpvThread::restore(const QString &updatedFilePath)
{
    //Set the restore flag.
    m_restoreFlag=true;
    //Save the new file path name.
    m_filePath=updatedFilePath;
    //Build the MPV handle.
    //Load the music.
    if(!buildMpvHandle() || !loadMusicFile(m_filePath))
    {
        //Failed to restore the state.
        return;
    }
    //Reset the position.
    setPosition(m_position-m_startPosition);
    //Check the playing state.
    if(m_state==Playing)
    {
        //Start to play the music.
        //Change the pause property for playing.
        int flag=0;
        mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
    }
}

void KNMusicBackendMpvThread::setVolume(const int &volume)
{
    //Save the volume data.
    m_volume=volume;
    //Check the mpv handle.
    if(m_mpvHandle)
    {
        //Update the volume data.
        double mpvVolume=(double)volume/100.0;
        /*
         * Using the scaler to scale the volume. It seems that the volume here
         * is more like the db unit sound. The sound under 50% doesn't change
         * much, but over 50% is to too sensitive. So I need to short the part
         * 0-50% and expand the 50%-100%, which is the Out series curves.
         */
        //
        QEasingCurve volumeCurve=QEasingCurve(QEasingCurve::OutQuad);
        mpvVolume=volumeCurve.valueForProgress(mpvVolume/100.0)*100.0;
        //Set the volume property.
        /*
         * From the source code of the BakaMPlayer, and nearly all the
         * documentation says that here should be ao-volume.
         * But it doesn't works on Ubuntu 16.04 with mpv 0.16. After I read
         * through all these codes, I find a property in /player/command.c at
         * line 3560.
         *
         *      {"volume", mp_property_volume},
         *
         * Then, I change the property name from ao-volume to volume. It fucky
         * works. I check through the version from 0.16 to 0.26. All these
         * versions contain the volume property.
         */
        mpv_set_property(m_mpvHandle, "volume", MPV_FORMAT_DOUBLE,
                         &mpvVolume);
    }
}

void KNMusicBackendMpvThread::setPosition(const qint64 &position)
{
    //Prepare the infile position.
    qint64 inFilePosition=position;
    //Check the start position.
    if(m_startPosition>-1 && m_endPosition>-1)
    {
        //Calculate the in file position.
        inFilePosition+=m_startPosition;
        //Check the position is valid or not.
        if(inFilePosition>m_endPosition)
        {
            //Update the in file position to valid data.
            inFilePosition=m_endPosition;
        }
    }
    //Recalculate the position data.
    double mpvPosition=(double)(inFilePosition)/1000.0;
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
    mpv_event *event=nullptr;
    //Check the handle first.
    if(m_mpvHandle)
    {
        //Get the event.
        event=mpv_wait_event(m_mpvHandle, 0);
    }
    //Check the event_id.
    while(event && event->event_id != MPV_EVENT_NONE)
    {
        //Handle the event.
        switch (event->event_id)
        {
        case MPV_EVENT_FILE_LOADED:
        {
            //Check the restore flag.
            if(m_restoreFlag)
            {
                //Cancel the restore flag.
                m_restoreFlag=false;
                //Do nothing when restoring the state.
                return;
            }
            //Set file loaded flag.
            m_fileLoaded=true;
            //Use sync command.
            int flag=1;
            mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
            //Update the state.
            setPlayingState(Paused);
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
                    emit positionChanged(m_position-m_startPosition);
                    //Check the position.
                    if(m_endPosition>0 && m_position>=m_endPosition)
                    {
                        //Execute the finish playing code.
                        finishPlaying();
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
            break;
        }
        case MPV_EVENT_END_FILE:
            //Check the file loaded flag.
            if(m_fileLoaded)
            {
                //Finish playing the music.
                finishPlaying();
            }
            else
            {
                //Failed to load the music file.
                emit loadFailed();
            }
            break;
        default:
            break;
        };
        //Fetch next event.
        //Check the handle first.
        if(m_mpvHandle)
        {
            //Get the event.
            event=mpv_wait_event(m_mpvHandle, 0);
        }
        //Process system event.
        qApp->processEvents();
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

inline bool KNMusicBackendMpvThread::loadMusicFile(const QString &filePath)
{
    //Save the file path.
    m_filePath=filePath;
    //Change the file path to UTF-8 array.
    const QByteArray utfFilePath=filePath.toUtf8();
    //Construct the arguments.
    const char *arguments[]={"loadfile", utfFilePath.data(), NULL};
    //Execute the command.
    return mpv_command(m_mpvHandle, arguments)==0;
}

inline bool KNMusicBackendMpvThread::buildMpvHandle()
{
    //Check the mpv handle is built before.
    if(m_mpvHandle)
    {
        //An existing handle is created.
        return false;
    }
    //Create an mpv handle.
    m_mpvHandle=mpv_create();
    //Check the mpv handle.
    if(!m_mpvHandle)
    {
        //Failed to create the mpv instance.
        return false;
    }
    //Initial the properties.
    // If you have a HWND, use: int64_t wid = (intptr_t)hwnd;
    int64_t wid=m_container->winId();
    mpv_set_option(m_mpvHandle, "wid", MPV_FORMAT_INT64, &wid);
    // *** Patches from Roberto Canteri<roberto.canteri@gmail.com> ***
    //Do not show image for audio tracks.
    mpv_set_option_string(m_mpvHandle, "audio-display", "no");
    //Disable video.
    mpv_set_option_string(m_mpvHandle, "vo", "null");
    // *** Patches End ***
    //Check the audio settings.
    if(!m_audioDevice.isEmpty())
    {
        //A specific audio device is set.
        mpv_set_option_string(m_mpvHandle, "audio-device",
                              m_audioDevice.toLatin1().data());
    }
    if(!m_bufferLength.isEmpty())
    {
        //A specific audio buffer length is set.
        mpv_set_option_string(m_mpvHandle, "audio-buffer",
                              m_bufferLength.toLatin1().data());
    }
    if(m_forceStereo)
    {
        //The audio channels force to stereo.
        mpv_set_option_string(m_mpvHandle, "audio-channels", "stereo");
    }
    if(m_sampleRate!=-1)
    {
        //Update the audio device sample rate.
        mpv_set_option_string(m_mpvHandle, "audio-samplerate",
                              QString::number(m_sampleRate).toLatin1().data());
    }
    // Let us receive property change events with MPV_EVENT_PROPERTY_CHANGE if
    // this property changes.
    mpv_observe_property(m_mpvHandle, 0, "playback-time", MPV_FORMAT_DOUBLE);
    //Set the wake up callback for the specific handle.
    mpv_set_wakeup_callback(m_mpvHandle, mpvWakeUp, this);
    //Initialized the mpv handle.
    mpv_initialize(m_mpvHandle);
    //Build the MPV handle successfully.
    return true;
}

inline void KNMusicBackendMpvThread::clearMpvHandle()
{
    //Check the handle is null or not.
    if(m_mpvHandle)
    {
        //Destroy the handle.
        mpv_terminate_destroy(m_mpvHandle);
        //Clear the handle.
        m_mpvHandle=nullptr;
    }
}

inline void KNMusicBackendMpvThread::finishPlaying()
{
    //Set the file loaded flag to false.
    m_fileLoaded=false;
    //Stop the playing.
    stop();
    //Emit the finished signal.
    emit finished();
}
