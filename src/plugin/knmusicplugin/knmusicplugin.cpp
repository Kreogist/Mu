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

//Dependence
#include "kncategorytab.h"
#include "kncategorytabbar.h"
#include "knhwidgetswitcher.h"
#include "knmousedetectheader.h"
#include "knsideshadowwidget.h"
#include "knvwidgetswitcher.h"
#include "knglobal.h"

//SDK Dependence.
#include "knmusicdetaildialog.h"
#include "knmusicparser.h"

//Ports
#include "knmusicdetaildialogpanel.h"
#include "knmusictagparser.h"
#include "knmusicsearchbase.h"
#include "knmusicsolomenubase.h"
#include "knmusicmultimenubase.h"
#include "knmusicplaylistbase.h"
#include "knmusicbackend.h"
#include "knmusicnowplayingbase.h"
#include "knmusicdetailtooltipbase.h"
#include "knmusicheaderplayerbase.h"
#include "knmusicplayerbase.h"

//Plugins
// Detail Dialog Panels.
#include "plugin/knmusicdetailpaneloverview/knmusicdetailpaneloverview.h"
// Tag Parsers.
#include "plugin/knmusictagid3v1/knmusictagid3v1.h"
#include "plugin/knmusictagid3v2/knmusictagid3v2.h"
#include "plugin/knmusictagapev2/knmusictagapev2.h"
// Solo Music Menu.
#include "plugin/knmusicsolomenu/knmusicsolomenu.h"
// Multi Music Menu.
#include "plugin/knmusicmultimenu/knmusicmultimenu.h"
// Search.
#include "plugin/knmusicsearch/knmusicsearch.h"
// Backends.
#ifdef ENABLE_BACKEND_BASS
#include "plugin/knmusicbackendbass/knmusicbackendbass.h"
#endif
// Now Playing.
#include "plugin/knmusicnowplaying/knmusicnowplaying.h"
// Detail Tooltip.
#include "plugin/knmusicdetailtooltip/knmusicdetailtooltip.h"
// Header Player.
#include "plugin/knmusicheaderplayer/knmusicheaderplayer.h"
// Playlist.
#include "plugin/knmusicplaylist/knmusicplaylist.h"

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
    m_headerPlayer(nullptr)
{
    //Initial the basic infrastructure.
    initialInfrastructure();
    //Initial the detail dialog plugins.
    initialDetailDialogPanel();
    //Initial parser.
    initialParserPlugin();
    //Initial global menus.
    initialSoloMenu(new KNMusicSoloMenu);
    initialMultiMenu(new KNMusicMultiMenu);

    //Initial the search.
    initialSearch(new KNMusicSearch);
#ifdef ENABLE_BACKEND_BASS
    initialBackend(new KNMusicBackendBass);
#endif
    //Initial the now playing.
    initialNowPlaying(new KNMusicNowPlaying);
    //Iniital the detail tooltip.
    initialDetailTooltip(new KNMusicDetailTooltip);
    //Iniital the header player.
    initialHeaderPlayer(new KNMusicHeaderPlayer);
    //Initial the plugins.
    initialPlaylist(new KNMusicPlaylist);

    //Start working threads.
    knMusicGlobal->startThreads();
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
    //Recover the tooltip menu.
    if(knMusicGlobal->detailTooltip() &&
            knMusicGlobal->detailTooltip()->parent()==nullptr)
    {
        knMusicGlobal->detailTooltip()->deleteLater();
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

void KNMusicPlugin::saveConfigure()
{
    //--Header Player--
    if(m_headerPlayer)
    {
        m_headerPlayer->saveConfigure();
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

void KNMusicPlugin::resizeEvent(QResizeEvent *event)
{
    //Do original resize event.
    KNAbstractMusicPlugin::resizeEvent(event);
    //Resize the shadow.
    m_topShadow->resize(width(),
                        m_topShadow->height());
}

void KNMusicPlugin::initialInfrastructure()
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
}

void KNMusicPlugin::initialDetailDialogPanel()
{
    //Add panels to detail dialog.
    knMusicGlobal->detailDialog()->addPanel(new KNMusicDetailPanelOverview);
}

void KNMusicPlugin::initialParserPlugin()
{
    //Get the music parser.
    KNMusicParser *parser=knMusicGlobal->parser();

    //Add tag parsers.
    parser->installTagParser(new KNMusicTagId3v1);
    parser->installTagParser(new KNMusicTagId3v2);
    parser->installTagParser(new KNMusicTagApev2);
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

void KNMusicPlugin::initialSearch(KNMusicSearchBase *search)
{
    //Set the parent of the search.
    search->setParent(this);
    //Add the widget to the header widget.
    m_headerRightLayout->addWidget(search->widget());
    //Set the activate action of the content widget.
    m_switcher->addAction(search->activateAction());
    //Set the search to music global.
    knMusicGlobal->setSearch(search);
}

void KNMusicPlugin::initialBackend(KNMusicBackend *backend)
{
    //Set the parent of the backend.
    backend->setParent(this);
    //Set the backend to music global.
    knMusicGlobal->setBackend(backend);
}

void KNMusicPlugin::initialNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //Set the parent of the now playing.
    nowPlaying->setParent(this);
    //Set the backend to now playing first.
    nowPlaying->setBackend(knMusicGlobal->backend());
    //Load configure
    nowPlaying->loadConfigure();
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
    //Check if the header player is nullptr.
    if(headerPlayer==nullptr)
    {
        return;
    }
    //Save the header player.
    m_headerPlayer=headerPlayer;
    //Set the backend and the now playing.
    m_headerPlayer->setBackend(knMusicGlobal->backend());
    m_headerPlayer->setNowPlaying(knMusicGlobal->nowPlaying());
    //Add the header player to the header left layout.
    m_headerWidgetContainer->addWidget(m_headerPlayer);
    //Link the header and the header player.
    connect(m_headerWidget, &KNMouseDetectHeader::requireActivateWidget,
            m_headerPlayer, &KNMusicHeaderPlayerBase::activate);
    connect(m_headerWidget, &KNMouseDetectHeader::requireInactivateWidget,
            m_headerPlayer, &KNMusicHeaderPlayerBase::inactivate);
    //Load the configuration.
    m_headerPlayer->loadConfigure();
}

void KNMusicPlugin::initialPlaylist(KNMusicPlaylistBase *playlist)
{
    //Add playlist content to tab.
    m_switcher->addWidget(playlist);
    m_tabBar->addTab(playlist->tab());
}
