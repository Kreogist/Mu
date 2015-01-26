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
#include "knmusicbackendphononthread.h"

KNMusicBackendPhononThread::KNMusicBackendPhononThread(QObject *parent) :
    KNMusicBackendThread(parent)
{
    //Initial the media object and audio output.
    m_mediaObject=new MediaObject;
//    m_mediaObject->moveToThread(&m_workingThread);
    m_mediaObject->setTickInterval(10);

    m_audioOutput=new AudioOutput(MusicCategory);
//    m_audioOutput->moveToThread(&m_workingThread);

    //Initial the state.
    m_state=KNMusic::StoppedState;

    //Link the media object and audio output.
    createPath(m_mediaObject, m_audioOutput);

    //Link signals.
    connect(m_mediaObject, &MediaObject::totalTimeChanged,
            this, &KNMusicBackendPhononThread::onActionDurationChanged);
    connect(m_mediaObject, &MediaObject::tick,
            this, &KNMusicBackendPhononThread::onActionPositionChanged);
    connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(onActionStateChanged(State,State)));
    connect(m_mediaObject, SIGNAL(finished()),
            this, SLOT(onActionFinished()));

    //Start working thread.
//    m_workingThread.start();
}

KNMusicBackendPhononThread::~KNMusicBackendPhononThread()
{
    //Block all signals.
    m_audioOutput->blockSignals(true);
    m_mediaObject->blockSignals(true);
    //Quit the working thread.
//    m_workingThread.quit();
//    m_workingThread.wait();

    //Recover memory.
    delete m_audioOutput;
    delete m_mediaObject;
}

bool KNMusicBackendPhononThread::loadFromFile(const QString &filePath)
{
    //Reset the state.
    resetState();
    //Generate the media source.
    m_mediaSource=MediaSource(QUrl::fromLocalFile(filePath));
    //Load the file to media source.
    m_mediaObject->setCurrentSource(m_mediaSource);
    //Set the load flag.
    m_loadFlag=true;
    return (m_mediaObject->errorType()==NoError);
}

void KNMusicBackendPhononThread::clear()
{
    //Remove the media sources.
    m_mediaObject->clear();
    //Clear the media source.
    m_mediaSource=MediaSource();
    //Reset the state.
    resetState();
}

void KNMusicBackendPhononThread::resetState()
{
    //Set the flags.
    m_loadFlag=false;
    m_ticking=false;
    //Set the state.
    m_state=KNMusic::StoppedState;
    //Clear the position data.
    m_startPosition=-1;
    m_endPosition=-1;
    m_totalDuration=-1;
    m_duration=-1;
}

void KNMusicBackendPhononThread::stop()
{
    //Operate the media object.
    m_mediaObject->stop();
}

void KNMusicBackendPhononThread::pause()
{
    //Operate the media object.
    m_mediaObject->pause();
}

void KNMusicBackendPhononThread::play()
{
    //Play the media.
    m_mediaObject->play();
}

int KNMusicBackendPhononThread::volume()
{
    return m_volumeProgress*10000;
}

qint64 KNMusicBackendPhononThread::duration()
{
    return m_mediaObject->totalTime();
}

qint64 KNMusicBackendPhononThread::position()
{
    return m_mediaObject->currentTime();
}

void KNMusicBackendPhononThread::setPlaySection(const qint64 &sectionStart,
                                                const qint64 &sectionDuration)
{
    //Save the start position and duration.
    m_startPosition=sectionStart;
    m_duration=sectionDuration;
    //Calculate the end position.
    m_endPosition=sectionStart+m_duration;
    //Check if file has been loaded.
    if(m_totalDuration!=-1)
    {
        //Check the start position is available or not.
        if(m_startPosition==-1)
        {
            //Set the play section to whole file.
            m_startPosition=0;
            m_endPosition=m_totalDuration;
            m_duration=m_totalDuration;
        }
        else
        {
            //If we goes here means the file has been loaded,
            //Check if the section play available right now.
            if(m_endPosition>m_totalDuration)
            {
                //Change the end position to the total duration.
                m_endPosition=m_totalDuration;
            }
            //Check if the start position is larger than end position
            if(m_startPosition>m_totalDuration)
            {
                //Set the start position to the total duration.
                m_startPosition=m_totalDuration;
            }
            //Calculate the new duration.
            m_duration=m_endPosition-m_startPosition;
            //Seek the file to the position.
            m_mediaObject->seek(m_startPosition);
        }
        //Emit the duration changed signal.
        emit durationChanged(m_duration);
        //Force set the loaded flag.
        m_loadFlag=true;
    }
}

void KNMusicBackendPhononThread::playSection(const qint64 &sectionStart,
                                             const qint64 &sectionDuration)
{
    //Set the section.
    setPlaySection(sectionStart, sectionDuration);
    //Play the main thread.
    play();
}

void KNMusicBackendPhononThread::setVolume(const int &volumeSize)
{
    //Save the user set volume size.
    m_volumeProgress=(qreal)volumeSize/10000.0;
    m_volumeSize=m_volumeCurve.valueForProgress(m_volumeProgress);
    //Try to set the audio output to user set volume.
    m_audioOutput->setVolume(m_volumeSize);
}

void KNMusicBackendPhononThread::setPosition(const qint64 &position)
{
    //Check if we are ticking.
    if(!m_ticking)
    {
        //Try to seek the media.
        m_mediaObject->seek(m_startPosition+position);
        //Send position changed signal.
        emit positionChanged(position);
    }
}

void KNMusicBackendPhononThread::onActionStateChanged(const State &newstate,
                                                      const State &oldstate)
{
    Q_UNUSED(oldstate)
    //Get the new state.
    int threadNewState;
    switch(newstate)
    {
    case Phonon::PlayingState:
        threadNewState=KNMusic::PlayingState;
        //We need to do something here.
        //First try to set the audio output to user set volume, set it again to
        //ensure the volume has been set.
        m_audioOutput->setVolume(m_volumeSize);
        //Then set the tick interval, copied from ProgressSlider.
        m_mediaObject->setTickInterval(100);
        break;
    case Phonon::PausedState:
        threadNewState=KNMusic::PausedState;
        break;
    default:
        threadNewState=KNMusic::StoppedState;
        break;
    }
    //Ignore the same state.
    if(threadNewState==m_state)
    {
        return;
    }
    //Save the new state.
    m_state=threadNewState;
    //Emit signal state changed signal.
    emit stateChanged(m_state);
}

void KNMusicBackendPhononThread::onActionDurationChanged(
        const qint64 &newTotalTime)
{
    //Check is the newTotalTime available.
    if(newTotalTime==-1)
    {
        return;
    }
    //Set ticking flag, though I don't know what this.
    //Check the loading flag.
    if(m_loadFlag)
    {
        m_ticking=true;
        //When we are here, this is an important signal.
        //It means the file has been loaded or the file cannot be loaded.
        emit loaded();
        //Check out the duration.
        m_totalDuration=newTotalTime;
        //Check is the position and duration available.
        if(m_startPosition==-1)
        {
            //Play the whole file.
            //Means we need to calculate the duration here.
            m_startPosition=0;
            m_duration=m_totalDuration;
            m_endPosition=m_totalDuration;
        }
        else
        {
            //Check if the section play available.
            if(m_endPosition>m_totalDuration)
            {
                //Change the end position to the total duration.
                m_endPosition=m_totalDuration;
            }
            //Check if the start position is larger than end position
            if(m_startPosition>m_totalDuration)
            {
                //Set the start position to the total duration.
                m_startPosition=m_totalDuration;
            }
            //Calculate the new duration.
            m_duration=m_endPosition-m_startPosition;
        }
        //We need to move the playing position to the start position.
        //If there's a start position has been set.
        if(m_startPosition!=0)
        {
            m_mediaObject->seek(m_startPosition);
        }
        //Emit the duration changed signal.
        emit durationChanged(m_duration);
        //Change the flag.
        m_loadFlag=false;
        m_ticking=false;
    }
}

void KNMusicBackendPhononThread::onActionPositionChanged(const qint64 &time)
{
    //Check if the time is 0, ignore this signal changed.
    //Because there's no reason for a file can be back to 0 if it's being played.
    if(time==0)
    {
        return;
    }
    //I don't know why should write this.
    //Thread lock?
    m_ticking=true;
    //Emit position changed signal.
    emit positionChanged(time-m_startPosition);
    //Check the time should be finished.
    if(m_endPosition>0 && time>=m_endPosition)
    {
        //Stop playing.
        stop();
        //Do things finished.
        onActionFinished();
    }
    //Unlock ticking.
    m_ticking=false;
}

void KNMusicBackendPhononThread::onActionFinished()
{
    emit finished();
}
