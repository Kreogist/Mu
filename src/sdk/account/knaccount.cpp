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
#include <QBuffer>
#include <QCryptographicHash>
#include <QDir>
#include <QFileInfo>
#include <QTimeZone>
#include <QJsonDocument>
#include <QUrlQuery>

#include "knutil.h"
#include "knglobal.h"
#include "knaccountdetails.h"
#include "knaccountutil.h"
#include "knconfigure.h"
#include "knconfiguremanager.h"

#include "knaccount.h"

#define CacheUsernameField "Username"
#define CachePasswordField "Password"
#define CacheNicknameField "DisplayName"
#define MaximumPasswordLength 151

using namespace AccountUtil;

KNAccount *KNAccount::m_instance=nullptr;

KNAccount::KNAccount(QObject *parent) :
    KNAccountBase(parent),
    m_cloudConfigureTableName(QString()),
    m_accountConfigure(nullptr),
    m_userConfigure(nullptr),
    m_accountDetails(new KNAccountDetails)
{
}

KNAccount::~KNAccount()
{
    //Remove the account detail pointer.
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

void KNAccount::saveConfigure()
{
    //Check cache configure.
    if(m_accountConfigure==nullptr)
    {
        //Ignore the cache configure.
        return;
    }
    //Get the avatar image cache path.
    QString avatarPath=
            knGlobal->dirPath(KNGlobal::AccountDir)+"/avatar.png";
    //Check whether the cache data is empty.
    if(m_accountDetails->cacheUserName().isEmpty())
    {
        //Clear the cache configure.
        m_accountConfigure->setData(CacheUsernameField, "");
        m_accountConfigure->setData(CachePasswordField, "");
        m_accountConfigure->setData(CacheNicknameField, "");
        //Remove the avatar.
        QFile::remove(avatarPath);
        //Misson complete.
        return;
    }
    //Save the detail info into cache configure.
    m_accountConfigure->setData(CacheUsernameField,
                                m_accountDetails->cacheUserName());
    m_accountConfigure->setData(CachePasswordField,
                                m_accountDetails->cachePassword());
    m_accountConfigure->setData(CacheNicknameField,
                                m_accountDetails->displayName());
    //Check the header pixmap.
    if(m_accountDetails->accountAvatar().isNull())
    {
        //Remove the cached header icon.
        //Check existance.
        if(QFileInfo::exists(avatarPath))
        {
            //Remove the file.
            QFile::remove(avatarPath);
        }
    }
    else
    {
        //Save the cached header icon.
        QFileInfo targetFile(avatarPath);
        //Generate the dir, ensure the directory is valid..
        if(!KNUtil::ensurePathValid(targetFile.absolutePath()).isEmpty())
        {
            //Save the pixmap to the target file.
            m_accountDetails->accountAvatar().save(
                        targetFile.absoluteFilePath(),
                        "png");
        }
    }
}

void KNAccount::setConfigureTableName(const QString &tableName)
{
    //Save the configure table.
    m_cloudConfigureTableName = tableName;
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
    password=KNAccountUtil::accessPassword(password);
    //Construct the register json struct.
    QJsonObject registerData;
    registerData.insert("username", userName);
    registerData.insert("password", password);
    registerData.insert("email", mail);
    registerData.insert(m_cloudConfigureTableName, "");
    //Generate the response cache and error code.
    QByteArray responseCache;
    int errorCode;
    //Execute the register action.
    if(!createRow("users",
                  jsonToString(registerData),
                  responseCache,
                  errorCode))
    {
        //Check the error code.
        switch(errorCode)
        {
        case 202:
            emit generateFailed(UserNameAlreadyTaken);
            break;
        case 203:
            emit generateFailed(EmailAlreadyTaken);
            break;
        default:
            emit generateFailed(UnknownRegisterError);
            break;
        }
        //Failed to post then failed.
        return false;
    }
    //Check the response data.
    registerData=QJsonDocument::fromJson(responseCache).object();
    //Check register response data.
    if(!registerData.contains("objectId"))
    {
        //Emit failed for unknown reason.
        emit generateFailed(UnknownRegisterError);
        //Or else failed to login.
        return false;
    }
    //Update the account information.
    fetchAccountInfo();
    //Emit register success signal.
    emit generateSuccess();
    //Upload the user configuration file.
    syncUserConfigure();
    //Mission complete.
    return true;
}

bool KNAccount::login(const QString &userName, QString password)
{
    //Check out whether user is already login.
    if(m_accountDetails->isLogin() ||
            //To avoid user copy and paste the hash result of the account
            //directly to the password to login, we will check out the length of
            //the password.
            password.length()>MaximumPasswordLength)
    {
        //Emit failed signal.
        emit loginFailed(InfoIncorrect);
        //You cannot login two account at the same time.
        return false;
    }
    //Encrypt the password.
    QString encryptedPassword=KNAccountUtil::accessPassword(password);
    //Generate the response cache.
    QJsonObject loginData;
    //Generate the error code cache.
    int errorCode;
    //Try to login.
    if(!loginWith(userName, encryptedPassword, loginData, errorCode))
    {
        //Check for the error code.
        switch(errorCode)
        {
        case 0:
            //Ask user to check Internet connection.
            emit loginFailed(LoginConnectionError);
            //Failed to login.
            return false;
        case 200:
            //Shouldn't go here.
            break;
        default:
        {
            //Treat all the other error as info incorrect.
            //Tried to use the original password.
            //Because when user reset the password, it will use the original
            //password.
            if(!loginWith(userName, password, loginData, errorCode))
            {
                //Check Internet connection error code.
                if(errorCode<1)
                {
                    //Ask user to check Internet connection.
                    emit loginFailed(LoginConnectionError);
                    //Failed to login.
                    return false;
                }
                //Emit failed signal.
                emit loginFailed(InfoIncorrect);
                //Failed to login, password is wrong.
                return false;
            }
            //Now login success, but we need to change the password to the
            //encrypted version.
            //Get the necessary information.
            QString sessionToken=loginData.value("sessionToken").toString(),
                    objectId=loginData.value("objectId").toString();
            //Set the password updater.
            loginData=QJsonObject();
            loginData.insert("password", encryptedPassword);
            //Generate the udpate response.
            QByteArray resetPassResponse;
            int errorCode;
            //Update the password.
            if(!updateRow("users/"+objectId,
                          jsonToString(loginData),
                          resetPassResponse,
                          errorCode,
                          sessionToken))
            {
                //Clear the password.
                m_accountDetails->resetAccountDetail();
                //Emit failed signal.
                emit loginFailed(LoginConnectionError);
                //Failed to change the password, failed to login.
                return false;
            }
            //Save the encrypted password as the cached password.
            m_accountDetails->setCachePassword(encryptedPassword);
        }
        }
    }
    //Update the user information.
    updateLocalDetails(loginData);
    //Emit success signal.
    emit loginSuccess();
    //Once we could login, we need to sync the configuration file.
    syncUserConfigure();
    //Successfully login.
    return true;
}

void KNAccount::autoLogin()
{
    //Check whether the account details contains user name and password.
    if(m_accountDetails->cacheUserName().isEmpty() ||
            m_accountDetails->cachePassword().isEmpty())
    {
        //Failed to auto login.
        return;
    }
    //Emit start auto login signal.
    emit startAutoLogin();
    //Prepare the response cache.
    QJsonObject loginData;
    int errorCode;
    //Tried to login with cache username and password.
    if(!loginWith(m_accountDetails->cacheUserName(),
                  m_accountDetails->cachePassword(),
                  loginData, errorCode))
    {
        //Check the login result.
        if(errorCode<1)
        {
            //For auto login failed.
            emit autoLoginFailed(LoginConnectionError);
        }
        else
        {
            //Emit failed signal.
            emit autoLoginFailed(InfoIncorrect);
            //Clear the cache user name and cache password.
            m_accountDetails->resetAccountDetail();
        }
        //Failed to login.
        return;
    }
    //Update the user information.
    updateLocalDetails(loginData);
    //Emit success signal.
    emit loginSuccess();
    //Once we could login, we need to sync the configuration file.
    syncUserConfigure();
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
        //Remove the previous avatar first.
        //Set the original path.
        QString avatarUrl="https://api.bmob.cn/1/files/" +
                            m_accountDetails->avatarPath();
        //Check the avatar path, if it's starts with http, then we will use the
        //new API to remove the previous image.
        if(m_accountDetails->avatarPath().startsWith("http"))
        {
            //Save the avatar data.
            const QJsonObject &avatarData=m_accountDetails->avatarData();
            //Get the file Url.
            QString fileUrl=avatarData.value("url").toString();
            //Combine the remove URL.
            avatarUrl="https://api.bmob.cn/2/files/" +
                    avatarData.value("cdn").toString() +
                    fileUrl.mid(fileUrl.indexOf('/', 8));
        }
        //Prepare the error code.
        int errorCode;
        //Remove the file.
        if(!removeFile(avatarUrl, errorCode))
        {
            //Emit the failed connection signal.
            emit updateInternetError();
            //Failed to remove the previous one, which is failed to update.
            return false;
        }
    }
    //Generate image cache.
    QByteArray imageBytes;
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
    //Prepare the response cache and error code.
    QByteArray responseCache;
    int errorCode;
    //Upload a file.
    if(!uploadFile(m_accountDetails->objectId() + "/avatar.jpg",
                   imageBytes, responseCache, errorCode, "image/jpeg"))
    {
        //Check the error code.
        if(errorCode<0)
        {
            //Emit the failed connection signal.
            emit updateInternetError();
            //Failed to upload the image.
            return false;
        }
        //Failed to upload the avatar.
        emit avatarUpdatedFailed();
        //Failed to upload the image.
        return false;
    }
    //Check the response data.
    QJsonObject updateAvatarRequest;
    //Update account details.
    updateAvatarRequest.insert("avatarPath", QString(responseCache));
    //Send update request.
    if(updateRemoteRow("users/"+m_accountDetails->objectId(),
                       updateAvatarRequest, false))
    {
        //Update the avatar successfully.
        emit avatarUpdatedSuccess();
        //Set the data to detail info.
        m_accountDetails->setAvatarPath(responseCache);
        return true;
    }
    //Else is failed.
    emit avatarUpdatedFailed();
    //Mission failed.
    return false;
}

void KNAccount::logout()
{
    //Reset the account details.
    m_accountDetails->resetAccountDetail();
    //Clear the caching data.
    m_accountConfigure->setData(CacheUsernameField, "");
    m_accountConfigure->setData(CachePasswordField, "");
    m_accountConfigure->setData(CacheNicknameField, "");
    //Clear the cached image.
    //Do the check for the path.
    if(!knGlobal->dirPath(KNGlobal::AccountDir).isEmpty())
    {
        //Remove the avatar file.
        QFile::remove(knGlobal->dirPath(KNGlobal::AccountDir)+"/avatar.png");
    }
}

bool KNAccount::fetchAccountInfo()
{
    //Check login state.
    if(!m_accountDetails->isLogin())
    {
        //Login first.
        return false;
    }
    //Prepare the response data cache.
    QByteArray responseData;
    int errorCode;
    //Get the account information.
    if(!fetchRow("classes/_User/"+m_accountDetails->objectId(),
                 responseData,
                 errorCode))
    {
        //Failed to fetch account information.
        return false;
    }
    //Update account detail information.
    updateLocalDetails(stringToJson(responseData));
    //Mission complete.
    return true;
}

bool KNAccount::updateAccountInfo(const QJsonObject &userInfo)
{
    //Call the update function.
    return updateRemoteRow("users/"+m_accountDetails->objectId(),
                           userInfo, true);
}

bool KNAccount::resetPassword(const QString &emailAddress)
{
    //Generate the reset password request data and response cache.
    QByteArray resetRequestData;
    {
        //Generate the post data.
        QJsonObject resetBody;
        //Insert the email data.
        resetBody.insert("email", emailAddress);
        //Translate the reset body to reset data.
        resetRequestData=jsonToString(resetBody);
    }
    //Prepare the response cache and error code.
    QByteArray responseCache;
    int errorCode;
    //Create a new row for the reqest queue.
    if(createRow("requestPasswordReset", resetRequestData,
                 responseCache, errorCode))
    {
        //Then success, the reset E-mail has been sent.
        emit resetEmailSendSuccess();
        //Mission complete.
        return true;
    }
    //Handling the error code.
    if(errorCode<0)
    {
        //Internet error.
        emit resetEmailError(ResetConnectionError);
        //Mission Failed.
        return false;
    }
    //Check the response of the code.
    QJsonObject errorMessage=stringToJson(responseCache);
    //If the error message code is 205, it means no user found with E-mail.
    if(errorMessage.contains("code") &&
            errorMessage.value("code").toInt()==205)
    {
        //Emit the no E-mail error.
        emit resetEmailError(ResetCannotFindEmail);
        //Mission Failed.
        return false;
    }
    //Unknown Internet error, ask user to try to send the email again.
    emit resetEmailError(ResetUnknownError);
    //Failed to send the reset password E-mail.
    return false;
}

void KNAccount::setAccountConfigure(KNConfigure *accountConfigure)
{
    //Save the cache configure.
    m_accountConfigure = accountConfigure;
    //Set the data from cache configure.
    if(m_accountConfigure==nullptr)
    {
        //Mission complete, still cannot use.
        return;
    }
    //Set data to user detail.
    QString cacheUsername=
            m_accountConfigure->data(CacheUsernameField).toString();
    //Check whether the cache username is valid.
    if(cacheUsername.isEmpty())
    {
        //Mission complete.
        return;
    }
    //Save the user name information to data.
    m_accountDetails->setCacheUserName(cacheUsername);
    m_accountDetails->setCachePassword(
                m_accountConfigure->data(CachePasswordField).toString());
    m_accountDetails->setDisplayName(
                m_accountConfigure->data(CacheNicknameField).toString());
}

void KNAccount::setUserConfigure(KNConfigure *userConfigure)
{
    //Save the user configure.
    m_userConfigure=userConfigure;
}

bool KNAccount::syncUserConfigure()
{
    //Check the configure ID.
    if(m_accountDetails->configureId().isEmpty())
    {
        int errorCode;
        //There is no configure ID here. Create a new table for it.
        if(!createCloudUserConfigure(errorCode))
        {
            //!FIXME: Process the error here.
            //Failed.
            return false;
        }
        //Mission complete.
        return true;
    }
    //Create the configure data.
    QJsonObject configureData;
    //Download the user configure data.
    if(!downloadUserConfigure(configureData))
    {
        //Failed to download the configure data.
        return false;
    }
    //Check the configure updated date and time.
    QDateTime cloudUpdatedTime=
            QDateTime::fromString(configureData.value("updatedAt").toString(),
                                  "yyyy-MM-dd HH:mm:ss");
    //The cloud updated time is stored for UTC/GMT+08:00
    cloudUpdatedTime.setTimeZone(QTimeZone(28800));
    //Remove the object Id, createdAt and updatedAt keys.
    configureData.remove("objectId");
    configureData.remove("createdAt");
    configureData.remove("updatedAt");
    //Change the cloud time to local time.
    cloudUpdatedTime=cloudUpdatedTime.toLocalTime();
    //Check the local configure file updated time.
    if(knConf->userConfigureUpdateTime() < cloudUpdatedTime)
    {
        //Replace the local configure definitely.
        m_userConfigure->setDataObject(configureData);
        //Complete.
        return true;
    }
    //Upload the latest version configure to the cloud.
    return uploadUserConfigure();
}

void KNAccount::startToWork()
{
    //Load the cache image.
    QString cachedAvatarPath=
            knGlobal->dirPath(KNGlobal::AccountDir)+"/avatar.png";
    //Check whether the image is exist and valid.
    if(QFileInfo::exists(cachedAvatarPath))
    {
        //Load the image.
        QPixmap cachedImage(cachedAvatarPath, "PNG");
        //If the cached image is not null, load it to account details.
        if(!cachedImage.isNull())
        {
            //Set the avatar.
            m_accountDetails->setAccountAvatar(cachedImage);
        }
    }
    //Do auto login
    autoLogin();
}

inline QByteArray KNAccount::jsonToString(const QJsonObject &data)
{
    //Use json document to translate a json object to byte array.
    return QJsonDocument(data).toJson(QJsonDocument::Compact);
}

QJsonObject KNAccount::stringToJson(const QByteArray &json)
{
    //Use josn document to parse the text.
    return QJsonDocument::fromJson(json).object();
}

inline bool KNAccount::loginWith(const QString &username,
                                  const QString &password,
                                  QJsonObject &loginData,
                                  int &errorCode)
{
    //Generate the login query.
    QMap<QString, QString> loginQuery;
    //Set the username and password.
    loginQuery.insert("username", username);
    loginQuery.insert("password", password);
    //Prepare the response cache.
    QByteArray responseCache;
    //Get the result of the data.
    if(fetchRow("login", responseCache, errorCode, loginQuery))
    {
        //Parse the login info.
        loginData=stringToJson(responseCache);
        //Set the login flag to true.
        m_accountDetails->setIsLogin(true);
        //Save the user name and password for auto login.
        m_accountDetails->setCacheUserName(username);
        m_accountDetails->setCachePassword(password);
        //Save the contact information.
        m_accountDetails->setObjectId(loginData.value("objectId").toString());
        m_accountDetails->setSessionToken(
                    loginData.value("sessionToken").toString());
        //Finished.
        return true;
    }
    //Failed, simply return false.
    return false;
}

inline bool KNAccount::updateTokenSession()
{
    //Generate the response cache.
    QJsonObject loginData;
    int errorCode;
    //Login with the cached username and password.
    if(!loginWith(m_accountDetails->cacheUserName(),
                  m_accountDetails->cachePassword(),
                  loginData, errorCode))
    {
        //Failed to fetch the new token session.
        return false;
    }
    //Update the account details.
    updateLocalDetails(loginData);
    //Update success.
    return true;
}

inline void KNAccount::updateLocalDetails(const QJsonObject &userInfo)
{
    //Sace the user info.
    m_accountDetails->setAccountInfo(userInfo);
    //Save the important data.
    m_accountDetails->setDisplayName(userInfo.value("displayName").toString());
    //Save the update configure id.
    m_accountDetails->setConfigureId(
                userInfo.value(m_cloudConfigureTableName).toString());
    //Check the avatar path.
    QString onlineAvatarPath=userInfo.value("avatarPath").toString();
    if(m_accountDetails->avatarPath()==onlineAvatarPath)
    {
        //Update complete.
        return;
    }
    //Update the avatar path.
    m_accountDetails->setAvatarPath(userInfo.value("avatarPath").toString());
    //Check avatar path.
    if(!m_accountDetails->avatarPath().isEmpty())
    {
        //Generate the avatar response cache.
        QByteArray avatarResponse;
        int errorCode;
        //Download the file,
        if(downloadFile(m_accountDetails->avatarPath().startsWith("http")?
                        m_accountDetails->avatarPath():
                        ("http://file.bmob.cn/" +
                         m_accountDetails->avatarPath()),
                        avatarResponse,
                        errorCode))
        {
            //Set the image image resource.
            m_accountDetails->setAccountAvatar(
                        QPixmap::fromImage(QImage::fromData(avatarResponse)));
        }
        else
        {
            //Error happens here.
            //Reset the account details avatar.
            m_accountDetails->setAccountAvatar(QPixmap());
        }
    }
    //Notify the account is update.
    emit m_accountDetails->accountUpdate();
}

inline bool KNAccount::updateRemoteRow(const QString &url,
                                        const QJsonObject &userInfo,
                                        bool withSignal)
{
    //Check login first.
    if(!m_accountDetails->isLogin())
    {
        //Check signal flag.
        if(withSignal)
        {
            //Failed to update user info.
            emit userInfoUpdateFailed();
        }
        //Failed to update account info.
        return false;
    }
    //Generate response data.
    QByteArray responseCache, requestData=jsonToString(userInfo);
    int errorCode;
    //Update the user row data.
    if(updateRow(url, requestData, responseCache, errorCode,
                 m_accountDetails->sessionToken()))
    {
        //Success.
        return true;
    }
    //Check the error code.
    if(errorCode==404 &&
            //One possibility of returning a 404 HTTP status code is the
            //token session id is expired.
            //Although it seems to be impossible, because from the official
            //document it says that the expired date is 1 year.
            stringToJson(responseCache).value("code").toInt()==206)
    {
        //Update the token session.
        if(!updateTokenSession())
        {
            //Failed to update the token session.
            return false;
        }
        //Retry with the new token session.
        return updateRow(url, requestData, responseCache, errorCode,
                         m_accountDetails->sessionToken());
    }
    //Failed to update the token session.
    return false;
}

inline bool KNAccount::createCloudUserConfigure(int &errorCode)
{
    //Check the user configureation.
    if(!m_userConfigure || !m_accountDetails->isLogin())
    {
        //Cannot create with null user configure.
        return false;
    }
    //Get the json data of the user configure.
    QJsonObject requestData=m_userConfigure->dataObject(),
                accessControlList, userAccessControl;
    //Set the user access.
    userAccessControl.insert("read", true);
    userAccessControl.insert("write", true);
    accessControlList.insert(m_accountDetails->objectId(), userAccessControl);
    //Insert the ACL to request data.
    QString aclData=QString(jsonToString(accessControlList));
    //Replace the acl data from " to \"
    aclData.replace("\"", "\\\"");
    requestData.insert("ACL", accessControlList);
    //Generate the response cache.
    QByteArray responseCache;
    //Create the row.
    if(!createRow("classes/" + m_cloudConfigureTableName,
                  jsonToString(requestData),
                  responseCache, errorCode))
    {
        //!FIXME: Failed to create the user configure row.
        return false;
    }
    //Check the response data.
    QJsonObject createResult=stringToJson(responseCache);
    //Get the object ID of the create result.
    QString configureObjectId=createResult.value("objectId").toString();
    m_accountDetails->setConfigureId(configureObjectId);
    //Update the account details.
    QJsonObject setConfigureId;
    setConfigureId.insert("MuConfig", configureObjectId);
    //Reset the error code.
    errorCode=0;
    //Update the user information.
    return updateRemoteRow("users/"+m_accountDetails->objectId(),
                           setConfigureId, false);
}

inline bool KNAccount::uploadUserConfigure()
{
    //Check the login state.
    if(!m_accountDetails->isLogin())
    {
        //Failed to download user configure.
        return false;
    }
    //Prepare the cache and error code.
    QByteArray responseCache;
    int errorCode;
    //Update the information.
    if(!updateRow("classes/" + m_cloudConfigureTableName + "/" +
                  m_accountDetails->configureId(),
                  jsonToString(m_userConfigure->dataObject()), responseCache,
                  errorCode, m_accountDetails->sessionToken()))
    {
        //Failed.
        return false;
    }
    qDebug()<<responseCache;
    return true;
}

inline bool KNAccount::downloadUserConfigure(QJsonObject &configureData)
{
    //Check the login state.
    if(!m_accountDetails->isLogin())
    {
        //Failed to download user configure.
        return false;
    }
    //Prepare the cache and error code.
    QByteArray responseCache;
    int errorCode;
    //Fetch the information.
    if(!fetchRow("classes/" + m_cloudConfigureTableName + "/" +
                 m_accountDetails->configureId(),
                 responseCache, errorCode, QMap<QString, QString>(),
                 m_accountDetails->sessionToken()))
    {
        //Failed.
        return false;
    }
    //Parse the response cache.
    configureData=stringToJson(responseCache);
    //Mission complete.
    return true;
}
