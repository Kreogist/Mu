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

#include "knmusicbackendphononthread.h"

#include <QDebug>

KNMusicBackendPhononThread::KNMusicBackendPhononThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_volumeCurve(QEasingCurve(QEasingCurve::OutCubic)),
    m_musicSource(Phonon::MediaSource()),
    m_mediaObject(new Phonon::MediaObject(this)),
    m_audioOutput(new Phonon::AudioOutput(Phonon::MusicCategory, this)),
    m_totalDuration(-1),
    m_duration(-1),
    m_startPosition(-1),
    m_endPosition(-1),
    m_pausedPosition(-1),
    m_state(Stopped),
    m_ticking(false)
{
    //Configure the media object.
    m_mediaObject->setTickInterval(10);

    //Link the media object and audio output together.
    Phonon::createPath(m_mediaObject, m_audioOutput);

    //Link the object to the thread.
    connect(m_mediaObject, &Phonon::MediaObject::tick,
            this, &KNMusicBackendPhononThread::onActionPositionChanged);
    connect(m_mediaObject, &Phonon::MediaObject::totalTimeChanged,
            this, &KNMusicBackendPhononThread::onActionDurationChanged);
    connect(m_mediaObject, &Phonon::MediaObject::finished,
            this, &KNMusicBackendPhononThread::finished);
    connect(m_mediaObject, &Phonon::MediaObject::stateChanged,
            this, &KNMusicBackendPhononThread::onActionStateChanged);
}

bool KNMusicBackendPhononThread::loadFile(const QString &filePath)
{
    //Stopping playing.
    stop();
    //Generate the url link for the current file.
    QUrl currentUrl=QUrl::fromLocalFile(filePath);
    //Reset the media object.
    resetMediaInformation();
    //Check if the music url is just the current file.
    if(currentUrl==m_musicSource.url())
    {
        //Update the duration.
        emit durationChanged(m_duration);
        //Emit the load success signal.
        emit loadSuccess();
        //Mission complete.
        return true;
    }
    //Reset the total duration of the music.
    m_totalDuration=-1;
    //Save the media source.
    m_musicSource=Phonon::MediaSource(currentUrl);
    //Load the file to media source.
    m_mediaObject->setCurrentSource(m_musicSource);
    //Give back the state.
    return (Phonon::NoError==m_mediaObject->state());
}

void KNMusicBackendPhononThread::reset()
{
    //Clear up the media object.
    m_mediaObject->clear();
    //Clear up the music source.
    m_musicSource=Phonon::MediaSource();
    //Reset all the other parameters.
    m_state=Stopped;
    m_ticking=false;
    //Reset the total duration.
    m_totalDuration=-1;
    //Reset the media information.
    resetMediaInformation();
}

void KNMusicBackendPhononThread::stop()
{
    //Stop the media object.
    m_mediaObject->stop();
}

void KNMusicBackendPhononThread::play()
{
    //Check the playing state.
    if(m_mediaObject->state()!=Phonon::PlayingState)
    {
        //Play the media.
        m_mediaObject->play();
    }
}

void KNMusicBackendPhononThread::pause()
{
    //Check the playing state.
    if(m_mediaObject->state()!=Phonon::PausedState)
    {
        //Pause the media object.
        m_mediaObject->pause();
        //Get the playing time, saved as paused position.
        m_pausedPosition=m_mediaObject->currentTime();
    }
}

int KNMusicBackendPhononThread::volume()
{
    //Give back the volume value.
    return m_volumeValue;
}

qint64 KNMusicBackendPhononThread::duration()
{
    //Give back the duration provides by the media object.
    return m_mediaObject->totalTime();
}

qint64 KNMusicBackendPhononThread::position()
{
    //Give back the current time of the media object.
    return m_mediaObject->currentTime();
}

int KNMusicBackendPhononThread::state() const
{
    //Give the state back.
    return m_state;
}

void KNMusicBackendPhononThread::setPlaySection(const qint64 &start,
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
    //Check out the total duration, it the file is loaded, we have to check the
    //start and end position.
    if(m_totalDuration!=-1)
    {
        //Check and update the positions.
        checkStartAndEndPosition();
    }
}

void KNMusicBackendPhononThread::setVolume(const int &volume)
{
    //Save the original volume.
    m_volumeValue=(qreal)volume / 10000.0;
    //Use the volume size to tweak the volume value.
    m_volumeSize=m_volumeCurve.valueForProgress(m_volumeValue);
    //Change the volume size if it's playing.
    if(!m_musicSource.url().isEmpty())
    {
        //Set the volume size to the audio output.
        m_audioOutput->setVolume(m_volumeSize);
    }
}

void KNMusicBackendPhononThread::setPosition(const qint64 &position)
{
    //Check out the music source state.
    if(m_musicSource.url().isEmpty())
    {
        //Ignore the invalid position.
        return;
    }
    //Try to seek the media.
    m_mediaObject->seek(m_startPosition + position);
    //Send the position changed signal.
    emit positionChanged(position);
}

void KNMusicBackendPhononThread::onActionDurationChanged(
        const qint64 &totalTime)
{
    //Check whether the new time is valid or not, and whether there's a url
    //loaded in the music source.
    if(totalTime==-1 || m_musicSource.url().isEmpty())
    {
        //Ignore the invalid duration changed signal.
        return;
    }
        //If we comes to here, that means the file is loaded.
        emit loadSuccess();
    //Save the duration as the total duration.
    m_totalDuration=totalTime;
    //Check the start and the end position.
    checkStartAndEndPosition();
}

void KNMusicBackendPhononThread::onActionPositionChanged(const qint64 &time)
{
    //Check out the time.
    //Sometimes phonon will give back 0 for a playing. I don't know why this bug
    //will happened. We won't let it happened.
    if(time==0)
    {
        //Ignore the 0 playing time.
        return;
    }
    //Emit the position changed signal.
    emit positionChanged(time - m_startPosition);
    //Check whether the time is reached the end position.
    if(m_endPosition > 0 && time>=m_endPosition)
    {
        //Stop playing.
        stop();
        //Emit the finished signal.
        emit finished();
    }
}

void KNMusicBackendPhononThread::onActionStateChanged(
        const Phonon::State &state,
        const Phonon::State &oldState)
{
    Q_UNUSED(oldState)
    //Prepare the new state.
    int latestState=Stopped;
    //Check out the new state.
    switch (state)
    {
    case Phonon::PlayingState:
        //Save the state information.
        latestState=Playing;
        //We need to do something here.
        //First try to set the audio to the user set volume, set it again here
        //to ensure the volumn has been set.
        m_audioOutput->setVolume(m_volumeSize);
        //Update the tick interval, copied from the ProgressSlider.
        m_mediaObject->setTickInterval(10);
        break;
    case Phonon::PausedState:
        //Save the state information.
        latestState=Paused;
        break;
    case Phonon::ErrorState:
        //Save the state as stopped.
        latestState=Stopped;
        //Emit load failed signal.
        emit loadFailed();
    default:
        break;
    }
    //Check the state.
    if(latestState==m_state)
    {
        //The state is not changed.
        m_state=Stopped;
    }
    //Save the latest state.
    m_state=latestState;
    //Emit the state changed signal.
    emit stateChanged(m_state);
}

void KNMusicBackendPhononThread::resetMediaInformation()
{
    //Reset the duration to total duration.
    m_duration=m_totalDuration;
    //Set the start position to be -1.
    m_startPosition=-1;
    //Set the default end position to the total duration.
    m_endPosition=m_totalDuration;
}

void KNMusicBackendPhononThread::checkStartAndEndPosition()
{
    //Check out the start position.
    if(m_startPosition==-1)
    {
        //The start position is invalid. Means we need to play the whole file.
        m_startPosition=0;
        //Set the whole file as the end position and the duration.
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
    //The duration is changed after check.
    emit durationChanged(m_duration);
    //We need to move the playing position to the start position.
    m_mediaObject->seek(m_startPosition);
}

