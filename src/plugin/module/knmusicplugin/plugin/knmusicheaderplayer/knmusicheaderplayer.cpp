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
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "knhighlightlabel.h"
#include "knscrolllabel.h"
#include "knprogressslider.h"
#include "knvolumeslider.h"
#include "kneditablelabel.h"
#include "knopacitybutton.h"
#include "knopacityanimebutton.h"

#include "knmusicheaderplayerappendmenu.h"
#include "knmusicdetaildialogbase.h"
#include "knmusicnowplayingbase.h"
#include "knmusicbackend.h"
#include "knmusicglobal.h"

#include "knglobal.h"

#include "knmusicheaderplayer.h"

#include <QDebug>

KNMusicHeaderPlayer::KNMusicHeaderPlayer(QWidget *parent) :
    KNMusicHeaderPlayerBase(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedSize(302, 66);

    //Initial music global.
    m_musicGlobal=KNMusicGlobal::instance();

    //Initial the pixmaps.
    m_playIcon=QPixmap(":/plugin/music/player/play.png");
    m_pauseIcon=QPixmap(":/plugin/music/player/pause.png");
    m_noRepeatIcon=QPixmap(":/plugin/music/loopmode/NoRepeat.png");
    m_repeatTrackIcon=QPixmap(":/plugin/music/loopmode/RepeatSingle.png");
    m_repeatAllIcon=QPixmap(":/plugin/music/loopmode/Repeat.png");
    m_muteIcon=QPixmap(":/plugin/music/player/mute.png");
    m_noMuteIcon=QPixmap(":/plugin/music/player/nomute.png");

    //Initial in out animation group.
    m_mouseIn=new QParallelAnimationGroup(this);
    m_mouseOut=new QParallelAnimationGroup(this);

    //Initial widgets.
    initialInformationPanel();
    initialPlayerControls();

    //Initial append widgets.
    initialAppendPanel();
    initialAppendMenu();

    //Connect drag play request.
    connect(this, &KNMusicHeaderPlayer::requireAnalysisFiles,
            this, &KNMusicHeaderPlayer::onActionPlayDragIn);
}

void KNMusicHeaderPlayer::loadConfigure()
{
    //Set the value, calculate by the range percentage.
    m_volumeSlider->setValue(
                m_volumeSlider->minimal()+
                (double)m_volumeSlider->range()*
                m_musicGlobal->configureData("Volume", 1.0).toDouble());
}

void KNMusicHeaderPlayer::saveConfigure()
{
    m_musicGlobal->setConfigureData("Volume",
                                    (double)m_volumeSlider->percentage());
}

void KNMusicHeaderPlayer::setBackend(KNMusicBackend *backend)
{
    m_backend=backend;
    //Set the volume range.
    m_volumeSlider->setRange(m_backend->volumeMinimal(),
                             m_backend->volumeMaximum());
    //Change the mouse step based on the range.
    int preferStep=(m_volumeSlider->maximum()-m_volumeSlider->minimal())/100;
    m_volumeSlider->setWheelStep(preferStep<1?1:preferStep);
    //Set the default volume.
    m_volumeSlider->setValue(m_volumeSlider->maximum());
    //Reset the player.
    reset();
    //Connect requests.
    connect(m_volumeIndicator, &KNOpacityButton::clicked,
            m_backend, &KNMusicBackend::changeMuteState);
    //Connect responds.
    connect(m_backend, &KNMusicBackend::positionChanged,
            this, &KNMusicHeaderPlayer::onActionPositionChanged);
    connect(m_backend, &KNMusicBackend::durationChanged,
            this, &KNMusicHeaderPlayer::onActionDurationChanged);
    connect(m_backend, &KNMusicBackend::playingStateChanged,
            this, &KNMusicHeaderPlayer::onActionPlayStateChanged);
    connect(m_backend, &KNMusicBackend::finished,
            this, &KNMusicHeaderPlayer::finished);
    connect(m_backend, &KNMusicBackend::muteStateChanged,
            [=](const bool &mute)
            {
                m_volumeIndicator->setIcon(mute?m_muteIcon:m_noMuteIcon);
            });
}

void KNMusicHeaderPlayer::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    m_nowPlaying=nowPlaying;
    //Connect request.
    connect(this, &KNMusicHeaderPlayer::requirePlayNext,
            m_nowPlaying, &KNMusicNowPlayingBase::playNext);
    connect(this, &KNMusicHeaderPlayer::requirePlayPrevious,
            m_nowPlaying, &KNMusicNowPlayingBase::playPrevious);
    connect(this, &KNMusicHeaderPlayer::requireChangeLoopState,
            m_nowPlaying, &KNMusicNowPlayingBase::changeLoopState);
    connect(this, &KNMusicHeaderPlayer::requireAnalysisFiles,
            m_nowPlaying, &KNMusicNowPlayingBase::playTemporaryFiles);
    //Connect responds.
    connect(m_nowPlaying, &KNMusicNowPlayingBase::loopStateChanged,
            this, &KNMusicHeaderPlayer::onActionLoopStateChanged);
    connect(m_nowPlaying, &KNMusicNowPlayingBase::requireResetPlayer,
            this, &KNMusicHeaderPlayer::reset);
    connect(m_nowPlaying, &KNMusicNowPlayingBase::nowPlayingChanged,
            this, &KNMusicHeaderPlayer::updatePlayerInfo);
    //Sync the data with now playing.
    onActionLoopStateChanged(m_nowPlaying->loopState());
}

KNMusicDetailInfo KNMusicHeaderPlayer::currentDetailInfo()
{
    return m_currentDetailInfo;
}

void KNMusicHeaderPlayer::reset()
{
    //Reset file path.
    m_currentFilePath.clear();
    //Set text.
    setTitle("");
    m_artist.clear();
    m_album.clear();
    updateArtistAndAlbum();
    //Set no album art icon.
    setAlbumArt(m_musicGlobal->noAlbumArt());
    //Set the duration and position.
    setDuration(0);
    setPositionText(0);
    //Ask to reset main thread.
    m_backend->resetMainPlayer();
    //Emit reset signal.
    emit playerReset();
}

void KNMusicHeaderPlayer::play()
{
    m_backend->play();
}

void KNMusicHeaderPlayer::activatePlayer()
{
    //Stop animations.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set parameters.
    m_showVolume->setStartValue(m_volumePanel->geometry());
    m_showPanel->setStartValue(m_controlPanel->geometry());
    m_showAppendPanel->setStartValue(m_appendPanel->geometry());
    //Start animation.
    m_mouseIn->start();
}

void KNMusicHeaderPlayer::inactivatePlayer()
{
    if(m_appendMenuShown)
    {
        return;
    }
    //Stop animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set parameters.
    m_hideVolume->setStartValue(m_volumePanel->geometry());
    m_hidePanel->setStartValue(m_controlPanel->geometry());
    m_hideAppendPanel->setStartValue(m_appendPanel->geometry());
    //Start animation.
    m_mouseOut->start();
}

void KNMusicHeaderPlayer::onActionLoopStateChanged(const int &state)
{
    //Change the icon.
    switch(state)
    {
    case RepeatTrack:
        m_loopStatus->setIcon(m_repeatTrackIcon);
        break;
    case RepeatAll:
        m_loopStatus->setIcon(m_repeatAllIcon);
        break;
    default:
        m_loopStatus->setIcon(m_noRepeatIcon);
        break;
    }
}

void KNMusicHeaderPlayer::setAlbumArt(const QPixmap &pixmap)
{
    m_albumArt->setPixmap(pixmap);
}

void KNMusicHeaderPlayer::setTitle(const QString &title)
{
    m_title->setText(title);
}

void KNMusicHeaderPlayer::setDuration(const qint64 &duration)
{
    //Change the progress slider range.
    m_progressSlider->setMaximum(duration);
    //Set duration display text.
    m_duration->setText(KNMusicGlobal::msecondToString(duration));
}

void KNMusicHeaderPlayer::onActionPlayDragIn(const QStringList &filePaths)
{
    m_nowPlaying->playTemporaryFiles(filePaths);
}

void KNMusicHeaderPlayer::onActionProgressPressed()
{
    m_progressPressed=true;
}

void KNMusicHeaderPlayer::onActionProgressReleased()
{
    m_progressPressed=false;
}

void KNMusicHeaderPlayer::onActionPositionEdited()
{
    //Get the latest text
    QString positionText=m_positionDisplay->text();
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

void KNMusicHeaderPlayer::setPositionText(const qint64 &position)
{
    if(!m_positionDisplay->editing())
    {
        m_positionDisplay->setText(KNMusicGlobal::msecondToString(position));
    }
}

void KNMusicHeaderPlayer::onActionPlayNPauseClicked()
{
    //Check is the current file path is empty or not.
    if(m_currentFilePath.isEmpty())
    {
        return;
    }
    //When play & pause pressed and the button is displaying play icon,
    //It means now is stopping or pausing. Should emit play signal.
    if(m_isShownPlay)
    {
        m_backend->play();
        return;
    }
    m_backend->pause();
}

void KNMusicHeaderPlayer::onActionVolumeSliderChanged(const qint64 &value)
{
    m_volumeIndicator->setOpacity(0.5+m_volumeSlider->percentage()/2);
    m_backend->setVolume((int)value);
}

void KNMusicHeaderPlayer::onActionInOutOpacityChange(const QVariant &value)
{
    QRect currentPos=value.toRect();
    qreal albumOpacity=qAbs(currentPos.height()==0?
                            1:(qreal)currentPos.y()/(qreal)currentPos.height());
    //Set the album art opacity.
    m_albumArtEffect->setOpacity(albumOpacity);
    m_title->setOpacity(albumOpacity);
    m_artistAndAlbum->setOpacity(albumOpacity);

    m_durationEffect->setOpacity(1-albumOpacity);
}

void KNMusicHeaderPlayer::onActionShowAppendMenu()
{
    //Ensure that there's file is playing.
    if(m_currentFilePath.isEmpty())
    {
        return;
    }
    //Set the flag.
    m_appendMenuShown=true;
    //Launch the menu.
    m_appendMenu->exec();
    //Reset the flag.
    m_appendMenuShown=false;
    //Ask the header widget to check the cursor.
    emit requireCheckCursor();
}

void KNMusicHeaderPlayer::onActionAppendMenuActionTriggered(int actionIndex)
{
    //Reset the flag.
    m_appendMenuShown=false;
    //Ask the header widget to check the cursor.
    emit requireCheckCursor();
    //Ensure that there's file is playing.
    if(m_currentFilePath.isEmpty())
    {
        return;
    }
    //Do the actions.
    switch(actionIndex)
    {
    case AppendRatingNoStar:
    case AppendRatingOneStar:
    case AppendRatingTwoStar:
    case AppendRatingThreeStar:
    case AppendRatingFourStar:
    case AppendRatingFiveStar:
        m_nowPlaying->setRating(actionIndex-AppendRatingNoStar);
        break;
    case AppendShowInGraphicShell:
        KNGlobal::showInGraphicalShell(m_currentFilePath);
        break;
    case AppendShowDetail:
        KNMusicGlobal::detailDialog()->showDialog(m_currentFilePath);
        break;
    case AppendLocateNowPlaying:
        m_nowPlaying->showCurrentIndexInOriginalTab();
        break;
    case AppendShowInSongs:
        emit requireShowInSongs();
        break;
    case AppendShowInArtists:
        emit requireShowInArtists();
        break;
    case AppendShowInAlbums:
        emit requireShowInAlbums();
        break;
    case AppendShowInGenres:
        emit requireShowInGenres();
        break;
    }
}

void KNMusicHeaderPlayer::onActionPositionChanged(const qint64 &position)
{
    emit positionChanged(position);
    if(!m_progressPressed)
    {
        m_progressSlider->setValue(position);
    }
}

void KNMusicHeaderPlayer::onActionDurationChanged(const qint64 &duration)
{
    //Change the slider maximum.
    setDuration(duration);
}

void KNMusicHeaderPlayer::onActionPlayStateChanged(const int &state)
{
    //If it's playing, then should display pause icon.
    if(state==PlayingState)
    {
        setPauseIconMode();
        return;
    }
    //Or else, whatever stopped or paused state, should display play icon.
    setPlayIconMode();
}

void KNMusicHeaderPlayer::setPosition(const qint64 &position)
{
    m_backend->setPosition(position);
}

inline void KNMusicHeaderPlayer::initialAlbumArt()
{
    //Initial album art label.
    m_albumArt=new KNHighlightLabel(this);
    m_albumArt->setAlbumSize(m_albumArtSize);
    m_albumArt->move(13, 2);
    //Initial album art effect.
    m_albumArtEffect=new QGraphicsOpacityEffect(m_albumArt);
    m_albumArtEffect->setOpacity(1.0);
    m_albumArt->setGraphicsEffect(m_albumArtEffect);
}

inline void KNMusicHeaderPlayer::initialLabels()
{
    //Initial the title scroll label.
    m_title=new KNScrollLabel(this);
    QFont scrollFont=m_title->font();
    scrollFont.setPixelSize(13);
    m_title->setFont(scrollFont);
    m_title->setGeometry(82,
                         5,
                         208,
                         m_title->height());
    configureScrollLabel(m_title);

    //Initial the artist and album.
    m_artistAndAlbum=new KNScrollLabel(this);
    m_artistAndAlbum->setFont(scrollFont);
    m_artistAndAlbum->setGeometry(82,
                                  25,
                                  208,
                                  m_artistAndAlbum->height());
    configureScrollLabel(m_artistAndAlbum);
}

inline void KNMusicHeaderPlayer::initialProrgess()
{
    //Initial panel.
    m_progressPanel=new QWidget(this);
    m_progressPanel->move(0, 45);
    m_progressPanel->setFixedWidth(302);

    //Initial layout.
    QBoxLayout *progressLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              m_progressPanel);
    progressLayout->setContentsMargins(0,0,0,0);
    progressLayout->setSpacing(0);
    m_progressPanel->setLayout(progressLayout);

    //Initial duration label.
    m_duration=new QLabel(this);
    m_duration->setContentsMargins(0,0,0,0);
    m_duration->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //Set effect.
    m_durationEffect=new QGraphicsOpacityEffect(this);
    m_durationEffect->setOpacity(0);
    m_duration->setGraphicsEffect(m_durationEffect);
    //Set time font
    QFont timeFont=m_duration->font();
    timeFont.setFamily("096MKSD");
    m_duration->setFont(timeFont);
    m_duration->setFixedWidth(78);
    //Set color.
    QPalette pal=m_duration->palette();
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    m_duration->setPalette(pal);
    progressLayout->addWidget(m_duration);

    //Initial the progress bar.
    m_progressSlider=new KNProgressSlider(this);
    m_progressSlider->setWheelStep(1000);
    connect(m_progressSlider, &KNProgressSlider::sliderPressed,
            this, &KNMusicHeaderPlayer::onActionProgressPressed);
    connect(m_progressSlider, &KNProgressSlider::sliderReleased,
            this, &KNMusicHeaderPlayer::onActionProgressReleased);
    connect(m_progressSlider, &KNProgressSlider::sliderMoved,
            this, &KNMusicHeaderPlayer::setPosition);
    connect(m_progressSlider, &KNProgressSlider::valueChanged,
            this, &KNMusicHeaderPlayer::setPositionText);
    progressLayout->addWidget(m_progressSlider);

    //Initial the position display.
    m_positionDisplay=new KNEditableLabel(this);
    //Set alignment.
    m_positionDisplay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //Set time font.
    m_positionDisplay->setFont(timeFont);
    //Set fixed width
    m_positionDisplay->setFixedWidth(
                m_positionDisplay->fontMetrics().width('6')*4+
                m_positionDisplay->fontMetrics().width(':'));

    //Set color.
    m_positionDisplay->setPalette(pal);
    //Set default text.
    setPositionText(0);
    progressLayout->addWidget(m_positionDisplay);
    //Connect edit finished signal.
    connect(m_positionDisplay, &KNEditableLabel::editingFinished,
            this, &KNMusicHeaderPlayer::onActionPositionEdited);

    //Initial loop state button.
    initialLoopState();
    progressLayout->addSpacing(5);
    progressLayout->addWidget(m_loopStatus);
}

inline void KNMusicHeaderPlayer::initialLoopState()
{
    m_loopStatus=new KNOpacityButton(this);
    m_loopStatus->setFixedSize(16, 16);
    //Set default state.
    onActionLoopStateChanged(NoRepeat);
    //Connect require change signal.
    connect(m_loopStatus, &KNOpacityButton::clicked,
            this, &KNMusicHeaderPlayer::requireChangeLoopState);
}

inline void KNMusicHeaderPlayer::initialControlPanel()
{
    //Initial the control panel.
    m_controlPanel=new QWidget(this);
    //Set initial position.
    m_controlPanel->setGeometry(generateOutPosition());
    QBoxLayout *panelLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           m_controlPanel);
    panelLayout->setContentsMargins(0,0,0,0);
    panelLayout->setSpacing(0);
    m_controlPanel->setLayout(panelLayout);

    //Initial controls.
    panelLayout->addStretch();
    //Previous button
    m_previous=new KNOpacityAnimeButton(this);
    m_previous->setFixedSize(m_buttonSize,
                             m_buttonSize);
    m_previous->setIcon(QPixmap(":/plugin/music/player/previous.png"));
    connect(m_previous, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::requirePlayPrevious);
    panelLayout->addWidget(m_previous, 0, Qt::AlignCenter);

    //Play and pause unibutton.
    m_playNPause=new KNOpacityAnimeButton(this);
    m_playNPause->setFixedSize(m_buttonSize,
                               m_buttonSize);
    setPlayIconMode();
    connect(m_playNPause, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::onActionPlayNPauseClicked);
    panelLayout->addWidget(m_playNPause, 0, Qt::AlignCenter);

    //Next button
    m_next=new KNOpacityAnimeButton(this);
    m_next->setFixedSize(m_buttonSize,
                         m_buttonSize);
    m_next->setIcon(QPixmap(":/plugin/music/player/next.png"));
    connect(m_next, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::requirePlayNext);
    panelLayout->addWidget(m_next, 0, Qt::AlignCenter);
    panelLayout->addStretch();

    //Initial the panel animation.
    m_showPanel=new QPropertyAnimation(this);
    configurePanelAnimation(m_showPanel);
    m_showPanel->setEndValue(generateInPosition());
    m_mouseIn->addAnimation(m_showPanel);
    m_hidePanel=new QPropertyAnimation(this);
    configurePanelAnimation(m_hidePanel);
    m_hidePanel->setEndValue(generateOutPosition());
    m_mouseOut->addAnimation(m_hidePanel);
}

inline void KNMusicHeaderPlayer::initialVolume()
{
    //Initial volume panel.
    m_volumePanel=new QWidget(this);
    m_volumePanel->setFixedWidth(85);
    m_volumePanel->move(width(), 10);

    QBoxLayout *volumeLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_volumePanel);
    volumeLayout->setContentsMargins(0,0,0,0);
    volumeLayout->setSpacing(0);
    m_volumePanel->setLayout(volumeLayout);

    //Initial indicator.
    m_volumeIndicator=new KNOpacityButton(this);
    m_volumeIndicator->setIcon(m_noMuteIcon);
    m_volumeIndicator->setFixedSize(13,13);
    volumeLayout->addWidget(m_volumeIndicator);

    //Volume Slider
    m_volumeSlider=new KNVolumeSlider(this);
    connect(m_volumeSlider, &KNVolumeSlider::valueChanged,
            this, &KNMusicHeaderPlayer::onActionVolumeSliderChanged);
    volumeLayout->addWidget(m_volumeSlider, 1);

    //Initial volume animations,
    m_showVolume=new QPropertyAnimation(this);
    configureVolumeAnimation(m_showVolume);
    m_showVolume->setEndValue(QRect(212,
                                    10,
                                    m_volumePanel->width(),
                                    m_volumePanel->height()));
    m_mouseIn->addAnimation(m_showVolume);
    m_hideVolume=new QPropertyAnimation(this);
    configureVolumeAnimation(m_hideVolume);
    m_hideVolume->setEndValue(QRect(width(),
                                    10,
                                    m_volumePanel->width(),
                                    m_volumePanel->height()));
    m_mouseOut->addAnimation(m_hideVolume);
}

inline void KNMusicHeaderPlayer::initialAppendPanel()
{
    //Initial panel.
    m_appendPanel=new QWidget(this);
    //Initial layout.
    QBoxLayout *panelLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           m_appendPanel);
    panelLayout->setContentsMargins(0,0,0,0);
    panelLayout->setSpacing(9);
    m_appendPanel->setLayout(panelLayout);

    //Initial main player and menu button.

    panelLayout->addStretch();
    //Show main player.
    m_showMainPlayer=new KNOpacityAnimeButton(this);
    m_showMainPlayer->setFixedSize(14,14);
    m_showMainPlayer->setIcon(QPixmap(":/plugin/music/player/fullscreen.png"));
    panelLayout->addWidget(m_showMainPlayer);
    connect(m_showMainPlayer, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::requireShowMainPlayer);
    //Show append menu.
    m_showAppendMenu=new KNOpacityAnimeButton(this);
    m_showAppendMenu->setFixedSize(14,14);
    m_showAppendMenu->setIcon(QPixmap(":/plugin/music/player/menu.png"));
    panelLayout->addWidget(m_showAppendMenu);
    connect(m_showAppendMenu, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::requireShowAppendMenu);
    panelLayout->addStretch();

    //Initial the append panel animation.
    m_showAppendPanel=new QPropertyAnimation(this);
    configureAppendPanelAnimation(m_showAppendPanel);
    int panelY=11;
    m_appendPanel->move(-m_appendPanel->width(), panelY);
    m_showAppendPanel->setEndValue(QRect(8,
                                         panelY,
                                         m_appendPanel->width(),
                                         m_appendPanel->height()));
    m_mouseIn->addAnimation(m_showAppendPanel);

    m_hideAppendPanel=new QPropertyAnimation(this);
    configureAppendPanelAnimation(m_hideAppendPanel);
    m_hideAppendPanel->setEndValue(QRect(-m_appendPanel->width(),
                                         panelY,
                                         m_appendPanel->width(),
                                         m_appendPanel->height()));
    m_mouseOut->addAnimation(m_hideAppendPanel);
}

inline void KNMusicHeaderPlayer::initialAppendMenu()
{
    //Initial the menu.
    m_appendMenu=new KNMusicHeaderPlayerAppendMenu(m_showAppendMenu);
    m_appendMenu->setFocusProxy(this);
    //Linked the menu actions.
    connect(m_appendMenu, &KNMusicHeaderPlayerAppendMenu::requireDoAction,
            this, &KNMusicHeaderPlayer::onActionAppendMenuActionTriggered);

    //Link the clicked
    connect(m_showAppendMenu, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::onActionShowAppendMenu);
}

inline void KNMusicHeaderPlayer::setPlayIconMode()
{
    //Set the icon and the flag.
    m_playNPause->setIcon(m_playIcon);
    m_isShownPlay=true;
}

inline void KNMusicHeaderPlayer::setPauseIconMode()
{
    //Set the icon and the flag.
    m_playNPause->setIcon(m_pauseIcon);
    m_isShownPlay=false;
}

inline void KNMusicHeaderPlayer::updateArtistAndAlbum()
{
    //Clear it first.
    m_artistAndAlbum->setText("");
    //Set the text.
    if(m_artist.isEmpty())
    {
        if(!m_album.isEmpty())
        {
            m_artistAndAlbum->setText(m_album);
        }
    }
    else
    {
        if(m_album.isEmpty())
        {
            m_artistAndAlbum->setText(m_artist);
        }
        else
        {
            m_artistAndAlbum->setText(m_artist + " - " + m_album);
        }
    }
}

inline void KNMusicHeaderPlayer::configureScrollLabel(KNScrollLabel *label)
{
    QPalette pal=label->palette();
    pal.setColor(QPalette::Text, QColor(255,255,255));
    label->setPalette(pal);
}

inline void KNMusicHeaderPlayer::configurePanelAnimation(QPropertyAnimation *animation)
{
    animation->setTargetObject(m_controlPanel);
    animation->setPropertyName("geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    connect(animation, &QPropertyAnimation::valueChanged,
            this, &KNMusicHeaderPlayer::onActionInOutOpacityChange);
}

inline void KNMusicHeaderPlayer::configureAppendPanelAnimation(QPropertyAnimation *animation)
{
    animation->setTargetObject(m_appendPanel);
    animation->setPropertyName("geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
}

inline void KNMusicHeaderPlayer::configureVolumeAnimation(QPropertyAnimation *animation)
{
    animation->setTargetObject(m_volumePanel);
    animation->setPropertyName("geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
}

void KNMusicHeaderPlayer::updatePlayerInfo()
{
    //Get the current information.
    KNMusicAnalysisItem analysisItem=m_nowPlaying->currentAnalaysisItem();
    m_currentDetailInfo=analysisItem.detailInfo;
    //Check is the playing file the current file. If it is, do nothing.
    if(m_currentFilePath==m_currentDetailInfo.filePath)
    {
        return;
    }
    //Save the new file path and emit file path changed signal.
    m_currentFilePath=m_currentDetailInfo.filePath;
    //Set the display data.
    setTitle(m_currentDetailInfo.textLists[Name]);
    m_artist=m_currentDetailInfo.textLists[Artist];
    m_album=m_currentDetailInfo.textLists[Album];
    updateArtistAndAlbum();
    QPixmap coverImage=QPixmap::fromImage(analysisItem.coverImage);
    setAlbumArt(coverImage.isNull()?m_musicGlobal->noAlbumArt():coverImage);
    //Ask to load lyrics.
    emit requireLoadLyrics(m_currentDetailInfo);
}

inline void KNMusicHeaderPlayer::initialInformationPanel()
{
    initialAlbumArt();
    initialLabels();
}

inline void KNMusicHeaderPlayer::initialPlayerControls()
{
    initialControlPanel();
    initialProrgess();
    initialVolume();
}
