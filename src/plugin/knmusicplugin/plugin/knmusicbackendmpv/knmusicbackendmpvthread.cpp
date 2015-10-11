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

#include "knmusicbackendmpvthread.h"

#include <QDebug>

KNMusicBackendMpvThread::KNMusicBackendMpvThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_totalDuration(-1),
    m_duration(-1),
    m_startPosition(-1),
    m_endPosition(-1),
    mpv(nullptr),
    m_state(MusicUtil::Stopped)
{
    // Qt sets the locale in the QApplication constructor, but libmpv requires
    // the LC_NUMERIC category to be set to "C", so change it back.
    std::setlocale(LC_NUMERIC, "C");
    //Initial the mpv instance.
    mpv=mpv_create();
    //Check out the mpv pointer.
    if(mpv==nullptr)
    {
        //The backend thread start failed.
        return;
    }
    //Enable default bindings, because we're lazy. Normally, a player using
    //mpv as backend would implement its own key bindings.
    mpv_set_option_string(mpv, "input-default-bindings", "yes");
    //Disable keyboard input on the X11 window.
    mpv_set_option_string(mpv, "input-vo-keyboard", "no");
    //Let us receive property change events with MPV_EVENT_PROPERTY_CHANGE if
    //this property changes.
    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);

    //From this point on, the wakeup function will be called. The callback
    //can come from any thread, so we use the QueuedConnection mechanism to
    //relay the wakeup in a thread-safe way.
    connect(this, &KNMusicBackendMpvThread::mpv_events,
            this, &KNMusicBackendMpvThread::on_mpv_events,
            Qt::QueuedConnection);
    //Set wake up callback.
    mpv_set_wakeup_callback(mpv, wakeup, this);
    //Initialize the mpv instance.
    mpv_initialize(mpv);
}

bool KNMusicBackendMpvThread::loadFile(const QString &filePath)
{
    //Check mpv instance first.
    if(mpv==nullptr)
    {
        //If the mpv instance is not loaded, then it cannot be true.
        return false;
    }
    //Reset the state.
    setState(MusicUtil::Stopped);
    //Get the file name.
    const QByteArray c_filename=filePath.toUtf8();
    //Generate the commands.
    const char *loadArgs[]={"loadfile", c_filename.data(), NULL};
    //Write the data to mpv.
    if(mpv_command(mpv, loadArgs)<0)
    {
        //Emit failed.
        emit loadFailed();
        //Failed to load the file.
        return false;
    }
    //Pause the mpv instance.
//    pause();
    //Get the duration.

    //Emit loaded success signal.
//    emit loadSuccess();
    return true;
}

void KNMusicBackendMpvThread::reset()
{
    ;
}

void KNMusicBackendMpvThread::stop()
{

}

void KNMusicBackendMpvThread::play()
{
    //Check the mpv instance first.
    //Check out the state.
    if(mpv==nullptr)
    {
        //If the mpv instance is not loaded, then it cannot be true.
        return;
    }
    //Disable the pause.
    bool pauseFlag=false;
    mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &pauseFlag);
    //Set state to playing state.
    setState(MusicUtil::Playing);
}

void KNMusicBackendMpvThread::pause()
{
    //Check the mpv instance first.
    if(mpv==nullptr)
    {
        //If the mpv instance is not loaded, then it cannot be true.
        return;
    }
    //Enable the pause.
    bool pauseFlag=true;
    mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &pauseFlag);
    //Set state to pausd.
    setState(MusicUtil::Paused);
}

int KNMusicBackendMpvThread::volume()
{
}

qint64 KNMusicBackendMpvThread::duration()
{
    ;
}

qint64 KNMusicBackendMpvThread::position()
{
}

int KNMusicBackendMpvThread::state() const
{
    ;
}

void KNMusicBackendMpvThread::setPlaySection(const qint64 &start,
                                             const qint64 &duration)
{
//    //Save the start position and duration.
//    m_startPosition=start;
//    //Check out the duration.
//    if(duration!=-1)
//    {
//        //Save the valid position, and calculate the new end position.
//        m_duration=duration;
//        //Calculate the end position of the file.
//        m_endPosition=m_startPosition+m_duration;
//    }
//    //Check out the total duration, it the file is loaded, we have to check the
//    //start and end position.
//    if(m_totalDuration!=-1)
//    {
//        //Check and update the positions.
//        checkStartAndEndPosition();
//    }
}

void KNMusicBackendMpvThread::setVolume(const int &volume)
{
}

void KNMusicBackendMpvThread::setPosition(const qint64 &position)
{
}

void KNMusicBackendMpvThread::on_mpv_events()
{
    while (mpv)
    {
        //Get the mpv_event from the mpv.
        mpv_event *event=mpv_wait_event(mpv, 0);
        //Check out the event.
        if(event->event_id==MPV_EVENT_NONE)
        {
            //All the event has been proessed.
            break;
        }
        //Handle the mpv event.
        switch(event->event_id)
        {
        case MPV_EVENT_PROPERTY_CHANGE:
        {
            //Get the event property.
            mpv_event_property *prop=(mpv_event_property *)event->data;
            //Check out the name of prop.
            if(strcmp(prop->name, "time-pos")==0)
            {
                //Get the prop data.
                double time=*(double *)prop->data;
                //Emit the time as the position changed signal.
//                emit positionChanged(time*1000.0);
            }
            break;
        }
        default:
            //Ignore uninteresting or unknown events.
            break;
        }
    }
}

void KNMusicBackendMpvThread::wakeup(void *ctx)
{
    //Recast this to mpv thread.
    KNMusicBackendMpvThread *mpvThread=(KNMusicBackendMpvThread *)ctx;
    //Emit the process event signal.
    emit mpvThread->mpv_events();
}

inline void KNMusicBackendMpvThread::setState(int state)
{
    //Save the state.
    m_state=state;
    //Emit the state changed signal.
    emit stateChanged(m_state);
}
