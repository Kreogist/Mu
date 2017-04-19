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
#include <QDir>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QBuffer>

#include "knfiledownloadmanager.h"
#include "knutil.h"

using namespace Util;

#include <QDebug>

KNFileDownloadManager::KNFileDownloadManager(QObject *parent) :
    QObject(parent),
    m_savePath(QString()),
    m_targetName(QString()),
    m_fileCache(QByteArray()),
    m_basedPosition(0),
    m_fileReply(nullptr),
    m_fileCacheSize(8), //Default cache size, unit MB.
    m_fileCachePos(0),
    m_pausedFlag(false)
{
}

void KNFileDownloadManager::downloadFile(const QString &url,
                                         const QString &targetFolder,
                                         const QString &rename,
                                         bool fromStart)
{
    //Check pointer.
    if(m_downloader.isNull())
    {
        //Reset the current downloader.
        reset();
        //Construct the pointer.
        m_downloader.reset(new QNetworkAccessManager());
        //Link the downloader.
        connect(m_downloader.data(), &QNetworkAccessManager::finished,
                this, &KNFileDownloadManager::onDownloaderFinished);
    }
    //Clear the paused flag.
    m_pausedFlag=false;
    //Save the new file name.
    m_targetName=rename;
    m_savePath=targetFolder;
    //Check the directory.
    QDir targetDir(m_savePath);
    //Write the content to file.
    if(!targetDir.mkpath(m_savePath))
    {
        //Failed to construct the directory.
        emit error(false, DownloadDirNotExist);
        return;
    }
    //Create the request url.
    QUrl requestUrl(QUrl::fromEncoded(url.toLocal8Bit()));
    //Get the suffix of the url path.
    QFileInfo urlInfo(url);
    //Check the rename flag.
    if(m_targetName.isEmpty())
    {
        //Use original name.
        m_targetName=requestUrl.fileName();
    }
    //Save the suffix.
    m_targetSuffix="."+urlInfo.suffix();
    //Create disk IO cache, reset the buffer.
    m_fileCache=QByteArray(m_fileCacheSize*1048576, '\0');
    m_fileCachePos=0;
    //Open the file, write the data.
    m_file.reset(new QFile(targetDir.filePath(m_targetName+".kmt")));
    //Check from start flag.
    if(fromStart)
    {
        //The position of download should be 0.
        m_basedPosition=0;
        //Open the target file, clear the previous data.
        if(!m_file->open(QIODevice::WriteOnly))
        {
            //Failed to write data to the new file.
            emit error(false, DownloadFailedToCreate);
            return;
        }
    }
    else
    {
        //Download from the end of the file.
        m_basedPosition=m_file->size();
        //Open the target file.
        if(!m_file->open(QIODevice::ReadWrite))
        {
            //Failed to write data to file.
            emit error(false, DownloadFailedToWrite);
            return;
        }
        //Seek to the end of the file.
        m_file->seek(m_basedPosition);
    }
    //Generate the request.
    QNetworkRequest headerRequest(requestUrl);
    //Set the request header.
    QString rangeField="bytes="+QString::number(m_basedPosition)+"-";
    headerRequest.setRawHeader("Range", rangeField.toLatin1());
    //Start to get the url header data.
    m_fileReply=m_downloader->get(headerRequest);
    //Link the file download reply signal.
    m_replyHandler.append(
                connect(m_fileReply, &QNetworkReply::downloadProgress,
                        this, &KNFileDownloadManager::replyDownloadProgress));

}

void KNFileDownloadManager::pause()
{
    //Check the current mission state.
    // First, the downloader is not initialized.
    if(m_downloader.isNull() ||
            //No reply operation is currently running.
            m_fileReply==nullptr ||
            //Already paused.
            m_pausedFlag)
    {
        //WTF, why you called this function?
        return;
    }
    //Set the cancel flag.
    m_pausedFlag=true;
    //Abort the reply to pause.
    m_fileReply->abort();
}

void KNFileDownloadManager::reset()
{
    //Disconnect all.
    m_replyHandler.disconnectAll();
    //Clear cache system.
    m_fileCachePos=0;
    m_fileCache=QByteArray();
    //Reset the downloader maanger.
    m_downloader.reset();
    //Clear the queue list.
    m_fileReply=nullptr;
    m_savePath=QString();
    //Clear all the flags.
    m_pausedFlag=false;
}

void KNFileDownloadManager::abort()
{
    //Check the pointer.
    if(!m_fileReply)
    {
        //No need to abort.
        return;
    }
    //Disconnect all from the reply.
    m_replyHandler.disconnectAll();
    //Abort the reply.
    m_fileReply->abort();
}

void KNFileDownloadManager::onDownloaderFinished(QNetworkReply *reply)
{
    //Check reply data.
    if(QNetworkReply::NoError==m_fileReply->error())
    {
        //Cut down all the reply connections.
        m_replyHandler.disconnectAll();
        //Write the left data in the socket to cache.
        flushToCache();
        //Write the left cache to disk.
        flushToFile();
        //Close target file, and rename the file.
        m_file->close();
        //Get the target directory.
        QString targetDir=QFileInfo((*m_file.data())).absolutePath()+"/";
        if(!m_file->rename(targetDir+m_targetName+m_targetSuffix))
        {
            //Tried to find another name.
            int triedTime=2;
            //While the tried time is true.
            while(!m_file->rename(targetDir+m_targetName+
                                  QString::number(triedTime)+m_targetSuffix))
            {
                //Increase the tried time.
                ++triedTime;
            }
        }
        //Reset the file to null.
        m_file.reset();
        //Clear the disk cache.
        clearDiskCache();
        //Emit download finished signal.
        emit finished();
        //Clear the reply pointer.
        m_fileReply->deleteLater();
        m_fileReply=nullptr;
        //Mission complete.
        return;
    }
    else if(QNetworkReply::OperationCanceledError==m_fileReply->error())
    {
        //Check the paused flag.
        if(m_pausedFlag)
        {
            //At the moment, the socket has been closed, we cannot read and data
            //from the socket.
            //However, we could still write the data in the cache to the file.
            flushToFile();
            //Emit the paused signal.
            emit paused(m_file->pos());
        }
        else
        {
            //Manually abort, which means it is abort.
            //We need to remove the template file.
            m_file->close();
            m_file->remove();
            //Reset the file pointer.
            m_file.reset();
            //Emit the paused signal.
            emit cancelled();
        }
        //Abandon the disk cache.
        clearDiskCache();
        //Clear the pointer.
        m_fileReply->deleteLater();
        m_fileReply=nullptr;
        return;
    }
    else
    {
        //Emit the network error.
        emit error(true, reply->error());
    }
}

void KNFileDownloadManager::replyDownloadProgress(const qint64 &bytesReceived,
                                                  const qint64 &bytesTotal)
{
    //Flush data first.
    flushToCache();
    //Emit signal.
    emit downloadProgress(bytesReceived+m_basedPosition,
                          bytesTotal+m_basedPosition);
}

inline void KNFileDownloadManager::flushToCache()
{
    //Check socket is still open or not.
    if(!m_fileReply->isOpen())
    {
        //Failed to read from socket.
        return;
    }
    //Read data from the reply buffer.
    QByteArray replyData=m_fileReply->readAll();
    //Check the reply data size is longer than the cache.
    while(m_fileCachePos+replyData.size()>=m_fileCache.size())
    {
        //Calculate the left data.
        int writeLength=m_fileCache.size()-m_fileCachePos;
        //Save data to buffer.
        //Write the left content to the cache.
        m_fileCache.replace(m_fileCachePos,
                            writeLength,
                            replyData.left(writeLength));
        //Write data to the file, the file must be open.
        m_file->write(m_fileCache);
        //Reset the buffer.
        m_fileCachePos=0;
        //Remove data from the reply data.
        replyData.remove(0, writeLength);
    }
    //The size of cache is enough for the cache.
    m_fileCache.replace(m_fileCachePos, replyData.size(), replyData);
    m_fileCachePos+=replyData.size();
}

inline void KNFileDownloadManager::flushToFile()
{
    //Check whether there is data left in the cache.
    if(m_fileCachePos>0)
    {
        //Resize the buffer.
        m_fileCache.resize(m_fileCachePos);
        //Flush cache to file.
        m_file->write(m_fileCache);
    }
}

inline void KNFileDownloadManager::clearDiskCache()
{
    //Clear the byte data.
    m_fileCache=QByteArray();
    m_fileCachePos=0;
}
