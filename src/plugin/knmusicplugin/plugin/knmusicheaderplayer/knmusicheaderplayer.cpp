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
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "kneditablelabel.h"
#include "knhighlightlabel.h"
#include "knvolumeslider.h"
#include "knopacityanimebutton.h"
#include "knopacitybutton.h"
#include "knscrolllabel.h"
#include "knthememanager.h"
#include "knprogressslider.h"

#include "knmusicbackend.h"
#include "knmusicnowplayingbase.h"

#include "knmusicglobal.h"

#include "knmusicheaderplayer.h"

#include <QDebug>

#define AlbumArtSize 61
#define panelY 11
#define m_buttonSize 38

KNMusicHeaderPlayer::KNMusicHeaderPlayer(QWidget *parent) :
    KNMusicHeaderPlayerBase(parent),
    m_previous(generateControlButton(":/plugin/music/player/previous.png")),
    m_playNPause(generateControlButton(":/plugin/music/player/play.png")),
    m_next(generateControlButton(":/plugin/music/player/next.png")),
    m_albumArt(new KNHighLightLabel(this)),
    m_title(new KNScrollLabel(this)),
    m_artistAlbum(new KNScrollLabel(this)),
    m_progressSlider(new KNProgressSlider(this)),
    m_volumeSlider(new KNVolumeSlider(this)),
    m_duration(new QLabel(this)),
    m_position(new KNEditableLabel(this)),
    m_loopState(new KNOpacityButton(this)),
    m_volumeIndicator(new KNOpacityButton(this)),
    m_controlPanel(new QWidget(this)),
    m_volumePanel(new QWidget(this)),
    m_appendPanel(new QWidget(this)),
    m_informationEffect(new QGraphicsOpacityEffect(this)),
    m_progressPressed(false),
    m_mouseIn(new QParallelAnimationGroup(this)),
    m_mouseOut(new QParallelAnimationGroup(this)),
    m_showControl(generateAnime(m_controlPanel)),
    m_hideControl(generateAnime(m_controlPanel)),
    m_showVolume(generateAnime(m_volumePanel)),
    m_hideVolume(generateAnime(m_volumePanel)),
    m_showAppend(generateAnime(m_appendPanel)),
    m_hideAppend(generateAnime(m_appendPanel)),
    m_backend(nullptr),
    m_nowPlaying(nullptr)
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(302, 66);

    //Initial the image sources.
    //--Loop State--
    m_loopStateIcon[NoRepeat]=QIcon(":/plugin/music/player/NoRepeat.png");
    m_loopStateIcon[RepeatAll]=QIcon(":/plugin/music/player/Repeat.png");
    m_loopStateIcon[RepeatTrack]=QIcon(":/plugin/music/player/RepeatSingle.png");
    m_loopStateIcon[Shuffle]=QIcon(":/plugin/music/player/Random.png");
    //--Mute State--
    m_muteIcon[false]=QIcon(":/plugin/music/player/mute_false.png");
    m_muteIcon[true]=QIcon(":/plugin/music/player/mute_true.png");
    //Initial the UI elements.
    //--Graphics Effect--
    m_informationEffect->setOpacity(1.0);
    // Album Art Label;
    m_albumArt->setFixedSize(AlbumArtSize, AlbumArtSize);
    m_albumArt->move(13, 4);
    m_albumArt->setGraphicsEffect(m_informationEffect);

    //--Title Label--
    QFont labelFont=m_title->font();
    labelFont.setPixelSize(13);
    m_title->setObjectName("HeaderPlayerLabel");
    knTheme->registerWidget(m_title);
    m_title->setFont(labelFont);
    m_title->setGeometry(80, 5, 215, m_title->sizeHint().height());

    //--Artist - Album Label--
    m_artistAlbum->setObjectName("HeaderPlayerLabel");
    knTheme->registerWidget(m_artistAlbum);
    m_artistAlbum->setFont(labelFont);
    m_artistAlbum->setGeometry(80, 25, 215, m_artistAlbum->sizeHint().height());

    //Control panel.
    m_controlPanel->setGeometry(generateOutPosition());
    //Initial layout of the control panel.
    QBoxLayout *panelLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           m_controlPanel);
    panelLayout->setContentsMargins(0,0,0,0);
    panelLayout->setSpacing(0);
    m_controlPanel->setLayout(panelLayout);
    panelLayout->addStretch();
    //--Previous Button--
    connect(m_previous, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::requirePlayPrevious);
    panelLayout->addWidget(m_previous, 0, Qt::AlignCenter);
    //--Play and Pause Button--
    connect(m_playNPause, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::onActionPlayNPause);
    panelLayout->addWidget(m_playNPause, 0, Qt::AlignCenter);
    //--Next Button--
    connect(m_next, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::requirePlayNext);
    panelLayout->addWidget(m_next, 0, Qt::AlignCenter);
    panelLayout->addStretch();

    //Volume Panel.
    m_volumePanel->setFixedWidth(85);
    m_volumePanel->move(width(), 10);
    //Initial layout of the volume panel.
    QBoxLayout *volumeLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_volumePanel);
    volumeLayout->setContentsMargins(0,0,0,0);
    volumeLayout->setSpacing(0);
    m_volumePanel->setLayout(volumeLayout);
    //Configure the volume indicator.
    m_volumeIndicator->setIcon(m_muteIcon[false]);
    m_volumeIndicator->setFixedSize(13,13);
    volumeLayout->addWidget(m_volumeIndicator);
    //Configure the volume slider.
    connect(m_volumeSlider, &KNVolumeSlider::valueChanged,
            this, &KNMusicHeaderPlayer::onActionVolumeChanged);
    volumeLayout->addWidget(m_volumeSlider, 1);

    //Generate a progress panel.
    QWidget *m_progressPanel=new QWidget(this);
    m_progressPanel->move(0, 45);
    m_progressPanel->setFixedWidth(302);
    //Initial layout of the progress panel.
    QBoxLayout *progressLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              m_progressPanel);
    progressLayout->setContentsMargins(0,0,0,0);
    progressLayout->setSpacing(0);
    m_progressPanel->setLayout(progressLayout);
    //Configure a font class.
    QFont timeFont=font();
    timeFont.setFamily("096MKSD");
    //--Duration Label--
    m_duration->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_duration->setContentsMargins(0,0,0,0);
    m_duration->setFixedWidth(71);
    m_duration->setFont(timeFont);
    m_duration->setObjectName("HeaderPlayerLabel");
    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            [=]
            {
                //Backup the alpha.
                int textColorAlpha=
                        m_duration->palette().color(
                            QPalette::WindowText).alpha();
                //Set the palette.
                m_duration->setPalette(
                            knTheme->getPalette(m_duration->objectName()));
                //Update the alpha.
                updateDurationPalette(textColorAlpha);
            });
    //Set the palette.
    m_duration->setPalette(
                knTheme->getPalette(m_duration->objectName()));
    //Initial the alpha.
    updateDurationPalette(0);
    progressLayout->addWidget(m_duration);
    progressLayout->addSpacing(5);
    //--Progress Bar--
    m_progressSlider->setWheelStep(1000);
    m_progressSlider->setMaximum(0);
    connect(m_progressSlider, &KNProgressSlider::sliderPressed,
            [=]{m_progressPressed=true;});
    connect(m_progressSlider, &KNProgressSlider::sliderReleased,
            [=]{m_progressPressed=false;});
    connect(m_progressSlider, &KNProgressSlider::sliderMoved,
            this, &KNMusicHeaderPlayer::setPosition);
    connect(m_progressSlider, &KNProgressSlider::valueChanged,
            this, &KNMusicHeaderPlayer::updatePositionText);
    progressLayout->addWidget(m_progressSlider);
    //--Position Label--
    m_position->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_position->setFont(timeFont);
    m_position->setFixedWidth(
                    m_position->fontMetrics().width('6')*5+
                    m_position->fontMetrics().width(':'));
    m_position->setObjectName("HeaderPlayerLabel");
    knTheme->registerWidget(m_position);
    //Link the position edit signal with player.
    connect(m_position, &KNEditableLabel::contentChanged,
            this, &KNMusicHeaderPlayer::onActionPositionEdited);
    //Set default text.
    updatePositionText(0);
    progressLayout->addWidget(m_position);
    //--Loop State--
    m_loopState->setFixedSize(16, 16);
    //Set the default state of the loop state.
    onActionLoopStateChange(NoRepeat);
    //Link the loop state button to loop change signal.
    connect(m_loopState, &KNOpacityButton::clicked,
            this, &KNMusicHeaderPlayer::requireChangeLoopState);
    progressLayout->addSpacing(5);
    progressLayout->addWidget(m_loopState);

    //Configure the animations.
    //Link the value changed signals.
    connect(m_showControl, &QPropertyAnimation::valueChanged,
            this, &KNMusicHeaderPlayer::onActionMouseInOut);
    connect(m_hideControl, &QPropertyAnimation::valueChanged,
            this, &KNMusicHeaderPlayer::onActionMouseInOut);
    //Set the end values.
    m_showControl->setEndValue(generateInPosition());
    m_hideControl->setEndValue(generateOutPosition());
    m_showVolume->setEndValue(QRect(212,
                                    10,
                                    m_volumePanel->width(),
                                    m_volumePanel->height()));
    m_hideVolume->setEndValue(QRect(width(),
                                    10,
                                    m_volumePanel->width(),
                                    m_volumePanel->height()));
    m_showAppend->setEndValue(QRect(8,
                                    panelY,
                                    m_appendPanel->width(),
                                    m_appendPanel->height()));
    m_hideAppend->setEndValue(QRect(-m_appendPanel->width(),
                                    panelY,
                                    m_appendPanel->width(),
                                    m_appendPanel->height()));
    //Configure the animation group.
    m_mouseIn->addAnimation(m_showVolume);
    m_mouseIn->addAnimation(m_showControl);
    m_mouseIn->addAnimation(m_showAppend);
    m_mouseOut->addAnimation(m_hideVolume);
    m_mouseOut->addAnimation(m_hideControl);
    m_mouseOut->addAnimation(m_hideAppend);

    //Link with the backend.
    if(knMusicGlobal->backend())
    {
        //Save the backend pointer.
        m_backend=knMusicGlobal->backend();
    }
    //Link with the now playing.
    if(knMusicGlobal->nowPlaying())
    {
        //Save the now playing pointer.
        m_nowPlaying=knMusicGlobal->nowPlaying();
        //Link the header player's control signal to now playing model.
        connect(this, &KNMusicHeaderPlayer::requireChangeLoopState,
                m_nowPlaying, &KNMusicNowPlayingBase::changeLoopState);
        //Link the now playing model's response to header player.
        connect(m_nowPlaying, &KNMusicNowPlayingBase::loopStateChanged,
                this, &KNMusicHeaderPlayer::onActionLoopStateChange);
    }
}

void KNMusicHeaderPlayer::activate()
{
    //Start mouse in and show animations.
    startAnime(m_mouseIn, m_showVolume, m_showControl, m_showAppend);
}

void KNMusicHeaderPlayer::inactivate()
{
    //Start mouse out and hide animations.
    startAnime(m_mouseOut, m_hideVolume, m_hideControl, m_hideAppend);
}

void KNMusicHeaderPlayer::updatePositionText(const qint64 &position)
{
    //Check if the position label is being editing.
    if(!m_position->isEditing())
    {
        //Update the position data to new time position
        m_position->setText(KNMusicUtil::msecondToString(position));
    }
}

inline void KNMusicHeaderPlayer::updateDurationPalette(const int &opacity)
{
    //Get the palette from the theme manager..
    QPalette pal=m_duration->palette();
    //Get the color from the palette.
    QColor windowTextColor=pal.color(QPalette::WindowText);
    //Update the palette of the window text of the opacity.
    pal.setColor(QPalette::WindowText, QColor(windowTextColor.red(),
                                              windowTextColor.green(),
                                              windowTextColor.blue(),
                                              opacity));
    //Update the palette.
    m_duration->setPalette(pal);
}

void KNMusicHeaderPlayer::onActionLoopStateChange(const int &state)
{
    //Check if the state is valid.
    if(state>-1 && state<LoopCount)
    {
        //Change the icon of the loop state button.
        m_loopState->setIcon(m_loopStateIcon[state]);
    }
}

void KNMusicHeaderPlayer::onActionMouseInOut(const QVariant &value)
{
    //Translate the value to rect.
    QRect currentPos=value.toRect();
    //Calculate the opacity.
    qreal opacity=qAbs(currentPos.height()==0?
                           1:(qreal)currentPos.y()/(qreal)currentPos.height());
    //Make sure the opacity is in the range.
    if(opacity>1.0)
    {
        opacity=1.0;
    }
    else if(opacity<0.0)
    {
        opacity=0.0;
    }
    //Apply the opacity to album art, title and artist-album labels.
    m_informationEffect->setOpacity(opacity);
    m_title->setOpacity(opacity);
    m_artistAlbum->setOpacity(opacity);
    //Update the duration label.
    updateDurationPalette((1.0-opacity)*255.0);
}

void KNMusicHeaderPlayer::onActionPositionEdited()
{
    //Get the latest text from the position.
    QString positionText=m_position->text();
    //Find the colon.
    int colonPosition=positionText.indexOf(':');
    //If we cannot find the colon, means it's not format as 'xx:xx'.
    if(-1==colonPosition)
    {
        //This might be a number, we treat it as second time.
        //Translate it to a number.
        bool translateSuccess=false;
        qint64 triedPositon=positionText.toLongLong(&translateSuccess);
        //If we succeed, set the position to that second.
        if(translateSuccess)
        {
            setPosition(triedPositon*1000);
        }
        return;
    }
    //Calculate the ms.
    qint64 minuatePart=positionText.left(colonPosition).toInt(),
           secondPart=positionText.mid(colonPosition+1).toInt(),
           preferPosition=(minuatePart*60+secondPart)*1000;
    if(preferPosition>0 && preferPosition<m_progressSlider->maximum())
    {
        setPosition(preferPosition);
    }
}

void KNMusicHeaderPlayer::onActionVolumeChanged(const qint64 &value)
{
    //Synchroism the opacity to the opacity button.
    m_volumeIndicator->setOpacity(0.5+m_volumeSlider->percentage()/2);
    //Check the backend.
    if(m_backend)
    {
        //Change the volume.
        m_backend->setVolume((int)value);
    }
}

void KNMusicHeaderPlayer::onActionPlayNPause()
{
    ;
}

inline void KNMusicHeaderPlayer::setPosition(const qint64 &position)
{
    //Check the backend before.
    if(m_backend)
    {
        //Call the set position of the backend.
        m_backend->setPosition(position);
    }
}

inline QPropertyAnimation *KNMusicHeaderPlayer::generateAnime(QObject *target)
{
    //Generate a property animation.
    QPropertyAnimation *animation=new QPropertyAnimation(target,
                                                         "geometry",
                                                         this);
    //Configure the animation.
    animation->setEasingCurve(QEasingCurve::OutCubic);
    //Give back the animation.
    return animation;
}

inline KNOpacityAnimeButton *KNMusicHeaderPlayer::generateControlButton(
        const QString &iconPath)
{
    //Generate a opacity anime button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    button->setFixedSize(m_buttonSize, m_buttonSize);
    //Set the icon of the button.
    button->setIcon(QIcon(iconPath));
    //Give back the button.
    return button;
}

inline void KNMusicHeaderPlayer::startAnime(QParallelAnimationGroup *group,
                                            QPropertyAnimation *volume,
                                            QPropertyAnimation *control,
                                            QPropertyAnimation *append)
{
    //Stop all the animation groups.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    volume->setStartValue(m_volumePanel->geometry());
    control->setStartValue(m_controlPanel->geometry());
    append->setStartValue(m_appendPanel->geometry());
    //Start animations.
    group->start();
}
