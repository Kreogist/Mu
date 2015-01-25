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
    m_mediaObject->moveToThread(&m_workingThread);
    m_mediaObject->setTickInterval(10);

    m_audioOutput=new AudioOutput(MusicCategory);
    m_audioOutput->moveToThread(&m_workingThread);

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
            this, SIGNAL(finished()));

    m_workingThread.start();
}

KNMusicBackendPhononThread::~KNMusicBackendPhononThread()
{
    m_workingThread.quit();
    m_workingThread.wait();

    delete m_audioOutput;
    delete m_mediaObject;
}

bool KNMusicBackendPhononThread::loadFromFile(const QString &filePath)
{
    //Generate the media source.
    m_mediaSource=MediaSource(QUrl::fromLocalFile(filePath));
    //Load the file to media source.
    m_mediaObject->setCurrentSource(m_mediaSource);
    return (m_mediaObject->errorType()==NoError);
}

void KNMusicBackendPhononThread::clear()
{
    //Remove the media sources.
    m_mediaObject->clear();
}

void KNMusicBackendPhononThread::resetState()
{
    ;
}

void KNMusicBackendPhononThread::stop()
{
    m_mediaObject->stop();
}

void KNMusicBackendPhononThread::pause()
{
    m_mediaObject->pause();
}

void KNMusicBackendPhononThread::play()
{
    //Play the media.
    m_mediaObject->play();
    //Try to set the audio output to user set volume, set it again to ensure the
    //volume has been set.
    m_audioOutput->setVolume(m_userSetVolume);
}

int KNMusicBackendPhononThread::volume()
{
    return m_userSetVolume*10000;
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
    ;
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
    m_userSetVolume=(qreal)volumeSize/10000.0;
    //Try to set the audio output to user set volume.
    m_audioOutput->setVolume(m_userSetVolume);
}

void KNMusicBackendPhononThread::setPosition(const qint64 &position)
{
    if(!m_ticking)
    {
        m_mediaObject->seek(position);
    }
}

void KNMusicBackendPhononThread::onActionStateChanged(const State &newstate,
                                                      const State &oldstate)
{
    //Get the new state.
    int threadNewState;
    switch(newstate)
    {
    case Phonon::PlayingState:
        threadNewState=KNMusic::PlayingState;
        //We need to do something here.
        m_mediaObject->setTickInterval(350);
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
    m_ticking=true;
    //When we are here, this is a important signal.
    //It means the file has been loaded or the file cannot be loaded.
    emit loaded();
    qDebug()<<newTotalTime<<m_mediaObject->state()<<m_mediaObject->errorType();
    //Emit the duration changed signal.
    emit durationChanged(newTotalTime);
    m_ticking=false;
}

void KNMusicBackendPhononThread::onActionPositionChanged(const qint64 &time)
{
    //I don't know why should write this.
    //Thread lock?
    m_ticking=true;
    //Emit position changed signal.
    emit positionChanged(time);
    //Unlock ticking.
    m_ticking=false;
}
