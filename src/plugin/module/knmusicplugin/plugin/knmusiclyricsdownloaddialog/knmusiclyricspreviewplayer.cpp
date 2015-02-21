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
#include <QBoxLayout>

#include "knopacitybutton.h"
#include "knprogressslider.h"
#include "knconnectionhandler.h"

#include "knmusicbackend.h"
#include "knmusiclrclyricsparser.h"
#include "knmusiclyricsmanager.h"
#include "knmusicscrolllyrics.h"

#include "knmusiclyricspreviewplayer.h"

KNMusicLyricsPreviewPlayer::KNMusicLyricsPreviewPlayer(QWidget *parent) :
    QWidget(parent)
{
    //Initial the resource.
    m_playIcon=QPixmap(":/plugin/music/preview/play.png");
    m_pauseIcon=QPixmap(":/plugin/music/preview/pause.png");

    //Initial the lyrics parser.
    m_lyricsManager=KNMusicLyricsManager::instance();
    m_parser=m_lyricsManager->lrcParser();

    //Initial the connection handler.
    m_backendLinks=new KNConnectionHandler(this);

    //Initial the player controls.
    initialPlayerControls();
    //Initial the lyrics display.
    m_lyricsDisplay=new KNMusicScrollLyrics(this);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QWidget *playerContainer=new QWidget(this);
    playerContainer->setContentsMargins(0,0,0,0);
    playerContainer->setAutoFillBackground(true);
    QPalette pal=playerContainer->palette();
    pal.setColor(QPalette::Window, QColor(0x60, 0x60, 0x60));
    playerContainer->setPalette(pal);
    mainLayout->addWidget(playerContainer);

    QBoxLayout *playerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    playerLayout->setContentsMargins(0,0,0,0);
    playerLayout->setSpacing(0);
    playerContainer->setLayout(playerLayout);

    playerLayout->addWidget(m_playNPause);
    playerLayout->addWidget(m_progress, 1);

    mainLayout->addWidget(m_lyricsDisplay, 1);
}

KNMusicLyricsPreviewPlayer::~KNMusicLyricsPreviewPlayer()
{

}

void KNMusicLyricsPreviewPlayer::setBackend(KNMusicBackend *backend)
{
    //Save the backend.
    m_backend=backend;
}

void KNMusicLyricsPreviewPlayer::linkBackend()
{
    //Connect to player controls.
    m_backendLinks->addConnectionHandle(
                connect(m_backend, &KNMusicBackend::previewPositionChanged,
                        this, &KNMusicLyricsPreviewPlayer::onActionPreviewPositionChanged));
    m_backendLinks->addConnectionHandle(
                connect(m_backend, &KNMusicBackend::previewDurationChanged,
                        this, &KNMusicLyricsPreviewPlayer::onActionPreviewDurationChanged));
    m_backendLinks->addConnectionHandle(
                connect(m_backend, &KNMusicBackend::previewPlayingStateChanged,
                        this, &KNMusicLyricsPreviewPlayer::onActionPreviewStatusChange));
    m_backendLinks->addConnectionHandle(
                connect(m_progress, &KNProgressSlider::sliderMoved,
                        m_backend, &KNMusicBackend::setPreviewPosition));
    //Connect to lyrics display.
    m_backendLinks->addConnectionHandle(
                connect(m_backend, &KNMusicBackend::previewPositionChanged,
                        m_lyricsDisplay, &KNMusicScrollLyrics::onActionPositionChange));
}

void KNMusicLyricsPreviewPlayer::resetPreviewPlayer()
{
    //Reset player button state.
    m_isButtonPlay=true;
    m_playNPause->setIcon(m_playIcon);
    //Reset the backend.
    m_backend->resetPreviewPlayer();
    //Reset the progress bar.
    m_progress->setValue(0);
    //Disconnect with the backend.
    m_backendLinks->disconnectAll();
}

void KNMusicLyricsPreviewPlayer::setLyrics(const QString &lyricsText)
{
    //Reset the lyrics display first.
    clearLyrics();

    //Parse the lyrics text.
    QList<qint64> positionList;
    QStringList textList;
    if(m_parser->parseData(lyricsText, positionList, textList))
    {
        m_lyricsDisplay->setLyricsData(positionList, textList);
    }
}

void KNMusicLyricsPreviewPlayer::clearLyrics()
{
    m_lyricsDisplay->onActionLyricsReset();
}

void KNMusicLyricsPreviewPlayer::onActionPlayNPauseClick()
{
    //According to the button icon to do the action.
    if(m_isButtonPlay)
    {
        m_backend->playPreview();
    }
    else
    {
        m_backend->pausePreview();
    }
}

void KNMusicLyricsPreviewPlayer::onActionProgressPressed()
{
    m_progressPressed=true;
}

void KNMusicLyricsPreviewPlayer::onActionProgressReleased()
{
    m_progressPressed=false;
}

void KNMusicLyricsPreviewPlayer::onActionPreviewStatusChange(const int &state)
{
    //Check the state.
    m_isButtonPlay=(state!=PlayingState);
    //Set the icon.
    m_playNPause->setIcon(m_isButtonPlay?m_playIcon:m_pauseIcon);
}

void KNMusicLyricsPreviewPlayer::onActionPreviewPositionChanged(const qint64 &position)
{
    if(!m_progressPressed)
    {
        m_progress->setValue(position);
    }
}

void KNMusicLyricsPreviewPlayer::onActionPreviewDurationChanged(const qint64 &duration)
{
    m_progress->setMaximum(duration);
}

inline void KNMusicLyricsPreviewPlayer::initialPlayerControls()
{
    //Initial the button.
    m_playNPause=new KNOpacityButton(this);
    m_playNPause->setFocusProxy(this);
    m_playNPause->setFixedSize(20, 20);
    m_playNPause->setIcon(m_playIcon);
    connect(m_playNPause, &KNOpacityButton::clicked,
            this, &KNMusicLyricsPreviewPlayer::onActionPlayNPauseClick);

    //Initial the progress button.
    m_progress=new KNProgressSlider(this);
    m_progress->setFocusProxy(this);
    m_progress->setWheelStep(1000);
    connect(m_progress, &KNProgressSlider::sliderPressed,
            this, &KNMusicLyricsPreviewPlayer::onActionProgressPressed);
    connect(m_progress, &KNProgressSlider::sliderReleased,
            this, &KNMusicLyricsPreviewPlayer::onActionProgressReleased);
}
