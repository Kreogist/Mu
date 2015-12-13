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
#include <QUrl>
#include <QTimer>

#include "knmusicbackendgstreamerthread.h"

#include <QDebug>

/* playbin2 flags */
typedef enum {
  GST_PLAY_FLAG_VIDEO         = (1 << 0), /* We want video output */
  GST_PLAY_FLAG_AUDIO         = (1 << 1), /* We want audio output */
  GST_PLAY_FLAG_TEXT          = (1 << 2)  /* We want subtitle output */
} GstPlayFlags;

KNMusicBackendGStreamerThread::KNMusicBackendGStreamerThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_filePath(QString()),
    m_startPosition(-1),
    m_endPosition(-1),
    m_duration(-1),
    m_totalDuration(-1),
    m_savedPosition(-1),
    m_tick(new QTimer(this)),
    m_playbin(NULL),
    m_seekFlag((GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT)),
    m_state(MusicUtil::Stopped),
    m_volume(10000),
    m_sectionSet(false)
{
    //Configure the timer.
    connect(m_tick, &QTimer::timeout,
            this, &KNMusicBackendGStreamerThread::onActionTick);
    //Link the process event.
    connect(this, &KNMusicBackendGStreamerThread::requireProcessEvent,
            this, &KNMusicBackendGStreamerThread::processEvents,
            Qt::QueuedConnection);
}

KNMusicBackendGStreamerThread::~KNMusicBackendGStreamerThread()
{
    //Reset the pipeline.
    resetPipeline();
}

bool KNMusicBackendGStreamerThread::loadFile(const QString &filePath)
{
    //Check out the file is loaded or not.
    if(filePath==m_filePath)
    {
        //Stop the thread first.
        stop();
        //Reset the parameter.
        resetParameter();
        //Update the duration.
        emit durationChanged(m_duration);
        //Emit the load success signal.
        emit loadSuccess();
        return true;
    }
    //Reset the pipeline.
    if(!rebuildPipeline())
    {
        //If the pipeline cannot be reset, then failed.
        return false;
    }
    //Reset the total duration.
    m_totalDuration=-1;
    //Reset the parameter.
    resetParameter();
    //Save the file path.
    m_filePath=filePath;
    //Load the file.
    {
        //Get the file url from the file path.
        QByteArray localUrl=QUrl::fromLocalFile(filePath).toString().toUtf8();
        //Set the uri to the playbin.
        g_object_set(m_playbin, "uri", localUrl.data(), NULL);
    }
    //Load the file successfully.
    return true;
}

void KNMusicBackendGStreamerThread::reset()
{
    //Stop the ticker.
    m_tick->stop();
    //Clear up the pipeline.
    resetPipeline();
    //Reset the total duration.
    m_totalDuration=-1;
    //Reset the parameter.
    resetParameter();
    //Clear up the file path.
    m_filePath=QString();
}

void KNMusicBackendGStreamerThread::stop()
{
    //Check the pointer.
    if(m_playbin!=NULL && m_state!=MusicUtil::Stopped)
    {
        //Set the state to be play.
        //And get the state changed result.
        gst_element_set_state(m_playbin, GST_STATE_PAUSED);
        //Stop ticking.
        m_tick->stop();
        //Emit the position changed signal.
        emit positionChanged(0);
        //Reset the position.
        setPosition(0);
        //Then the result is success, we can change the state.
        setPlayingState(MusicUtil::Stopped);
    }
}

void KNMusicBackendGStreamerThread::play()
{
    //Check the pointer.
    if(m_playbin && m_state!=MusicUtil::Playing)
    {
        //Set the state to be play.
        //And get the state changed result.
        GstStateChangeReturn stateResult=
                gst_element_set_state(m_playbin, GST_STATE_PLAYING);
        //Check out the result.
        if (GST_STATE_CHANGE_FAILURE != stateResult)
        {
            //Start ticking.
            m_tick->start();
            //Then the result is success, we can change the state.
            setPlayingState(MusicUtil::Playing);
        }
    }
}

void KNMusicBackendGStreamerThread::pause()
{
    //Check the pointer.
    if(m_playbin && m_state==MusicUtil::Playing)
    {
        //Set the state to be play.
        //And get the state changed result.
        GstStateChangeReturn stateResult=
                gst_element_set_state(m_playbin, GST_STATE_PAUSED);
        //Check out the result.
        if (GST_STATE_CHANGE_FAILURE != stateResult)
        {
            //Then the result is success, we can change the state.
            setPlayingState(MusicUtil::Paused);
            //Stop ticking.
            m_tick->stop();
        }
    }
}

int KNMusicBackendGStreamerThread::volume()
{
    //Give back the main volume.
    return m_volume;
}

qint64 KNMusicBackendGStreamerThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendGStreamerThread::position()
{
    //Check out the pipeline.
    if(m_playbin)
    {
        //Generate the gstreamer position.
        gint64 position;
        //Output the data.
        gboolean result=gst_element_query_position(m_playbin,
                                                   GST_FORMAT_TIME,
                                                   &position);
        //Check the result.
        if(result)
        {
            //Give back the position data.
            //Remember it will use nanosecond but not msecond.
            return position/1000000 - m_startPosition;
        }
    }
    //A NULL playbin return a null value.
    return -1;
}

int KNMusicBackendGStreamerThread::state() const
{
    return m_state;
}

void KNMusicBackendGStreamerThread::setPlaySection(const qint64 &start,
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

GstElement *KNMusicBackendGStreamerThread::playbin()
{
    return m_playbin;
}

void KNMusicBackendGStreamerThread::save()
{
    //Stop the ticker.
    m_tick->stop();
    //Save the position of the current thread.
    gst_element_query_position(m_playbin,
                               GST_FORMAT_TIME,
                               &m_savedPosition);
    //Playbin has to be reset first.
    //Check playbin is null or not.
    if(m_playbin!=NULL)
    {
        //If it's not null, reset the state.
        gst_element_set_state(m_playbin, GST_STATE_NULL);
        //Reduce the ref of the playbin.
        gst_object_unref(m_playbin);
        //Reset the playbin pointer.
        m_playbin=NULL;
    }
}

void KNMusicBackendGStreamerThread::restore(const QString &updatedFilePath)
{
    //Check out the saved position, if it's -1, means it never saved before.
    //Ignore the invalid call.
    if(m_savedPosition==-1)
    {
        return;
    }
    //Rebuild the pipeline.
    rebuildPipeline();
    //Check out the updated file path.
    QString restoreFilePath=
            updatedFilePath.isEmpty()?m_filePath:updatedFilePath;
    //Reload the playbin.
    {
        //Get the file url from the file path.
        QByteArray localUrl=
                QUrl::fromLocalFile(restoreFilePath).toString().toUtf8();
        //Set the uri to the playbin.
        g_object_set(m_playbin, "uri", localUrl.data(), NULL);
    }
    //Reset the position, seek the playbin pipeline.
    gst_element_seek_simple(m_playbin,
                            GST_FORMAT_TIME,
                            m_seekFlag,
                            m_savedPosition);
    //Check out the state.
    if(m_state==Playing)
    {
        //Start the updater.
        m_tick->start();
        //Play the playbin.
        gst_element_set_state(m_playbin, GST_STATE_PLAYING);
    }
    //Reset the saved position.
    m_savedPosition=-1;
}

void KNMusicBackendGStreamerThread::setVolume(const int &volume)
{
    //Save the volume size.
    m_volume=volume;
    //Check the playbin is null or not.
    if(m_playbin)
    {
        //Translate the volume to gdouble.
        gdouble playbinVolume=(gdouble)volume/10000.0;
        //Set the volume
        g_object_set(G_OBJECT(m_playbin), "volume", playbinVolume, NULL);
    }
}

void KNMusicBackendGStreamerThread::setPosition(const qint64 &position)
{
    //Check the playbin pointer first
    if(m_playbin && m_startPosition!=-1)
    {
        //Seek the playbin pipeline.
        gst_element_seek_simple(m_playbin,
                                GST_FORMAT_TIME,
                                m_seekFlag,
                                (m_startPosition + position) * GST_MSECOND);
    }
}

void KNMusicBackendGStreamerThread::updateDuration()
{
    //Prepare the fake total duration.
    gint64 totalDuration;
    //Query the duration, because it can could be played, so it's 100% we could
    //get the duration..
    while(!gst_element_query_duration(m_playbin,
                                      GST_FORMAT_TIME,
                                      &totalDuration));
    //Check out the total duration.
    if(totalDuration==0)
    {
        //Ignore the invalid value.
        return;
    }
    //Save the total duration, we are using millisecond while gstreamer is
    //using nanosecond.
    m_totalDuration=(qint64)(totalDuration)/1000000;
    //Check if section has been set then update the position.
    if(m_sectionSet)
    {
        //Check out the start and end position.
        updateStartAndEndPosition();
    }
    //If we comes to here, that means the file is loaded.
    emit loadSuccess();
}

void KNMusicBackendGStreamerThread::onActionTick()
{
    //Check out the pipeline.
    if(m_playbin)
    {
        //Generate the gstreamer position.
        gint64 position;
        //Output the data.
        gboolean result=gst_element_query_position(m_playbin,
                                                   GST_FORMAT_TIME,
                                                   &position);
        //Check the result.
        if(result)
        {
            //Give back the position data.
            //Remember it will use nanosecond but not msecond.
            qint64 totalPosition=position/1000000;
            //Emit the position changed signal.
            emit positionChanged(totalPosition-m_startPosition);
            //Check the total position is greater than end position.
            if(m_endPosition>0 && totalPosition>=m_endPosition)
            {
                //Stop the pipeline
                stop();
                //Emit finished signal.
                emit finished();
            }
        }
    }
}

void KNMusicBackendGStreamerThread::processEvents(GstBus *bus,
                                                  GstMessage *message)
{
    Q_UNUSED(bus)
    //Get the message type.
    GstMessageType messageType=GST_MESSAGE_TYPE(message);
    //Check out the message type
    switch (messageType)
    {
    case GST_MESSAGE_EOS:
    {
        //The pipeline stopped playing, stop the pipeline, emit finished signal.
        //Stop the pipeline
        stop();
        //Emit finished signal.
        emit finished();
        break;
    }
    case GST_MESSAGE_ERROR:
    {
        //File will be loaded failed.
        emit loadFailed();
        break;
    }
    default:
        //Ignore all the other messages.
        break;
    }
}

gboolean KNMusicBackendGStreamerThread::busWatch(GstBus *bus,
                                                 GstMessage *message,
                                                 gpointer data)
{
    switch (GST_MESSAGE_TYPE (message))
    {
    case GST_MESSAGE_STATE_CHANGED:
    {
        //Target thread.
        KNMusicBackendGStreamerThread *thread=
                static_cast<KNMusicBackendGStreamerThread *>(data);
        if(GST_MESSAGE_SRC(message)==GST_OBJECT(thread->playbin()))
        {
            //Get the threads.
            GstState previousState, currentState, pendingState;
            gst_message_parse_state_changed(message,
                                            &previousState,
                                            &currentState,
                                            &pendingState);
            //Check the previous state and current state.
            if(previousState==GST_STATE_NULL &&
                    currentState==GST_STATE_READY)
            {
                //Update the duration.
                thread->updateDuration();
            }
        }
    }
    default:
        //Retranslate the data to a gstreamer-thread.
        //Emit the process event signal.
        static_cast<KNMusicBackendGStreamerThread *>(data)->requireProcessEvent(
                    bus,
                    message);

    }
    //Give back successful.
    return TRUE;

}

inline void KNMusicBackendGStreamerThread::resetPipeline()
{
    //Check playbin is null or not.
    if(m_playbin!=NULL)
    {
        //If it's not null, reset the state.
        gst_element_set_state(m_playbin, GST_STATE_NULL);
        //Reduce the ref of the playbin.
        gst_object_unref(m_playbin);
        //Reset the playbin pointer.
        m_playbin=NULL;
    }
    //Reset the state.
    setPlayingState(MusicUtil::Stopped);
}

inline void KNMusicBackendGStreamerThread::resetParameter()
{
    //Reset the duration to total duration.
    m_duration=m_totalDuration;
    //Reset the start position to -1, end position to duration.
    m_startPosition=-1;
    m_endPosition=m_duration;
    //Reset the section set flag.
    m_sectionSet=false;
}

inline bool KNMusicBackendGStreamerThread::rebuildPipeline()
{
    //Reset the playbin.
    resetPipeline();
    //Initial the playbin.
    m_playbin=gst_element_factory_make("playbin", "playbin");
    //Check playbin.
    if(m_playbin==NULL)
    {
        //Something wrong here.
        return false;
    }
    //Configure the playbin pipeline.
    //Set the playbin for audio only.
    gint playbinFlags;
    g_object_get(m_playbin, "flags", &playbinFlags, NULL);
    //Enabled audio.
    playbinFlags |= GST_PLAY_FLAG_AUDIO;
    //Disabled video and subtitle.
    playbinFlags &= ~GST_PLAY_FLAG_VIDEO;
    playbinFlags &= ~GST_PLAY_FLAG_TEXT;
    //Set the flag back to playbin.
    g_object_set(m_playbin, "flags", playbinFlags, NULL);
    //Add watching function.
    {
        //Get the bus from playbin pipeline.
        GstBus *bus=gst_pipeline_get_bus(GST_PIPELINE(m_playbin));
        //Add bus watch.
        gst_bus_add_watch(bus,
                          &KNMusicBackendGStreamerThread::busWatch,
                          (gpointer)this);
        //Remove the bus reference.
        gst_object_unref(bus);
    }
    //Reset the volume size.
    setVolume(m_volume);
    //Finished.
    return true;
}

inline void KNMusicBackendGStreamerThread::updateStartAndEndPosition()
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

inline void KNMusicBackendGStreamerThread::setPlayingState(const int &state)
{
    //Save the state.
    m_state=state;
    //Emit the changed signal.
    emit stateChanged(m_state);
}
