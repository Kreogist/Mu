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

#ifndef KNACCOUNTPANEL_H
#define KNACCOUNTPANEL_H

#include <QWidget>

class KNHWidgetSwitcher;
class KNAccountLoginPanel;
class KNAccountRegisterPanel;
class KNAccountWaitingPanel;
class KNAccountDetailPanel;
/*!
 * \brief The KNAccountPanel class provide a widget which could manage the
 * Kreogist Account. It could login, register and display the account
 * information.
 */
class KNAccountPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountPanel widget.
     * \param parent The parent widget.s
     */
    explicit KNAccountPanel(QWidget *parent = 0);

signals:
    /*!
     * \brief Ask the Kreogist Account to login with specific username and
     * password.
     * \param username Username or email of Kreogist Account.
     * \param password The raw password.
     */
    void requireLogin(QString username, QString password);

    /*!
     * \brief Ask the Kreogist Account to register a new account information
     * with given parameters.
     * \param username The prefer username.
     * \param password User password.
     * \param email User preferred E-mail account.
     */
    void requireGenerate(QString username, QString password, QString email);

    /*!
     * \brief When user is asking for logout, this signal will be emit.
     */
    void requireLogout();

    /*!
     * \brief This signal is asked for notification to resize it to fit the
     * account panel.
     */
    void requireResize();

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionShowRegister();
    void onActionShowLogin();

    void onActionRegister();
    void onActionRegisterSuccess();
    void onActionRegisterFailed(int errorCode);

    void onActionLogin();
    void onActionLoginSuccess();
    void onActionLoginFailed(int errorCode);
    void onActionAutoLoginFailed(int errorCode);

    void onActionLogout();

    void onActionOperateSuccess();

    void onActionStartAutoLogin();

private:
    enum AccountPanelIndex
    {
        LoginPanel,
        RegisterPanel,
        WaitingPanel,
        DetailPanel
    };

    KNHWidgetSwitcher *m_switcher;
    //Login the panel.
    KNAccountLoginPanel *m_loginPanel;
    //Register panel.
    KNAccountRegisterPanel *m_generatePanel;
    //Wating panel.
    KNAccountWaitingPanel *m_waitingPanel;
    //Detail panel.
    KNAccountDetailPanel *m_detailPanel;
};

#endif // KNACCOUNTPANEL_H
