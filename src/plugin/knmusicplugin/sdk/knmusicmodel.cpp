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

#include "knmusicmodel.h"

#include <QDebug>

KNMusicModel::KNMusicModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_detailInfos(QList<KNMusicDetailInfo>()),
    m_totalDuration(0),
    m_playingIndex(QPersistentModelIndex()),
    m_playingIcon(QIcon(":/plugin/music/public/playingicon.png")),
    m_cannotPlayIcon(QIcon(":/plugin/music/public/cannotplay.png"))
{

}

void KNMusicModel::appendRow(const KNMusicDetailInfo &detailInfo)
{
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(),
                    m_detailInfos.size(),
                    m_detailInfos.size() + 1);
    //Append the data at the end of the list.
    m_detailInfos.append(detailInfo);
    //Add the duration to the total duration counter.
    m_totalDuration+=detailInfo.duration;
    //As the documentation said, called this after insert rows.
    endInsertRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
}

void KNMusicModel::appendRows(const QList<KNMusicDetailInfo> &detailInfos)
{
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(),
                    m_detailInfos.size(),
                    m_detailInfos.size() + detailInfos.size());
    //Append the data at the end of the rows.
    m_detailInfos.append(detailInfos);
    //Add all the duration to the total duration counter.
    for(auto i=detailInfos.constBegin(); i!=detailInfos.constEnd(); ++i)
    {
        //Add each duration to the total duration.
        m_totalDuration+=(*i).duration;
    }
    //As the documentation said, called this after insert rows.
    endInsertRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
}

bool KNMusicModel::insertRow(int row, const KNMusicDetailInfo &detailInfo)
{
    //Check the row first.
    Q_ASSERT(row>-1 && row<m_detailInfos.size());
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(), row, row + 1);
    //Insert the detail info into the list.
    m_detailInfos.insert(row, detailInfo);
    //Add the duration to the total duration counter.
    m_totalDuration+=detailInfo.duration;
    //As the documentation said, called this after insert rows.
    endInsertRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
    return true;
}

bool KNMusicModel::updateRow(int row, KNMusicDetailInfo detailInfo)
{
    //Check the row first.
    Q_ASSERT(row>-1 && row<m_detailInfos.size());
    //Get the original detail info.
    KNMusicDetailInfo previousDetailInfo=m_detailInfos.at(row);
    //Remove the old duration from the total duration.
    m_totalDuration-=previousDetailInfo.duration;
    //Copy some data from the previous detail info.
    detailInfo.dateAdded=previousDetailInfo.dateAdded;
    detailInfo.textLists[AlbumRating]=previousDetailInfo.textLists[AlbumRating];
    detailInfo.textLists[DateAdded]=previousDetailInfo.textLists[DateAdded];
    detailInfo.textLists[Plays]=previousDetailInfo.textLists[Plays];
    detailInfo.textLists[Rating]=previousDetailInfo.textLists[Rating];
    //Replace to the new detail info.
    m_detailInfos.replace(row, detailInfo);
    //Add the new duration to the total duration.
    m_totalDuration+=detailInfo.duration;
    //Emit the data changed signal.
    emit dataChanged(index(row, 0),
                     index(row, columnCount()));
    return true;
}

bool KNMusicModel::replaceRow(int row, const KNMusicDetailInfo &detailInfo)
{
    //Check the row first.
    Q_ASSERT(row>-1 && row<m_detailInfos.size());
    //Get the original detail info.
    KNMusicDetailInfo previousDetailInfo=m_detailInfos.at(row);
    //Remove the old duration from the total duration.
    m_totalDuration-=previousDetailInfo.duration;
    //Replace to the new detail info.
    m_detailInfos.replace(row, detailInfo);
    //Add the new duration to the total duration.
    m_totalDuration+=detailInfo.duration;
    //Emit the data changed signal.
    emit dataChanged(index(row, 0),
                     index(row, columnCount()));
    return true;
}

bool KNMusicModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    //Remove those datas from the list.
    while(rows--)
    {
        //Take away the detail info, and remove the duration .
        m_totalDuration-=m_detailInfos.takeAt(position).duration;
    }
    //As the documentation said, called this after remove rows.
    endRemoveRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
    return true;
}

void KNMusicModel::clear()
{
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), 0, m_detailInfos.size()-1);
    //Clear the detail info list.
    m_detailInfos.clear();
    //Clear the total duration.
    m_totalDuration=0;
    //As the documentation said, called this after remove rows.
    endRemoveRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
}

int KNMusicModel::rowCount(const QModelIndex &parent) const
{
    //No sub rows for every avaliable parent.
    if(parent.isValid())
    {
        return 0;
    }
    //The row count is actually the songs size.
    return m_detailInfos.size();
}

int KNMusicModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //This is a fixed column model.
    return MusicColumnCount;
}

QVariant KNMusicModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    //Ignore the vertical request.
    if(Qt::Vertical==orientation)
    {
        return QAbstractTableModel::headerData(section, Qt::Vertical, role);
    }
    //For horizontal headers, the caption should be the
    switch(role)
    {
    case Qt::DisplayRole:
        return section==MusicDataCount?
                    QString():
                    knMusicGlobal->treeViewHeaderText(section);
    case Qt::TextAlignmentRole:
        //Check the section.
        switch(section)
        {
        //For Size and Time columns, make it right and vertical center.
        case Size:
        case Time:
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        default:
            //For other columns, make it left and vertical center.
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        }
    default:
        return QAbstractTableModel::headerData(section, Qt::Horizontal, role);
    }

}

Qt::ItemFlags KNMusicModel::flags(const QModelIndex &index) const
{
    //Only rating and album rating column should be editable.
    switch(index.column())
    {
    case Rating:
    case AlbumRating:
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;

    default:
        return QAbstractTableModel::flags(index);
    }
}

QVariant KNMusicModel::data(const QModelIndex &index, int role) const
{
    //Check the index first, if the index is invalid, ignore the request.
    if(!index.isValid())
    {
        return false;
    }
    //Get the current detail info.
    const KNMusicDetailInfo &detailInfo=m_detailInfos.at(index.row());
    //Get the data of the role.
    switch(role)
    {
    //For property role, no matther what the index is, the data should be all
    //the same.
    case FilePathRole:
        return detailInfo.filePath;
    case FileNameRole:
        return detailInfo.fileName;
    case StartPositionRole:
        return detailInfo.startPosition;
    case ArtworkKeyRole:
        return detailInfo.coverImageHash;
    case TrackFileRole:
        return detailInfo.trackFilePath;
    case TrackIndexRole:
        return detailInfo.trackIndex;
    case CannotPlayFlagRole:
        return detailInfo.cannotPlay;
    //For display and edit role, return the same data from the text list.
    case Qt::DisplayRole:
    case Qt::EditRole:
        return index.column()>=MusicDataCount?
                    QString():
                    detailInfo.textLists[index.column()];
    case Qt::TextAlignmentRole:
        //Check the section.
        switch(index.column())
        {
        //For Size and Time columns, make it right and vertical center.
        case Size:
        case Time:
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        default:
            //For other columns, make it left and vertical center.
            return QVariant(Qt::AlignVCenter);
        }
    case Qt::DecorationRole:
        //Only when the column is MusicRowState column, it can have a decoration
        //icon.
        if(index.column()==MusicRowState)
        {
            //If the row is playing, return the playing icon.
            if(index.row()==m_playingIndex.row())
            {
                return m_playingIcon;
            }
            //When the row cannot be played, return a cannot playing icon.
            if(detailInfo.cannotPlay)
            {
                return m_cannotPlayIcon;
            }
            //Otherwise, nothing.
        }
        //All the default columns don't hold a icon.
        return QVariant();
    default:
        return QVariant();
    }
}

bool KNMusicModel::setData(const QModelIndex &index,
                           const QVariant &value,
                           int role)
{
    //Check the valid of the index first.
    if(!index.isValid())
    {
        return false;
    }
    //Get the detail info.
    KNMusicDetailInfo detailInfo=m_detailInfos.at(index.row());
    //Change the value according to the index.
    switch(role)
    {
    case CannotPlayFlagRole:
        //Set the cannot playing flag.
        detailInfo.cannotPlay=value.toBool();
        //Replace the detail info.
        m_detailInfos.replace(index.row(), detailInfo);
        //Emit the data changed signal.
        emit dataChanged(index, index, QVector<int>(1, Qt::DisplayRole));
        //Set has been done.
        return true;
    case Qt::DisplayRole:
        //We will only support to change the display role data from 0 to
        //MusicDataCount.
        if(index.column()<MusicDataCount)
        {
            //Set the new text.
            detailInfo.textLists[index.column()]=value.toString();
            //Replace the detail info.
            m_detailInfos.replace(index.row(), detailInfo);
            //Emit the data changed signal.
            emit dataChanged(index, index, QVector<int>(1, Qt::DisplayRole));
            //Set has been done.
            return true;
        }
        return false;
    default:
        return false;
    }
}

quint64 KNMusicModel::totalDuration() const
{
    return m_totalDuration;
}

void KNMusicModel::addPlayingTimes(const QPersistentModelIndex &index)
{
    //Check the index first. If it's invalid, ignore the add request.
    if(!index.isValid())
    {
        return;
    }
    //Get the rating data index.
    QModelIndex ratingIndex=this->index(index.row(), Rating);
    //It's a text format data, translate it to string first, then to numberous.
    setData(ratingIndex,
            data(ratingIndex, Qt::DisplayRole).toString().toLongLong() + 1,
            Qt::DisplayRole);
}

QModelIndex KNMusicModel::playingIndex() const
{
    return m_playingIndex;
}

KNMusicDetailInfo KNMusicModel::rowDetailInfo(const int &row)
{
    return m_detailInfos.at(row);
}

void KNMusicModel::setPlayingIndex(const QModelIndex &playingRow)
{
    m_playingIndex = playingRow;
}
