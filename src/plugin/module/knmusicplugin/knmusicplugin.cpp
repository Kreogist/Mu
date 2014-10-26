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

//Music Global.
#include "knmusicglobal.h"

//Ports
#include "knmusicbackend.h"
#include "knmusicparser.h"
#include "knmusicsolomenubase.h"
#include "knmusicheaderplayerbase.h"
#include "knmusicheaderlyricsbase.h"
#include "knmusicnowplayingbase.h"
#include "knmusicplaylistmanagerbase.h"
#include "knmusictab.h"

//Plugins
#include "plugin/knmusicbackendbass/knmusicbackendbass.h"
#include "plugin/knmusicbackendbass/knmusicbassanalysiser.h"
#include "plugin/knmusicheaderplayer/knmusicheaderplayer.h"
#include "plugin/knmusicsolomenu/knmusicsolomenu.h"
#include "plugin/knmusicmultimenu/knmusicmultimenu.h"
#include "plugin/knmusicheaderlyrics/knmusicheaderlyrics.h"
#include "plugin/knmusicnowplaying/knmusicnowplaying.h"
#include "plugin/knmusicplaylistmanager/knmusicplaylistmanager.h"

#include "kncategorytabwidget.h"
#include "knpreferencewidgetspanel.h"
#include "knmusicplugin.h"

#include <QDebug>

KNMusicPlugin::KNMusicPlugin(QObject *parent) :
    KNAbstractMusicPlugin(parent)
{
    //Initial infrastructure.
    initialInfrastructure();
    //Initial parser.
    initialParser();
    //Initial menus.
    initialSoloMenu(new KNMusicSoloMenu);
    initialMultiMenu(new KNMusicMultiMenu);

    //Load plugins.
    loadBackend(new KNMusicBackendBass);
    loadHeaderPlayer(new KNMusicHeaderPlayer);
    loadHeaderLyrics(new KNMusicHeaderLyrics);
    loadNowPlaying(new KNMusicNowPlaying);
    loadPlaylistManager(new KNMusicPlaylistManager);

    //Do the translation at the last.
    retranslate();
}

KNMusicPlugin::~KNMusicPlugin()
{
    //Stop threads.
    m_parserThread.quit();
    m_parserThread.wait();
    //Delete all the plugins.
    while(!m_pluginList.isEmpty())
    {
        delete m_pluginList.takeFirst();
    }
}

QString KNMusicPlugin::caption()
{
    return tr("Music");
}

QPixmap KNMusicPlugin::icon()
{
    return QPixmap(":/plugin/music/common/icon.png");
}

QPixmap KNMusicPlugin::headerIcon()
{
    return QPixmap(":/plugin/configure/music/headicon.png");
}

QPixmap KNMusicPlugin::preferenceIcon()
{
    return QPixmap(":/plugin/configure/music/icon.png");
}

QWidget *KNMusicPlugin::centralWidget()
{
    return m_centralWidget;
}

QWidget *KNMusicPlugin::headerWidget()
{
    return m_headerWidget;
}

KNPreferenceWidgetsPanel *KNMusicPlugin::preferencePanelWidget()
{
    return m_preferencePanel;
}

void KNMusicPlugin::loadBackend(KNMusicBackend *plugin)
{
    if(m_backend==nullptr)
    {
        m_backend=plugin;
        //Add plugin to the list.
        m_pluginList.append(m_backend);
    }
}

void KNMusicPlugin::loadHeaderPlayer(KNMusicHeaderPlayerBase *plugin)
{
    if(m_headerPlayer==nullptr)
    {
        m_headerPlayer=plugin;
        //Configure the header player.
        m_headerPlayer->setBackend(m_backend);
        //Restore configure.
        m_headerPlayer->restoreConfigure();
        //Add plugin to the list.
        m_pluginList.append(m_headerPlayer);
        //Add to main window.
        addLeftHeaderWidget(m_headerPlayer);
    }
}

void KNMusicPlugin::loadHeaderLyrics(KNMusicHeaderLyricsBase *plugin)
{
    //Add plugin to the list.
    m_pluginList.append(plugin);
    //Link the display to the header player.
    plugin->setHeaderPlayer(m_headerPlayer);
    //Add widget to the header.
    addLeftHeaderWidget(plugin, 1);
}

void KNMusicPlugin::loadNowPlaying(KNMusicNowPlayingBase *plugin)
{
    //Set the header player.
    plugin->setHeaderPlayer(m_headerPlayer);
    //Add plugin to list.
    m_pluginList.append(plugin);
    //Set global now playing plugin.
    KNMusicGlobal::setNowPlaying(plugin);
}

void KNMusicPlugin::loadPlaylistManager(KNMusicPlaylistManagerBase *plugin)
{
    m_pluginList.append(plugin);
    //Add tabs.
    addMusicTab(plugin->categoryTab());
}

void KNMusicPlugin::onArgumentsAvailable(const QStringList &data)
{
    KNMusicGlobal::nowPlaying()->playTemporaryFiles(data);
}

void KNMusicPlugin::addMusicCategory(const QPixmap &icon,
                                     const QString &caption,
                                     QWidget *widget)
{
    m_centralWidget->addTab(icon, caption, widget);
}

void KNMusicPlugin::addLeftHeaderWidget(QWidget *widget,
                                        int stretch,
                                        Qt::Alignment alignment)
{
    m_headerLeftLayout->addWidget(widget, stretch, alignment);
}

void KNMusicPlugin::addRightHeaderWidget(QWidget *widget,
                                         int stretch,
                                         Qt::Alignment alignment)
{
    m_headerRightLayout->addWidget(widget, stretch, alignment);
}

void KNMusicPlugin::initialInfrastructure()
{
    //Initial the music global.
    m_musicGlobal=KNMusicGlobal::instance();
    m_musicGlobal->setNoAlbumArt(QPixmap(":/plugin/music/common/noalbum.png"));

    //Initial preference panel.
    m_preferencePanel=new KNPreferenceWidgetsPanel;
    m_preferencePanel->setPanelCaption("Music");
    m_musicGlobal->setPreferencePanel(m_preferencePanel);

    //Initial central widget.
    m_centralWidget=new KNCategoryTabWidget;

    //Initial header widget.
    m_headerWidget=new QWidget;
    //Set header properties.
    m_headerWidget->setContentsMargins(0,0,0,0);
    //Set header layout.
    QBoxLayout *headerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_headerWidget);
    headerLayout->setContentsMargins(0,0,0,0);
    headerLayout->setSpacing(0);
    m_headerWidget->setLayout(headerLayout);
    //Initial left layout.
    m_headerLeftLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                      headerLayout->widget());
    m_headerLeftLayout->setContentsMargins(0,0,0,0);
    m_headerLeftLayout->setSpacing(0);
    headerLayout->addLayout(m_headerLeftLayout);
    //Initial right layout.
    m_headerRightLayout=new QBoxLayout(QBoxLayout::RightToLeft,
                                       headerLayout->widget());
    m_headerRightLayout->setContentsMargins(0,0,0,0);
    m_headerRightLayout->setSpacing(0);
    headerLayout->addLayout(m_headerRightLayout);
}

void KNMusicPlugin::initialParser()
{
    //Initial the music parser.
    KNMusicParser *parser=new KNMusicParser;

    //Install all plugins here.
    parser->installAnalysiser(new KNMusicBassAnalysiser);

    //Add this to plugin list.
    m_pluginList.append(parser);
    //Move to working thread.
    parser->moveToThread(&m_parserThread);
    //Set the parser.
    KNMusicGlobal::setParser(parser);
}

void KNMusicPlugin::initialSoloMenu(KNMusicSoloMenuBase *soloMenu)
{
    //Add this to plugin list.
    m_pluginList.append(soloMenu);
    //Set the solo menu.
    KNMusicGlobal::setSoloMenu(soloMenu);
}

void KNMusicPlugin::initialMultiMenu(KNMusicMultiMenuBase *multiMenu)
{
    //Add this to plugin list.
    m_pluginList.append(multiMenu);
    //Set the multi menu.
    KNMusicGlobal::setMultiMenu(multiMenu);
}

void KNMusicPlugin::addMusicTab(KNMusicTab *musicTab)
{
    //Just add them to a new music category.
    addMusicCategory(musicTab->icon(),
                     musicTab->caption(),
                     musicTab->widget());
}

void KNMusicPlugin::startThreads()
{
    m_parserThread.start();
}
