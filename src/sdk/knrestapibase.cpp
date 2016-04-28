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
#include <QUrl>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

#include "knrestapibase.h"

#include <QDebug>

KNRestApiBase::KNRestApiBase(QObject *parent) :
    QObject(parent),
    m_downloadedSize(-1),
    m_lastDownloadedSize(-1),
    m_currentReply(nullptr),
    m_timeout(new QTimer),
    m_networkManager(new QNetworkAccessManager)
{
    //Configure timeout timer.
    m_timeout->setInterval(30000);
    m_timeout->setSingleShot(true);
}

KNRestApiBase::~KNRestApiBase()
{
    //Check out the current reply is running.
    if(m_currentReply)
    {
        //Abort the reply.
        m_currentReply->abort();
        //Delete the reply.
        m_currentReply->deleteLater();
        //Reset the pointer.
        m_currentReply=nullptr;
    }
    //Recover the memory.
    m_networkManager->deleteLater();
    //Stop the timer.
    m_timeout->deleteLater();
}

void KNRestApiBase::setWorkingThread(QThread *thread)
{
    //Move the downloader to the thread.
    moveToThread(thread);
    //Move the children to the thread.
    m_networkManager->moveToThread(thread);
    m_timeout->moveToThread(thread);
}

int KNRestApiBase::deleteResource(const QNetworkRequest &request,
                                  bool clearCache)
{
    //Clear the access cache.
    if(clearCache)
    {
        m_networkManager->clearAccessCache();
    }
    //Reset the reply pointer.
    m_currentReply=nullptr;
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    m_currentReply=m_networkManager->deleteResource(request);
    //Check reply pointer.
    if(m_currentReply)
    {
        //Link the reply with the wating loop.
        linkHandler(m_currentReply, &stuckWatingLoop);
        //Link the timeout to event lop.
        m_timeoutHandler.append(
                    connect(m_timeout, &QTimer::timeout,
                            &stuckWatingLoop, &QEventLoop::quit));
        //Before we start the loop, we need to check whether the reply is done.
        if(m_currentReply && m_currentReply->isRunning())
        {
            //Start timer.
            m_timeout->start();
            //Start stucked loop.
            stuckWatingLoop.exec();
        }
        //Stop the time out timer.
        m_timeout->stop();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Initial the response code.
        int responseCode=-1;
        //Check whether reply is still available.
        if(!m_currentReply)
        {
            //Failed when current reply is not valid at all.
            return -1;
        }
        //Check the reply running result, get the response code.
        if(m_currentReply->isRunning())
        {
            //Abord the reply, timeout.
            m_currentReply->abort();
        }
        else
        {
            //Update the response code.
            responseCode=m_currentReply->attribute(
                QNetworkRequest::HttpStatusCodeAttribute).toInt();
        }
        //Clear the reply.
        m_currentReply->deleteLater();
        //Reset the current reply pointer.
        m_currentReply=nullptr;
        //Complete.
        return responseCode;
    }
    //Failed if no reply.
    return -1;
}

int KNRestApiBase::put(const QNetworkRequest &request,
                       const QByteArray &parameter,
                       QByteArray &responseData,
                       bool clearCache)
{
    //Clear the response data array.
    responseData.clear();
    //Clear the access cache.
    if(clearCache)
    {
        m_networkManager->clearAccessCache();
    }
    //Generate the quit handler and the reply pointer.
    m_currentReply=nullptr;
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    m_currentReply=m_networkManager->put(request, parameter);
    //Check reply pointer.
    if(m_currentReply)
    {
        //Link the reply with the wating loop.
        linkHandler(m_currentReply, &stuckWatingLoop);
        //Link the timeout to event lop.
        m_timeoutHandler.append(
                    connect(m_timeout, &QTimer::timeout,
                            &stuckWatingLoop, &QEventLoop::quit));
        //Before we start the loop, we need to check whether the reply is done.
        if(m_currentReply && m_currentReply->isRunning())
        {
            //Start timer.
            m_timeout->start();
            //Start stucked loop.
            stuckWatingLoop.exec();
        }
        //Stop the timer.
        m_timeout->stop();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Initial the response code.
        int responseCode=-1;
        //Check whether reply is still available.
        if(!m_currentReply)
        {
            //Failed when current reply is not valid at all.
            return -1;
        }
        //Check out the reply.
        if(m_currentReply->isRunning())
        {
            //It should be timeout.
            m_currentReply->abort();
        }
        else
        {
            //Get the data from the reply.
            responseData=m_currentReply->readAll();
            //Get the response code.
            responseCode=m_currentReply->attribute(
                        QNetworkRequest::HttpStatusCodeAttribute).toInt();
        }
        //Clear the reply.
        m_currentReply->deleteLater();
        //Reset reply pointer.
        m_currentReply=nullptr;
        //Complete.
        return responseCode;
    }
    //Failed if no reply.
    return -1;
}

int KNRestApiBase::get(const QNetworkRequest &request,
                       QByteArray &responseData,
                       bool clearCache)
{
    //Clear the response data array.
    responseData.clear();
    //Clear the access cache.
    if(clearCache)
    {
        m_networkManager->clearAccessCache();
    }
    //Reset downloaded size.
    m_downloadedSize=-1;
    m_lastDownloadedSize=-1;
    //Generate the quit handler and the reply pointer.
    m_currentReply=nullptr;
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    m_currentReply=m_networkManager->get(request);
    //Check reply pointer.
    if(m_currentReply)
    {
        //Link the reply with the wating loop.
        linkHandler(m_currentReply, &stuckWatingLoop);
        //Link the current reply downloaded signal to updater.
        m_timeoutHandler.append(
                    connect(m_currentReply, &QNetworkReply::downloadProgress,
                            this, &KNRestApiBase::onActionGetDownloading));
        m_timeoutHandler.append(
                    connect(this, &KNRestApiBase::timeout,
                            &stuckWatingLoop, &QEventLoop::quit));
        //Link the timeout to downloaded check.
        m_timeoutHandler.append(
                    connect(m_timeout, &QTimer::timeout,
                            this, &KNRestApiBase::onActionDownloadCheck,
                            Qt::QueuedConnection));
        //Before we start the loop, we need to check whether the reply is done.
        if(m_currentReply &&
                (!m_currentReply->isFinished()))
        {
            //Start timer.
            m_timeout->start();
            //Start stucked loop.
            stuckWatingLoop.exec();
        }
        //Stop the timer.
        m_timeout->stop();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Initial the response code.
        int responseCode=-1;
        //Check whether reply is still available.
        if(!m_currentReply)
        {
            //Failed when current reply is not valid at all.
            return -1;
        }
        //Check out whether it's timeout.
        if(m_currentReply->isRunning())
        {
            //It should be time out.
            m_currentReply->abort();
        }
        else
        {
            //Get the response code.
            responseCode=m_currentReply->attribute(
                        QNetworkRequest::HttpStatusCodeAttribute).toInt();
            //Get the data from the reply.
            responseData=m_currentReply->readAll();
        }
        //Clear the reply.
        m_currentReply->deleteLater();
        //Reset the current reply.
        m_currentReply=nullptr;
        //Mission complete.
        return responseCode;
    }
    //Failed to connect to server.
    return -1;
}

int KNRestApiBase::post(QNetworkRequest request,
                        const QByteArray &parameter,
                        QByteArray &responseData,
                        bool clearCache)
{
    //Clear the response data array.
    responseData.clear();
    //Clear the access cache.
    if(clearCache)
    {
        m_networkManager->clearAccessCache();
    }
    //Generate the quit handler and the reply pointer.
    m_currentReply=nullptr;
    //Configure the header of the request.
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                      parameter.size());
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    m_currentReply=m_networkManager->post(request, parameter);
    //Check reply pointer.
    if(m_currentReply)
    {
        //Link the reply with the wating loop.
        linkHandler(m_currentReply, &stuckWatingLoop);
        //Link the timeout to event lop.
        m_timeoutHandler.append(
                    connect(m_timeout, &QTimer::timeout,
                            &stuckWatingLoop, &QEventLoop::quit));
        //Before we start the loop, we need to check whether the reply is done.
        if(m_currentReply &&
                (!m_currentReply->isFinished()))
        {
            //Start timer.
            m_timeout->start();
            //Start stucked loop.
            stuckWatingLoop.exec();
        }
        //Stop the timer.
        m_timeout->stop();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Initial the response code.
        int responseCode=-1;
        //Check whether reply is still available.
        if(!m_currentReply)
        {
            //Failed when current reply is not valid at all.
            return -1;
        }
        //Check out whether it's timeout.
        if(m_currentReply->isRunning())
        {
            //It should be time out.
            m_currentReply->abort();
        }
        else
        {
            //Get the response code.
            responseCode=m_currentReply->attribute(
                        QNetworkRequest::HttpStatusCodeAttribute).toInt();
            //Get the data from the reply.
            responseData=m_currentReply->readAll();
        }
        //Clear the reply.
        m_currentReply->deleteLater();
        //Reset the current reply.
        m_currentReply=nullptr;
        //Complete.
        return responseCode;
    }
    //Failed if no reply.
    return -1;
}

QNetworkRequest KNRestApiBase::generateRequest(const QString &url,
                                               const QVariant &cookie,
                                               const QString &referer)
{
    //Generate the network request, initial it with the url.
    QNetworkRequest request;
    //Set default content type header.
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
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

void KNRestApiBase::setTimeout(int timeout)
{
    //Update the timeout timer.
    m_timeout->setInterval(timeout);
}

void KNRestApiBase::onActionGetDownloading(const qint64 &size, const qint64 &)
{
    //Save the downloaded size.
    m_downloadedSize=size;
}

void KNRestApiBase::onActionDownloadCheck()
{
    //If there's no data downloaded, timeout.
    if(m_downloadedSize==m_lastDownloadedSize)
    {
        //Emit timeout signal.
        emit timeout();
        //Failed to download.
        return;
    }
    //If there's any data downloaded, then continue waiting.
    m_lastDownloadedSize=m_downloadedSize;
    //Start timer.
    m_timeout->start();
}

inline void KNRestApiBase::linkHandler(QNetworkReply *reply,
                                       QEventLoop *eventLoop)
{
    //Link the reply with the wating loop.
    m_timeoutHandler.append(
                connect(reply, &QNetworkReply::finished,
                        eventLoop, &QEventLoop::quit));
    m_timeoutHandler.append(
                connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                        eventLoop, SLOT(quit())));
    m_timeoutHandler.append(
                connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
                        eventLoop, SLOT(quit())));
}
