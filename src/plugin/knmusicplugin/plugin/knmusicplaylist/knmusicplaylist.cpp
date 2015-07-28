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
#include <QSplitter>

#include "kncategorytab.h"
#include "knemptystatewidget.h"
#include "knlocalemanager.h"

#include "sdk/knmusicplaylistemptyhint.h"
#include "sdk/knmusicplaylistlist.h"
#include "sdk/knmusicplaylistlistmodel.h"
#include "sdk/knmusicplaylistviewer.h"
#include "sdk/knmusicplaylistmanager.h"

#include "knmusicglobal.h"

#include "knmusicplaylist.h"

KNMusicPlaylist::KNMusicPlaylist(QWidget *parent) :
    KNMusicPlaylistBase(parent),
    m_tab(new KNCategoryTab(this)),
    m_container(new KNEmptyStateWidget(this)),
    m_playlistList(new KNMusicPlaylistList(this)),
    m_playlistViewer(new KNMusicPlaylistViewer(this)),
    m_playlistManager(new KNMusicPlaylistManager(this))
{
    //Configure the tab.
    m_tab->setIcon(QIcon(":/plugin/music/playlist/icon.png"));
    //Set the playlist folder path.
    m_playlistManager->setPlaylistDirPath(
                knMusicGlobal->musicLibPath()+"/Playlist");
    //Set the playlist list model to the playlist list.
    m_playlistList->setPlaylistList(m_playlistManager->playlistList());

    //Link the GUI with che playlist manager.
    connect(m_playlistManager, &KNMusicPlaylistManager::requireShowContent,
            m_container, &KNEmptyStateWidget::showContentWidget);
    connect(m_playlistManager, &KNMusicPlaylistManager::requireHideContent,
            m_container, &KNEmptyStateWidget::showEmptyWidget);
    connect(m_playlistList, &KNMusicPlaylistList::requireCreatePlaylist,
            this, &KNMusicPlaylist::onActionCreatePlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireShowPlaylist,
            [=](const QModelIndex &index)
            {
                m_playlistViewer->setPlaylist(
                            m_playlistManager->playlist(index));
            });

    //Generate the empty hint widget.
    KNMusicPlaylistEmptyHint *emptyHint=new KNMusicPlaylistEmptyHint(this);
    //Link the empty hint with the container.
    connect(emptyHint, &KNMusicPlaylistEmptyHint::requireAddPlaylist,
            this, &KNMusicPlaylist::onActionCreatePlaylist);
    //Set the empty widget and content widget.
    m_container->setEmptyWidget(emptyHint);
    //Get the splitter.
    QSplitter *contentContainer=generateSplitter();
    //Configure the splitter.
    contentContainer->setChildrenCollapsible(false);
    //Set the content widget as the splitter.
    m_container->setContentWidget(contentContainer);
    //Add the playlist list and playlist viewer to the splitter.
    contentContainer->addWidget(m_playlistList);
    contentContainer->addWidget(m_playlistViewer);
    //Set the default stretch factor.
    contentContainer->setStretchFactor(1, 1);

    //Link to retranslate.
    knI18n->link(this, &KNMusicPlaylist::retranslate);
    retranslate();
}

QAbstractButton *KNMusicPlaylist::tab()
{
    return m_tab;
}

void KNMusicPlaylist::showEvent(QShowEvent *event)
{
    //If the playlist list has never been load before, load it.
    if(!m_playlistManager->isPlaylistListLoaded())
    {
        //Load the playlist list.
        m_playlistManager->loadPlaylistList();
        //Set the load flag.
        m_playlistManager->setPlaylistListLoaded(true);
        //Get the playlist list temporarily.
        KNMusicPlaylistListModel *playlistList=
                m_playlistManager->playlistList();
        //Check if there's any playlist exist in the playlist list, select the
        //first one.
        if(playlistList->rowCount()>0)
        {
            //Select the first index.
            m_playlistList->showPlaylist(playlistList->index(0));
        }
    }
    //Do the original show event.
    KNMusicPlaylistBase::showEvent(event);
}

void KNMusicPlaylist::resizeEvent(QResizeEvent *event)
{
    //Resize the tab widget.
    KNMusicPlaylistBase::resizeEvent(event);
    //Resize the container.
    m_container->resize(size());
}

void KNMusicPlaylist::retranslate()
{
    //Update the tab title.
    m_tab->setText(tr("Playlist"));
}

void KNMusicPlaylist::onActionCreatePlaylist()
{
    //Generate a empty playlist model in the playlist manager, and get the index
    //of the model.
    QModelIndex playlistIndex=m_playlistManager->createPlaylist();
    //Show that playlist.
    m_playlistList->showPlaylist(playlistIndex);
    //Ask the list view to rename it.
    m_playlistList->renamePlaylist(playlistIndex);
    //Make the container switch to the content widget.
    m_container->showContentWidget();
}

QSplitter *KNMusicPlaylist::generateSplitter()
{
    //Generate the splitter.
    QSplitter *splitter=new QSplitter(this);
    //Configure the splitter.
    splitter->setHandleWidth(0);
    //Return the splitter.
    return splitter;
}
