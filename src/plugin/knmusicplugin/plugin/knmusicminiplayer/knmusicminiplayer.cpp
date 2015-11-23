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
#include <QBoxLayout>

#include "knopacityanimebutton.h"
#include "knthememanager.h"
#include "kneditablelabel.h"

#include "knmusichscrolllyrics.h"
#include "knmusicglobal.h"
#include "knmusiclyricsmanager.h"

#include "knmusicminiplayer.h"

#include <QDebug>

KNMusicMiniPlayer::KNMusicMiniPlayer(QWidget *parent) :
    KNMusicMiniPlayerBase(parent),
    m_container(new QWidget(this)),
    m_icon(new KNImageLabel(this)),
    m_position(new KNEditableLabel(this)),
    m_previous(generateButton(":/plugin/music/player/mini_backward.png")),
    m_playNPause(generateButton()),
    m_next(generateButton(":/plugin/music/player/mini_forward.png")),
    m_mute(generateButton()),
    m_close(generateButton(":/plugin/music/player/close_mini.png")),
    m_lyrics(new KNMusicHScrollLyrics(this))
{
    setObjectName("MiniPlayer");
    //Set property.
    setAutoFillBackground(true);
    setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
    setFixedSize(fontMetrics().height()*28, fontMetrics().height()<<1);

    //Configure the scroll lyrics.
    m_lyrics->setObjectName("MiniLyrics");
    m_lyrics->setBackend(knMusicGlobal->lyricsManager()->backend());
    m_lyrics->setFixedSize(size());
    knTheme->registerWidget(m_lyrics);
    //Configure the container.
    QBoxLayout *containerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                               this);
    containerLayout->setContentsMargins(0,0,0,0);
    containerLayout->setSpacing(0);
    m_container->setLayout(containerLayout);
    //Add widgets to container layout.
    containerLayout->addWidget(m_icon);
    containerLayout->addWidget(m_previous);
    containerLayout->addWidget(m_playNPause);
    containerLayout->addWidget(m_next);

    //Register the theme.
    knTheme->registerWidget(this);
}

void KNMusicMiniPlayer::setBackend(KNMusicBackend *backend)
{
    ;
}

void KNMusicMiniPlayer::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    ;
}

void KNMusicMiniPlayer::enterEvent(QEvent *event)
{
    //Do original event.
    KNMusicMiniPlayerBase::enterEvent(event);
    ;
}

void KNMusicMiniPlayer::leaveEvent(QEvent *event)
{
    //Do original event
    KNMusicMiniPlayerBase::leaveEvent(event);
    ;
}

inline KNOpacityAnimeButton *KNMusicMiniPlayer::generateButton(
        const QString &iconPath)
{
    //Generate the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Set the icon.
    if(!iconPath.isEmpty())
    {
        button->setIcon(QIcon(iconPath));
    }
    //Resize the button.
    button->setFixedSize(16, 16);
    //Give back the button.
    return button;
}
