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
#include "knaccountdetails.h"

KNAccountDetails::KNAccountDetails(QObject *parent) :
    QObject(parent),
    m_objectId(QString()),
    m_sessionToken(QString()),
    m_cacheUserName(QString()),
    m_cachePassword(QString()),
    m_displayName(QString()),
    m_avatarPath(QString()),
    m_accountAvatar(QPixmap()),
    m_isLogin(false)
{
}

QString KNAccountDetails::objectId() const
{
    return m_objectId;
}

void KNAccountDetails::setObjectId(const QString &objectId)
{
    m_objectId = objectId;
}

QString KNAccountDetails::sessionToken() const
{
    return m_sessionToken;
}

void KNAccountDetails::setSessionToken(const QString &sessionToken)
{
    m_sessionToken = sessionToken;
}

QString KNAccountDetails::cacheUserName() const
{
    return m_cacheUserName;
}

void KNAccountDetails::setCacheUserName(const QString &cacheUserName)
{
    m_cacheUserName = cacheUserName;
}

QString KNAccountDetails::cachePassword() const
{
    return m_cachePassword;
}

void KNAccountDetails::setCachePassword(const QString &cachePassword)
{
    m_cachePassword = cachePassword;
}

QString KNAccountDetails::displayName() const
{
    return m_displayName.isEmpty() ? m_cacheUserName : m_displayName;
}

void KNAccountDetails::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

QPixmap KNAccountDetails::accountAvatar() const
{
    return m_accountAvatar;
}

void KNAccountDetails::setAccountAvatar(const QPixmap &accountAvatar)
{
    //Save the avatar.
    m_accountAvatar = accountAvatar;
    //Emit the update the avatar signal.
    emit accountAvatarUpdate();
}

bool KNAccountDetails::isLogin() const
{
    return m_isLogin;
}

void KNAccountDetails::resetAccountDetail()
{
    //Clear cache.
    m_objectId.clear();
    m_sessionToken.clear();
    m_cacheUserName.clear();
    m_cachePassword.clear();
    //Reset the account details.
    m_isLogin=false;
    m_displayName.clear();
    m_accountAvatar=QPixmap();
    m_accountInfo=QJsonObject();
    //Emit reset signal.
    emit accountReset();
    emit accountAvatarUpdate();
}

void KNAccountDetails::setIsLogin(bool isLogin)
{
    //Save login state.
    m_isLogin = isLogin;
}

QString KNAccountDetails::avatarPath() const
{
    return m_avatarPath;
}

QVariant KNAccountDetails::data(const QString &key)
{
    //Check whether the object contains the key.
    if(!m_accountInfo.contains(key))
    {
        //Return a null value.
        return QVariant();
    }
    //Get the data back.
    QJsonValue value=m_accountInfo.value(key);
    //Check the type of the value.
    switch(value.type())
    {
    //If the value type is bool, double, string and array, translate the
    //data to the specific type and use the default construct function of
    //the QVariant to build the value.
    case QJsonValue::Bool:
        return value.toBool();
    case QJsonValue::Double:
        return value.toDouble();
    case QJsonValue::String:
        return value.toString();
    default:
        //Ignore the object, it should never have object.
        return QVariant();
    }
}

void KNAccountDetails::setAvatarPath(const QString &avatarPath)
{
    m_avatarPath = avatarPath;
}

void KNAccountDetails::setAccountInfo(const QJsonObject &accountInfo)
{
    m_accountInfo = accountInfo;
}
