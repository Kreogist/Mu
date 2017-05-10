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

#ifndef KNACCOUNT_H
#define KNACCOUNT_H

#include <QJsonObject>

#include "knaccountbase.h"

#define knAccount (KNAccount::instance())

class KNAccountDetails;
class KNConfigure;
/*!
 * \brief The KNAccount class provides the backend for Kreogist Account. It
 * supports all the operations user will do for Kreogist Account.
 */
class KNAccount : public KNAccountBase
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

    /*!
     * \brief Save configure to account configure.
     */
    void saveConfigure();

signals:
    /*!
     * \brief When auto login failed, this signal will be emitted and show the
     * reason.
     * \param errorCode The login error code.
     */
    void autoLoginFailed(int errorCode);

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

    /*!
     * \brief When auto login start, this signal will be emitted.
     */
    void startAutoLogin();

    /*!
     * \brief When an Internet error is detected in updating user info, this
     * signal will be emit.
     */
    void updateInternetError();

    /*!
     * \brief When the reset email has been send successfully, this signal will
     * be emitted.
     */
    void resetEmailSendSuccess();

    /*!
     * \brief If the reset e-mail contains any error, this signal will be
     * emitted.
     * \param errorCode The error code of ResetPasswordErrorCode defines in the
     * account util.
     */
    void resetEmailError(int errorCode);

public slots:
    /*!
     * \brief Set the name of the configure table in the cloud.
     * \param cloudConfigureTableName The name of the table.
     */
    void setConfigureTableName(const QString &tableName);

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
     * \brief Do login with the cache username and password in the user detail
     * cache.
     */
    void autoLogin();

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
     * \brief Get the account information from server, update the detail info.
     * \return If we could update the account info successfully, return true.
     */
    bool fetchAccountInfo();

    /*!
     * \brief Change the account information stores online.
     * \param userInfo All the new information about the account stored in JSON
     * format.
     * \return If we could update the account data successfully, then return
     * true.
     */
    bool updateAccountInfo(const QJsonObject &userInfo);

    /*!
     * \brief Ask to reset one user password.
     * \param emailAddress User E-mail account.
     * \return If we could send the reset request successfully, then return
     * true.
     */
    bool resetPassword(const QString &emailAddress);

    /*!
     * \brief Set the account configure to the account manager.
     * \param accountConfigure Account cache configure.
     */
    void setAccountConfigure(KNConfigure *accountConfigure);

    /*!
     * \brief Set the user configure to the account manager.
     * \param userConfigure The user configure data pointer.
     */
    void setUserConfigure(KNConfigure *userConfigure);

    /*!
     * \brief Update the configure data. If the local configure is later than
     * the online version, we will replace the online setting. If the online
     * setting is later than the local setting, we will replace the local
     * setting.
     * \return If the configure sync successfully, return true.
     */
    bool syncUserConfigure();

    /*!
     * \brief This is the initial online working slots for global instance to
     * start up.
     */
    void startToWork();

private:
    explicit KNAccount(QObject *parent = 0);
    KNAccount(const KNAccount &);
    KNAccount(KNAccount &&);

    inline QByteArray jsonToString(const QJsonObject &data);
    inline QJsonObject stringToJson(const QByteArray &json);
    inline bool loginWith(const QString &username,
                          const QString &password,
                          QJsonObject &loginData,
                          int &errorCode);
    inline bool updateTokenSession();
    inline void updateLocalDetails(const QJsonObject &userInfo);
    inline bool updateRemoteRow(const QString &url,
                                const QJsonObject &userInfo,
                                bool withSignal);
    inline bool createCloudUserConfigure(int &errorCode);
    inline bool uploadUserConfigure();
    inline bool downloadUserConfigure(QJsonObject &configureData);

    static KNAccount *m_instance;

    QString m_cloudConfigureTableName;
    KNConfigure *m_accountConfigure, *m_userConfigure;
    KNAccountDetails *m_accountDetails;
};

#endif // KNACCOUNT_H
