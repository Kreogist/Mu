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
#include <QEventLoop>

#include "knmusicbackendqtavthread.h"

#include <QDebug>

KNMusicBackendQtAVThread::KNMusicBackendQtAVThread(QObject *parent) :
    KNMusicBackendThread(parent)
{
    //Initial the player and enable audio.
    m_player=new AVPlayer(this);
    m_player->enableAudio(true);
    //Linked the duration changed signal.
    connect(m_player, &AVPlayer::positionChanged,
            this, &KNMusicBackendQtAVThread::onActionPositionChanged);
}

KNMusicBackendQtAVThread::~KNMusicBackendQtAVThread()
{
    ;
}

void KNMusicBackendQtAVThread::loadFromFile(const QString &filePath)
{
    //Check the file path.
    if(m_player->file()==filePath)
    {
        //Stop playing.
        stop();
        //Reset playing parameter state.
        resetState();
    }
    //Emit the file changed signal.
    emit filePathChanged(filePath);
    //Don't use load! Use setFile!
    m_player->setFile(filePath);
    //Reset playing parameter state.
    resetState();
}

void KNMusicBackendQtAVThread::clear()
{
    ;
}

void KNMusicBackendQtAVThread::resetState()
{
    //Get the duration.
    m_duration=m_totalDuration;
    //Set the start position at the very beginning.
    m_startPosition=0;
    //Set the default end position as the whole file.
    m_endPosition=m_duration;
}

void KNMusicBackendQtAVThread::stop()
{
    m_player->stop();
}

void KNMusicBackendQtAVThread::pause()
{
    m_player->pause(true);
}

void KNMusicBackendQtAVThread::play()
{
    if(m_player->isPaused())
    {
        m_player->pause(false);
        return;
    }
    m_player->play();
}

int KNMusicBackendQtAVThread::volume()
{
    return m_player->audio()==nullptr?
              0:m_player->audio()->volume();
}

qint64 KNMusicBackendQtAVThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendQtAVThread::position()
{
    return m_player->position()-m_startPosition;
}

void KNMusicBackendQtAVThread::setPlaySection(const qint64 &sectionStart,
                                              const qint64 &sectionDuration)
{
    if(sectionStart!=-1)
    {
        m_player->setStartPosition(sectionStart);
        m_player->setStopPosition(sectionStart+sectionDuration);
    }
}

void KNMusicBackendQtAVThread::playSection(const qint64 &sectionStart,
                                          const qint64 &sectionDuration)
{
    //Set the section.
    setPlaySection(sectionStart, sectionDuration);
    //Play the main thread.
    play();
}

void KNMusicBackendQtAVThread::setVolume(const int &volumeSize)
{
    if(m_player->audio()!=nullptr)
    {
        m_player->audio()->setVolume((qreal)volumeSize/10000.0);
    }
}

void KNMusicBackendQtAVThread::setPosition(const qint64 &position)
{
    if(m_player->file().isEmpty())
    {
        return;
    }
    //Change the position into second.
    m_player->setPosition(position+m_startPosition);
}

void KNMusicBackendQtAVThread::onActionPositionChanged(const qint64 &position)
{
    qDebug()<<position;
    //Emit the reduced position.
    emit positionChanged(position);
}
