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
#include <QGraphicsOpacityEffect>

#include "knhighlightlabel.h"

#include "knmusicglobal.h"

#include "knmusicheaderplayer.h"

#define AlbumArtSize 61

KNMusicHeaderPlayer::KNMusicHeaderPlayer(QWidget *parent) :
    KNMusicHeaderPlayerBase(parent),
    m_albumArt(new KNHighLightLabel(this)),
    m_informationEffect(new QGraphicsOpacityEffect(this))
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    setFixedHeight(66);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //Initial the UI elements.
    // Graphics Effect;
    m_informationEffect->setOpacity(1.0);
    // Album Art;
    m_albumArt->setFixedSize(AlbumArtSize, AlbumArtSize);
    m_albumArt->move(13, 4);
    m_albumArt->setGraphicsEffect(m_informationEffect);
    // Title;
}

