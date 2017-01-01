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
    m_downloader(new KNFileDownloadManager())
{
    //Configure the downloader.
    m_downloader->moveToThread(&m_downloaderThread);
    connect(this, &KNMusicStoreDownloadManager::requireDownloadFile,
            m_downloader, &KNFileDownloadManager::downloadFile,
            Qt::QueuedConnection);
    connect(this, &KNMusicStoreDownloadManager::requirePause,
            m_downloader, &KNFileDownloadManager::pause,
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
    //Check the list size.
    if(m_downloadItemList.isEmpty())
    {
        //The mission will be start later.
        itemData.state=MissionRunning;
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
    if(m_downloadItemList.size()==1)
    {
        //Start the mission
        startMission(url, directoryPath, fileName, true);
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

void KNMusicStoreDownloadManager::startAll()
{
    //Check the download item list.
    if(m_downloadItemList.isEmpty())
    {
        //Ignore empty list operaiton.
        return;
    }
    //Loop from top to the end, set all the data to waiting.
    for(int i=1; i<m_downloadItemList.size(); ++i)
    {
        //Get the item from the list.
        DownloadItemMetadata item=m_downloadItemList.at(i);
        //Change the state.
        item.state=MissionWaiting;
        //Replace the item.
        m_downloadItemList.replace(i, item);
    }
    //Emit data changed signal.
    emit dataChanged(index(0, 1), index(m_downloadItemList.size()-1, 1));
    //Get the first item.
    DownloadItemMetadata item=m_downloadItemList.first();
    //Change the state to download.
    item.state=MissionRunning;
    //Replace the item.
    m_downloadItemList.replace(0, item);
    //Start to download the item.
    startMission(item.url, item.directoryPath, item.fileName, false);
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
    //Update the downloaded item data, it should be the first item in the list.
    DownloadItemMetadata updatedItem=m_downloadItemList.first();
    //Change the bytes received.
    updatedItem.downSize=bytesReceived;
    updatedItem.totalSize=bytesTotal;
    //Update the mega bytes.
    updatedItem.downMegaSize=(qreal)bytesReceived/1048576.0;
    updatedItem.totalMegaSize=(qreal)bytesTotal/1048576.0;
    //Update the item.
    updateItem(0, updatedItem);
}

void KNMusicStoreDownloadManager::onDownloadFinished()
{
    //Reduce one internet connection.
    knMusicStoreGlobal->reduceConnectionCounter(1);
    //Remove the first item in the model.
    beginRemoveRows(QModelIndex(), 0, 0);
    //Remove the first item.
    m_downloadItemList.removeFirst();
    //Mission complete.
    endRemoveRows();
    //Check the item list size.
    if(!m_downloadItemList.isEmpty())
    {
        //Get the item from list.
        DownloadItemMetadata itemData=m_downloadItemList.first();
        //Change the mission state.
        itemData.state=MissionRunning;
        //Update the item.
        updateItem(0, itemData);
        //Start next mission.
        startMission(itemData.url,
                     itemData.directoryPath,
                     itemData.fileName,
                     true);
    }
}

void KNMusicStoreDownloadManager::onDownloadPaused(const qint64 &pausedSize)
{
    //Reduce one internet connection.
    knMusicStoreGlobal->reduceConnectionCounter(1);
    //Get the item from list.
    //! FIXME: change this to find the item.
    DownloadItemMetadata itemData=m_downloadItemList.first();
    //Change the mission state.
    itemData.state=MissionPaused;
    //Update the paused size.
    itemData.downSize=pausedSize;
    itemData.downMegaSize=pausedSize/1048576.0;
    //Update the item.
    updateItem(0, itemData);
    //Find next item which is not paused.
    //! FIXME: Add codes here.
}

inline void KNMusicStoreDownloadManager::startMission(
        const QString &url,
        const QString &directoryPath,
        const QString &fileName,
        bool fromStart)
{
    //Increase one internet connection.
    knMusicStoreGlobal->addConnectionCounter(1);
    //Start download mission.
    emit requireDownloadFile(url, directoryPath, fileName, fromStart);
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
