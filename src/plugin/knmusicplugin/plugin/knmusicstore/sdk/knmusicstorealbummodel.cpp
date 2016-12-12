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

#include "knlocalemanager.h"

#include "knmusicstorealbummodel.h"

#include <QDebug>

KNMusicStoreAlbumModel::KNMusicStoreAlbumModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    //Link retranslator.
    knI18n->link(this, &KNMusicStoreAlbumModel::retranslate);
    retranslate();
}

void KNMusicStoreAlbumModel::clear()
{
    //Start to reset the model.
    beginResetModel();
    //Clear the song list.
    m_songInfoList.clear();
    //Reset model finished.
    endResetModel();
    //Emit the row count change signal.
    emit rowCountChanged(0);
}

void KNMusicStoreAlbumModel::reserve(int songCount)
{
    //Check the song count.
    if(m_songInfoList.size()!=0 || songCount==0)
    {
        //Nothing need to do.
        return;
    }
    //Start insert item.
    beginInsertRows(QModelIndex(), 0, songCount);
    //Update the item list.
    for(int i=0; i<songCount; ++i)
    {
        //Append empty data at the end of the list.
        m_songInfoList.append(KNMusicStoreSongInfo());
    }
    //Finish insert item.
    endInsertRows();
    //Emit the row count changed signal.
    emit rowCountChanged(songCount);
}

void KNMusicStoreAlbumModel::replace(int i,
                                     const KNMusicStoreSongInfo &songInfo)
{
    //Replace the data.
    m_songInfoList.replace(i, songInfo);
    //Emit the data changed signal.
    emit dataChanged(index(i, 0), index(i, columnCount()));
}

int KNMusicStoreAlbumModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The list size is the row counter.
    return m_songInfoList.size();
}

int KNMusicStoreAlbumModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The column size is fixed.
    return AlbumModelColumnCount;
}

QVariant KNMusicStoreAlbumModel::data(const QModelIndex &index, int role) const
{
    //Get the index.
    const KNMusicStoreSongInfo &item=m_songInfoList.at(index.row());
    //Check the role.
    switch(role)
    {
    //Display data.
    case Qt::DisplayRole:
    case Qt::EditRole:
        //Check the index column.
        switch(index.column())
        {
        case AlbumModelIndex:
            return item.index;
        case AlbumModelName:
            return item.name;
        case AlbumModelArtist:
            return item.artist;
        case AlbumModelDuration:
            return item.duration;
        default:
            return QVariant();
        }
        return QVariant();
    case Qt::TextAlignmentRole:
        return index.column()==AlbumModelDuration ?
                    QVariant(Qt::AlignRight | Qt::AlignVCenter) :
                    QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    default:
        return QVariant();
    }
}

QVariant KNMusicStoreAlbumModel::headerData(int section,
                                            Qt::Orientation orientation,
                                            int role) const
{
    //Check the orientation.
    if(orientation==Qt::Vertical)
    {
        //No data will be displayed on the vertical header.
        return QVariant();
    }
    //Check the section data.
    switch(role)
    {
    case Qt::DisplayRole:
        return m_headerText[section];
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    default:
        return QVariant();
    }
}

void KNMusicStoreAlbumModel::retranslate()
{
    //Translate the header text.
    m_headerText[AlbumModelName]=tr("Name");
    m_headerText[AlbumModelArtist]=tr("Artist");
    m_headerText[AlbumModelDuration]=tr("Duration");
}
