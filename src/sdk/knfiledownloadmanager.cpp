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

#include "knfiledownloadmanager.h"

#include <QDebug>

KNFileDownloadManager::KNFileDownloadManager(QObject *parent) :
    QObject(parent)
{
}

void KNFileDownloadManager::downloadFile(const QString &url,
                                         const QString &targetFolder)
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
    //Save the path.
    m_savePathList.append(targetFolder);
    //Generate the request.
    QNetworkRequest headerRequest(QUrl::fromEncoded(url.toLocal8Bit()));
    //Start to get the url header data.
    QNetworkReply *fileReply=m_downloader->get(headerRequest);
    //Link the file download reply signal.
    connect(fileReply, &QNetworkReply::downloadProgress,
            this, &KNFileDownloadManager::replyDownloadProgress);
    //Add reply to list.
    m_fileList.append(fileReply);
}

void KNFileDownloadManager::reset()
{
    //Reset the downloader maanger.
    m_downloader.reset();
    //Clear the queue list.
    m_fileList.clear();
    m_savePathList.clear();
}

void KNFileDownloadManager::onDownloaderFinished(QNetworkReply *reply)
{
    //Check reply data.
    if(QNetworkReply::NoError==reply->error())
    {
        //Get the index of the item.
        int itemIndex=m_fileList.indexOf(reply);
        //Check the directory.
        QDir targetDir(m_savePathList.at(itemIndex));
        //Write the content to file.
        if(!targetDir.mkpath(m_savePathList.at(itemIndex)))
        {
            //Failed to construct the directory.
            //! FIXME: Add error signal here.
            //Remove the reply.
            reply->deleteLater();
            return;
        }
        //Open the file, write the data.
        QFile targetFile(targetDir.filePath(reply->url().fileName()));
        //Open the target file.
        if(!targetFile.open(QIODevice::WriteOnly))
        {
            //Failed to write data to file.
            //!FIXME: Add cannot open file error signal here.
            //Remove reply.
            reply->deleteLater();
            return;
        }
        //Write content to file.
        targetFile.write(reply->readAll());
        //Close target file.
        targetFile.close();
        //Emit download finished signal.
        //! FIXME: Emit complete signal.
        //Clear reply.
        reply->deleteLater();
        //Mission complete.
        return;
    }
    ;
}

void KNFileDownloadManager::replyDownloadProgress(const qint64 &bytesReceived,
                                                  const qint64 &bytesTotal)
{
    qDebug()<<bytesReceived<<"/"<<bytesTotal;
}
