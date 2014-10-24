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
#include "knmusicplaylistlistitem.h"
#include "knmusicplaylisttab.h"
#include "knmusicplaylistlist.h"
#include "knmusicnowplayingbase.h"

#include "knmusicplaylistmanager.h"

#include <QDebug>

KNMusicPlaylistManager::KNMusicPlaylistManager(QObject *parent) :
    KNMusicPlaylistManagerBase(parent)
{
    //Initial playlist icon.
    m_playlistIcon=QIcon(":/plugin/music/playlist/playlist.png");
    //Initial playlist viewer UI.
    m_playlistTab=new KNMusicPlaylistTab(this);
    //Generate the playlist list.
    m_playlistList=new KNMusicPlaylistList(this);
    //Set the playlist list.
    m_playlistTab->setPlaylistList(m_playlistList);

    //Link the UI's request.
    connect(m_playlistTab, &KNMusicPlaylistTab::requireGeneratePlaylist,
            this, &KNMusicPlaylistManager::onActionAddPlaylist);
    connect(m_playlistTab, &KNMusicPlaylistTab::currentPlaylistChanged,
            this, &KNMusicPlaylistManager::onActionCurrentPlaylistChanged);
    //When the data of playlist list has been changed, update the detail.
    connect(m_playlistList, &KNMusicPlaylistList::itemChanged,
            m_playlistTab, &KNMusicPlaylistTab::onActionPlaylistItemChanged);
}

KNMusicTab *KNMusicPlaylistManager::categoryTab()
{
    return m_playlistTab;
}

void KNMusicPlaylistManager::onActionAddPlaylist(const QString &caption)
{
    KNMusicPlaylistListItem *playlistItem=generatePlaylist(caption);
    m_playlistList->appendRow(playlistItem);
    //Set the new playlist to the current playlist.
    m_playlistTab->setCurrentPlaylist(playlistItem->index());
    //Let user rename it automatically.
    m_playlistTab->editPlaylistName(playlistItem->index());
}

void KNMusicPlaylistManager::onActionCurrentPlaylistChanged(const QModelIndex &current,
                                                            const QModelIndex &previous)
{
    //Ask UI to display the playlist item.
    m_playlistTab->displayPlaylistItem(m_playlistList->playlistItemFromIndex(current));
}

KNMusicPlaylistListItem *KNMusicPlaylistManager::generatePlaylist(const QString &caption)
{
    //Generate a default playlist.
    KNMusicPlaylistListItem *playlistItem=new KNMusicPlaylistListItem();
    playlistItem->setIcon(m_playlistIcon);
    playlistItem->setText(caption);
    return playlistItem;
}
