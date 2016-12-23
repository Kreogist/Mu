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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "knmusicstorehomelistmodel.h"

KNMusicStoreHomeListModel::KNMusicStoreHomeListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_albumArtSize(125)
{
}

int KNMusicStoreHomeListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The list size is the row count.
    return m_itemList.size();
}

QVariant KNMusicStoreHomeListModel::data(const QModelIndex &index,
                                         int role) const
{
    //Check the index.
    if(!index.isValid())
    {
        //For invalid index, all the data is invalid.
        return QVariant();
    }
    //Get the album item.
    const KNMusicStoreHomeItem &item=m_itemList.at(index.row());
    //Check the role.
    switch(role)
    {
    case Qt::DisplayRole:
        return item.title;
    case Qt::DecorationRole:
        return item.artwork;
    case Qt::UserRole + 1:
        return item.subheading;
    case Qt::UserRole + 2:
        return item.customData;
    default:
        return QVariant();
    }
}

QPixmap KNMusicStoreHomeListModel::albumArt(int row) const
{
    //Directly return the artwork without construct the variant.
    return m_itemList.at(row).artwork;
}

KNMusicStoreHomeItem KNMusicStoreHomeListModel::homeItem(int row) const
{
    //Directly return the item.
    return m_itemList.at(row);
}

void KNMusicStoreHomeListModel::appendRow(const KNMusicStoreHomeItem &homeItem)
{
    //Start to insert the item.
    beginInsertRows(QModelIndex(), m_itemList.size(), m_itemList.size());
    //Append to the list.
    m_itemList.append(homeItem);
    //Finish.
    endInsertRows();
}

void KNMusicStoreHomeListModel::reset()
{
    //Check the list size.
    if(m_itemList.isEmpty())
    {
        //Ignore the reset request.
        return;
    }
    //Start to reset data.
    beginResetModel();
    //Clear the item list data.
    m_itemList.clear();
    //Reserve 32 item data size.
    m_itemList.reserve(32);
    //Finish reset data.
    endResetModel();
}

void KNMusicStoreHomeListModel::setAlbumArt(int row, const QPixmap &albumArt)
{
    //Get the item.
    KNMusicStoreHomeItem updatedItem=m_itemList.at(row);
    //Save the new album art.
    updatedItem.artwork=albumArt.scaled(m_albumArtSize, m_albumArtSize,
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);
    //Update the list.
    m_itemList.replace(row, updatedItem);
    //Emit the data changed signal.
    emit dataChanged(index(row), index(row));
}

int KNMusicStoreHomeListModel::albumArtSize() const
{
    return m_albumArtSize;
}

void KNMusicStoreHomeListModel::setAlbumArtSize(int albumArtSize)
{
    m_albumArtSize = albumArtSize;
}
