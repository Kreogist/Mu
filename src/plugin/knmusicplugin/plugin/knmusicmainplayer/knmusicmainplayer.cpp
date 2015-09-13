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
#include "knthememanager.h"
#include "knopacityanimebutton.h"

#include "knmusicmainplayer.h"

KNMusicMainPlayer::KNMusicMainPlayer(QWidget *parent) :
    KNMusicMainPlayerBase(parent),
    m_hideMainPlayer(new KNOpacityAnimeButton(this))
{
    setObjectName("MainPlayer");
    //Set properties.
    setAutoFillBackground(true);

    //Configure the main player.
    m_hideMainPlayer->setFixedSize(32, 32);
    m_hideMainPlayer->move(16, 16);
    m_hideMainPlayer->setIcon(
                QPixmap(":/plugin/music/player/hide_mainplayer.png"));
    connect(m_hideMainPlayer, &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayer::requireHide);

    //Register the widget to the theme manager.
    knTheme->registerWidget(this);
}

void KNMusicMainPlayer::setBackend(KNMusicBackend *backend)
{
    ;
}

void KNMusicMainPlayer::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    ;
}

