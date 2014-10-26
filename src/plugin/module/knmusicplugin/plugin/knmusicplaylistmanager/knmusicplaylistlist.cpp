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
#include "knmusicplaylistlistitem.h"

#include "knmusicplaylistlist.h"

#include <QDebug>

KNMusicPlaylistList::KNMusicPlaylistList(QObject *parent) :
    QStandardItemModel(parent)
{
    ;
}

void KNMusicPlaylistList::loadDataFile()
{
    //After loading from disk. Connect row detect signal.
    connect(this, &KNMusicPlaylistList::rowsInserted,
            this, &KNMusicPlaylistList::onActionRowInserted);
    connect(this, &KNMusicPlaylistList::rowsRemoved,
            this, &KNMusicPlaylistList::onActionRowRemoved);
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
    //Insert the playlist data in to json array.
    for(int i=first; i<=last; i++)
    {
        KNMusicPlaylistListItem *currentItem=playlistItem(i);
        qDebug()<<currentItem->playlistFilePath();
//        m_playlistListData.insert();
        ;
    }
}

void KNMusicPlaylistList::onActionRowRemoved(const QModelIndex &parent,
                                             int first,
                                             int last)
{
    Q_UNUSED(parent);
}
