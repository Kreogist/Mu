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
    return m_displayName;
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
    m_accountAvatar = accountAvatar;
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
    //Emit reset signal.
    emit accountReset();
}

void KNAccountDetails::setIsLogin(bool isLogin)
{
    m_isLogin = isLogin;
}

QString KNAccountDetails::avatarPath() const
{
    return m_avatarPath;
}

void KNAccountDetails::setAvatarPath(const QString &avatarPath)
{
    m_avatarPath = avatarPath;
}
