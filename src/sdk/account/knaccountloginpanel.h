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
#ifndef KNACCOUNTLOGINPANEL_H
#define KNACCOUNTLOGINPANEL_H

#include "account/knaccountutil.h"

#include <QWidget>

class QLabel;
class KNAccountDetails;
class KNImageLabel;
class KNSaoSubMenu;
class KNLabelLineEdit;
class KNOpacityAnimeButton;
/*!
 * \brief The KNAccountLoginPanel class provides a widget to login to Kreogist
 * Account. The widget style is SAO style, notice this widget could only be used
 * in notification center.
 */
class KNAccountLoginPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountPanel widget.
     * \param parent The parent widget.
     */
    explicit KNAccountLoginPanel(QWidget *parent = 0);

    /*!
     * \brief Get the username.
     * \return Username line edit box text.
     */
    QString username() const;

    /*!
     * \brief Get the password.
     * \return Password line edit box text.
     */
    QString password() const;

signals:
    /*!
     * \brief Ask to show the register panel.
     */
    void requireRegister();

    /*!
     * \brief Ask to do the login.
     */
    void requireLogin();

    /*!
     * \brief This signal will only be asked when auto login failed.
     */
    void requireRelogin();

public slots:
    /*!
     * \brief Clear all the data of the text box.
     */
    void clearInputData();

    /*!
     * \brief When there's error occur on login, show the error.
     * \param errorCode The error code. It should be in the enumeration
     * KNAccountUtil::LoginErrorCode.
     */
    void onActionLoginError(int errorCode);

    /*!
     * \brief When there's error occur on auto login, show the error.
     * \param errorCode The error code. It should be in the enumeration
     * KNAccountUtil::LoginErrorCode.
     */
    void onActionAutoLoginError(int errorCode);

    /*!
     * \brief Set the account details class pointer to the panel.
     * \param accountDetails The account details object pointer.
     */
    void setAccountDetails(KNAccountDetails *accountDetails);

private slots:
    void retranslate();
    void onActionForgetPassword();
    void onActionLoginWithAnotherAccount();
    void onActionCheckInput();

private:
    enum AccountActions
    {
        RegisterAccount,
        ForgetPassword,
        LoginWithAnotherAccount,
        AccountActionCount
    };
    inline KNOpacityAnimeButton *generateButton(const QString &iconPath);
    QAction *m_menuActions[AccountActionCount];
    QPixmap m_kreogistLogo, m_anonymousLogo;
    QPalette m_errorPal, m_hintPal;
    QString m_loginFailedTitle, m_subTitleText,
            m_errorDescription[KNAccountUtil::LoginErrorCodeCount];
    KNAccountDetails *m_accountDetails;
    KNImageLabel *m_avatarLabel;
    QLabel *m_title, *m_subTitle, *m_accountDisplayName;
    KNLabelLineEdit *m_username, *m_password;
    KNOpacityAnimeButton *m_login, *m_relogin, *m_others;
    KNSaoSubMenu *m_actionMenu;
};

#endif // KNACCOUNTLOGINPANEL_H
