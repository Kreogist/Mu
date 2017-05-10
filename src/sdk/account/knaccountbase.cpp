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
#include <QUrlQuery>
#include <QJsonDocument>

#include "knaccountdetails.h"

#include "knaccountbase.h"

#include <QDebug>

KNAccountBase::KNAccountBase(QObject *parent) :
    KNRestApiBase(parent),
    m_accountDetails(new KNAccountDetails)
{
}

KNAccountBase::~KNAccountBase()
{
    //Delete the detail information.
    delete m_accountDetails;
    m_accountDetails=nullptr;
}

bool KNAccountBase::createRow(const QString &tableClassName,
                              const QByteArray &parameter,
                              QByteArray &responseData,
                              int &errorCode)
{
    //Reset the error code.
    errorCode=0;
    //Do the POST operation to create a new row.
    // A row could be:
    //  1. A user row, which stores the basic information of a user. It has a
    //special table class name, as "users"
    //  2. A content row, which store the data of a user.
    //Get the response code of the post operation.
    // HTTP 201 = Created.
    if((errorCode=post(generateCloudRequest("1/"+tableClassName),
                       parameter,
                       responseData,
                       false))!=201)
    {
        //Check the response data content.
        if(!responseData.isEmpty())
        {
            //Change the error code to data.
            errorCode=QJsonDocument::fromJson(responseData).object().value(
                        "code").toInt();
        }
        //Failed to done the create data.
        return false;
    }
    //Row created.
    return true;
}

bool KNAccountBase::updateRow(const QString &tableClassName,
                              const QByteArray &parameter,
                              QByteArray &responseData,
                              int &errorCode,
                              const QString &sessionToken)
{
    //Reset the error code.
    errorCode=0;
    //Generate the update request.
    QNetworkRequest updateRequest=generateCloudRequest("1/"+tableClassName);
    //Check the session token state.
    if(!sessionToken.isEmpty())
    {
        //Set the session token.
        updateRequest.setRawHeader("X-Bmob-Session-Token",
                                   sessionToken.toUtf8());
    }
    //Update the row of a single row.
    //Get the response code of the post operation.
    // HTTP 200 = Ok.
    return (errorCode=put(updateRequest, parameter, responseData, false))==200;
}

bool KNAccountBase::fetchRow(const QString &tableClassName,
                             QByteArray &responseData,
                             int &errorCode,
                             const QMap<QString, QString> &queryMap)
{
    //Reset the error code.
    errorCode=0;
    //Generate the request.
    QNetworkRequest fetchRequest=generateCloudRequest("1/"+tableClassName);
    //Check the query map.
    if(!queryMap.isEmpty())
    {
        //Insert the query data.
        QUrlQuery fetchQuery;
        //Insert all the query conditions.
        for(auto i=queryMap.begin(); i!=queryMap.end(); ++i)
        {
            //Add the query item
            fetchQuery.addQueryItem(i.key(), i.value());
        }
        //Insert login query to login url.
        QUrl queryFetchUrl=fetchRequest.url();
        queryFetchUrl.setQuery(fetchQuery);
        //Set back to the request.
        fetchRequest.setUrl(queryFetchUrl);
    }
    //Get the error code data.
    // HTTP 200 = Ok.
    if((errorCode=get(fetchRequest, responseData, false))!=200)
    {
        //Failed to update the information
        return false;
    }
    //Success.
    return true;
}

bool KNAccountBase::uploadFile(const QString &url,
                               const QByteArray &fileContent,
                               QByteArray &responseData,
                               int &errorCode,
                               const QString &fileType)
{
    //Reset the error code.
    errorCode=0;
    //Generate the file upload request.
    QNetworkRequest uploadRequest=generateCloudRequest("2/files/"+url);
    //Check the file type data.
    if(!fileType.isEmpty())
    {
        //Configure the content type.
        uploadRequest.setHeader(QNetworkRequest::ContentTypeHeader, fileType);
    }
    //Do the POST request, transfering the file content.
    return (errorCode=post(uploadRequest, fileContent,
                           responseData, false))==200;
}

bool KNAccountBase::downloadFile(const QString &url,
                                 QByteArray &responseData,
                                 int &errorCode)
{
    //Reset the error code.
    errorCode=0;
    //To download file is pretty simple, a GET request could done this.
    //Generate the file download request.
    QNetworkRequest downloadRequest;
    //Set the url.
    downloadRequest.setUrl(QUrl(url));
    //Get the data.
    // Success HTTP status code is 200 - Ok.
    return (errorCode=get(downloadRequest, responseData, false))==200;
}

bool KNAccountBase::removeFile(const QString &url,
                               int &errorCode)
{
    //Reset the error code.
    errorCode=0;
    //Generate a request from the url.
    QNetworkRequest removeRequest=generateCloudRequest(url);
    //Delete the resource.
    errorCode=deleteResource(removeRequest, false);
    //Check the error code.
    // 200 = Ok, 404 = Page Not Found (Resource already removed)
    return errorCode==200 || errorCode==404;
}

inline QNetworkRequest KNAccountBase::generateCloudRequest(const QString &url)
{
    //Generate a request.
    QNetworkRequest request;
    //Set the url.
    request.setUrl(QUrl("https://api.bmob.cn/"+url));
    //Configure the request.
    request.setRawHeader("X-Bmob-Application-Id",
                         "71a30357da3ec74b3ee3bf8fa969075f");
    request.setRawHeader("X-Bmob-REST-API-Key",
                         "d2d522da26665024157835d22a42b42c");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //Give back the request.
    return request;
}
