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
#include "knmusicstorealbumlistmodel.h"

#include <QDebug>

using namespace MusicStoreUtil;

KNMusicStoreAlbumListModel::KNMusicStoreAlbumListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

void KNMusicStoreAlbumListModel::appendItem(
        const StoreAlbumListItem &item)
{
    //Add this sentence before appending.
    beginInsertRows(QModelIndex(),
                    m_albumList.size(),
                    m_albumList.size());
    //Append data.
    m_albumList.append(item);
    //End insert.
    endInsertRows();
}

void KNMusicStoreAlbumListModel::clear()
{
    //Check album list is empty first.
    if(m_albumList.isEmpty())
    {
        //If it's empty, then we don't need to check.
        return;
    }
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), 0, m_albumList.size()-1);
    //Remove all the data.
    m_albumList.clear();
    //As the documentation said, called this after remove rows.
    endRemoveRows();
}

int KNMusicStoreAlbumListModel::rowCount(const QModelIndex &parent) const
{
    //Ignore the parent.
    Q_UNUSED(parent);
    //Give the size as the row count.
    return m_albumList.size();
}

QVariant KNMusicStoreAlbumListModel::data(const QModelIndex &index,
                                          int role) const
{
    //Check the index is valid or not.
    if(!index.isValid())
    {
        //Ignore the invalid index.
        return QVariant();
    }
    //Get the item.
    const StoreAlbumListItem &currentItem=m_albumList.at(index.row());
    //Check the row.
    switch(role)
    {
    case Qt::DisplayRole:
        return currentItem.name;
    case Qt::DecorationRole:
        return currentItem.albumArt;
    case AlbumArtistRole:
        return currentItem.artist;
    case AlbumFetchDataRole:
        return currentItem.albumData;
    default:
        return QVariant();
    }
}

QPixmap KNMusicStoreAlbumListModel::albumArt(int row)
{
    //Check the validation of the index.
    if(row<m_albumList.size())
    {
        //Give the index.
        return m_albumList.at(row).albumArt;
    }
    //Give a null pixmap for null index.
    return QPixmap();
}

QString KNMusicStoreAlbumListModel::albumTitle(int row)
{
    //Check the validation of the index.
    return (row<m_albumList.size()) ? m_albumList.at(row).name : QString();
}

QString KNMusicStoreAlbumListModel::artist(int row)
{
    //Check the validation of the index.
    return (row<m_albumList.size()) ? m_albumList.at(row).artist : QString();
}

QVariant KNMusicStoreAlbumListModel::albumData(int row)
{
    //Check the validation of the index.
    return (row<m_albumList.size()) ?
                m_albumList.at(row).albumData : QVariant();
}
