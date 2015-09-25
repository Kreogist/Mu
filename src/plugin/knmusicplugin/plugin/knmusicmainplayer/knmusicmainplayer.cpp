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
#include <QLabel>

#include "kneditablelabel.h"
#include "knthememanager.h"
#include "knopacityanimebutton.h"
#include "knprogressslider.h"
#include "knglassanimebutton.h"

#include "knmusiclyricsmanager.h"
#include "knmusicbackend.h"
#include "knmusicscrolllyrics.h"
#include "knmusicmainplayerpanel.h"
#include "knmusicnowplayingbase.h"

#include "knmusicmainplayer.h"

#include <QDebug>

KNMusicMainPlayer::KNMusicMainPlayer(QWidget *parent) :
    KNMusicMainPlayerBase(parent),
    m_playIcon(QIcon(":/plugin/music/player/play_dark.png")),
    m_pauseIcon(QIcon(":/plugin/music/player/pause_dark.png")),
    m_contentLayout(nullptr),
    m_backend(nullptr),
    m_hideMainPlayer(new KNOpacityAnimeButton(this)),
    m_detailInfoPanel(new KNMusicMainPlayerPanel(this)),
    m_lyricsPanel(new KNMusicScrollLyrics(this)),
    m_playlistPanel(new QWidget(this)),
    m_controlPanel(new QWidget(this)),
    m_progressSlider(new KNProgressSlider(this)),
    m_duration(new QLabel(this)),
    m_position(new KNEditableLabel(this)),
    m_loopMode(new KNOpacityAnimeButton(this)),
    m_progressPressed(false)
{
    setObjectName("MainPlayer");
    //Set properties.
    setAutoFillBackground(true);

    //Initial the resources.
    m_loopStateIcon[NoRepeat]    =
            QIcon(":/plugin/music/player/NoRepeat.png");
    m_loopStateIcon[RepeatTrack] =
            QIcon(":/plugin/music/player/RepeatSingle.png");
    m_loopStateIcon[RepeatAll]   =
            QIcon(":/plugin/music/player/Repeat.png");
    m_loopStateIcon[Shuffle]     =
            QIcon(":/plugin/music/player/Random.png");
    m_volumeSizeIcon[NoVolume]=QIcon(":/plugin/music/player/volume_0.png");
    m_volumeSizeIcon[Volume1] =QIcon(":/plugin/music/player/volume_1.png");
    m_volumeSizeIcon[Volume2] =QIcon(":/plugin/music/player/volume_2.png");
    m_volumeSizeIcon[Volume3] =QIcon(":/plugin/music/player/volume_3.png");

    //Configure the hide main player button.
    m_hideMainPlayer->setFixedSize(32, 32);
    m_hideMainPlayer->setIcon(
                QPixmap(":/plugin/music/player/hide_mainplayer.png"));
    connect(m_hideMainPlayer, &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayer::requireHide);
    //Configure the lyrics.
    m_lyricsPanel->setObjectName("MainPlayerLyrics");
    m_lyricsPanel->setBackend(knMusicGlobal->lyricsManager()->backend());
    knTheme->registerWidget(m_lyricsPanel);

    //Configure control panel.
    // Configure progress slider.
    m_progressSlider->setWheelStep(1000);
    m_progressSlider->setMaximum(0);
    connect(m_progressSlider, &KNProgressSlider::sliderPressed,
            [=]{m_progressPressed=true;});
    connect(m_progressSlider, &KNProgressSlider::sliderReleased,
            [=]{m_progressPressed=false;});
    connect(m_progressSlider, &KNProgressSlider::sliderMoved,
            this, &KNMusicMainPlayer::setPosition);
    connect(m_progressSlider, &KNProgressSlider::valueChanged,
            this, &KNMusicMainPlayer::updatePositionText);
    // Initial and configure the buttons.
    for(int i=0; i<ControlButtonsCount; i++)
    {
        //Generate the button.
        m_controlButtons[i]=new KNGlassAnimeButton(this);
        //Resize the button, don't worry of this size, give any number is okay.
        m_controlButtons[i]->setFixedSize(46, 46);
    }
    //Set the buttons icon.
    m_controlButtons[ButtonPrev]->setIcon(
                QPixmap(":/plugin/music/player/previous_dark.png"));
    m_controlButtons[ButtonPlayNPause]->setIcon(
                m_playIcon);
    m_controlButtons[ButtonNext]->setIcon(
                QPixmap(":/plugin/music/player/next_dark.png"));
    //Link the play and pause button.
    connect(m_controlButtons[ButtonPlayNPause], &KNGlassAnimeButton::clicked,
            this, &KNMusicMainPlayer::onActionPlayNPauseClicked);
    //Configure the button.
    m_controlButtons[0]->setShowLeftLine(true);
    // Configure time labels.
    m_duration->setObjectName("MainPlayerLabel");
    m_position->setObjectName("MainPlayerLabel");
    knTheme->registerWidget(m_duration);
    knTheme->registerWidget(m_position);
    QFont timeFont=m_duration->font();
    timeFont.setFamily("096MKSD");
    m_duration->setFont(timeFont);
    m_position->setFont(timeFont);

    //Register the widget to the theme manager.
    knTheme->registerWidget(this);

    //Set the main player layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addSpacing(16);
    mainLayout->addWidget(m_hideMainPlayer);
    mainLayout->addSpacing(16);
    setLayout(mainLayout);
    //Initial the content layout.
    m_contentLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                   mainLayout->widget());
    m_contentLayout->setContentsMargins(0,0,0,0);
    m_contentLayout->setSpacing(0);
    mainLayout->addLayout(m_contentLayout, 1);
    //Add widget to content layout.
    m_contentLayout->addWidget(m_detailInfoPanel);
    m_contentLayout->addWidget(m_lyricsPanel);
    m_contentLayout->addWidget(m_playlistPanel);
    //Configure the control panel.
    //Generate the layout.
    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             m_controlPanel);
    controlLayout->setContentsMargins(0,0,0,0);
    m_controlPanel->setLayout(controlLayout);
    //Add widgets to control panels.
    controlLayout->addWidget(m_progressSlider);
    //Add to main playout.
    mainLayout->addWidget(m_controlPanel);
    //Generate the button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            controlLayout->widget());
    buttonLayout->setContentsMargins(2,0,2,0);
    buttonLayout->setSpacing(0);
    controlLayout->addLayout(buttonLayout, 1);
    //Add widgets to button layout.
    buttonLayout->addWidget(m_position, 0, Qt::AlignTop);
    //Generate the left part layout.
    QBoxLayout *buttonLeftLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                controlLayout->widget());
    buttonLeftLayout->setSpacing(0);
    buttonLeftLayout->addStretch();
    buttonLayout->addLayout(buttonLeftLayout, 1);
    //Add widgets to left layout.
    //Add control buttons.
    for(int i=0; i<ControlButtonsCount; i++)
    {
        buttonLayout->addWidget(m_controlButtons[i], 0, Qt::AlignBottom);
    }
    //Generate the right part layout.
    QBoxLayout *buttonRightLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                controlLayout->widget());
    buttonRightLayout->setSpacing(0);
    buttonRightLayout->addStretch();
    buttonLayout->addLayout(buttonRightLayout, 1);
    buttonLayout->addWidget(m_duration, 0, Qt::AlignTop);
}

void KNMusicMainPlayer::setBackend(KNMusicBackend *backend)
{
    //Save the backend pointer.
    m_backend=backend;
    //Check whether the backend is null or not.
    if(m_backend==nullptr)
    {
        return;
    }
    //Link to the backend.
    ;
    //Connect the response.
    connect(m_backend, &KNMusicBackend::durationChanged,
            this, &KNMusicMainPlayer::updateDuration);
    connect(m_backend, &KNMusicBackend::positionChanged,
            [=](const qint64 &position)
            {
                //Update the value of progress slider when the progress slider
                //is not being edited.
                if(!m_progressPressed)
                {
                    //Update the slider value.
                    m_progressSlider->setValue(position);
                }
            });
    connect(m_backend, &KNMusicBackend::playingStateChanged,
            [=](const int &state)
            {
                //If it's playing, then should display pause icon.
                m_controlButtons[ButtonPlayNPause]->setIcon(
                            (state==Playing)?m_pauseIcon:m_playIcon);
            });
}

void KNMusicMainPlayer::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //Link the now playing with the widget.
    connect(nowPlaying, &KNMusicNowPlayingBase::nowPlayingChanged,
            this, &KNMusicMainPlayer::onActionAnalysisItemChanged);
    connect(nowPlaying, &KNMusicNowPlayingBase::nowPlayingReset,
            [=]
            {
                //Generate a empty analysis item.
                KNMusicAnalysisItem item;
                //Set the analysis item to be the empty one.
                onActionAnalysisItemChanged(item);
            });
    //Link the button controls to the now playing
    connect(m_controlButtons[ButtonPrev], &KNGlassAnimeButton::clicked,
            nowPlaying, &KNMusicNowPlayingBase::playPrevious);
    connect(m_controlButtons[ButtonNext], &KNGlassAnimeButton::clicked,
            nowPlaying, &KNMusicNowPlayingBase::playNext);
}

void KNMusicMainPlayer::resizeEvent(QResizeEvent *event)
{
    //Do the resize.
    KNMusicMainPlayerBase::resizeEvent(event);
    //Set the default mode to column 1.
    int columnCount=1;
    //Check the size of the main player.
    //If the width is 1.5 times greater than the height, then column two.
    if((height()+(height()>>2)) <= width())
    {
        columnCount=2;
    }
    //If the width is two times greater than the height, then column three.
    if(height()+(height()>>1) <= width())
    {
        columnCount=3;
    }

    //Get the smaller one of the width and height.
    int parameterSize=qMin(width(), height());
    //Calculate the font size.
    int fontSize=parameterSize/30;
    //Use the original font size as the spacing of the layout.
    //!FIXME: add codes here.
    //Set the minimum size of the font to 15.
    if(fontSize<15)
    {
        fontSize=15;
    }
    //Resize the font.
    QFont textFont=font();
    textFont.setPixelSize(fontSize);
    //Set the new font.
    m_lyricsPanel->setFont(textFont);
    m_lyricsPanel->setSpacing(fontSize>>1);
}

void KNMusicMainPlayer::onActionAnalysisItemChanged(
        const KNMusicAnalysisItem &item)
{
    //Update the panel data.
    m_detailInfoPanel->setAnalysisItem(item);
}

void KNMusicMainPlayer::onActionPlayNPauseClicked()
{
    //Check the backend pointer.
    if(!m_backend)
    {
        //Give back.
        return;
    }
    //Check out the backend status.
    if(m_backend->state()==Playing)
    {
        //Pause the backend.
        m_backend->pause();
        //Mission complete.
        return;
    }
    //Or else we have to play the backend.
    m_backend->play();
}

void KNMusicMainPlayer::updatePositionText(const qint64 &position)
{
    //Check if the position label is being editing.
    if(!m_position->isEditing())
    {
        //Update the position data to new time position
        m_position->setText(KNMusicUtil::msecondToString(position));
    }
}

void KNMusicMainPlayer::updateDuration(const qint64 &duration)
{
    //Change the progress slider range.
    m_progressSlider->setMaximum(duration);
    //Set duration display text.
    m_duration->setText(KNMusicUtil::msecondToString(duration));
}

void KNMusicMainPlayer::setPosition(const qint64 &position)
{
    //Check the backend first.
    if(m_backend)
    {
        //Change the position.
        m_backend->setPosition(position);
    }
}

