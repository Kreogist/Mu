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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "knconnectionhandler.h"
#include "knmusiclyricsglobal.h"

#include "knmusiclyricsdownloader.h"

KNMusicLyricsDownloader::KNMusicLyricsDownloader(QObject *parent) :
    QObject(parent)
{
    //Initial the network access manager.
    m_networkManager=new QNetworkAccessManager(this);
    //Initial the timer.
    m_timeout=new QTimer(this);
    m_timeout->setSingleShot(true);
    //Waiting for 5 seconds.
    m_timeout->setInterval(5000);
}

void KNMusicLyricsDownloader::get(const QString &url, QByteArray &responseData)
{
    m_timeout->stop();
    //Clear the data first.
    responseData.clear();
    m_networkManager->clearAccessCache();
    //Generate the request.
    QNetworkRequest currentRequest;
    currentRequest.setUrl(QUrl(url));
    QNetworkReply *currentReply=nullptr;
    KNConnectionHandler quiterHandle;
    //Wait for response, using the event loop, generate the loop.
    QEventLoop stuckWaitingLoop;
    //Link the finished and timeout counter to quit loop.
    quiterHandle+=connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
                          &stuckWaitingLoop, SLOT(quit()));
    quiterHandle+=connect(m_timeout, SIGNAL(timeout()),
                          &stuckWaitingLoop, SLOT(quit()));
    //Do GET.
    currentReply=m_networkManager->get(currentRequest);
    //Start counting.
    m_timeout->start();
    //Start loop.
    stuckWaitingLoop.exec();
    //Disconnect all the links.
    quiterHandle.disConnectAll();
    //Check if there's reply.
    if(currentReply==nullptr)
    {
        return;
    }
    //Get the data.
    responseData=currentReply->readAll();
    //Clear the reply.
    delete currentReply;
}

QString KNMusicLyricsDownloader::writeLyricsFile(const KNMusicDetailInfo &detailInfo,
                                                 const QString &content)
{
    //Get the complete base file name of the original file.
    QFileInfo musicFileInfo(detailInfo.filePath);
    //Generate the lyrics file path
    QString lyricsFilePath=KNMusicLyricsGlobal::lyricsFolderPath() + "/" +
            musicFileInfo.completeBaseName() + ".lrc";
    QFile lyricsFile(lyricsFilePath);
    //Try to open the file.
    if(lyricsFile.open(QIODevice::WriteOnly))
    {
        //Write the data to the file.
        QTextStream lyricsStream(&lyricsFile);
        lyricsStream << content << flush;
        //Close the file.
        lyricsFile.close();
        //Return the file path.
        return lyricsFilePath;
    }
    return QString();
}
