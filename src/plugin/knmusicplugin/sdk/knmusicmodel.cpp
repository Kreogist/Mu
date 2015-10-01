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
#include <QMimeData>
#include <QJsonDocument>
#include <QJsonArray>
#include <QLinkedList>

#include "knutil.h"
#include "knmusicnowplayingbase.h"

#include "knmusicmodel.h"

#include <QDebug>

QStringList KNMusicModel::m_dropMimeTypes=QStringList();
QVariant KNMusicModel::m_alignLeft=QVariant(Qt::AlignLeft | Qt::AlignVCenter);
QVariant KNMusicModel::m_alignCenter=QVariant(Qt::AlignCenter);
QVariant KNMusicModel::m_alignRight=QVariant(Qt::AlignRight | Qt::AlignVCenter);

KNMusicModel::KNMusicModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_detailInfos(QList<KNMusicDetailInfo>()),
    m_totalDuration(0),
    m_playingIndex(QPersistentModelIndex()),
    m_playingIcon(QVariant(QIcon(":/plugin/music/public/playingicon.png"))),
    m_cannotPlayIcon(QVariant(QIcon(":/plugin/music/public/cannotplay.png"))),
    m_nullValue(QVariant())
{
    //Build drop mime types for the first time.
    if(m_dropMimeTypes.isEmpty())
    {
        //Set the drop mime types for original types first.
        m_dropMimeTypes=QAbstractTableModel::mimeTypes();
        //Add url format.
        m_dropMimeTypes.append("text/uri-list");
        //Initial cache data.

        //Add music detail info list.
        m_dropMimeTypes.append(ModelMimeType);
        m_dropMimeTypes.append(ModelType);
        m_dropMimeTypes.append(ModelRowData);
        m_dropMimeTypes.append(ModelRowList);
    }
}

void KNMusicModel::appendFiles(const QStringList &filePaths)
{
    //Simply ask to analysis files.
    emit requireAnalysisFiles(filePaths);
}

void KNMusicModel::appendUrls(const QList<QUrl> &urls)
{
    //Simply ask to analysis the urls.
    emit requireAnalysisFiles(KNUtil::urlListToPathList(urls));
}

void KNMusicModel::appendRow(const KNMusicDetailInfo &detailInfo)
{
    //Follow the documentation, we have to do this.
    //Remember the index here should be the same.
    //Url link:
    //https://forum.qt.io/topic/28850/qtreeview-with-qsortfilterproxymodel-displ
    //ays-an-empty-row-after-changing-the-qabstractitemmodel/5
    //Copy of the link:
    /*
     * The issue here is that you tell your model that you are inserting two
     *  rows:
     * @this->beginInsertRows(QModelIndex() ,
     *                        rootItem->childCount(),
     *                        rootItem->childCount() + 1);@
     * According to
     * http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#beginInsertRows,
     * you have to specify the first and last row number. Which, if you add only
     * one row, should be the same. Remove the +1, it will probably solve your
     * issue.
     */
    beginInsertRows(QModelIndex(),
                    m_detailInfos.size(),
                    m_detailInfos.size());
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
    //Ignore the empty adding request.
    if(detailInfos.isEmpty())
    {
        return;
    }
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(),
                    m_detailInfos.size(),
                    m_detailInfos.size() + detailInfos.size() - 1);
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
    //The reason of why the row and row the same, see the comment in function
    //appendRow().
    beginInsertRows(QModelIndex(), row, row);
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

bool KNMusicModel::insertMusicRows(int row,
                                   const QList<KNMusicDetailInfo> &detailInfos)
{
    //Check the row first.
    Q_ASSERT(row>-1 && row<=m_detailInfos.size());
    //Ignore the empty list.
    if(detailInfos.isEmpty())
    {
        return true;
    }
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(), row, row + detailInfos.size() -1);
    //Insert the data to the detail info list.
    for(int i=detailInfos.size()-1; i>-1; --i)
    {
        //Insert the data to the specific position.
        m_detailInfos.insert(row, detailInfos.at(i));
        //Add the duration to the total duration counter.
        m_totalDuration+=detailInfos.at(i).duration;
    }
    //As the documentation said, called this after insert rows.
    endInsertRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
    return true;
}

bool KNMusicModel::updateRow(int row, KNMusicAnalysisItem analysisItem)
{
    //Check the row first.
    Q_ASSERT(row>-1 && row<m_detailInfos.size());
    //Get the original detail info.
    const KNMusicDetailInfo &previousDetailInfo=m_detailInfos.at(row);
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Copy some data from the previous detail info.
    detailInfo.dateAdded=previousDetailInfo.dateAdded;
    detailInfo.textLists[AlbumRating]=previousDetailInfo.textLists[AlbumRating];
    detailInfo.textLists[DateAdded]=previousDetailInfo.textLists[DateAdded];
    detailInfo.textLists[Plays]=previousDetailInfo.textLists[Plays];
    detailInfo.textLists[Rating]=previousDetailInfo.textLists[Rating];
    //Replace the row with the new detail info.
    return replaceRow(row, detailInfo);
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
                     index(row, columnCount()-1));
    return true;
}

bool KNMusicModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    //Check whether the playing index row is in the the position.
    if(m_playingIndex.isValid() &&
            (m_playingIndex.row() >= position &&
             m_playingIndex.row() <= rows+position))
    {
        //We have to tell the now playing to reset the current playing.
        emit playingItemRemoved();
    }
    //Remove those datas from the list.
    while(rows--)
    {
        //Take away the detail info, and remove the duration.
        m_totalDuration-=m_detailInfos.takeAt(position).duration;
    }
    //As the documentation said, called this after remove rows.
    endRemoveRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
    return true;
}

void KNMusicModel::removeRowList(QList<int> rows)
{
    //Sort the rows, with lambda expressions.
    //We want it to sort the list from greater one to lesser one.
    std::sort(rows.begin(), rows.end(),
              [](const int &a, const int &b){return a>b;});
    //Remove all the data in the rows.
    for(int i : rows)
    {
        //Remove the rows from the last to the first.
        removeRow(i);
    }
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
            return m_alignRight;
        default:
            //For other columns, make it left and vertical center.
            return m_alignLeft;
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
        return QAbstractTableModel::flags(index) |
                Qt::ItemIsEditable |
                Qt::ItemIsDragEnabled |
                Qt::ItemIsDropEnabled |
                Qt::ItemNeverHasChildren;

    default:
        return QAbstractTableModel::flags(index) |
                Qt::ItemIsDragEnabled |
                Qt::ItemIsDropEnabled |
                Qt::ItemNeverHasChildren;
    }
}

bool KNMusicModel::canDropMimeData(const QMimeData *data,
                                   Qt::DropAction action,
                                   int row,
                                   int column,
                                   const QModelIndex &parent) const
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);
    //For urls and mime type we can accept.
    if (data->hasUrls() ||
            data->hasFormat(ModelMimeType))
    {
        return true;
    }
    //Ignore the others.
    return false;
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
    //For display and edit role, return the same data from the text list.
    case Qt::DisplayRole:
    case Qt::EditRole:
        return index.column()>=MusicDataCount?
                    m_nullValue:
                    detailInfo.textLists[index.column()];
    case Qt::TextAlignmentRole:
        //Check the section.
        switch(index.column())
        {
        //For Size and Time columns, make it right and vertical center.
        case Size:
        case Time:
            return m_alignRight;
        default:
            //For other columns, make it left and vertical center.
            return m_alignLeft;
        }
    case Qt::DecorationRole:
        //Only when the column is MusicRowState column, it can have a decoration
        //icon.
        if(index.column()==MusicRowState)
        {
            //When the row cannot be played, return a cannot playing icon.
            if(detailInfo.cannotPlay)
            {
                return m_cannotPlayIcon;
            }
            //If the row is playing, return the playing icon.
            if(index.row()==m_playingIndex.row())
            {
                return m_playingIcon;
            }
            //Otherwise, nothing.
        }
        //All the default columns don't hold a icon.
        return m_nullValue;
    //For property role, no matther what the index is, the data should be all
    //the same.
    case FileSizeRole:
        return detailInfo.size;
    case DurationRole:
        return detailInfo.duration;
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
    default:
        return m_nullValue;
    }
}

QMimeData *KNMusicModel::mimeData(const QModelIndexList &indexes) const
{
    //Check the indexes size.
    if(indexes.count() <= 0)
    {
        return nullptr;
    }
    //Generate a mime data.
    QMimeData *data=new QMimeData();
    //Generate the detail info and url list.
    QJsonArray detailInfoList, rowList;
    QList<QUrl> urlList;
    //Add all the detail info to the list.
    for(auto i=indexes.begin(); i!=indexes.end(); ++i)
    {
        //Check the row is inside the range.
        if((*i).row()>-1 && (*i).row()<m_detailInfos.size())
        {
            //Get the detail info.
            const KNMusicDetailInfo &detailInfo=m_detailInfos.at((*i).row());
            //Add all the detail info to the list.
            detailInfoList.append(KNMusicUtil::detailInfoToObject(detailInfo));
            //Add the url to the list.
            urlList.append(QUrl::fromLocalFile(detailInfo.filePath));
            //Add the row to row list.
            rowList.append((*i).row());
        }
    }
    //Set url list to mime data.
    data->setUrls(urlList);
    //Set the model data to mime data.
    data->setData(ModelMimeType, QByteArray::number((qint64)this));
    data->setData(ModelRowData, QJsonDocument(detailInfoList).toBinaryData());
    data->setData(ModelRowList, QJsonDocument(rowList).toBinaryData());
    //Give back the data.
    return data;
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
    {
        //Set the cannot playing flag.
        detailInfo.cannotPlay=value.toBool();
        //Replace the detail info.
        m_detailInfos.replace(index.row(), detailInfo);
        //For this situation, the changed column is the MusicRowState.
        QModelIndex changedIndex=this->index(index.row(), MusicRowState);
        //Emit the data changed signal.
        emit dataChanged(changedIndex,
                         changedIndex,
                         QVector<int>(1, Qt::DecorationRole));
        //Set has been done.
        return true;

    }
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

void KNMusicModel::addPlayingTimes(const QPersistentModelIndex &playedRow)
{
    //Check the index first. If it's invalid, ignore the add request.
    if(!playedRow.isValid())
    {
        return;
    }
    //Get the play times data index.
    QModelIndex playedIndex=index(playedRow.row(), Plays);
    //It's a text format data, translate it to string first, then to numberous.
    setData(playedIndex,
            data(playedIndex, Qt::DisplayRole).toString().toLongLong() + 1,
            Qt::DisplayRole);
}

QPersistentModelIndex KNMusicModel::playingIndex() const
{
    return m_playingIndex;
}

KNMusicDetailInfo KNMusicModel::rowDetailInfo(const int &row)
{
    return m_detailInfos.at(row);
}

int KNMusicModel::detailInfoRow(const KNMusicDetailInfo &detailInfo)
{
    return m_detailInfos.indexOf(detailInfo);
}

Qt::DropActions KNMusicModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

bool KNMusicModel::dropMimeData(const QMimeData *data,
                                Qt::DropAction action,
                                int row,
                                int column,
                                const QModelIndex &parent)
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(parent)
    //Check move or copy action enabled.
    if(action==Qt::MoveAction || action==Qt::CopyAction)
    {
        if(data->hasUrls())
        {
            //Ask to add files to model.
            emit requireAnalysisFiles(KNUtil::urlListToPathList(data->urls()));
            //Mission complete.
            return true;
        }
    }
    //Or else false.
    return false;
}

QStringList KNMusicModel::mimeTypes() const
{
    return m_dropMimeTypes;
}

QString KNMusicModel::textData(const int &row, const int &column) const
{
    return m_detailInfos.at(row).textLists[column].toString();
}

bool KNMusicModel::moveRows(const QModelIndex &sourceParent,
                            int sourceRow,
                            int count,
                            const QModelIndex &destinationParent,
                            int destinationChild)
{
    Q_UNUSED(sourceParent)
    Q_UNUSED(destinationParent)
    //Check the source row and destination child.
    if(sourceRow==destinationChild ||  //If you didn't do any move.
            sourceRow==destinationChild-1 ||
            count==m_detailInfos.size()) //Or you are trying to move all rows.
    {
        //We don't need to move any thing.
        return true;
    }
    //Generate a temporary list.
    QLinkedList<KNMusicDetailInfo> clipboardList;
    //Check the destinationChild is valid or not.
    if(destinationChild==-1)
    {
        //It is invalid, means we have to move to the end of the model.
        beginMoveRows(QModelIndex(),
                      sourceRow,
                      sourceRow+count-1,
                      QModelIndex(),
                      m_detailInfos.size());
        //Get the destination child.
        while(count--)
        {
            //Take the item, add to clipboard list.
            clipboardList.append(m_detailInfos.takeAt(sourceRow));
        }
        //Append the clipboard list to end of the raw list.
        while(!clipboardList.isEmpty())
        {
            //Append the clipboard from the first to the last.
            m_detailInfos.append(clipboardList.takeFirst());
        }
    }
    else
    {
        //Follow the documentation, call this function first.
        beginMoveRows(QModelIndex(),
                      sourceRow,
                      sourceRow+count-1,
                      QModelIndex(),
                      destinationChild);
        //Calculate the target position.
        int targetPosition=destinationChild;
        //Get the destination child.
        while(count--)
        {
            //Check the source row is greater or lesser than the target position.
            if(sourceRow < targetPosition)
            {
                --targetPosition;
            }
            //Take the item, add to clipboard list.
            clipboardList.append(m_detailInfos.takeAt(sourceRow));
        }
        //Insert the clipboard list to target position.
        while(!clipboardList.isEmpty())
        {
            //Insert the clipboard from the first to the last.
            m_detailInfos.insert(targetPosition, clipboardList.takeFirst());
        }
    }
    //Follow the documentation, call this function after move all the rows.
    endMoveRows();
    //Emit the data changed signal.
    emit dataChanged(index(sourceRow, 0),
                     index(destinationChild, columnCount()-1));
    //Mission complete.
    return true;
}

void KNMusicModel::setPlayingIndex(const QModelIndex &playingRow)
{
    //Check the previous playing index.
    if(m_playingIndex.isValid())
    {
        //Save the previous row.
        int previousRow=m_playingIndex.row();
        //Reset the playing index.
        m_playingIndex=QPersistentModelIndex();
        //If it's valid, emit data update of the current playing row.
        emit dataChanged(index(previousRow, MusicRowState),
                         index(previousRow, MusicRowState));
    }
    //Save the current playing index.
    m_playingIndex=playingRow;
    //Check if the previous playing row is valid.
    if(m_playingIndex.isValid())
    {
        //Update all the data of the previous one and the current row.
        emit dataChanged(index(m_playingIndex.row(), MusicRowState),
                         index(m_playingIndex.row(), MusicRowState));
    }
}

void KNMusicModel::appendDetailInfo(const KNMusicDetailInfo &detailInfo)
{
    //Add data to the detail info list.
    m_detailInfos.append(detailInfo);
}

void KNMusicModel::initialTotalDuration(const quint64 &totalDuration)
{
    //Save the total duration.
    m_totalDuration=totalDuration;
}
