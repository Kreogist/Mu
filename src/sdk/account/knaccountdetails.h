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
#ifndef KNACCOUNTDETAILS_H
#define KNACCOUNTDETAILS_H

#include <QPixmap>

#include <QObject>

/*!
 * \brief The KNAccountDetails class
 */
class KNAccountDetails : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief KNAccountDetails
     * \param parent
     */
    explicit KNAccountDetails(QObject *parent = 0);

    QString objectId() const;

    QString sessionToken() const;

    QString cacheUserName() const;

    QString cachePassword() const;

    QString displayName() const;

    QPixmap accountAvatar() const;

    bool isLogin() const;

    QString avatarPath() const;

signals:
    void accountReset();
    void accountUpdate();

public slots:
    void resetAccountDetail();
    void setObjectId(const QString &objectId);
    void setSessionToken(const QString &sessionToken);
    void setCacheUserName(const QString &cacheUserName);
    void setCachePassword(const QString &cachePassword);
    void setDisplayName(const QString &displayName);
    void setAccountAvatar(const QPixmap &accountAvatar);
    void setIsLogin(bool isLogin);
    void setAvatarPath(const QString &avatarPath);

private:
    //Account information.
    //Account IDs.
    QString m_objectId, m_sessionToken, m_cacheUserName, m_cachePassword;
    //Account details.
    QString m_displayName, m_avatarPath;
    QPixmap m_accountAvatar;
    bool m_isLogin;
};

#endif // KNACCOUNTDETAILS_H
