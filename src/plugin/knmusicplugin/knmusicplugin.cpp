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
#include <QPropertyAnimation>
#include <QSignalMapper>

//Dependence
#include "kncategorytab.h"
#include "kncategorytabbar.h"
#include "knhwidgetswitcher.h"
#include "knmousedetectheader.h"
#include "knsideshadowwidget.h"
#include "knvwidgetswitcher.h"
#include "knglobal.h"
#include "knplatformextras.h"

//SDK Dependence.
#include "knmusicdetaildialog.h"
#include "knmusicparser.h"
#include "knmusiclyricsmanager.h"
#include "knmusiclyricsbackend.h"
#include "knmusicdetailtageditpanel.h"

//Ports
#include "knmusicdetaildialogpanel.h"
#include "knmusictagparser.h"
#include "knmusicsearchbase.h"
#include "knmusicsolomenubase.h"
#include "knmusicmultimenubase.h"
#include "knmusictranscoderbase.h"
#include "knmusicbackend.h"
#include "knmusicnowplayingbase.h"
#include "knmusicdetailtooltipbase.h"
#include "knmusicheaderplayerbase.h"
#include "knmusicplayerbase.h"
#include "knmusicplaylistbase.h"
#include "knmusiclibrarybase.h"
#include "knmusictab.h"
#include "knmusiclyricsdownloaddialogbase.h"
#include "knmusicminiplayerbase.h"
#include "knmusicstorebase.h"

//Plugins
// Detail Dialog Panels.
#include "plugin/knmusicdetailpaneloverview/knmusicdetailpaneloverview.h"
#include "plugin/knmusicdetailpanelartwork/knmusicdetailpanelartwork.h"
// Tag Parsers.
#include "plugin/knmusictagid3v1/knmusictagid3v1.h"
#include "plugin/knmusictagid3v2/knmusictagid3v2.h"
#include "plugin/knmusictagid3v2/knmusictagwav.h"
#include "plugin/knmusictagapev2/knmusictagapev2.h"
#include "plugin/knmusictagflac/knmusictagflac.h"
#include "plugin/knmusictagm4a/knmusictagm4a.h"
#include "plugin/knmusictagwma/knmusictagwma.h"
// Transcoder.
#include "plugin/knmusicffmpegtranscoder/knmusicffmpegtranscoder.h"
// List Parsers.
#include "plugin/knmusiccuelistparser/knmusiccuelistparser.h"
// Lyrics Downloader.
#include "plugin/knmusicneteaselyrics/knmusicneteaselyrics.h"
#include "plugin/knmusickgmusiclyrics/knmusickgmusiclyrics.h"
#include "plugin/knmusicttplayerlyrics/knmusicttplayerlyrics.h"
#include "plugin/knmusicxiamilyrics/knmusicxiamilyrics.h"
#include "plugin/knmusicttpodlyrics/knmusicttpodlyrics.h"
#include "plugin/knmusicqqlyrics/knmusicqqlyrics.h"
// Lyrics Download Dialog.
#include "plugin/knmusiclyricsdownloaddialog/knmusiclyricsdownloaddialog.h"
// Solo Music Menu.
#include "plugin/knmusicsolomenu/knmusicsolomenu.h"
// Multi Music Menu.
#include "plugin/knmusicmultimenu/knmusicmultimenu.h"
// Search.
#include "plugin/knmusicsearch/knmusicsearch.h"
// Backends.
#ifdef ENABLE_BACKEND_BASS
#include "plugin/knmusicbackendbass/knmusicbackendbass.h"
#include "plugin/knmusicbackendbass/knmusicbassanalysiser.h"
#endif
#ifdef ENABLE_BACKEND_QTAV
#include "plugin/knmusicbackendqtav/knmusicbackendqtav.h"
#endif
#ifdef ENABLE_BACKEND_GSTREAMER
#include "plugin/knmusicbackendgstreamer/knmusicbackendgstreamer.h"
#endif
// Analysiser
#ifdef ENABLED_FFMPEG_ANALYSISER
#include "plugin/knmusicffmpeganalysiser/knmusicffmpeganalysiser.h"
#endif
// Now Playing.
#include "plugin/knmusicnowplaying/knmusicnowplaying.h"
// Detail Tooltip.
#include "plugin/knmusicdetailtooltip/knmusicdetailtooltip.h"
// Header Player.
#include "plugin/knmusicheaderplayer/knmusicheaderplayer.h"
// Main Player.
#include "plugin/knmusicmainplayer/knmusicmainplayer.h"
// Mini Player.
#include "plugin/knmusicminiplayer/knmusicminiplayer.h"
// Library.
#include "plugin/knmusiclibrary/knmusiclibrary.h"
// Playlist.
#include "plugin/knmusicplaylist/knmusicplaylist.h"
// Store
#include "plugin/knmusicstore/knmusicstore.h"

//Globals.
#include "knmusicglobal.h"

#include "knmusicplugin.h"

#include <QDebug>

#define ShadowHeight 15

KNMusicPlugin::KNMusicPlugin(QWidget *parent) :
    KNAbstractMusicPlugin(parent),
    m_headerWidget(new KNMouseDetectHeader(this)),
    m_headerWidgetContainer(new KNVWidgetSwitcher(m_headerWidget)),
    m_headerRightLayout(nullptr),
    m_tabBar(new KNCategoryTabBar(this)),
    m_switcher(new KNHWidgetSwitcher(this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadowWidget::TopShadow,
                                       this)),
    m_showInMapper(new QSignalMapper(this)),
    m_floatPlaylistList(nullptr),
    m_searchBox(nullptr),
    m_flowPlaylistListAnime(new QPropertyAnimation(this)),
    m_headerPlayer(nullptr),
    m_mainPlayer(nullptr),
    m_miniPlayer(nullptr),
    m_library(nullptr)
{
    //Initial the basic infrastructure.
    initialInfrastructure();
    //Link the UI element assistants.
    connect(m_showInMapper,
            static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
            m_tabBar, &KNCategoryTabBar::setCurrentIndex);
}

KNMusicPlugin::~KNMusicPlugin()
{
    //Recover the solo menu.
    if(knMusicGlobal->soloMenu() &&
            knMusicGlobal->soloMenu()->parent()==nullptr)
    {
        knMusicGlobal->soloMenu()->deleteLater();
    }
    //Recover the multi menu.
    if(knMusicGlobal->multiMenu() &&
            knMusicGlobal->multiMenu()->parent()==nullptr)
    {
        knMusicGlobal->multiMenu()->deleteLater();
    }
    //Recover the lyrics download menu.
    if(knMusicGlobal->lyricsDownloadDialog() &&
            knMusicGlobal->lyricsDownloadDialog()->parent()==nullptr)
    {
        knMusicGlobal->lyricsDownloadDialog()->deleteLater();
    }
    //Recover the tooltip menu.
    if(knMusicGlobal->detailTooltip() &&
            knMusicGlobal->detailTooltip()->parent()==nullptr)
    {
        knMusicGlobal->detailTooltip()->deleteLater();
    }
    //Recover the mini player.
    if(m_miniPlayer &&
            m_miniPlayer->parent()==nullptr)
    {
        m_miniPlayer->deleteLater();
    }
}

QWidget *KNMusicPlugin::headerWidget()
{
    return m_headerWidget;
}

QPixmap KNMusicPlugin::icon()
{
    return QPixmap(":/plugin/music/public/icon.png");
}

QString KNMusicPlugin::title()
{
    return tr("Music");
}

void KNMusicPlugin::loadPlugins()
{
    //Initial the detail dialog plugins.
    initialDetailDialogPanel();
    //Initial parser.
    initialParserPlugin();
    //Initial lyrics download plugin.
    initialLyricsPlugin();
    //Initial global menus.
    initialSoloMenu(new KNMusicSoloMenu);
    initialMultiMenu(new KNMusicMultiMenu);
    //Initial the lyrics download dialog.
    initialLyricsDownloadDialog(new KNMusicLyricsDownloadDialog);

    //Initial the search.
    initialSearch(new KNMusicSearch);
#ifdef ENABLE_BACKEND_BASS
    initialBackend(new KNMusicBackendBass);
#endif
#ifdef ENABLE_BACKEND_PHONON
    initialBackend(new KNMusicBackendPhonon);
#endif
#ifdef ENABLE_BACKEND_QTAV
    initialBackend(new KNMusicBackendQtAV);
#endif
#ifdef ENABLE_BACKEND_MPV
    initialBackend(new KNMusicBackendMpv);
#endif
#ifdef ENABLE_BACKEND_GSTREAMER
    initialBackend(new KNMusicBackendGStreamer);
#endif
    //Initial the now playing.
    initialNowPlaying(new KNMusicNowPlaying);
    //Iniital the detail tooltip.
    initialDetailTooltip(new KNMusicDetailTooltip);
    //Iniital the header player.
    initialHeaderPlayer(new KNMusicHeaderPlayer);
    //Initial the main player.
    initialMainPlayer(new KNMusicMainPlayer);
    //Initial the mini player.
    initialMiniPlayer(new KNMusicMiniPlayer);
    //Initial the library tab.
    initialLibrary(new KNMusicLibrary);
    //Initial the playlist tab.
    initialPlaylist(new KNMusicPlaylist);
    //Initial the store tab.
    //initialStore(new KNMusicStore);

    //Start working threads.
    knMusicGlobal->startThreads();
}

QWidget *KNMusicPlugin::mainPlayer()
{
    return m_mainPlayer;
}

QWidget *KNMusicPlugin::searchWidget()
{
    return m_searchBox;
}

bool KNMusicPlugin::isWorking()
{
    //If the library or playlist is working, the music library is working.
    return m_library->isWorking();
}

void KNMusicPlugin::saveConfigure()
{
    //--Header Player--
    if(m_headerPlayer)
    {
        m_headerPlayer->saveConfigure();
    }
    //--Mini Player--
    if(m_miniPlayer)
    {
        m_miniPlayer->saveConfigure();
    }
}

void KNMusicPlugin::onArgumentsAvailable(const QStringList &data)
{
    //Check the now playing is valid.
    if(knMusicGlobal->nowPlaying())
    {
        //Ask the now playing to play those files as temporary files.
        knMusicGlobal->nowPlaying()->playTemporaryFiles(data);
    }
}

void KNMusicPlugin::setPlatformExtras(KNPlatformExtras *platformExtras)
{
    //Check out the pointer.
    if(platformExtras==nullptr)
    {
        //Ignore the invalid pointer.
        return;
    }
    //Get now playing.
    KNMusicNowPlayingBase *nowPlaying=knMusicGlobal->nowPlaying();
    //Check the pointer.
    if(nowPlaying!=nullptr)
    {
        connect(platformExtras, &KNPlatformExtras::requirePlayPrev,
                nowPlaying, &KNMusicNowPlayingBase::playPrevious);
        connect(platformExtras, &KNPlatformExtras::requirePlayNext,
                nowPlaying, &KNMusicNowPlayingBase::playNext);
        connect(platformExtras, &KNPlatformExtras::requireChangeLoopState,
                nowPlaying, &KNMusicNowPlayingBase::changeLoopState);
        connect(nowPlaying, &KNMusicNowPlayingBase::loopStateChanged,
                [=](const int &loopState)
                {
                    switch (loopState)
                    {
                    case NoRepeat:
                        platformExtras->onActionLoopStateChanged(
                                    KNPlatformExtras::ButtonNoRepeat);
                        break;
                    case RepeatAll:
                        platformExtras->onActionLoopStateChanged(
                                    KNPlatformExtras::ButtonRepeatAll);
                        break;
                    case RepeatTrack:
                        platformExtras->onActionLoopStateChanged(
                                    KNPlatformExtras::ButtonRepeat);
                        break;
                    case Shuffle:
                        platformExtras->onActionLoopStateChanged(
                                    KNPlatformExtras::ButtonShuffle);
                        break;
                    default:
                        break;
                    }
                });
        connect(nowPlaying, &KNMusicNowPlayingBase::nowPlayingChanged,
                [=](const KNMusicAnalysisItem &analysisItem)
                {
                    //Construct a platform playing items.
                    KNPlatformExtras::PlatformPlayingInfo playingInfo;
                    //Check the item can't be null.
                    playingInfo.isNull=false;
                    //Save the information.
                    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
                    //Set the information.
                    playingInfo.name=detailInfo.textLists[Name].toString();
                    playingInfo.artist=detailInfo.textLists[Artist].toString();
                    playingInfo.album=detailInfo.textLists[Album].toString();
                    //Set the platform extras information.
                    platformExtras->onActionNowPlayingChanged(playingInfo);
                });
        connect(nowPlaying, &KNMusicNowPlayingBase::nowPlayingReset,
                [=]
                {
                    //Set a null information.
                    platformExtras->onActionNowPlayingChanged(
                                KNPlatformExtras::PlatformPlayingInfo());
                });
        //Sync the loop state data by calling a fake loop state changed signal.
        nowPlaying->loopStateChanged(nowPlaying->loopState());
    }
    //Get backend.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the pointer.
    if(backend!=nullptr)
    {
        connect(platformExtras, &KNPlatformExtras::requirePlay,
                backend, &KNMusicBackend::play);
        connect(platformExtras, &KNPlatformExtras::requireVolumeUp,
                backend, &KNMusicBackend::volumeUp);
        connect(platformExtras, &KNPlatformExtras::requireVolumeDown,
                backend, &KNMusicBackend::volumeDown);
        connect(platformExtras, &KNPlatformExtras::requireChangeMuteState,
                backend, &KNMusicBackend::changeMuteState);
        connect(platformExtras, &KNPlatformExtras::requirePause,
                backend, &KNMusicBackend::pause);
        connect(backend, &KNMusicBackend::muteStateChanged,
                platformExtras, &KNPlatformExtras::onActionMuteStateChanged);
        connect(backend, &KNMusicBackend::playingStateChanged,
                [=](const int &state)
                {
                    platformExtras->onActionPlayStateChanged(
                                state==Playing);
                });
    }
}

void KNMusicPlugin::showMiniPlayer()
{
    //Check and show the mini player.
    if(m_miniPlayer)
    {
        //Show the mini player.
        m_miniPlayer->show();
    }
}

void KNMusicPlugin::hideMiniPlayer()
{
    //Check mini player first.
    if(!m_miniPlayer)
    {
        //If there's no mini player, then do nothing.
        return;
    }
    //Hide the mini player.
    m_miniPlayer->hide();
}

void KNMusicPlugin::resizeEvent(QResizeEvent *event)
{
    //Do original resize event.
    KNAbstractMusicPlugin::resizeEvent(event);
    //Resize the shadow.
    m_topShadow->resize(width(),
                        m_topShadow->height());
    //Check the flow playlist list widget.
    if(m_floatPlaylistList)
    {
        //Reset the widget geometry.
        m_floatPlaylistList->setGeometry(width(),
                                        0,
                                        m_floatPlaylistList->width(),
                                        height());
    }
}

void KNMusicPlugin::onActionShowInSongs()
{
    //Check out library pointer first.
    if(m_library!=nullptr)
    {
        //Called show in songs slot of the library.
        m_library->showInSongTab();
    }
}

void KNMusicPlugin::onActionShowInArtists()
{
    //Check out library pointer first.
    if(m_library!=nullptr)
    {
        //Called show in artists slot of the library.
        m_library->showInArtistTab();
    }
}

void KNMusicPlugin::onActionShowInAlbums()
{
    //Check out library pointer first.
    if(m_library!=nullptr)
    {
        //Called show in albums slot of the library.
        m_library->showInAlbumTab();
    }
}

void KNMusicPlugin::onActionShowInGenres()
{
    //Check out library pointer first.
    if(m_library!=nullptr)
    {
        //Called show in genre slot of the library.
        m_library->showInGenreTab();
    }
}

void KNMusicPlugin::onActionHideMiniPlayer()
{
    //Ask to show the main window.
    emit requireShowMainWindow();
    //Hide the mini player.
    hideMiniPlayer();
}

void KNMusicPlugin::onActionShowMiniPlayer()
{
    //Ask to hide the main window.
    emit requireHideMainWindow();
    //Show the mini player.
    showMiniPlayer();
}

void KNMusicPlugin::onActionShowPlaylistFlow()
{
    //Check pointer first.
    if(m_floatPlaylistList==nullptr)
    {
        //Failed to start the animation.
        return;
    }
    //Reset the connections.
    disconnect(m_flowPlaylistListAnime, &QPropertyAnimation::finished, 0, 0);
    //Show the widget.
    m_floatPlaylistList->show();
    m_floatPlaylistList->setEnabled(true);
    //Stop the animation.
    m_flowPlaylistListAnime->stop();
    //Configure the start and end value of the animation.
    m_flowPlaylistListAnime->setStartValue(m_floatPlaylistList->geometry());
    m_flowPlaylistListAnime->setEndValue(
                QRect(width()-m_floatPlaylistList->width(),
                      0,
                      m_floatPlaylistList->width(),
                      height()));
    //Start the animation.
    m_flowPlaylistListAnime->start();
}

void KNMusicPlugin::onActionHidePlaylistFlow()
{
    //Check the pointer first.
    //Check pointer first.
    if(!m_floatPlaylistList)
    {
        //Failed to start the animation.
        return;
    }
    //Add hide connection.
    connect(m_flowPlaylistListAnime, &QPropertyAnimation::finished,
            m_floatPlaylistList, &QWidget::hide);
    m_floatPlaylistList->setEnabled(false);
    //Stop the animation.
    m_flowPlaylistListAnime->stop();
    //Configure the start and end value of the animation.
    m_flowPlaylistListAnime->setStartValue(m_floatPlaylistList->geometry());
    m_flowPlaylistListAnime->setEndValue(
                QRect(width(),
                      0,
                      m_floatPlaylistList->width(),
                      height()));
    //Start the animation.
    m_flowPlaylistListAnime->start();
}

void KNMusicPlugin::onMusicFileDropped(const QList<QUrl> &urlList)
{
    onArgumentsAvailable(KNUtil::urlListToPathList(urlList));
}

inline void KNMusicPlugin::initialInfrastructure()
{
    //Initial the music global.
    KNMusicGlobal::initial(this);

    //Configure the header widget.
    m_headerWidget->setContentsMargins(0,0,0,0);
    //Set header layout.
    QBoxLayout *headerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_headerWidget);
    headerLayout->setContentsMargins(0,0,10,0);
    headerLayout->setSpacing(0);
    m_headerWidget->setLayout(headerLayout);
    //Initial the vertical widget switcher.
    headerLayout->addWidget(m_headerWidgetContainer, 1);
    //Initial right layout.
    m_headerRightLayout=new QBoxLayout(QBoxLayout::RightToLeft,
                                       headerLayout->widget());
    m_headerRightLayout->setContentsMargins(10,0,0,0);
    m_headerRightLayout->setSpacing(0);
    headerLayout->addLayout(m_headerRightLayout);

    //Link the tab bar and switcher.
    connect(m_tabBar, &KNCategoryTabBar::currentIndexChange,
            m_switcher, &KNHWidgetSwitcher::setCurrentIndex);

    //Configure the top shadow position.
    m_topShadow->move(0, m_tabBar->height());
    m_topShadow->setFixedHeight(ShadowHeight);

    //Initial layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Add widget to layout.
    mainLayout->addWidget(m_tabBar);
    mainLayout->addWidget(m_switcher, 1);

    //Initial the animation.
    m_flowPlaylistListAnime->setDuration(200);
    m_flowPlaylistListAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_flowPlaylistListAnime->setPropertyName("geometry");
}

inline void KNMusicPlugin::initialPlayer(KNMusicPlayerBase *player)
{
    //Set the backend and the now playing.
    player->setBackend(knMusicGlobal->backend());
    player->setNowPlaying(knMusicGlobal->nowPlaying());
    //Link the player's signal to slot.
    connect(player, &KNMusicPlayerBase::requireShowInSongs,
            this, &KNMusicPlugin::onActionShowInSongs);
    connect(player, &KNMusicPlayerBase::requireShowInArtists,
            this, &KNMusicPlugin::onActionShowInArtists);
    connect(player, &KNMusicPlayerBase::requireShowInAlbums,
            this, &KNMusicPlugin::onActionShowInAlbums);
    connect(player, &KNMusicPlayerBase::requireShowInGenres,
            this, &KNMusicPlugin::onActionShowInGenres);
}

inline void KNMusicPlugin::addMusicTab(KNMusicTab *musicTab)
{
    //Check the music tab first.
    if(musicTab==nullptr)
    {
        return;
    }
    //Link the tab show in signal to the show in mapper.
    connect(musicTab, &KNMusicTab::requireShowTab,
            m_showInMapper,
            static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    m_showInMapper->setMapping(musicTab, m_switcher->count());
    //Add music content to tab manager.
    m_switcher->addWidget(musicTab);
    m_tabBar->addTab(musicTab->tab());
}

inline QAction *KNMusicPlugin::generateControlAction(int k1, int k2)
{
    QAction *controlAction=new QAction(this);
    QList<QKeySequence> shortcuts;
    shortcuts.append(QKeySequence(k1));
    shortcuts.append(QKeySequence(k2));
    controlAction->setShortcuts(shortcuts);
    controlAction->setShortcutContext(Qt::ApplicationShortcut);
    knGlobal->mainWindow()->addAction(controlAction);
    return controlAction;
}

void KNMusicPlugin::initialDetailDialogPanel()
{
    //Get the detail dialog.
    KNMusicDetailDialog *detailDialog=knMusicGlobal->detailDialog();

    //Add panels to detail dialog.
    detailDialog->addPanel(new KNMusicDetailPanelOverview);
    detailDialog->addTagEditPanel(new KNMusicDetailTagEditPanel);
    detailDialog->addPanel(new KNMusicDetailPanelArtwork);
}

void KNMusicPlugin::initialParserPlugin()
{
    //Get the music parser.
    KNMusicParser *parser=knMusicGlobal->parser();

    //Add tag parsers.
    parser->installTagParser(new KNMusicTagId3v1);
    parser->installTagParser(new KNMusicTagWma);
    parser->installTagParser(new KNMusicTagWav);
    parser->installTagParser(new KNMusicTagApev2);
    parser->installTagParser(new KNMusicTagM4a);
    parser->installTagParser(new KNMusicTagFlac);
    parser->installTagParser(new KNMusicTagId3v2);

    //Add analysiser.
#ifdef ENABLE_BACKEND_BASS
    parser->installAnalysiser(new KNMusicBassAnalysiser);
#endif
#ifdef ENABLED_FFMPEG_ANALYSISER
    parser->installAnalysiser(new KNMusicFfmpegAnalysiser);
#endif

    //Add list parsers.
    parser->installListParser(new KNMusicCueListParser);
}

void KNMusicPlugin::initialLyricsPlugin()
{
    //Get the lyrics manager.
    KNMusicLyricsManager *lyricsManager=knMusicGlobal->lyricsManager();

    //Add the downloader to lyrics manager.
    lyricsManager->appendDownloader(new KNMusicNeteaseLyrics);
    lyricsManager->appendDownloader(new KNMusicKgmusicLyrics);
    lyricsManager->appendDownloader(new KNMusicTtplayerLyrics);
    lyricsManager->appendDownloader(new KNMusicXiamiLyrics);
    lyricsManager->appendDownloader(new KNMusicTtpodLyrics);
    lyricsManager->appendDownloader(new KNMusicQQLyrics);
}

void KNMusicPlugin::initialSoloMenu(KNMusicSoloMenuBase *soloMenu)
{
    //Set the solo menu to the music global.
    knMusicGlobal->setSoloMenu(soloMenu);
}

void KNMusicPlugin::initialMultiMenu(KNMusicMultiMenuBase *multiMenu)
{
    //Set the multi menu to the music global.
    knMusicGlobal->setMultiMenu(multiMenu);
}

void KNMusicPlugin::initialLyricsDownloadDialog(
        KNMusicLyricsDownloadDialogBase *downloadDialog)
{
    //Set the download dialog to the music global.
    knMusicGlobal->setLyricsDownloadDialog(downloadDialog);
}

void KNMusicPlugin::initialSearch(KNMusicSearchBase *search)
{
    //Set the parent of the search.
    search->setParent(this);
    //Save the search widget.
    m_searchBox=search->widget();
    //Set the search to music global.
    knMusicGlobal->setSearch(search);
}

void KNMusicPlugin::initialBackend(KNMusicBackend *backend)
{
    //Set the parent of the backend.
    backend->setParent(this);
    //Link the backend to lyrics manager's backend.
    connect(backend, &KNMusicBackend::positionChanged,
            knMusicGlobal->lyricsManager()->backend(),
            &KNMusicLyricsBackend::setPosition);
    //Load all the operating actions to main window.
    QAction *playNPauseAction=
            generateControlAction(Qt::Key_F8,
                                  Qt::Key_MediaTogglePlayPause);
    connect(playNPauseAction, &QAction::triggered,
            backend, &KNMusicBackend::playNPause);
    QAction *muteAction=generateControlAction(Qt::Key_F10, Qt::Key_VolumeMute);
    connect(muteAction, &QAction::triggered,
            backend, &KNMusicBackend::changeMuteState);
    QAction *volumeDownAction=
            generateControlAction(Qt::Key_F11, Qt::Key_VolumeDown);
    connect(volumeDownAction, &QAction::triggered,
            backend, &KNMusicBackend::volumeDown);
    QAction *volumeUpAction=
            generateControlAction(Qt::Key_F12, Qt::Key_VolumeUp);
    connect(volumeUpAction, &QAction::triggered,
            backend, &KNMusicBackend::volumeUp);
    //Set the backend to music global.
    knMusicGlobal->setBackend(backend);
}

void KNMusicPlugin::initialNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //Set the parent of the now playing.
    nowPlaying->setParent(this);
    //Set the backend to now playing first.
    nowPlaying->setBackend(knMusicGlobal->backend());
    //Load configure.
    nowPlaying->loadConfigure();
    //Link the now playing with the lyrics manager.
    connect(nowPlaying, &KNMusicNowPlayingBase::nowPlayingChanged,
            knMusicGlobal->lyricsManager(), &KNMusicLyricsManager::loadLyrics);
    connect(nowPlaying, &KNMusicNowPlayingBase::nowPlayingReset,
            knMusicGlobal->lyricsManager(),
            &KNMusicLyricsManager::resetBackend);
    //Load opearting actions to main window.
    QAction *nextAction=generateControlAction(Qt::Key_F9, Qt::Key_MediaNext);
    connect(nextAction, &QAction::triggered,
            nowPlaying, &KNMusicNowPlayingBase::playNext);
    QAction *previousAction=
            generateControlAction(Qt::Key_F7, Qt::Key_MediaPrevious);
    connect(previousAction, &QAction::triggered,
            nowPlaying, &KNMusicNowPlayingBase::playPrevious);
    //Set the now playing to music global.
    knMusicGlobal->setNowPlaying(nowPlaying);
}

void KNMusicPlugin::initialDetailTooltip(KNMusicDetailTooltipBase *tooltip)
{
    //Set the detail tooltip to music global.
    knMusicGlobal->setDetailTooltip(tooltip);
}

void KNMusicPlugin::initialHeaderPlayer(KNMusicHeaderPlayerBase *headerPlayer)
{
    //Check if the header player is nullptr, or a header player is already
    //loaded.
    if(headerPlayer==nullptr || m_headerPlayer!=nullptr)
    {
        return;
    }
    //Save the header player.
    m_headerPlayer=headerPlayer;
    //Check the pointer.
    if(m_headerPlayer==nullptr)
    {
        return;
    }
    //Set the basic stuffs of a player.
    initialPlayer(m_headerPlayer);
    //Generate the header player's container.
    QWidget *container=new QWidget(m_headerWidgetContainer);
    //Generate the container layout.
    QBoxLayout *containerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                               container);
    containerLayout->setContentsMargins(0,0,0,0);
    containerLayout->setSpacing(0);
    container->setLayout(containerLayout);
    //Add player and lyrics to the layout.
    containerLayout->addWidget(m_headerPlayer);
    containerLayout->addWidget(m_headerPlayer->lyrics(), 1);
    //Add the header player to the header left layout.
    m_headerWidgetContainer->addWidget(container);
    //Link the header widget to the header player.
    connect(m_headerWidget, &KNMouseDetectHeader::requireActivateWidget,
            m_headerPlayer, &KNMusicHeaderPlayerBase::activate);
    connect(m_headerWidget, &KNMouseDetectHeader::requireInactivateWidget,
            m_headerPlayer, &KNMusicHeaderPlayerBase::inactivate);
    //Link the header player request.
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireShowMainPlayer,
            this, &KNMusicPlugin::requireShowMainPlayer);
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireShowMiniPlayer,
            this, &KNMusicPlugin::onActionShowMiniPlayer);
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireCheckCursor,
            m_headerWidget, &KNMouseDetectHeader::checkCursor);
    //Link the drop event.
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::urlsDropped,
            this, &KNMusicPlugin::onMusicFileDropped);
    //Load the configuration.
    m_headerPlayer->loadConfigure();
}

void KNMusicPlugin::initialMainPlayer(KNMusicMainPlayerBase *mainPlayer)
{
    //Check if the header player is nullptr.
    if(mainPlayer==nullptr || m_mainPlayer!=nullptr)
    {
        return;
    }
    //Save the header player.
    m_mainPlayer=mainPlayer;
    //Set the basic stuffs of a player.
    initialPlayer(m_mainPlayer);
    //Link the request.
    connect(m_mainPlayer, &KNMusicMainPlayerBase::requireHide,
            this, &KNMusicPlugin::requireHideMainPlayer);
    //Link the drop event.
    connect(m_mainPlayer, &KNMusicMainPlayerBase::urlsDropped,
            this, &KNMusicPlugin::onMusicFileDropped);
}

void KNMusicPlugin::initialMiniPlayer(KNMusicMiniPlayerBase *miniPlayer)
{
    //Check if the mini player is nullptr.
    if(miniPlayer==nullptr || m_miniPlayer!=nullptr)
    {
        return;
    }
    //Save the mini player.
    m_miniPlayer=miniPlayer;
    //Set the basic stuffs of a player.
    initialPlayer(m_miniPlayer);
    //Load the configure.
    m_miniPlayer->loadConfigure();
    //Hide the mini player.
    m_miniPlayer->hide();
    //Link the player request.
    connect(m_miniPlayer, &KNMusicMiniPlayerBase::requireHidePlayer,
            this, &KNMusicPlugin::onActionHideMiniPlayer);
    connect(m_miniPlayer, &KNMusicMiniPlayerBase::requireCloseMainWindow,
            this, &KNMusicPlugin::requireCloseMainWindow);
    //Link the drop event.
    connect(m_miniPlayer, &KNMusicMiniPlayerBase::urlsDropped,
            this, &KNMusicPlugin::onMusicFileDropped);
}

void KNMusicPlugin::initialPlaylist(KNMusicPlaylistBase *playlist)
{
    //Add playlist content to music plugin.
    addMusicTab(playlist);
    //Save the playlist list display widget.
    m_floatPlaylistList=playlist->playlistFloatList();
    //Configure the playlist list.
    if(m_floatPlaylistList)
    {
        //Reset the properties.
        m_floatPlaylistList->setParent(this);
        //Hide the list.
        m_floatPlaylistList->hide();
        m_floatPlaylistList->setEnabled(false);
        //Regeometry the playlist list.
        m_floatPlaylistList->setGeometry(width(), 0, 100, height());
        //Configure the animation object.
        m_flowPlaylistListAnime->setTargetObject(m_floatPlaylistList);
    }
}

void KNMusicPlugin::initialLibrary(KNMusicLibraryBase *library)
{
    //Save the library plugin.
    m_library=library;
    //Check the library plugin first.
    if(m_library==nullptr)
    {
        return;
    }
    //Set the library relationship.
    m_library->setParent(this);
    //Set the now playing.
    m_library->setNowPlaying(knMusicGlobal->nowPlaying());
    //Link the library show request to music plugin.
    connect(m_library, &KNMusicLibraryBase::requireShowPlaylistList,
            this, &KNMusicPlugin::onActionShowPlaylistFlow);
    connect(m_library, &KNMusicLibraryBase::requireHidePlaylistList,
            this, &KNMusicPlugin::onActionHidePlaylistFlow);
    //Add tabs to the switcher.
    addMusicTab(m_library->songTab());
    addMusicTab(m_library->artistTab());
    addMusicTab(m_library->albumTab());
    addMusicTab(m_library->genreTab());
}

void KNMusicPlugin::initialStore(KNMusicStoreBase *store)
{
    //Save the store plugin.
    m_store=store;
    //Load all the plugins.
    m_store->loadPlugins();
    //Add tabs to the switcher.
    addMusicTab(store);
}
