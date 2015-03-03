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
#include <QTimeLine>
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>

#include "knconfigure.h"

#include "knmusicheaderplayerbase.h"
#include "knmusiclyricsmanager.h"

#include "knmusicheaderlyrics.h"

#include <QDebug>

KNMusicHeaderLyrics::KNMusicHeaderLyrics(QWidget *parent) :
    KNMusicHeaderLyricsBase(parent)
{
    //Initial the music global, music configure and lyrics manager.
    m_musicGlobal=KNMusicGlobal::instance();

    m_musicConfigure=m_musicGlobal->musicConfigure();

    m_lyricsManager=KNMusicLyricsManager::instance();
    connect(m_lyricsManager, &KNMusicLyricsManager::lyricsReset,
            this, &KNMusicHeaderLyrics::onActionLyricsReset);
    connect(m_lyricsManager, &KNMusicLyricsManager::lyricsUpdate,
            this, &KNMusicHeaderLyrics::onActionLyricsUpdate);
#ifdef Q_OS_WIN32
    //Set line spacing specially for Windows. It seems that in Qt 5.4, line
    //spacing has been calculate into a text line.
    setSpacing(0);
#endif

    //Connect apply preference signal.
    connect(KNGlobal::instance(), &KNGlobal::requireApplyPreference,
            this, &KNMusicHeaderLyrics::loadConfigure);
}

KNMusicHeaderLyrics::~KNMusicHeaderLyrics()
{
}

void KNMusicHeaderLyrics::setHeaderPlayer(KNMusicHeaderPlayerBase *player)
{
    //Save the player.
    m_player=player;
    //Link the player.
    connect(m_player, &KNMusicHeaderPlayerBase::playerReset,
            this, &KNMusicHeaderLyrics::onActionLyricsReset);
    connect(m_player, &KNMusicHeaderPlayerBase::positionChanged,
            this, &KNMusicHeaderLyrics::onActionPositionChange);
}

void KNMusicHeaderLyrics::onActionLyricsUpdate()
{
    //Reset the lyrics first.
    onActionLyricsReset();
    const KNMusicDetailInfo &detailInfo=
            m_player->currentAnalysisItem().detailInfo;
    //Check is the updated lyrics is the player's lyrics.
    if(m_lyricsManager->musicDetailInfo().filePath==detailInfo.filePath &&
            m_lyricsManager->musicDetailInfo().trackFilePath==detailInfo.trackFilePath &&
            m_lyricsManager->musicDetailInfo().trackIndex==detailInfo.trackIndex &&
            (!(m_lyricsManager->positionList().isEmpty())))
    {
        //Set the lyrics data.
        setLyricsData(m_lyricsManager->positionList(),
                      m_lyricsManager->textList());
    }
}

void KNMusicHeaderLyrics::loadConfigure()
{
    //Update the spacing.
    setSpacing(m_musicConfigure->getData("TextSpacing",
                                         spacing()).toInt());
    //Update the font.
    setFont(m_musicConfigure->getData("LyricsFont", font()).value<QFont>());
    //Update the lyrics.
    update();
}
