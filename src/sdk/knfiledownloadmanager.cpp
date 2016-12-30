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
    QObject(parent),
    m_savePath(QString()),
    m_rename(QString()),
    m_fileReply(nullptr)
{
}

void KNFileDownloadManager::downloadFile(const QString &url,
                                         const QString &targetFolder,
                                         const QString &rename)
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
    //Save the new file name.
    m_rename=rename;
    //Save the path.
    m_savePath=targetFolder;
    //Generate the request.
    QNetworkRequest headerRequest(QUrl::fromEncoded(url.toLocal8Bit()));
    //Start to get the url header data.
    m_fileReply=m_downloader->get(headerRequest);
    //Link the file download reply signal.
    m_replyHandler.append(
                connect(m_fileReply, &QNetworkReply::downloadProgress,
                        this, &KNFileDownloadManager::replyDownloadProgress));
}

void KNFileDownloadManager::reset()
{
    //Disconnect all.
    m_replyHandler.disconnectAll();
    //Reset the downloader maanger.
    m_downloader.reset();
    //Clear the queue list.
    m_fileReply=nullptr;
    m_savePath=QString();
}

void KNFileDownloadManager::onDownloaderFinished(QNetworkReply *reply)
{
    //Check reply data.
    if(QNetworkReply::NoError==reply->error())
    {
        //Cut down all the reply connections.
        m_replyHandler.disconnectAll();
        //Check the directory.
        QDir targetDir(m_savePath);
        //Write the content to file.
        if(!targetDir.mkpath(m_savePath))
        {
            //Failed to construct the directory.
            //! FIXME: Add error signal here.
            //Remove the reply.
            reply->deleteLater();
            return;
        }
        //Get the new file name
        QString savedFileName;
        //Check the rename flag.
        if(m_rename.isEmpty())
        {
            //Use original name.
            savedFileName=reply->url().fileName();
        }
        else
        {
            //Get the suffix of the url path.
            QFileInfo urlInfo(reply->url().toString());
            //Get the suffix.
            savedFileName=m_rename+"."+urlInfo.suffix();
        }
        //Open the file, write the data.
        QFile targetFile(targetDir.filePath(savedFileName));
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
        //Clear the pointer.
        m_fileReply=nullptr;
        qDebug()<<"Download finished.";
        //Mission complete.
        return;
    }
    //! FIXME: Add error operations.
    ;
}

void KNFileDownloadManager::replyDownloadProgress(const qint64 &bytesReceived,
                                                  const qint64 &bytesTotal)
{
    qDebug()<<bytesReceived<<bytesTotal;
    //Emit signal.
    emit downloadProgress(bytesReceived, bytesTotal);
}
