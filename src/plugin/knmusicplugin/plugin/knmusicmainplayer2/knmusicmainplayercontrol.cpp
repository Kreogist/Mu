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
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QTimeLine>

#include "kndpimanager.h"
#include "knopacitybutton.h"
#include "knvolumeslider.h"
#include "kneditablelabel.h"
#include "knprogressslider.h"
#include "knopacityanimebutton.h"
#include "knthememanager.h"

#include "knmusicbackend.h"
#include "knmusicnowplayingbase.h"

#include "knmusicmainplayercontrol.h"

#define ButtonSpace 21
#define ButtonSize 32
#define BorderSpacing 32

KNMusicMainPlayerControl::KNMusicMainPlayerControl(QWidget *parent) :
    QWidget(parent),
    m_playIcon(QPixmap(":/plugin/music/player/play_light.png")),
    m_pauseIcon(QPixmap(":/plugin/music/player/pause_light.png")),
    m_backend(nullptr),
    m_changeOpacity(new QTimeLine(200, this)),
    m_opacity(new QGraphicsOpacityEffect(this)),
    m_buttonOpacity(new QGraphicsOpacityEffect(this)),
    m_position(new KNEditableLabel(this)),
    m_duration(new QLabel(this)),
    m_progressSlider(new KNProgressSlider(this)),
    m_loopMode(generateButton()),
    m_previous(generateButton(":/plugin/music/player/previous_light.png")),
    m_playNPause(generateButton()),
    m_next(generateButton(":/plugin/music/player/next_light.png")),
    m_panelSwitch(generateButton()),
    m_hideMainPlayer(new KNOpacityAnimeButton(this)),
    m_volumeIcon(new KNOpacityButton(this)),
    m_volumeSlider(new KNVolumeSlider(this)),
    m_currentOpacity(0),
    m_firstStageVolume(-1),
    m_secondStageVolume(-1),
    m_progressPressed(false)
{
    //Set properties.
    setFixedHeight(knDpi->height(93));
    setGraphicsEffect(m_opacity);
    //Initial the resources.
    m_loopStateIcon[NoRepeat]    =
            QPixmap(":/plugin/music/player/NoRepeat_main.png");
    m_loopStateIcon[RepeatTrack] =
            QPixmap(":/plugin/music/player/RepeatSingle_main.png");
    m_loopStateIcon[RepeatAll]   =
            QPixmap(":/plugin/music/player/Repeat_main.png");
    m_loopStateIcon[Shuffle]     =
            QPixmap(":/plugin/music/player/Random_main.png");
    m_volumeSizeIcon[NoVolume]=QPixmap(":/plugin/music/player/volume_0.png");
    m_volumeSizeIcon[Volume1] =QPixmap(":/plugin/music/player/volume_1.png");
    m_volumeSizeIcon[Volume2] =QPixmap(":/plugin/music/player/volume_2.png");
    m_volumeSizeIcon[Volume3] =QPixmap(":/plugin/music/player/volume_3.png");
    //Configure buttons.
    m_playNPause->setIcon(m_playIcon);
    m_loopMode->setIcon(m_loopStateIcon[NoRepeat]);
    m_volumeIcon->setFixedSize(knDpi->size(ButtonSize, ButtonSize));
    m_volumeIcon->setIcon(m_volumeSizeIcon[NoVolume]);
    //Configure the hide main player button.
    m_hideMainPlayer->setFixedSize(knDpi->size(32, 32));
    m_hideMainPlayer->setIcon(
                QPixmap(":/plugin/music/player/hide_mainplayer.png"));
    m_hideMainPlayer->setGraphicsEffect(m_buttonOpacity);
    connect(m_hideMainPlayer, &KNOpacityAnimeButton::mouseEnter,
            this, &KNMusicMainPlayerControl::opacityMoveIn);
    connect(m_hideMainPlayer, &KNOpacityAnimeButton::mouseLeave,
            this, &KNMusicMainPlayerControl::opacityMoveOut);
    // Configure time labels.
    m_duration->setObjectName("MainPlayerDurationLabel");
    m_position->setObjectName("MainPlayerLabel");
    knTheme->registerWidget(m_duration);
    knTheme->registerWidget(m_position);
    QFont timeFont=m_duration->font();
    timeFont.setFamily("096MKSD");
    timeFont.setPixelSize(14);
    m_duration->setFont(timeFont);
    m_position->setFont(timeFont);
    //Configure progress slider.
    m_progressSlider->setWheelStep(1000);
    m_progressSlider->setMaximum(0);
    connect(m_progressSlider, &KNProgressSlider::sliderPressed,
            [=]{m_progressPressed=true;});
    connect(m_progressSlider, &KNProgressSlider::sliderReleased,
            [=]{m_progressPressed=false;});
    connect(m_progressSlider, &KNProgressSlider::sliderMoved,
            this, &KNMusicMainPlayerControl::setPosition);
    connect(m_progressSlider, &KNProgressSlider::valueChanged,
            this, &KNMusicMainPlayerControl::updatePositionText);
    //Configure volume slider.
    m_volumeSlider->setMinimumWidth(knDpi->width(100));
    m_volumeSlider->setMaximumWidth(knDpi->width(150));
    //Configure the time line.
    m_opacity->setOpacity(0.0);
    m_buttonOpacity->setOpacity(0.0);
    m_changeOpacity->setUpdateInterval(16);
    m_changeOpacity->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_changeOpacity, &QTimeLine::frameChanged,
            [=](int frameValue)
            {
                //Save the value.
                m_currentOpacity=frameValue;
                //Update the opacity.
                qreal opacity=(qreal)frameValue/255.0;
                m_opacity->setOpacity(opacity);
                m_buttonOpacity->setOpacity(opacity);
            });

    //Set the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add the progress slider.
    mainLayout->addWidget(m_progressSlider);
    //Add the button layout.
    int buttonSpace=knDpi->width(ButtonSpace);
    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(0);
    mainLayout->addLayout(controlLayout, 1);
    //Add button to the layout.
    QBoxLayout *leftLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    controlLayout->addLayout(leftLayout, 1);
    leftLayout->addSpacing(knDpi->width(BorderSpacing));
    leftLayout->addWidget(m_position);
    leftLayout->addWidget(m_duration);
    leftLayout->addStretch();
    //Add control buttons.
    controlLayout->addWidget(m_loopMode);
    controlLayout->addSpacing(buttonSpace);
    controlLayout->addWidget(m_previous);
    controlLayout->addSpacing(buttonSpace);
    controlLayout->addWidget(m_playNPause);
    controlLayout->addSpacing(buttonSpace);
    controlLayout->addWidget(m_next);
    controlLayout->addSpacing(buttonSpace);
    controlLayout->addWidget(m_panelSwitch);
    QBoxLayout *rightLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    controlLayout->addLayout(rightLayout, 1);
    rightLayout->addStretch();
    rightLayout->addWidget(m_volumeIcon);
    rightLayout->addWidget(m_volumeSlider);
    rightLayout->addSpacing(knDpi->width(BorderSpacing));
}

void KNMusicMainPlayerControl::setBackend(KNMusicBackend *backend)
{
    //Save the backend pointer.
    m_backend=backend;
    //Check whether the backend is null or not.
    if(m_backend==nullptr)
    {
        return;
    }
    //Link to the backend.
    connect(m_volumeSlider, &KNVolumeSlider::valueChanged,
            m_backend, &KNMusicBackend::setVolume);
    connect(m_playNPause, &KNOpacityAnimeButton::clicked,
            m_backend, &KNMusicBackend::playNPause);
    //Connect the response.
    connect(m_backend, &KNMusicBackend::volumeChanged,
            this, &KNMusicMainPlayerControl::onVolumeChanged);
    connect(m_backend, &KNMusicBackend::durationChanged,
            this, &KNMusicMainPlayerControl::updateDuration);
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
                m_playNPause->setIcon((state==Playing)?m_pauseIcon:m_playIcon);
            });
    //Calculate the volume stage data.
    int volumeStage=(m_backend->maximumVolume()-m_backend->minimalVolume())/3;
    m_firstStageVolume=m_backend->minimalVolume()+volumeStage;
    m_secondStageVolume=m_firstStageVolume+volumeStage;
    //Sync data from the backend.
    m_volumeSlider->setRange(m_backend->minimalVolume(),
                             m_backend->maximumVolume());
    //Change the mouse step based on the range.
    int preferStep=(m_volumeSlider->maximum()-m_volumeSlider->minimal())/100;
    m_volumeSlider->setWheelStep(preferStep<1?1:preferStep);
    //Sync the volume data via called the slot.
    onVolumeChanged(m_backend->volume());
}

void KNMusicMainPlayerControl::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    connect(nowPlaying, &KNMusicNowPlayingBase::loopStateChanged,
            this, &KNMusicMainPlayerControl::onLoopStateChanged);
    //Link the button controls to the now playing
    connect(m_previous, &KNOpacityAnimeButton::clicked,
            nowPlaying, &KNMusicNowPlayingBase::playPrevious);
    connect(m_next, &KNOpacityAnimeButton::clicked,
            nowPlaying, &KNMusicNowPlayingBase::playNext);
    connect(m_loopMode, &KNOpacityAnimeButton::clicked,
            nowPlaying, &KNMusicNowPlayingBase::changeLoopState);
    //Sync the states from the now playing.
    onLoopStateChanged(nowPlaying->loopState());
}

void KNMusicMainPlayerControl::enterEvent(QEvent *event)
{
    //Do the original enter event.
    QWidget::enterEvent(event);
    //Display all the widget.
    opacityMoveIn();
}

void KNMusicMainPlayerControl::leaveEvent(QEvent *event)
{
    //Do the original leave event.
    QWidget::leaveEvent(event);
    //Hide all the widget.
    opacityMoveOut();
}

void KNMusicMainPlayerControl::onVolumeChanged(int volumeSize)
{
    //Check out the backend first.
    if(!m_backend)
    {
        return;
    }
    //Check is the size is the minimum.
    if(volumeSize==m_backend->minimalVolume())
    {
        //When the backend is the minimum, it should be treated as no volume.
        m_volumeIcon->setIcon(m_volumeSizeIcon[NoVolume]);
    }
    //Check is the size in the first stage.
    else if(volumeSize<m_firstStageVolume)
    {
        //Change the volume icon to stage 1.
        m_volumeIcon->setIcon(m_volumeSizeIcon[Volume1]);
    }
    //Check is the size in the second stage.
    else if(volumeSize<m_secondStageVolume)
    {
        //Change the volume icon to stage 2.
        m_volumeIcon->setIcon(m_volumeSizeIcon[Volume2]);
    }
    else
    {
        //Then all the other volume is in the third stage.
        m_volumeIcon->setIcon(m_volumeSizeIcon[Volume3]);
    }
    //Check the volume slider is the same as the current volume.
    if(volumeSize!=m_volumeSlider->value())
    {
        //Block the value changed signal.
        m_volumeSlider->blockSignals(true);
        m_volumeSlider->setValue(volumeSize);
        m_volumeSlider->blockSignals(false);
    }
}

void KNMusicMainPlayerControl::updateDuration(const qint64 &duration)
{
    //Change the progress slider range.
    m_progressSlider->setMaximum(duration);
    //Set duration display text.
    m_duration->setText("/"+KNMusicUtil::msecondToString(duration));
}

void KNMusicMainPlayerControl::updatePositionText(const qint64 &position)
{
    //Check if the position label is being editing.
    if(!m_position->isEditing())
    {
        //Update the position data to new time position
        m_position->setText(KNMusicUtil::msecondToString(position));
    }
}

void KNMusicMainPlayerControl::onLoopStateChanged(int state)
{
    //Change the icon.
    m_loopMode->setIcon(m_loopStateIcon[state]);
}

inline void KNMusicMainPlayerControl::opacityMoveIn()
{
    //Change the opacity to 1.0.
    changeOpacity(255);
}

inline void KNMusicMainPlayerControl::opacityMoveOut()
{
    //Change the opacity to 0.0
    changeOpacity(0);
}

inline void KNMusicMainPlayerControl::setPosition(const qint64 &position)
{
    //Check the backend first.
    if(m_backend)
    {
        //Change the position.
        m_backend->setPosition(position);
    }
}

inline KNOpacityAnimeButton *KNMusicMainPlayerControl::generateButton(
        const QString &imagePath)
{
    //Create the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    if(!imagePath.isEmpty())
    {
        //Check the icon image path.
        button->setIcon(QIcon(imagePath));
    }
    button->setFixedSize(knDpi->size(ButtonSize, ButtonSize));
    return button;
}

inline void KNMusicMainPlayerControl::changeOpacity(int endFrame)
{
    //Stop the time line.
    m_changeOpacity->stop();
    //Update the time line value.
    m_changeOpacity->setFrameRange(m_currentOpacity, endFrame);
    //Start the time line.
    m_changeOpacity->start();
}

KNOpacityAnimeButton *KNMusicMainPlayerControl::hideMainPlayer() const
{
    return m_hideMainPlayer;
}
