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

KNRestApiBase::KNRestApiBase(QObject *parent) :
    QObject(parent),
    m_networkManager(new QNetworkAccessManager)
{
}

KNRestApiBase::~KNRestApiBase()
{
    //Recover the memory.
    m_networkManager->deleteLater();
}

void KNRestApiBase::setWorkingThread(QThread *thread)
{
    //Move the downloader to the thread.
    moveToThread(thread);
    //Move the children to the thread.
    m_networkManager->moveToThread(thread);
}

int KNRestApiBase::deleteResource(const QNetworkRequest &request)
{
    //Clear the access cache.
    m_networkManager->clearAccessCache();
    //Generate the quit handler and the reply pointer.
    QNetworkReply *reply=nullptr;
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    reply=m_networkManager->deleteResource(request);
    //Check reply pointer.
    if(reply)
    {
        //Link the reply with the wating loop.
        m_timeoutHandler.append(
                    connect(reply, &QNetworkReply::finished,
                            &stuckWatingLoop, &QEventLoop::quit));
        m_timeoutHandler.append(
                    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                            &stuckWatingLoop, SLOT(quit())));
        //Start stucked loop.
        stuckWatingLoop.exec();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Get the response code.
        int responseCode=reply->attribute(
                    QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //Clear the reply.
        reply->deleteLater();
        //Complete.
        return responseCode;
    }
    //Failed if no reply.
    return -1;
}

int KNRestApiBase::put(const QNetworkRequest &request,
                       const QByteArray &parameter,
                       QByteArray &responseData)
{
    //Clear the response data array and clear the access cache.
    responseData.clear();
    m_networkManager->clearAccessCache();
    //Generate the quit handler and the reply pointer.
    QNetworkReply *reply=nullptr;
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    reply=m_networkManager->put(request, parameter);
    //Check reply pointer.
    if(reply)
    {
        //Link the reply with the wating loop.
        m_timeoutHandler.append(
                    connect(reply, &QNetworkReply::finished,
                            &stuckWatingLoop, &QEventLoop::quit));
        m_timeoutHandler.append(
                    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                            &stuckWatingLoop, SLOT(quit())));
        //Start stucked loop.
        stuckWatingLoop.exec();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Get the data from the reply.
        responseData=reply->readAll();
        //Get the response code.
        int responseCode=reply->attribute(
                    QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //Clear the reply.
        reply->deleteLater();
        //Complete.
        return responseCode;
    }
    //Failed if no reply.
    return -1;
}

int KNRestApiBase::get(const QNetworkRequest &request, QByteArray &responseData)
{
    //Clear the response data array and clear the access cache.
    responseData.clear();
    m_networkManager->clearAccessCache();
    //Generate the quit handler and the reply pointer.
    QNetworkReply *reply=nullptr;
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    reply=m_networkManager->get(request);
    //Check reply pointer.
    if(reply)
    {
        //Link the reply with the wating loop.
        m_timeoutHandler.append(
                    connect(reply, &QNetworkReply::finished,
                            &stuckWatingLoop, &QEventLoop::quit));
        m_timeoutHandler.append(
                    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                            &stuckWatingLoop, SLOT(quit())));
        //Start stucked loop.
        stuckWatingLoop.exec();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Get the response code.
        int responseCode=reply->attribute(
                    QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //Get the data from the reply.
        responseData=reply->readAll();
        //Clear the reply.
        reply->deleteLater();
        //Mission complete.
        return responseCode;
    }
    //Failed to connect to server.
    return -1;
}

int KNRestApiBase::post(QNetworkRequest request,
                        const QByteArray &parameter,
                        QByteArray &responseData)
{
    //Clear the response data array and clear the access cache.
    responseData.clear();
    m_networkManager->clearAccessCache();
    //Generate the quit handler and the reply pointer.
    QNetworkReply *reply=nullptr;
    //Configure the header of the request.
    request.setHeader(QNetworkRequest::ContentLengthHeader,
                      parameter.size());
    //Generate the waiting loop.
    QEventLoop stuckWatingLoop;
    //Launch the network manager.
    reply=m_networkManager->post(request, parameter);
    //Check reply pointer.
    if(reply)
    {
        //Link the reply with the wating loop.
        m_timeoutHandler.append(
                    connect(reply, &QNetworkReply::finished,
                            &stuckWatingLoop, &QEventLoop::quit));
        m_timeoutHandler.append(
                    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                            &stuckWatingLoop, SLOT(quit())));
        //Start stucked loop.
        stuckWatingLoop.exec();
        //Disconnect all.
        m_timeoutHandler.disconnectAll();
        //Get the data from the reply.
        responseData=reply->readAll();
        //Get the response code.
        int responseCode=reply->attribute(
                    QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //Clear the reply.
        reply->deleteLater();
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
