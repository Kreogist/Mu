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
#include <QBuffer>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

#include "knaccountdetails.h"
#include "knaccountutil.h"

#include "knaccount.h"

#include <QDebug>

KNAccount *KNAccount::m_instance=nullptr;

KNAccount::KNAccount(QObject *parent) :
    KNRestApiBase(parent),
    m_accountDetails(new KNAccountDetails(this))
{
}

bool KNAccount::generateAccount(const QString &userName,
                                QString password,
                                const QString &mail)
{
    //Check whether we have already register a user.
    if(m_accountDetails->isLogin())
    {
        //You cannot register a user when login.
        return false;
    }
    //Encrypt the password.
    password=accessPassword(password);
    //Construct the json struct.
    QJsonObject registerData;
    //Save the data.
    registerData.insert("username", userName);
    registerData.insert("password", password);
    registerData.insert("email", mail);
    //Generate the response cache.
    {
        QByteArray responseCache;
        //Send the JSON document to Kreogist cloud.
        if(post(generateKreogistRequest("users"),
                QJsonDocument(registerData).toJson(QJsonDocument::Compact),
                responseCache)!=201 || responseCache.isEmpty())
        {
            //Emit the register error.
            switch(QJsonDocument::fromJson(responseCache).object().value(
                       "code").toInt())
            {
            case 202:
                emit generateFailed(KNAccountUtil::UserNameAlreadyTaken);
                break;
            case 203:
                emit generateFailed(KNAccountUtil::EmailAlreadyTaken);
                break;
            default:
                emit generateFailed(KNAccountUtil::UnknownRegisterError);
                break;
            }
            //Failed to post then failed.
            return false;
        }
        //Check the response data.
        registerData=QJsonDocument::fromJson(responseCache).object();
    }
    //Check register response data.
    if(registerData.contains("objectId"))
    {
        //Successfully register the user id.
        //Save the user name and password for auto login.
        m_accountDetails->setCacheUserName(userName);
        m_accountDetails->setCachePassword(password);
        //Save the contact information.
        m_accountDetails->setObjectId(
                    registerData.value("objectId").toString());
        m_accountDetails->setSessionToken(
                    registerData.value("sessionToken").toString());
        //Set the login.
        m_accountDetails->setIsLogin(true);
        //Emit register success signal.
        emit generateSuccess();
        //Update the account information.
        refreshAccountInfo();
        //Mission complete.
        return true;
    }
    //Emit failed for unknown reason.
    emit generateFailed(KNAccountUtil::UnknownRegisterError);
    //Or else failed to login.
    return false;
}

bool KNAccount::login(const QString &userName,
                      QString password)
{
    //Check out whether user is already login.
    if(m_accountDetails->isLogin())
    {
        //Emit failed signal.
        emit loginFailed(KNAccountUtil::InfoIncorrect);
        //You cannot login two account at the same time.
        return false;
    }
    //Generate the login request.
    QNetworkRequest loginRequest=generateKreogistRequest("login");
    //Encrypt the password.
    QString encryptedPassword=accessPassword(password);
    //Construct the json struct.
    //Try to login with encrypted password first.
    QUrlQuery loginQuery;
    //Insert user name.
    loginQuery.addQueryItem("username", userName);
    loginQuery.addQueryItem("password", encryptedPassword);
    //Insert login query to login url.
    QUrl loginUrl=loginRequest.url();
    loginUrl.setQuery(loginQuery);
    loginRequest.setUrl(loginUrl);
    //Get the login result.
    //Generate the response cache.
    QByteArray responseCache;
    //Send the JSON data to Kreogist Cloud.
    if(get(loginRequest, responseCache)==200)
    {
        //Save the password as encrypt password.
        password=encryptedPassword;
    }
    else
    {
        //Tried to use the original password.
        //Because when they are tring to reset the password, it will use
        //their original password.
        QUrlQuery rawLoginQuery;
        rawLoginQuery.addQueryItem("username", userName);
        rawLoginQuery.addQueryItem("password", password);
        loginUrl.setQuery(rawLoginQuery);
        loginRequest.setUrl(loginUrl);
        //Retry the get data from Kreogist Cloud.
        if(get(loginRequest, responseCache)!=200)
        {
            //Emit failed signal.
            emit loginFailed(KNAccountUtil::InfoIncorrect);
            //Failed to login, password is wrong.
            return false;
        }
        //Parse the response cache.
        QJsonObject passwordSetter=
                QJsonDocument::fromJson(responseCache).object();
        //Then we need to update the password into encrypted password.
        QString sessionToken=passwordSetter.value("sessionToken").toString(),
                objectId=passwordSetter.value("objectId").toString();
        //Set the password setter.
        passwordSetter=QJsonObject();
        passwordSetter.insert("password", encryptedPassword);
        //Generate the udpate response.
        QByteArray passwordUpdateResponse;
        //Update the password.
        loginRequest.setRawHeader("X-Bmob-Session-Token",
                                  sessionToken.toUtf8());
        //Update the url to user's url.
        loginRequest.setUrl("https://api.bmob.cn/1/users/" + objectId);
        //Put the new information.
        if(put(loginRequest,
               QJsonDocument(passwordSetter).toJson(QJsonDocument::Compact),
               passwordUpdateResponse)!=200)
        {
            //Emit failed signal.
            emit loginFailed(KNAccountUtil::InfoIncorrect);
            //Failed to change the password, failed to login.
            return false;
        }
        //Change the password.
        password=encryptedPassword;
    }
    //Check the response data.
    QJsonObject loginData=QJsonDocument::fromJson(responseCache).object();
    //Save the user name and password for auto login.
    m_accountDetails->setCacheUserName(userName);
    m_accountDetails->setCachePassword(password);
    //Save the contact information.
    m_accountDetails->setObjectId(loginData.value("objectId").toString());
    m_accountDetails->setSessionToken(
                loginData.value("sessionToken").toString());
    //Set the login.
    m_accountDetails->setIsLogin(true);
    //Update the user information.
    updateDetails(loginData);
    //Emit success signal.
    emit loginSuccess();
    //Successfully login.
    return true;
}

bool KNAccount::setAvatar(const QPixmap &avatarImage)
{
    //Check out the avatar image size.
    if(avatarImage.width()>100 || avatarImage.height()>100 ||
            (!m_accountDetails->isLogin()))
    {
        //Failed to update the avatar.
        emit avatarUpdatedFailed();
        //Rescaled the image before uploading.
        return false;
    }
    //Check whether it contains image before.
    if(!m_accountDetails->avatarPath().isEmpty())
    {
        //Clear the raw image.
        deleteResource(generateRequest(
                           "files/"+m_accountDetails->avatarPath()));
    }
    //Generate image cache.
    QByteArray imageBytes, responseBytes;
    {
        //Generate the buffer.
        QBuffer imageBuffer(&imageBytes);
        //Open the buffer.
        if(!imageBuffer.open(QIODevice::WriteOnly))
        {
            //Failed to update the avatar.
            emit avatarUpdatedFailed();
            //Failed to save image.
            return false;
        }
        //Save the pixamp.
        avatarImage.save(&imageBuffer, "JPG");
        //Close buffer.
        imageBuffer.close();
    }
    //Upload the account data.
    QNetworkRequest avatarRequest=generateKreogistRequest(
                "files/" + m_accountDetails->objectId() + "/avatar.jpg");
    //Configure the image.
    avatarRequest.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");
    //Post the request.
    if(post(avatarRequest, imageBytes, responseBytes)!=201)
    {
        //Failed to update the avatar.
        emit avatarUpdatedFailed();
        //Failed to upload the image.
        return false;
    }
    //Check the response data.
    QJsonObject responseObject=QJsonDocument::fromJson(responseBytes).object(),
            updateObject;
    //Update account details.
    updateObject.insert("avatarPath",
                        responseObject.value("url").toString());
    //Send update request.
    updateOnlineAccount(updateObject, false);
    //Update the avatar successfully.
    emit avatarUpdatedSuccess();
    return true;
}

void KNAccount::logout()
{
    //Reset the account details.
    m_accountDetails->resetAccountDetail();
}

bool KNAccount::updateAccountInfo(const QJsonObject &userInfo)
{
    return updateOnlineAccount(userInfo, true);
}

bool KNAccount::refreshAccountInfo()
{
    //Check login state.
    if(m_accountDetails->isLogin())
    {
        //Login first.
        return false;
    }
    //Prepare the response data cache.
    QByteArray responseData;
    //Get the account information.
    if(get(generateKreogistRequest("classes/_User/" +
                                   m_accountDetails->objectId()),
           responseData)!=200)
    {
        //Failed to fetch account information.
        return false;
    }
    //Update account detail information.
    updateDetails(QJsonDocument::fromJson(responseData).object());
    //Mission complete.
    return true;
}

inline bool KNAccount::updateTokenSession()
{
    //Generate the login request.
    QNetworkRequest loginRequest=generateKreogistRequest("login");
    //Construct the json struct.
    //Try to login with encrypted password first.
    QUrlQuery loginQuery;
    //Insert user name.
    loginQuery.addQueryItem("username", m_accountDetails->cacheUserName());
    loginQuery.addQueryItem("password", m_accountDetails->cachePassword());
    //Insert login query to login url.
    QUrl loginUrl=loginRequest.url();
    loginUrl.setQuery(loginQuery);
    loginRequest.setUrl(loginUrl);
    //Get the login result.
    //Generate the response cache.
    QByteArray responseCache;
    //Send the JSON data to Kreogist Cloud.
    if(get(loginRequest, responseCache)!=200)
    {
        //Failed to fetch the new token session.
        return false;
    }
    //Check the response data.
    QJsonObject loginData=QJsonDocument::fromJson(responseCache).object();
    //Save the session token data.
    m_accountDetails->setSessionToken(
                loginData.value("sessionToken").toString());
    //Update the account details.
    updateDetails(loginData);
    //Update success.
    return true;
}

inline int KNAccount::accountPut(QNetworkRequest &request,
                                 const QByteArray &parameter,
                                 QByteArray &responseData)
{
    //Do original put.
    int result=put(request, parameter, responseData);
    //Check whether the result is failed.
    if(result==404)
    {
        //Check the response data code.
        //Code 206 means: User cannot be altered without sessionToken Error.
        //We have to relogin with the cache data.
        if(QJsonDocument::fromJson(responseData).object().value("code").toInt()
                ==206 && updateTokenSession())
        {
            //Update request session token.
            request.setRawHeader("X-Bmob-Session-Token",
                                 m_accountDetails->sessionToken().toUtf8());
            //Give back the new data.
            return put(request, parameter, responseData);
        }
        //Give the original.
        return result;
    }
    //Or else we will simply give back the result.
    return result;
}

inline QNetworkRequest KNAccount::generateKreogistRequest(const QString &url)
{
    //Generate a request.
    QNetworkRequest request;
    //Set the url.
    request.setUrl(QUrl("https://api.bmob.cn/1/"+url));
    //Configure the request.
    request.setRawHeader("X-Bmob-Application-Id",
                         "71a30357da3ec74b3ee3bf8fa969075f");
    request.setRawHeader("X-Bmob-REST-API-Key",
                         "d2d522da26665024157835d22a42b42c");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //Give back the request.
    return request;
}

QString KNAccount::accessPassword(const QString &rawPassword)
{
    //Use MD5 and SHA-3 to combine the access password.
    return bytesToHex(QCryptographicHash::hash(
                          rawPassword.toUtf8(),
                          QCryptographicHash::Sha3_512).append
                      (QCryptographicHash::hash(
                           rawPassword.toUtf8(),
                           QCryptographicHash::Md5)));
}

QString KNAccount::bytesToHex(const QByteArray &bytes)
{
    //Create the cache string.
    QString hexCache;
    //For all the bytes.
    for(int i=0; i<bytes.size(); ++i)
    {
        //Add all bytes to hex cache.
        hexCache.append(QString::number(static_cast<quint8>(bytes.at(i)), 16));
    }
    //Give back the hex cache.
    return hexCache;
}

bool KNAccount::updateOnlineAccount(const QJsonObject &userInfo,
                                    bool withSignal)
{
    //Check login first.
    if(!m_accountDetails->isLogin())
    {
        //Check signal flags.
        if(withSignal)
        {
            //Failed to update user info.
            emit userInfoUpdateFailed();
        }
        //Failed to update account info.
        return false;
    }
    //Tring to update account info.
    QNetworkRequest updateRequest=generateKreogistRequest(
                "users/"+m_accountDetails->objectId());
    //Set the session token.
    updateRequest.setRawHeader("X-Bmob-Session-Token",
                               m_accountDetails->sessionToken().toUtf8());
    //Generate response data.
    QByteArray responseData;
    //PUT the json data.
    if(accountPut(updateRequest,
                  QJsonDocument(userInfo).toJson(QJsonDocument::Compact),
                  responseData)!=200)
    {
        //Check signal flags.
        if(withSignal)
        {
            //Failed to update user info.
            emit userInfoUpdateFailed();
        }
        //Failed to update info.
        return false;
    }
    //Successfully update user data.
    //Get current user latest data.
    if(get(generateKreogistRequest("users/"+m_accountDetails->objectId()),
           responseData)!=200)
    {
        //Check signal flags.
        if(withSignal)
        {
            //Failed to update user info.
            emit userInfoUpdateFailed();
        }
        //Failed to update info.
        return false;
    }
    //Update user info.
    updateDetails(QJsonDocument::fromJson(responseData).object());
    //Check signal flags.
    if(withSignal)
    {
        //Emit success signal.
        emit userInfoUpdateSuccess();
    }
    //Mission complete.
    return true;
}

inline void KNAccount::updateDetails(const QJsonObject &userInfo)
{
    //Save the important data.
    m_accountDetails->setDisplayName(userInfo.value("displayName").toString());
    m_accountDetails->setAvatarPath(userInfo.value("avatarPath").toString());
    //Check avatar path.
    if(!m_accountDetails->avatarPath().isEmpty())
    {
        //Tring to get avatar from the paths.
        QNetworkRequest avatarRequest;
        //Set the url.
        avatarRequest.setUrl("http://file.bmob.cn/"+
                             m_accountDetails->avatarPath());
        QByteArray imageResponse;
        //Get the account avatar file.
        if(get(avatarRequest, imageResponse)==200)
        {
            //Set the image image resource.
            m_accountDetails->setAccountAvatar(
                        QPixmap::fromImage(QImage::fromData(imageResponse)));
        }
    }
    //We have to notify the account is update.
    emit m_accountDetails->accountUpdate();
}

KNAccount::~KNAccount()
{
    m_accountDetails->deleteLater();
}

void KNAccount::initial(QObject *parent)
{
    //Check static pointer.
    if(m_instance==nullptr)
    {
        //Initial the Kreogist Account system.
        m_instance=new KNAccount(parent);
    }
}

KNAccount *KNAccount::instance()
{
    return m_instance;
}

KNAccountDetails *KNAccount::accountDetails()
{
    return m_accountDetails;
}
