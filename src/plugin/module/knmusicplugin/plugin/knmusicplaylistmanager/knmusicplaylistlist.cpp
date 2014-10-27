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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "knconnectionhandler.h"
#include "knmusicplaylistlistitem.h"
#include "knmusicplaylistlistassistant.h"

#include "knmusicplaylistlist.h"

#include <QDebug>

KNMusicPlaylistList::KNMusicPlaylistList(QObject *parent) :
    QStandardItemModel(parent)
{
    //Initial the change handle.
    m_rowChangeHandle=new KNConnectionHandler(this);
    //When item changed, set the change flag.
    connect(this, &KNMusicPlaylistList::itemChanged,
            this, &KNMusicPlaylistList::onActionItemChanged);
}

KNMusicPlaylistModel *KNMusicPlaylistList::playlistModel(const int &row)
{
    return playlistItem(row)->playlistModel();
}

KNMusicPlaylistListItem *KNMusicPlaylistList::playlistItem(const int &row)
{
    return (KNMusicPlaylistListItem *)item(row, 0);
}

KNMusicPlaylistListItem *KNMusicPlaylistList::playlistItemFromIndex(const QModelIndex &index)
{
    return (KNMusicPlaylistListItem *)itemFromIndex(index);
}

void KNMusicPlaylistList::onActionRowInserted(const QModelIndex &parent,
                                              int first,
                                              int last)
{
    Q_UNUSED(parent)
    //Insert the playlist file path in to json array.
    for(int i=first; i<=last; i++)
    {
        KNMusicPlaylistListItem *currentItem=playlistItem(i);
        m_playlistListData.insert(i, currentItem->playlistFilePath());
    }
}

void KNMusicPlaylistList::onActionRowRemoved(const QModelIndex &parent,
                                             int first,
                                             int last)
{
    Q_UNUSED(parent);
    //Insert the playlist file path in to json array.
    for(int i=first; i<=last; i++)
    {
        m_playlistListData.removeAt(i);
    }
}
QStringList KNMusicPlaylistList::playlistListData() const
{
    return m_playlistListData;
}

void KNMusicPlaylistList::setPlaylistListData(const QStringList &playlistListData)
{
    //Disconnect all.
    m_rowChangeHandle->disConnectAll();
    //Set the stirng list.
    m_playlistListData = playlistListData;
    //After set the new string list. Connect row detect signal.
    m_rowChangeHandle->addConnectionHandle(
                connect(this, &KNMusicPlaylistList::rowsInserted,
                        this, &KNMusicPlaylistList::onActionRowInserted));
    m_rowChangeHandle->addConnectionHandle(
                connect(this, &KNMusicPlaylistList::rowsRemoved,
                        this, &KNMusicPlaylistList::onActionRowRemoved));
}

void KNMusicPlaylistList::onActionItemChanged(QStandardItem *item)
{
    KNMusicPlaylistListItem *currentItem=static_cast<KNMusicPlaylistListItem *>(item);
    currentItem->setChanged(true);
}

