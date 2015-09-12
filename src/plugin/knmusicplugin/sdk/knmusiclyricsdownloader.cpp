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
#include <QUrl>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

#include "knmusiclyricsdownloader.h"

KNMusicLyricsDownloader::KNMusicLyricsDownloader(QObject *parent) :
    QObject(parent),
    m_networkManager(new QNetworkAccessManager),
    m_timeout(new QTimer)
{
    //Configure the timer.
    m_timeout->setInterval(5000);
    m_timeout->setSingleShot(true);
}

KNMusicLyricsDownloader::~KNMusicLyricsDownloader()
{
    //Recover the memory.
    m_timeout->deleteLater();
    m_networkManager->deleteLater();
}

void KNMusicLyricsDownloader::setWorkingThread(QThread *thread)
{
    //Move the children to the thread.
    m_timeout->moveToThread(thread);
    m_networkManager->moveToThread(thread);
    //Move the downloader to the thread.
    moveToThread(thread);
}

void KNMusicLyricsDownloader::get(const QNetworkRequest &request,
                                  QByteArray &responseData)
{
    //Stop the timer to ensure it's not launching.
    m_timeout->stop();
    //Clear the response data array and clear the access cache.
    responseData.clear();
    m_networkManager->clearAccessCache();
    //Generate the quit handler and the reply pointer.
    QNetworkReply *reply=nullptr;
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Link the network manager and time out counter.
    m_timeoutHandler.append(
                connect(m_networkManager, &QNetworkAccessManager::finished,
                        &stuckWatingLoop, &QEventLoop::quit));
    m_timeoutHandler.append(
                connect(m_timeout, &QTimer::timeout,
                        &stuckWatingLoop, &QEventLoop::quit));
    //Start timeout counting.
    m_timeout->start();
    //Launch the network manager.
    reply=m_networkManager->get(request);
    //Start stucked loop.
    stuckWatingLoop.exec();
    //Stop the timer.
    m_timeout->stop();
    //Disconnect all.
    m_timeoutHandler.disconnectAll();
    //Check the reply.
    if(reply==nullptr)
    {
        //If there's no data reply, then exit.
        return;
    }
    //Get the data from the reply.
    responseData=reply->readAll();
    //Clear the reply.
    reply->deleteLater();
}

void KNMusicLyricsDownloader::post(QNetworkRequest request,
                                   const QByteArray &parameter,
                                   QByteArray &responseData)
{
    //Stop the timer to ensure it's not launching.
    m_timeout->stop();
    //Clear the response data array and clear the access cache.
    responseData.clear();
    m_networkManager->clearAccessCache();
    //Generate the quit handler and the reply pointer.
    QNetworkReply *reply=nullptr;
    //Configure the header of the request.
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                      parameter.size());
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Link the network manager and time out counter.
    m_timeoutHandler.append(
                connect(m_networkManager, &QNetworkAccessManager::finished,
                        &stuckWatingLoop, &QEventLoop::quit));
    m_timeoutHandler.append(
                connect(m_timeout, &QTimer::timeout,
                        &stuckWatingLoop, &QEventLoop::quit));
    //Start timeout counting.
    m_timeout->start();
    //Launch the network manager.
    reply=m_networkManager->post(request, parameter);
    //Start stucked loop.
    stuckWatingLoop.exec();
    //Stop the timer.
    m_timeout->stop();
    //Disconnect all.
    m_timeoutHandler.disconnectAll();
    //Check the reply.
    if(reply==nullptr)
    {
        //If there's no data reply, then exit.
        return;
    }
    //Get the data from the reply.
    responseData=reply->readAll();
    //Clear the reply.
    reply->deleteLater();
}

QNetworkRequest KNMusicLyricsDownloader::generateRequest(const QString &url,
                                                         const QVariant &cookie,
                                                         const QString &referer)
{
    //Generate the network request, initial it with the url.
    QNetworkRequest request;
    //Set url of the request.
    request.setUrl(QUrl(url));
    //If there's cookie, set the cookie.
    if(!cookie.isNull())
    {
        //Set the custom cookie.
        request.setHeader(QNetworkRequest::CookieHeader, cookie);
    }
    //If the referer has a custom data.
    if(!referer.isEmpty())
    {
        //Set the "Referer" and "Origin".
        request.setRawHeader("Referer", referer.toStdString().data());
        request.setRawHeader("Original", referer.toStdString().data());
    }
    //Give back the request.
    return request;
}
