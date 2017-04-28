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
#include <QSignalMapper>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "sao/knsaosubmenu.h"
#include "knglobal.h"
#include "knutil.h"
#include "knlocalemanager.h"
#include "kneditablelabel.h"
#include "knhighlightlabel.h"
#include "knvolumeslider.h"
#include "knopacityanimebutton.h"
#include "knopacitybutton.h"
#include "knscrolllabel.h"
#include "knthememanager.h"
#include "knprogressslider.h"
#include "kngraphicsgloweffect.h"
#include "knconfigure.h"

#include "knmusictab.h"
#include "knmusicproxymodel.h"
#include "knmusicdetaildialog.h"
#include "knmusiclyricsmanager.h"
#include "knmusicbackend.h"
#include "knmusicnowplayingbase.h"
#include "knmusicscrolllyrics.h"
#include "knmusiclyricsdownloaddialogbase.h"

#include "knmusicglobal.h"

#include "knmusicheaderplayer.h"

#include <QDebug>

#define AlbumArtSize 61
#define GlowRadius 9.0
#define panelY 11
#define m_buttonSize 38

#define PlayerVolume QString("PlayerVolume")
#define PlayerMute QString("Mute")
#define PlayerLoopState QString("LoopState")

KNMusicHeaderPlayer::KNMusicHeaderPlayer(QWidget *parent) :
    KNMusicHeaderPlayerBase(parent),
    m_headerLyrics(new KNMusicScrollLyrics(this)),
    m_previous(generateControlButton(
                   ":/plugin/music/player/previous_light.png")),
    m_playNPause(generateControlButton(":/plugin/music/player/play_light.png")),
    m_next(generateControlButton(":/plugin/music/player/next_light.png")),
    m_showMainPlayer(generateAppendButton(
                         ":/plugin/music/player/fullscreen.png")),
    m_showMiniPlayer(generateAppendButton(
                         ":/plugin/music/player/showmini.png")),
    m_showAppendMenu(generateAppendButton(
                         ":/plugin/music/player/menu.png")),
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
    m_appendMenu(new KNSaoSubMenu(m_showAppendMenu)),
    m_actionTrigger(new QSignalMapper(this)),
    m_informationEffect(new QGraphicsOpacityEffect(this)),
    m_iconPlay(QIcon(":/plugin/music/player/play_light.png")),
    m_iconPause(QIcon(":/plugin/music/player/pause_light.png")),
    m_mouseIn(new QParallelAnimationGroup(this)),
    m_mouseOut(new QParallelAnimationGroup(this)),
    m_showControl(generateAnime(m_controlPanel)),
    m_hideControl(generateAnime(m_controlPanel)),
    m_showVolume(generateAnime(m_volumePanel)),
    m_hideVolume(generateAnime(m_volumePanel)),
    m_showAppend(generateAnime(m_appendPanel)),
    m_hideAppend(generateAnime(m_appendPanel)),
    m_backend(nullptr),
    m_nowPlaying(nullptr),
    m_cacheConfigure(
        knGlobal->cacheConfigure()->getConfigure("MusicHeaderPlayer")),
    m_musicConfigure(
        knMusicGlobal->configure()->getConfigure("MusicHeaderPlayer")),
    m_lyricsConfigure(m_musicConfigure->getConfigure("Lyrics")),
    m_appendMenuShown(false),
    m_progressPressed(false)
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    setFixedSize(302, 66);

    //Initial the image sources.
    //--Loop State--
    m_iconLoopState[NoRepeat]=QIcon(":/plugin/music/player/NoRepeat.png");
    m_iconLoopState[RepeatAll]=QIcon(":/plugin/music/player/Repeat.png");
    m_iconLoopState[RepeatTrack]=
            QIcon(":/plugin/music/player/RepeatSingle.png");
    m_iconLoopState[Shuffle]=QIcon(":/plugin/music/player/Random.png");
    //--Mute State--
    m_iconMute[false]=QIcon(":/plugin/music/player/mute_false.png");
    m_iconMute[true]=QIcon(":/plugin/music/player/mute_true.png");
    //Initial the UI elements.
    //--Graphics Effect--
    //Configure opacity effect.
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
    //Generate the glow effect for title text.
    KNGraphicsGlowEffect *glowEffect=new KNGraphicsGlowEffect(this);
    glowEffect->setColor(QColor(0,0,0));
    glowEffect->setRadius(GlowRadius);
    m_title->setGraphicsEffect(glowEffect);

    //--Artist - Album Label--
    m_artistAlbum->setObjectName("HeaderPlayerLabel");
    knTheme->registerWidget(m_artistAlbum);
    m_artistAlbum->setFont(labelFont);
    m_artistAlbum->setGeometry(80, 25, 215, m_artistAlbum->sizeHint().height());
    //Generate the glow effect for artist and album text.
    glowEffect=new KNGraphicsGlowEffect(this);
    glowEffect->setColor(QColor(0,0,0));
    glowEffect->setRadius(GlowRadius);
    m_artistAlbum->setGraphicsEffect(glowEffect);

    //Append panel.
    //Initial layout of the append panel.
    QBoxLayout *appendLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_appendPanel);
    appendLayout->setContentsMargins(0,0,0,0);
    appendLayout->setSpacing(9);
    m_appendPanel->setLayout(appendLayout);
    appendLayout->addStretch();
    //Configure the main player button.
    connect(m_showMainPlayer, &KNOpacityButton::clicked,
            this, &KNMusicHeaderPlayer::requireShowMainPlayer);
    //Add to append panel.
    appendLayout->addWidget(m_showMainPlayer);
    //Configure the mini player button.
    connect(m_showMiniPlayer, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::requireShowMiniPlayer);
    appendLayout->addWidget(m_showMiniPlayer);
    //Configure the append menu button.
    connect(m_showAppendMenu, &KNOpacityAnimeButton::clicked,
            this, &KNMusicHeaderPlayer::showAppendMenu);
    appendLayout->addWidget(m_showAppendMenu);
    appendLayout->addStretch();
    //Reset the append panel geometry.
    m_appendPanel->setGeometry(QRect(-m_appendPanel->width(),
                                     panelY,
                                     m_appendPanel->width(),
                                     m_appendPanel->height()));

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
    panelLayout->addWidget(m_previous, 0, Qt::AlignCenter);
    //--Play and Pause Button--
    panelLayout->addWidget(m_playNPause, 0, Qt::AlignCenter);
    //--Next Button--
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
    m_volumeIndicator->setIcon(m_iconMute[false]);
    m_volumeIndicator->setFixedSize(13,13);
    volumeLayout->addWidget(m_volumeIndicator);
    //Configure the volume slider.
    connect(m_volumeSlider, &KNVolumeSlider::valueChanged,
            this, &KNMusicHeaderPlayer::onActionVolumeChanged);
    volumeLayout->addWidget(m_volumeSlider, 1);

    //Generate a progress panel.
    QWidget *progressPanel=new QWidget(this);
    progressPanel->move(0, 45);
    progressPanel->setFixedWidth(302);
    //Initial layout of the progress panel.
    QBoxLayout *progressLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              progressPanel);
    progressLayout->setContentsMargins(0,0,0,0);
    progressLayout->setSpacing(0);
    progressPanel->setLayout(progressLayout);
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
    progressLayout->addSpacing(5);
    progressLayout->addWidget(m_loopState);

    //Configure the append menu.
    m_appendMenu->setFocusProxy(this);
    //Generate the sub menus.
    for(int i=0; i<SubMenuCount; i++)
    {
        m_subMenus[i]=new KNSaoSubMenu(m_appendMenu);
    }
    //Set the icons.
    m_subMenus[SubMenuRating]->menuAction()->setIcon(
                QIcon("://saomenuicons/rating.png"));
    m_subMenus[SubMenuLocate]->menuAction()->setIcon(
                QIcon("://saomenuicons/locate_original.png"));
    //Link the action mapper.
    connect(m_actionTrigger, SIGNAL(mapped(int)),
            this, SLOT(appendActionTriggered(int)));
    //Generate the actions.
    for(int i=0; i<MenuActionsCount; i++)
    {
        //Generate the action.
        m_menuActions[i]=new QAction(this);
        //Link to the action trigger.
        connect(m_menuActions[i], SIGNAL(triggered()),
                m_actionTrigger, SLOT(map()));
        //Mapped the action.
        m_actionTrigger->setMapping(m_menuActions[i], i);
    }
    //Configure the rating menu.
    QIcon ratingIcon=QIcon("://saomenuicons/rating.png");
    QByteArray starTextData;
    starTextData.append((char)226);
    starTextData.append((char)152);
    starTextData.append((char)133);
    QString starText=starTextData;
    for(int i=AppendRatingNoStar; i<=AppendRatingFiveStar; i++)
    {
        //Set action properties
        m_menuActions[i]->setIcon(ratingIcon);
        m_menuActions[i]->setText(starText.repeated(i-AppendRatingNoStar));
        //Add to rating menu.
        m_subMenus[SubMenuRating]->addAction(m_menuActions[i]);
    }
    m_appendMenu->addMenu(m_subMenus[SubMenuRating]);
    //Configure the get info menu.
    m_menuActions[AppendShowDetail]->setIcon(
                QIcon("://saomenuicons/get_info.png"));
    m_appendMenu->addAction(m_menuActions[AppendShowDetail]);
    //Configure the download lyrics.
    m_menuActions[AppendDownloadLyrics]->setIcon(
                QIcon("://saomenuicons/download_lyrics.png"));
    m_appendMenu->addAction(m_menuActions[AppendDownloadLyrics]);
    //Configure the show in graphics menu
    m_menuActions[AppendShowInGraphicShell]->setIcon(
                QIcon("://saomenuicons/showInGraphicsShell.png"));
    m_appendMenu->addAction(m_menuActions[AppendShowInGraphicShell]);
    //Add actions to locate menu.
    m_menuActions[AppendLocateNowPlaying]->setIcon(
                QIcon("://saomenuicons/locate_original.png"));
    m_subMenus[SubMenuLocate]->addAction(m_menuActions[AppendLocateNowPlaying]);
    m_menuActions[AppendShowInSongs]->setIcon(
                QIcon("://saomenuicons/locate_songs.png"));
    m_subMenus[SubMenuLocate]->addAction(m_menuActions[AppendShowInSongs]);
    m_menuActions[AppendShowInArtists]->setIcon(
                QIcon("://saomenuicons/locate_artists.png"));
    m_subMenus[SubMenuLocate]->addAction(m_menuActions[AppendShowInArtists]);
    m_menuActions[AppendShowInAlbums]->setIcon(
                QIcon("://saomenuicons/locate_albums.png"));
    m_subMenus[SubMenuLocate]->addAction(m_menuActions[AppendShowInAlbums]);
    m_menuActions[AppendShowInGenres]->setIcon(
                QIcon("://saomenuicons/locate_genres.png"));
    m_subMenus[SubMenuLocate]->addAction(m_menuActions[AppendShowInGenres]);
    //Append locate menu.
    m_appendMenu->addMenu(m_subMenus[SubMenuLocate]);

    //Configure the lyrics.
    //Use the lyrics manager backend.
    m_headerLyrics->setObjectName("HeaderLyrics");
    knTheme->registerWidget(m_headerLyrics);
    m_headerLyrics->setBackend(knMusicGlobal->lyricsManager()->backend());
    connect(m_lyricsConfigure, &KNConfigure::valueChanged,
            this, &KNMusicHeaderPlayer::onLyricsConfigureChanged);
    onLyricsConfigureChanged();

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

    //Link the restranslate of the locale.
    knI18n->link(this, &KNMusicHeaderPlayer::retranslate);
    retranslate();
}

void KNMusicHeaderPlayer::setBackend(KNMusicBackend *backend)
{
    //Save the backend pointer.
    m_backend=backend;
    //Check whether the backend is null or not.
    if(m_backend==nullptr)
    {
        return;
    }
    //Update the volume range.
    m_volumeSlider->setRange(backend->minimalVolume(),
                             backend->maximumVolume());
    //Change the mouse step based on the range.
    int preferStep=(m_volumeSlider->maximum()-m_volumeSlider->minimal())/100;
    m_volumeSlider->setWheelStep(preferStep<1?1:preferStep);
    //Reset the header player.
    reset();
    //Connect request to the backend.
    connect(m_volumeIndicator, &KNOpacityButton::clicked,
            m_backend, &KNMusicBackend::changeMuteState);
    connect(m_playNPause, &KNOpacityAnimeButton::clicked,
            [=]
            {
                //Check the state of the backend.
                //If the backend is now at the playing state, pause the backend.
                if(m_backend->state()==Playing)
                {
                    m_backend->pause();
                    return;
                }
                //Start to play the main thread.
                m_backend->play();
            });
    //Connect the response.
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
    //Update the duration of the slider and the label.
    connect(m_backend, &KNMusicBackend::durationChanged,
            this, &KNMusicHeaderPlayer::updateDuration);
    connect(m_backend, &KNMusicBackend::playingStateChanged,
            [=](const int &state)
            {
                //If it's playing, then should display pause icon.
                m_playNPause->setIcon((state==Playing)?m_iconPause:m_iconPlay);
            });
    connect(m_backend, &KNMusicBackend::volumeChanged,
            [=](const int &volumeSize)
            {
                //Block the volume slider.
                m_volumeSlider->blockSignals(true);
                //Change the opacity.
                m_volumeIndicator->setOpacity(0.5+
                                              m_volumeSlider->percentage()/2);
                //Sync the value.
                m_volumeSlider->setValue(volumeSize);
                //Release the block.
                m_volumeSlider->blockSignals(false);
            });
    connect(m_backend, &KNMusicBackend::muteStateChanged,
                [=](const bool &mute)
                {
                    //Update the icon of volume indicator.
                    m_volumeIndicator->setIcon(m_iconMute[mute]);
                });
}

void KNMusicHeaderPlayer::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //Save the now playing pointer.
    m_nowPlaying=nowPlaying;
    //Check whether the now playing is null.
    if(m_nowPlaying==nullptr)
    {
        return;
    }
    //Link the header player's control signal to now playing model.
    connect(m_loopState, &KNOpacityButton::clicked,
            m_nowPlaying, &KNMusicNowPlayingBase::changeLoopState);
    connect(m_next, &KNOpacityAnimeButton::clicked,
            m_nowPlaying, &KNMusicNowPlayingBase::playNext);
    connect(m_previous, &KNOpacityAnimeButton::clicked,
            m_nowPlaying, &KNMusicNowPlayingBase::playPrevious);
    //Link the now playing model's response to header player.
    connect(m_nowPlaying, &KNMusicNowPlayingBase::loopStateChanged,
            this, &KNMusicHeaderPlayer::onActionLoopStateChange);
    connect(m_nowPlaying, &KNMusicNowPlayingBase::nowPlayingChanged,
            this, &KNMusicHeaderPlayer::onActionNowPlayingChanged);
    connect(m_nowPlaying, &KNMusicNowPlayingBase::nowPlayingReset,
            this, &KNMusicHeaderPlayer::reset);
}

QWidget *KNMusicHeaderPlayer::lyrics()
{
    return m_headerLyrics;
}

void KNMusicHeaderPlayer::activate()
{
    //Start mouse in and show animations.
    startAnime(m_mouseIn, m_showVolume, m_showControl, m_showAppend);
}

void KNMusicHeaderPlayer::inactivate()
{
    //Check the shown status.
    if(m_appendMenuShown)
    {
        return;
    }
    //Start mouse out and hide animations.
    startAnime(m_mouseOut, m_hideVolume, m_hideControl, m_hideAppend);
}

void KNMusicHeaderPlayer::loadConfigure()
{
    //--From cache configure--
    if(m_backend)
    {
        //Load the previous volume settings.
        m_backend->setVolume(
                    m_backend->minimalVolume()+
                    (double)(m_volumeSlider->range())*
                    m_cacheConfigure->data(PlayerVolume, 0.5).toDouble());
        //Sync the volume indicator opacity.
        m_volumeIndicator->setOpacity(0.5+m_volumeSlider->percentage()/2);
        //Get the mute state and set to backend.
        m_backend->setMute(m_cacheConfigure->data(PlayerMute, false).toBool());
    }
    if(m_nowPlaying)
    {
        //Get the loop state.
        m_nowPlaying->setLoopState(
                    m_cacheConfigure->data(PlayerLoopState, NoRepeat).toInt());
    }
}

void KNMusicHeaderPlayer::saveConfigure()
{
    //--Write to cache configure--
    //Write the volume range.
    m_cacheConfigure->setData(PlayerVolume, m_volumeSlider->percentage());
    //Write the mute state.
    if(m_backend)
    {
        m_cacheConfigure->setData(PlayerMute, m_backend->mute());
    }
    //Write loop state.
    if(m_nowPlaying)
    {
        m_cacheConfigure->setData(PlayerLoopState, m_nowPlaying->loopState());
    }
}

void KNMusicHeaderPlayer::reset()
{
    //Clear the labels.
    m_title->setText("");
    m_artistAlbum->setText("");
    //Set no album art icon.
    m_albumArt->setPixmap(knMusicGlobal->noAlbumArt());
    //Set the duration and position to 0.
    updateDuration(0);
    m_position->setText(KNMusicUtil::msecondToString(0));
    //Hide the main player and append menu button.
    m_showMainPlayer->hide();
    m_showMiniPlayer->hide();
    m_showAppendMenu->hide();
}

void KNMusicHeaderPlayer::retranslate()
{
    //Set tooltip for all the buttons.
    m_showMainPlayer->setToolTip(tr("Show main player"));
    m_showMiniPlayer->setToolTip(tr("Show mini player"));
    m_showAppendMenu->setToolTip(tr("More"));
    //Set the sub menu title.
    m_subMenus[SubMenuRating]->menuAction()->setText(tr("Rating"));
    m_subMenus[SubMenuLocate]->menuAction()->setText(tr("Locate"));
    //Set the no stars title.
    m_menuActions[AppendRatingNoStar]->setText(tr("(No star)"));
    //Set the detail info title.
    m_menuActions[AppendShowDetail]->setText(tr("Get Info"));
    //Set the download lyrics title.
    m_menuActions[AppendDownloadLyrics]->setText(tr("Download Lyrics"));
    //Set the action title.
#ifdef Q_OS_WIN32
    m_menuActions[AppendShowInGraphicShell]->setText(tr("Show in Explorer"));
#endif
#ifdef Q_OS_MAC
    m_menuActions[AppendShowInGraphicShell]->setText(tr("Show in Finder"));
#endif
#ifdef Q_OS_LINUX
    m_menuActions[AppendShowInGraphicShell]->setText(
                tr("Show the contains folder"));
#endif
    m_menuActions[AppendLocateNowPlaying]->setText(tr("Locate the playing"
                                                      " song"));
    m_menuActions[AppendShowInSongs]->setText(tr("Show in songs"));
    m_menuActions[AppendShowInArtists]->setText(tr("Show in artists"));
    m_menuActions[AppendShowInAlbums]->setText(tr("Show in albums"));
    m_menuActions[AppendShowInGenres]->setText(tr("Show in genres"));
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
        m_loopState->setIcon(m_iconLoopState[state]);
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
    qint64 minutePart=positionText.left(colonPosition).toInt(),
           secondPart=positionText.mid(colonPosition+1).toInt(),
           preferPosition=(minutePart*60+secondPart)*1000;
    if(preferPosition>-1 && preferPosition<m_progressSlider->maximum())
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

void KNMusicHeaderPlayer::onActionNowPlayingChanged(
        const KNMusicAnalysisItem &analysisItem)
{
    //Get the current detail information.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Update the label text data.
    m_title->setText(detailInfo.textLists[Name].toString());
    setAristAndAlbum(detailInfo.textLists[Artist].toString(),
                     detailInfo.textLists[Album].toString());
    //Update the cover album art image.
    QPixmap albumArt=QPixmap::fromImage(analysisItem.coverImage);
    m_albumArt->setPixmap(albumArt.isNull()?
                              knMusicGlobal->noAlbumArt():albumArt);
    //When it has been asked to update player info, then show the buttons.
    m_showMainPlayer->show();
    m_showMiniPlayer->show();
    m_showAppendMenu->setVisible(detailInfo.url.isEmpty());
    //Configure the loop button.
    m_loopState->setVisible(detailInfo.url.isEmpty());
}

void KNMusicHeaderPlayer::onLyricsConfigureChanged()
{
    //Get the lyrics font.
    QFont lyricsFont=m_lyricsConfigure->data("Font").value<QFont>();
    //Update the header lyrics font.
    m_headerLyrics->setFont(lyricsFont);
}

void KNMusicHeaderPlayer::showAppendMenu()
{
    //Ensure there's a file is playing.
    //Check the playing file path.
    if(m_nowPlaying==nullptr ||
            m_nowPlaying->playingItem().detailInfo.filePath.isEmpty())
    {
        return;
    }
    //Set the menu shown flag.
    m_appendMenuShown=true;
    //Launch the menu.
    m_appendMenu->exec();
    //Reset the flag.
    m_appendMenuShown=false;
    //Ask the header widget to check the cursor.
    emit requireCheckCursor();
}

void KNMusicHeaderPlayer::appendActionTriggered(const int &actionIndex)
{
    //Reset the flag.
    m_appendMenuShown=false;
    //Ask the header widget to check the cursor.
    emit requireCheckCursor();
    //Prepare the playing item.
    KNMusicAnalysisItem playingItem;
    //Ensure that there's a file is playing.
    if(m_nowPlaying==nullptr)
    {
        return;
    }
    //Save the playing item.
    playingItem=m_nowPlaying->playingItem();
    //Check the playing file path.
    if(playingItem.detailInfo.filePath.isEmpty())
    {
        //Ignore the invalid playint item.
        return;
    }
    //Check the action index.
    switch(actionIndex)
    {
    case AppendRatingNoStar:
    case AppendRatingOneStar:
    case AppendRatingTwoStar:
    case AppendRatingThreeStar:
    case AppendRatingFourStar:
    case AppendRatingFiveStar:
        //Check the now playing.
        if(m_nowPlaying)
        {
            //Set the plaing rating.
            m_nowPlaying->setPlayingRating(actionIndex-AppendRatingNoStar);
        }
        break;
    case AppendShowInGraphicShell:
        KNUtil::showInGraphicalShell(playingItem.detailInfo.filePath);
        break;
    case AppendDownloadLyrics:
        //Check the pointer of download dialog and model.
        //Check the validation of the item index.
        if(knMusicGlobal->lyricsDownloadDialog() && m_nowPlaying
                && m_nowPlaying->playingIndex().isValid())
        {
            //Get the download dialog.
            KNMusicLyricsDownloadDialogBase *downloadDialog=
                    knMusicGlobal->lyricsDownloadDialog();
            //Set the detail info to the download dialog.
            downloadDialog->setDetailInfo(
                        m_nowPlaying->playingItem().detailInfo);
            //Show up the download lyrics dialog, and give it the detail info.
            downloadDialog->exec();
        }
        break;
    case AppendShowDetail:
        //Check the now playing.
        if(m_nowPlaying && m_nowPlaying->playingModel())
        {
            //Show the detail of the playing item.
            knMusicGlobal->detailDialog()->showDialog(
                        playingItem,
                        m_nowPlaying->playingModel(),
                        m_nowPlaying->playingModel()->mapFromSource(
                            m_nowPlaying->playingIndex()));
        }
        break;
    case AppendLocateNowPlaying:
    {
        //Get the playing tab from now playing.
        KNMusicTab *playingTab=m_nowPlaying->playingTab();
        //Check the playing tab.
        if(playingTab)
        {
            //Show the music model and the playing index.
            playingTab->showIndex(m_nowPlaying->playingMusicModel(),
                                  m_nowPlaying->playingIndex());
        }
        break;
    }
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

inline KNOpacityAnimeButton *KNMusicHeaderPlayer::generateAppendButton(
        const QString &iconPath)
{
    //Generate a opacity button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    button->setFixedSize(14, 14);
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

inline void KNMusicHeaderPlayer::setAristAndAlbum(const QString &artist,
                                                  const QString &album)
{
    //Clear the label first.
    m_artistAlbum->setText("");
    //Set the text.
    if(artist.isEmpty())
    {
        //Set the label to display the 'Album'
        m_artistAlbum->setText(album);
    }
    else
    {
        //Check if the album is empty.
        if(album.isEmpty())
        {
            //Set the lable to display 'Artist'
            m_artistAlbum->setText(artist);
        }
        else
        {
            //Set the lable to display 'Artist - Album'
            m_artistAlbum->setText(artist + " - " + album);
        }
    }
}

void KNMusicHeaderPlayer::updateDuration(const qint64 &duration)
{
    //Change the progress slider range.
    m_progressSlider->setMaximum(duration);
    //Set duration display text.
    m_duration->setText(KNMusicUtil::msecondToString(duration));
}
