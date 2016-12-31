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
    connect(m_downloader, &KNFileDownloadManager::downloadProgress,
            this, &KNMusicStoreDownloadManager::onDownloadProgress,
            Qt::QueuedConnection);
    connect(m_downloader, &KNFileDownloadManager::finished,
            this, &KNMusicStoreDownloadManager::onDownloadFinished,
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
    //Check the list size.
    if(m_downloadItemList.isEmpty())
    {
        //The mission will be start later.
        itemData.isDownloading=true;
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
        startMission(url, directoryPath, fileName);
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
        return item.isDownloading;
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
    updateFirstItem(updatedItem);
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
        itemData.isDownloading=true;
        //Update the item.
        updateFirstItem(itemData);
        //Start next mission.
        startMission(itemData.url, itemData.directoryPath, itemData.fileName);
    }
}

inline void KNMusicStoreDownloadManager::startMission(
        const QString &url,
        const QString &directoryPath,
        const QString &fileName)
{
    //Increase one internet connection.
    knMusicStoreGlobal->addConnectionCounter(1);
    //Start download mission.
    emit requireDownloadFile(url, directoryPath, fileName);
}

inline void KNMusicStoreDownloadManager::updateFirstItem(
        const DownloadItemMetadata &updatedItem)
{
    //Update the item.
    m_downloadItemList.replace(0, updatedItem);
    //Emit data chagned signal.
    emit dataChanged(index(0, 0), index(0, DownloadItemColumnCount-1));
}
