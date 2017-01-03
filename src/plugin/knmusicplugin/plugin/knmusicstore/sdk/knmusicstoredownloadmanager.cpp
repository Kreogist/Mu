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
#include "knfiledownloadmanager.h"

#include "knmusicstoreutil.h"
#include "knmusicstoreglobal.h"

#include "knmusicstoredownloadmanager.h"

using namespace MusicStoreUtil;

KNMusicStoreDownloadManager::KNMusicStoreDownloadManager(QObject *parent):
    QAbstractTableModel(parent),
    m_downloader(new KNFileDownloadManager()),
    m_isRunning(false)
{
    //Configure the downloader.
    m_downloader->moveToThread(&m_downloaderThread);
    connect(this, &KNMusicStoreDownloadManager::requireDownloadFile,
            m_downloader, &KNFileDownloadManager::downloadFile,
            Qt::QueuedConnection);
    connect(this, &KNMusicStoreDownloadManager::requirePause,
            m_downloader, &KNFileDownloadManager::pause,
            Qt::QueuedConnection);
    connect(this, &KNMusicStoreDownloadManager::requireAbort,
            m_downloader, &KNFileDownloadManager::abort,
            Qt::QueuedConnection);
    connect(m_downloader, &KNFileDownloadManager::downloadProgress,
            this, &KNMusicStoreDownloadManager::onDownloadProgress,
            Qt::QueuedConnection);
    connect(m_downloader, &KNFileDownloadManager::finished,
            this, &KNMusicStoreDownloadManager::onDownloadFinished,
            Qt::QueuedConnection);
    connect(m_downloader, &KNFileDownloadManager::paused,
            this, &KNMusicStoreDownloadManager::onDownloadPaused,
            Qt::QueuedConnection);
    //Start the thread.
    m_downloaderThread.start();
}

KNMusicStoreDownloadManager::~KNMusicStoreDownloadManager()
{
    //Quit and wait thread.
    m_downloaderThread.quit();
    m_downloaderThread.wait();
    //Delete the downloader.
    m_downloader->deleteLater();
}

void KNMusicStoreDownloadManager::appendItem(const QString &url,
                                             const QString &directoryPath,
                                             const QString &fileName,
                                             const QString &songTitle)
{
    //Construct the item.
    DownloadItemMetadata itemData;
    //Save the metadata.
    itemData.songTitle=songTitle;
    //Save the mission info.
    itemData.url=url;
    itemData.directoryPath=directoryPath;
    itemData.fileName=fileName;
    itemData.state=MissionWaiting;
    //Check the item list.
    if(m_downloadItemList.isEmpty())
    {
        //Emit the not empty signal.
        emit modelEmptyStateChange(false);
    }
    //Add begin item.
    beginInsertRows(QModelIndex(),
                    m_downloadItemList.size(),
                    m_downloadItemList.size());
    //Append to the list.
    m_downloadItemList.append(itemData);
    //End insert rows.
    endInsertRows();
    //Check the mission list.
    if(!m_isRunning)
    {
        //Start the mission
        launchMission(m_downloadItemList.size()-1);
    }
}

QVariant KNMusicStoreDownloadManager::data(const QModelIndex &index,
                                           int role) const
{
    //Check the index.
    if(!index.isValid())
    {
        //Invalid value.
        return QVariant();
    }
    //Get the item.
    const DownloadItemMetadata &item=m_downloadItemList.at(index.row());
    //Check the role.
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        //Check the index column.
        switch(index.column())
        {
        case ColumnIndex:
            return QString::number(index.row()+1);
        case ColumnFilename:
            return item.songTitle;
        default:
            return QVariant();
        }
    case Qt::TextAlignmentRole:
        switch(index.column())
        {
        case ColumnIndex:
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        default:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        }
    case DownBytesRole:
        return item.downSize;
    case TotalBytesRole:
        return item.totalSize;
    case DownMegabytesRole:
        return item.downMegaSize;
    case TotalMegabytesRole:
        return item.totalMegaSize;
    case StateRole:
        return item.state;
    default:
        return QVariant();
    }
}

int KNMusicStoreDownloadManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Row count is the list size.
    return m_downloadItemList.size();
}

int KNMusicStoreDownloadManager::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Fixed column count.
    return DownloadItemColumnCount;
}

bool KNMusicStoreDownloadManager::isPaused(int row) const
{
    //Check whether the item is in paused state.
    return MissionPaused==m_downloadItemList.at(row).state;
}

void KNMusicStoreDownloadManager::startMission(int missionRow)
{
    //Check the mission state.
    DownloadItemMetadata item=m_downloadItemList.at(missionRow);
    //Check the item state.
    if(MissionPaused==item.state)
    {
        //We only processed the paused item.
        if(m_isRunning)
        {
            //Set the item state to waiting, make the item in the waiting list.
            item.state=MissionWaiting;
            //Replace the item.
            updateItem(missionRow, item);
        }
        else
        {
            //Start downloading mission.
            launchMission(missionRow);
        }
    }
}

void KNMusicStoreDownloadManager::pauseMission(int missionRow)
{
    //Check the mission state.
    DownloadItemMetadata item=m_downloadItemList.at(missionRow);
    //Check the state.
    switch(item.state)
    {
    case MissionWaiting:
        //The mission is now in the waiting list.
        //Change state, save it.
        item.state=MissionPaused;
        //Update the item.
        updateItem(missionRow, item);
        break;
    case MissionRunning:
        //Quite simple, because we only download one item per time, so we only
        //need to emit the pause signal.
        emit requirePause();
        break;
    case MissionPaused:
        //Already paused.
        break;
    }
}

void KNMusicStoreDownloadManager::removeMissions(QList<int> missionRows)
{
    //Check the working mission is in the rows.
    if(missionRows.contains(m_currentIndex.row()))
    {
        //Cancel the mission.
        emit requireAbort();
        //Clear the current index.
        m_currentIndex=QModelIndex();
        //Reset the running state.
        m_isRunning=false;
    }
    //Get the number from the end of the list.
    while(!missionRows.isEmpty())
    {
        //Get the row index.
        int currentRow=missionRows.takeLast();
        //Begin to remove the row.
        beginRemoveRows(QModelIndex(), currentRow, currentRow);
        //Remove the item from the rows.
        m_downloadItemList.removeAt(currentRow);
        //Remove complete.
        endRemoveRows();
    }
    //Check the model is empty or not, if empty, emit the signal.
    checkModelEmpty();
}

void KNMusicStoreDownloadManager::startAll()
{
    //Check the download item list.
    if(m_downloadItemList.isEmpty())
    {
        //Ignore empty list operaiton.
        return;
    }
    //Loop from top to the end, set all the data to waiting.
    for(int i=0; i<m_downloadItemList.size(); ++i)
    {
        //Get the item from the list.
        DownloadItemMetadata item=m_downloadItemList.at(i);
        //Check the running state.
        if(MissionPaused==item.state)
        {
            //Change the state.
            item.state=MissionWaiting;
            //Replace the item.
            m_downloadItemList.replace(i, item);
        }
    }
    //Emit data changed signal.
    emit dataChanged(index(0, 1), index(m_downloadItemList.size()-1, 1));
    //Check the running state.
    if(!m_isRunning)
    {
        //Start to download the first item.
        launchMission(0);
    }
}

void KNMusicStoreDownloadManager::pauseAll()
{
    //Loop from top to the end, set all the data to pause.
    for(int i=0; i<m_downloadItemList.size(); ++i)
    {
        //Get the item from the list.
        DownloadItemMetadata item=m_downloadItemList.at(i);
        //Change the state.
        item.state=MissionPaused;
        //Replace the item.
        m_downloadItemList.replace(i, item);
    }
    //Emit data changed signal.
    emit dataChanged(index(0, 1), index(m_downloadItemList.size()-1, 1));
    //Pause the download backend, using signal to do this in async way.
    emit requirePause();
}

void KNMusicStoreDownloadManager::onDownloadProgress(
        const qint64 &bytesReceived,
        const qint64 &bytesTotal)
{
    //Update the downloaded item data.
    DownloadItemMetadata updatedItem=
            m_downloadItemList.at(m_currentIndex.row());
    //Change the bytes received.
    updatedItem.downSize=bytesReceived;
    updatedItem.totalSize=bytesTotal;
    //Update the mega bytes.
    updatedItem.downMegaSize=(qreal)bytesReceived/1048576.0;
    updatedItem.totalMegaSize=(qreal)bytesTotal/1048576.0;
    //Update the item.
    updateItem(m_currentIndex.row(), updatedItem);
}

void KNMusicStoreDownloadManager::onDownloadFinished()
{
    //Change the working state.
    m_isRunning=false;
    //Reduce one internet connection.
    knMusicStoreGlobal->reduceConnectionCounter(1);
    //Get the current item row.
    int currentRow=m_currentIndex.row();
    //Remove the first item in the model.
    beginRemoveRows(QModelIndex(), currentRow, currentRow);
    //Remove the first item.
    m_downloadItemList.removeAt(currentRow);
    //Mission complete.
    endRemoveRows();
    //Reset the index.
    m_currentIndex=QModelIndex();
    //Check the model is empty or not, if empty, emit the signal.
    if(!checkModelEmpty())
    {
        //Check the item list size.
        startNextAvailableMission();
    }
}

void KNMusicStoreDownloadManager::onDownloadPaused(const qint64 &pausedSize)
{
    //Change the working state.
    m_isRunning=false;
    //Reduce one internet connection.
    knMusicStoreGlobal->reduceConnectionCounter(1);
    //Get the item from list.
    DownloadItemMetadata itemData=m_downloadItemList.at(m_currentIndex.row());
    //Change the mission state.
    itemData.state=MissionPaused;
    //Update the paused size.
    itemData.downSize=pausedSize;
    itemData.downMegaSize=pausedSize/1048576.0;
    //Update the item.
    updateItem(m_currentIndex.row(), itemData);
    //Clear the current index.
    m_currentIndex=QModelIndex();
    //Find next item which is not paused.
    startNextAvailableMission();
}

inline bool KNMusicStoreDownloadManager::checkModelEmpty()
{
    //Get the result.
    bool emptyState=m_downloadItemList.isEmpty();
    //Check the empty state.
    if(emptyState)
    {
        //Emit the signal.
        emit modelEmptyStateChange(emptyState);
    }
    //And it return the true as well.
    return emptyState;
}

inline void KNMusicStoreDownloadManager::startNextAvailableMission()
{
    //Search from the top of the list.
    for(int i=0; i<m_downloadItemList.size(); ++i)
    {
        //Check the state of the mission.
        if(MissionWaiting==m_downloadItemList.at(i).state)
        {
            //Start the mission, if the total size is not 0, then it could be
            //paused in the previous session.
            launchMission(i);
            //Mission complete.
            return;
        }
    }
}

inline void KNMusicStoreDownloadManager::launchMission(
        int missionIndex)
{
    //Update the running state.
    m_isRunning=true;
    //Get the item from list.
    DownloadItemMetadata item=m_downloadItemList.at(missionIndex);
    //Change the mission state.
    item.state=MissionRunning;
    //Update the item.
    updateItem(missionIndex, item);
    //Save the index.
    m_currentIndex=index(missionIndex, 0);
    //Increase one internet connection.
    knMusicStoreGlobal->addConnectionCounter(1);
    //Start download mission.
    emit requireDownloadFile(item.url,
                             item.directoryPath,
                             item.fileName,
                             0==item.totalSize);
}

inline void KNMusicStoreDownloadManager::updateItem(
        int row,
        const DownloadItemMetadata &updatedItem)
{
    //Update the item.
    m_downloadItemList.replace(row, updatedItem);
    //Emit data chagned signal.
    emit dataChanged(index(row, 0), index(row, DownloadItemColumnCount-1));
}
