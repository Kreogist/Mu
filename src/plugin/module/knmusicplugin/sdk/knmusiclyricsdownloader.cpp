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

#include "knconnectionhandler.h"

#include "knmusiclyricsdownloader.h"

KNMusicLyricsDownloader::KNMusicLyricsDownloader(QObject *parent) :
    QObject(parent)
{
    //Initial the network access manager.
    m_networkManager=new QNetworkAccessManager;
    //Initial the timer.
    m_timeout=new QTimer;
    m_timeout->setSingleShot(true);
    //Waiting for 5 seconds.
    m_timeout->setInterval(5000);
}

KNMusicLyricsDownloader::~KNMusicLyricsDownloader()
{
    delete m_timeout;
    delete m_networkManager;
}

void KNMusicLyricsDownloader::setWorkingThread(QThread *thread)
{
    //Move to the thread.
    moveToThread(thread);
    //Change children's working thread.
    m_timeout->moveToThread(thread);
    m_networkManager->moveToThread(thread);
}

void KNMusicLyricsDownloader::get(const QString &url,
                                  QByteArray &responseData,
                                  const QVariant &cookie,
                                  const QString &referer)
{
    networkProcess(Get, url, responseData, QByteArray(), cookie, referer);
}

void KNMusicLyricsDownloader::post(const QString &url,
                                   QByteArray &responseData,
                                   const QByteArray &parameter,
                                   const QVariant &cookie,
                                   const QString &referer)
{
    networkProcess(Post, url, responseData, parameter, cookie, referer);
}

inline void KNMusicLyricsDownloader::networkProcess(int type,
                                                    const QString &url,
                                                    QByteArray &responseData,
                                                    const QByteArray &parameter,
                                                    const QVariant &cookie,
                                                    const QString &referer)
{
    //Stop the timer.
    m_timeout->stop();
    //Clear the data first.
    responseData.clear();
    m_networkManager->clearAccessCache();
    //Generate the request.
    QNetworkRequest currentRequest;
    //Set the data to request.
    currentRequest.setUrl(QUrl(url));
    if(!cookie.isNull())
    {
        currentRequest.setHeader(QNetworkRequest::CookieHeader,
                                 cookie);
    }
    if(!referer.isEmpty())
    {
        currentRequest.setRawHeader("Referer", referer.toStdString().data());
        currentRequest.setRawHeader("Origin", referer.toStdString().data());
    }
    //Generate the reply and quit handler.
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
    switch(type)
    {
    case Post:
    {
        currentRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                                 "application/x-www-form-urlencoded");
        currentRequest.setHeader(QNetworkRequest::ContentLengthHeader,
                                 parameter.size());
        currentReply=m_networkManager->post(currentRequest, parameter);
        break;
    }
    case Get:
    {
        currentReply=m_networkManager->get(currentRequest);
        break;
    }
    }
    //Start counting.
    m_timeout->start();
    //Start loop.
    stuckWaitingLoop.exec();
    //Disconnect all the links.
    quiterHandle.disconnectAll();
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
