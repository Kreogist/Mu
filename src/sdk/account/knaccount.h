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
#ifndef KNACCOUNT_H
#define KNACCOUNT_H

#include <QImage>

#include "knrestapibase.h"

#define knAccount (KNAccount::instance())

class KNAccountDetails;
class KNConfigure;
/*!
 * \brief The KNAccount class provides the backend for Kreogist Account. It
 * supports all the operations user will do for Kreogist Account.
 */
class KNAccount : public KNRestApiBase
{
    Q_OBJECT
public:
    ~KNAccount();

    /*!
     * \brief Initial the Kreogist Account system with given parent.
     * \param parent The parent object pointer.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the Kreogist Account system object instance.
     * \return The singleton instance of Kreogist Account system.
     */
    static KNAccount *instance();

    /*!
     * \brief Get the account details pointer.
     * \return The account detail object pointer.
     */
    KNAccountDetails *accountDetails();

signals:
    /*!
     * \brief When login success, this signal will be emitted.
     */
    void loginSuccess();

    /*!
     * \brief When login failed, this signal will be emitted.
     * \param errorCode The login error code.
     */
    void loginFailed(int errorCode);

    /*!
     * \brief Generate request has been successfully done.
     */
    void generateSuccess();

    /*!
     * \brief Generate request failed, and the error code will show the reason.
     * \param errorCode The register error code.
     */
    void generateFailed(int errorCode);

    /*!
     * \brief When avatar is being updated, this signal will be emitted.
     */
    void avatarUpdatedSuccess();

    /*!
     * \brief When avatar updated failed, this signal will be emitted.
     */
    void avatarUpdatedFailed();

    /*!
     * \brief userInfoUpdateSuccess
     */
    void userInfoUpdateSuccess();

    /*!
     * \brief userInfoUpdateFailed
     */
    void userInfoUpdateFailed();

public slots:
    /*!
     * \brief Generate a Kreogist Account with the basic information.
     * \param userName User name of the account.
     * \param password Password of the account.
     * \param mail The E-mail address of the account.
     * \return If we could generate the account, then return true.
     */
    bool generateAccount(const QString &userName,
                         QString password,
                         const QString &mail);

    /*!
     * \brief Login the Kreogist Account with their user name and password.
     * \param userName The user name of the Kreogist Account. It could be E-mail
     * or user name.
     * \param password The password of the account.
     * \return If the user name and password is correct and login successfully,
     *  return true.
     */
    bool login(const QString &userName, QString password);

    /*!
     * \brief Set avatar pixmap to the account.
     * \param avatarImage The avatar image.
     * \return If user never login or the image is larger than 200x200 (neither
     * widget or height is huge), return false. If upload success, return true.
     */
    bool setAvatar(const QPixmap &avatarImage);

    /*!
     * \brief Logout the account.
     */
    void logout();

    /*!
     * \brief Update the account information.
     * \param userInfo All the new information about the account stored in JSON
     * format.
     * \return If we could update the account data successfully, then return
     * true.
     */
    bool updateAccountInfo(const QJsonObject &userInfo);

    /*!
     * \brief Get the account information from server, update the detail info.
     * \return If we could update the account info successfully, return true.
     */
    bool refreshAccountInfo();

    /*!
     * \brief Get the access password by input the raw password.
     * \param rawPassword The raw password.
     * \return Give back the raw password.
     */
    static QString accessPassword(const QString &rawPassword);

    /*!
     * \brief Change a byte array into a string which is the hex text of the
     * bytes.
     * \param bytes The byte array.
     * \return The Hex string.
     */
    static QString bytesToHex(const QByteArray &bytes);

private:
    inline bool updateOnlineAccount(const QJsonObject &userInfo,
                                    bool withSignal=true);
    inline bool updateTokenSession();
    inline int accountPut(QNetworkRequest &request,
                          const QByteArray &parameter,
                          QByteArray &responseData);

    inline QNetworkRequest generateKreogistRequest(const QString &url);
    inline void updateDetails(const QJsonObject &userInfo);

    static KNAccount *m_instance;

    explicit KNAccount(QObject *parent = 0);
    KNAccount(const KNAccount &);
    KNAccount(KNAccount &&);

    KNConfigure *m_cacheConfigure;
    KNAccountDetails *m_accountDetails;
};

#endif // KNACCOUNT_H
