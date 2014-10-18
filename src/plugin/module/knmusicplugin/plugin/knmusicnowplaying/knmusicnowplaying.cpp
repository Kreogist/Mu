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
#include "knmusicsingleplaylistmodel.h"

#include "knmusicnowplaying.h"

KNMusicNowPlaying::KNMusicNowPlaying(QObject *parent) :
    KNMusicNowPlayingBase(parent)
{
    //Initial temporary model.
    m_temporaryModel=new KNMusicSinglePlaylistModel(this);
    ;
}

void KNMusicNowPlaying::setHeaderPlayer(KNMusicHeaderPlayerBase *headerPlayer)
{
    if(m_headerPlayer==nullptr)
    {
        //Save the player.
        m_headerPlayer=headerPlayer;
        //Connect request.
        connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requirePlayNext,
                this, &KNMusicNowPlaying::playNext);
        connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requirePlayPrevious,
                this, &KNMusicNowPlaying::playPrevious);
        connect(m_headerPlayer, &KNMusicHeaderPlayerBase::finished,
                this, &KNMusicNowPlaying::onActionPlayingFinished);
        connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireChangeLoopState,
                this, &KNMusicNowPlaying::changeLoopState);
        //Connect responds.
        connect(this, &KNMusicNowPlaying::loopStateChanged,
                m_headerPlayer, &KNMusicHeaderPlayerBase::onActionLoopStateChanged);
    }
}

void KNMusicNowPlaying::playNext()
{
    ;
}

void KNMusicNowPlaying::playPrevious()
{
    ;
}

void KNMusicNowPlaying::onActionPlayingFinished()
{
    //If current mode is track repeat, just play it again. :)
    if(m_loopMode==RepeatTrack)
    {
        m_headerPlayer->play();
        return;
    }
    //Or else, we need to play the next.
    playNext();
}

void KNMusicNowPlaying::changeLoopState()
{
    //Switch to the next loop mode.
    m_loopMode=(m_loopMode+1)%LoopCount;
    //Emit the loop changed signal.
    emit loopStateChanged(m_loopMode);
}
