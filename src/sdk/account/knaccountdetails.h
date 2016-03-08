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
#include <QJsonObject>
#include <QVariant>

#include <QObject>

/*!
 * \brief The KNAccountDetails class stored the basic information of a Kreogist
 * account.
 */
class KNAccountDetails : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountDetails object.
     * \param parent The parent object pointer.
     */
    explicit KNAccountDetails(QObject *parent = 0);

    /*!
     * \brief Get the user account unique object ID.
     * \return User object ID.
     */
    QString objectId() const;

    /*!
     * \brief Get the current session token text.
     * \return Session token text.
     */
    QString sessionToken() const;

    /*!
     * \brief Get cached user name.
     * \return Last login user name.
     */
    QString cacheUserName() const;

    /*!
     * \brief Get the cache password. It won't be the original password, it's
     * the password encrypted result of SHA-3 and MD5 combined.
     * \return User cached password.
     */
    QString cachePassword() const;

    /*!
     * \brief Get the display name of the current account.
     * \return
     */
    QString displayName() const;

    /*!
     * \brief Get the account avatar picture.
     * \return Get the account avatar pixmap.
     */
    QPixmap accountAvatar() const;

    /*!
     * \brief Get whether user has login to Kreogist Account.
     * \return If user has stored cache username and password before, return
     * true.
     */
    bool isLogin() const;

    /*!
     * \brief Get the online avatar path of the account.
     * \return The avatar file path.
     */
    QString avatarPath() const;

    /*!
     * \brief Get a data from the account info via the key.
     * \param key The data key.
     * \return The data.
     */
    QVariant data(const QString &key);

signals:
    /*!
     * \brief When the account information is reseted, this signal will be
     * emitted.
     */
    void accountReset();

    /*!
     * \brief When the account avatar information is updated, this signal will
     * be emitted.
     */
    void accountAvatarUpdate();

    /*!
     * \brief When account information is updated, this signal will be emitted.
     */
    void accountUpdate();

public slots:
    /*!
     * \brief Reset the account detail information. Clear all the caches.
     */
    void resetAccountDetail();

    /*!
     * \brief Set the user unique Object ID
     * \param objectId User unique Object ID.
     */
    void setObjectId(const QString &objectId);

    /*!
     * \brief Set the session token string data.
     * \param sessionToken The session token text.
     */
    void setSessionToken(const QString &sessionToken);

    /*!
     * \brief Set cache username.
     * \param cacheUserName Login username.
     */
    void setCacheUserName(const QString &cacheUserName);

    /*!
     * \brief Set the cache password for automatically relogin.
     * \param cachePassword Encrypted password.
     */
    void setCachePassword(const QString &cachePassword);

    /*!
     * \brief Set the display name text.
     * \param displayName User set display name.
     */
    void setDisplayName(const QString &displayName);

    /*!
     * \brief Set the account avatar pixmap which downloaded from online path.
     * \param accountAvatar User account avatar.
     */
    void setAccountAvatar(const QPixmap &accountAvatar);

    /*!
     * \brief Set the login state of the user account.
     * \param isLogin The login state. True for login.
     */
    void setIsLogin(bool isLogin);

    /*!
     * \brief Set the avatar image online path.
     * \param avatarPath Online avatar file path.
     */
    void setAvatarPath(const QString &avatarPath);

    /*!
     * \brief Set the account information, we could get data from here for
     * extension.
     * \param accountInfo Account information JSON object.
     */
    void setAccountInfo(const QJsonObject &accountInfo);

private:
    //Account information.
    QJsonObject m_accountInfo;
    //Account IDs.
    QString m_objectId, m_sessionToken, m_cacheUserName, m_cachePassword;
    //Account details.
    QString m_displayName, m_avatarPath;
    QPixmap m_accountAvatar;
    bool m_isLogin;
};

#endif // KNACCOUNTDETAILS_H
