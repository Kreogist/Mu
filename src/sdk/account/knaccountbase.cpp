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
#include "knaccountdetails.h"

#include "knaccountbase.h"

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
    if((errorCode=post(generateRequest("1/"+tableClassName), responseData,
                       parameter, false))!=201)
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
    QNetworkRequest updateRequest=generateRequest("1/"+tableClassName);
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
    if((errorCode=put(updateRequest, parameter, responseData, false))!=200)
    {
        //Check the response data.
        //!FIXME: Add error processing code here.
        //Failed to update the information.
        return false;
    }
    //Mission complete.
    return true;
}

bool KNAccountBase::fetchRow(const QString &tableClassName,
                             QByteArray &responseData,
                             int &errorCode)
{
    //Reset the error code.
    errorCode=0;
    //Get the error code data.
    // HTTP 200 = Ok.
    if((errorCode=get(generateRequest("1/"+tableClassName),
                      responseData,
                      false))!=200)
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
                               const QString &fileType)
{
    //Generate the file upload request.
    QNetworkRequest uploadRequest=generateRequest("2/files/"+url);
    //Check the file type data.
    if(!fileType.isEmpty())
    {
        //Configure the content type.
        uploadRequest.setHeader(QNetworkRequest::ContentTypeHeader, fileType);
    }
    //Do the POST request, transfering the file content.
    if((error=post(uploadRequest, fileContent, responseData, false))!=200)
    {
        //Failed to update the information.
        return false;
    }
    //Success.
    return true;
}

inline QNetworkRequest KNAccountBase::generateRequest(const QString &url)
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
