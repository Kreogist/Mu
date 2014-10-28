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
#include <QMimeData>

#include "knglobal.h"
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

Qt::DropActions KNMusicPlaylistList::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags KNMusicPlaylistList::flags(const QModelIndex &index) const
{
    //Enabled drag and drop flags when the index is valid, you can drag and drop
    //files to one playlist.
    if(index.isValid())
    {
        return Qt::ItemIsDropEnabled |
                Qt::ItemIsDragEnabled |
                QStandardItemModel::flags(index);
    }
    //Or else, just enabled drop, you can create a new playlist with these new
    //files.
    return Qt::ItemIsDropEnabled;
}

QStringList KNMusicPlaylistList::mimeTypes() const
{
    //Add url list to mimetypes, but I don't know why should add uri.
    //14.08.21: Add org.kreogist.mu/MusicModelRow for music row.
    QStringList types=QStandardItemModel::mimeTypes();
    types<<"text/uri-list"
         <<"org.kreogist.mu/MusicModelRow";
    return types;
}

bool KNMusicPlaylistList::dropMimeData(const QMimeData *data,
                                       Qt::DropAction action,
                                       int row,
                                       int column,
                                       const QModelIndex &parent)
{
    Q_UNUSED(column);
    //When mimedata contains url data, and ensure that move&copy action enabled.
    if((action==Qt::MoveAction || action==Qt::CopyAction))
    {
//        //Check is the data has format of music row list flag.
//        if(data->hasFormat("org.kreogist.mu/MusicModelRow"))
//        {
//            if(parent.isValid())
//            {
//                emit requireAddRowsToPlaylist(parent.row());
//            }
//            else
//            {
//                emit requireCreatePlaylistFromRow(row);
//            }
//            return true;
//        }
        //Check is the data contains urls,
        if(data->hasUrls())
        {
            if(parent.isValid())
            {
                //If user drag these data to a playlist, add these files to the playlist.
                emit requireAddToPlaylist(parent.row(),
                                          KNGlobal::urlToPathList(data->urls()));
            }
            else
            {
                //We should create a new one for it.
                emit requireCreatePlaylist(row,
                                           KNGlobal::urlToPathList(data->urls()));
            }
            return true;
        }
    }
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

