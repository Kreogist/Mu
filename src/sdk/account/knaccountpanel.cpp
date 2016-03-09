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
//
#include "knnotification.h"

#include "knaccount.h"
#include "knaccountdetails.h"
//Account widget.
#include "knaccountloginpanel.h"
#include "knaccountregisterpanel.h"
#include "knaccountwaitingpanel.h"
#include "knaccountdetailpanel.h"

#include "knhwidgetswitcher.h"

#include "knaccountpanel.h"

#include <QDebug>

KNAccountPanel::KNAccountPanel(QWidget *parent) :
    QWidget(parent),
    m_switcher(new KNHWidgetSwitcher(this)),
    m_loginPanel(new KNAccountLoginPanel(this)),
    m_generatePanel(new KNAccountRegisterPanel(this)),
    m_waitingPanel(new KNAccountWaitingPanel(this)),
    m_detailPanel(new KNAccountDetailPanel(this))
{
    //Set property.
    setFixedHeight(270);
    //Configure the panel.
    m_loginPanel->setAccountDetails(knAccount->accountDetails());
    m_detailPanel->setAccountDetails(knAccount->accountDetails());

    //Link current panel to account backend.
    connect(this, &KNAccountPanel::requireLogin,
            knAccount, &KNAccount::login);
    connect(this, &KNAccountPanel::requireGenerate,
            knAccount, &KNAccount::generateAccount);
    connect(this, &KNAccountPanel::requireLogout,
            knAccount, &KNAccount::logout);
    connect(knAccount, &KNAccount::generateFailed,
            this, &KNAccountPanel::onActionRegisterFailed);
    connect(knAccount, &KNAccount::generateSuccess,
            this, &KNAccountPanel::onActionRegisterSuccess);
    connect(knAccount, &KNAccount::loginFailed,
            this, &KNAccountPanel::onActionLoginFailed);
    connect(knAccount, &KNAccount::autoLoginFailed,
            this, &KNAccountPanel::onActionAutoLoginFailed);
    connect(knAccount, &KNAccount::loginSuccess,
            this, &KNAccountPanel::onActionLoginSuccess);
    connect(knAccount, &KNAccount::startAutoLogin,
            this, &KNAccountPanel::onActionStartAutoLogin);

    connect(knAccount, &KNAccount::userInfoUpdateSuccess,
            this, &KNAccountPanel::onActionOperateSuccess);
    connect(knAccount, &KNAccount::avatarUpdatedSuccess,
            this, &KNAccountPanel::onActionOperateSuccess);
    connect(knAccount, &KNAccount::avatarUpdatedFailed,
            this, &KNAccountPanel::onActionAvatarUpdatedFailed);
    connect(knAccount, &KNAccount::userInfoUpdateFailed,
            this, &KNAccountPanel::onActionUserInfoUpdateFailed);
    connect(knAccount, &KNAccount::updateInternetError,
            this, &KNAccountPanel::onActionUpdateInternetError);
    //Link panel signal.
    connect(m_loginPanel, &KNAccountLoginPanel::requireRegister,
            this, &KNAccountPanel::onActionShowRegister);
    connect(m_loginPanel, &KNAccountLoginPanel::requireLogin,
            this, &KNAccountPanel::onActionLogin);
    connect(m_loginPanel, &KNAccountLoginPanel::requireRelogin,
            knAccount, &KNAccount::autoLogin);

    connect(m_generatePanel, &KNAccountRegisterPanel::requireRegister,
            this, &KNAccountPanel::onActionRegister);
    connect(m_generatePanel, &KNAccountRegisterPanel::cancelRegister,
            this, &KNAccountPanel::onActionShowLogin);

    connect(m_detailPanel, &KNAccountDetailPanel::requireLogout,
            this, &KNAccountPanel::onActionLogout);
    connect(m_detailPanel, &KNAccountDetailPanel::requireUpdateAvatar,
            knAccount, &KNAccount::setAvatar, Qt::QueuedConnection);
    connect(m_detailPanel, &KNAccountDetailPanel::requireUpdateInfo,
            knAccount, &KNAccount::updateAccountInfo, Qt::QueuedConnection);
    //Add widget to switcher.
    m_switcher->addWidget(m_loginPanel);
    m_switcher->addWidget(m_generatePanel);
    m_switcher->addWidget(m_waitingPanel);
    m_switcher->addWidget(m_detailPanel);
}

void KNAccountPanel::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the switcher,
    m_switcher->resize(size());
}

void KNAccountPanel::onActionShowRegister()
{
    //Switch the widget selector to register panel.
    m_switcher->setCurrentIndex(RegisterPanel);
}

void KNAccountPanel::onActionShowLogin()
{
    //Switch the widget selector to login panel.
    m_switcher->setCurrentIndex(LoginPanel);
}

void KNAccountPanel::onActionRegister()
{
    //Switch to waiting panel.
    m_switcher->setCurrentWidget(WaitingPanel);
    //Launch waiting panel animation.
    m_waitingPanel->startTicking();
    //Emit register signal.
    emit requireGenerate(m_generatePanel->username(),
                         m_generatePanel->password(),
                         m_generatePanel->email());
}

void KNAccountPanel::onActionRegisterSuccess()
{
    //Stop ticking.
    m_waitingPanel->stopTicking();
    //Show the user detail panel.
    m_switcher->setCurrentWidget(DetailPanel);
    //Clear the register panel.
    m_generatePanel->clearInputData();
    //Resize the account panel.
    setFixedHeight(120);
    //Emit resize signal.
    emit requireResize();
}

void KNAccountPanel::onActionRegisterFailed(int errorCode)
{
    //Stop ticking.
    m_waitingPanel->stopTicking();
    //Show the register panel again.
    m_switcher->setCurrentWidget(RegisterPanel);
    //Set the error code to register panel.
    m_generatePanel->onActionRegisterError(errorCode);
}

void KNAccountPanel::onActionLogin()
{
    //Switch to waiting panel.
    m_switcher->setCurrentWidget(WaitingPanel);
    //Launch waiting panel animation.
    m_waitingPanel->startTicking();
    //Emit login signal.
    emit requireLogin(m_loginPanel->username(), m_loginPanel->password());
}

void KNAccountPanel::onActionLoginSuccess()
{
    //Stop ticking.
    m_waitingPanel->stopTicking();
    //Show the user detail panel.
    m_switcher->setCurrentIndex(DetailPanel);
    //Clear the login panel.
    m_loginPanel->clearInputData();
    //Resize the account panel.
    setFixedHeight(140);
    //Emit resize signal.
    emit requireResize();
}

void KNAccountPanel::onActionLoginFailed(int errorCode)
{
    //Stop ticking.
    m_waitingPanel->stopTicking();
    //Set the error code to register panel.
    m_loginPanel->onActionLoginError(errorCode);
    //Show the login panel again.
    m_switcher->setCurrentWidget(LoginPanel);
}

void KNAccountPanel::onActionAutoLoginFailed(int errorCode)
{
    //Stop ticking.
    m_waitingPanel->stopTicking();
    //Set the error code to register panel.
    m_loginPanel->onActionAutoLoginError(errorCode);
    //Show the login panel again.
    m_switcher->setCurrentWidget(LoginPanel);
}

void KNAccountPanel::onActionLogout()
{
    //Show the login panel again.
    m_switcher->setCurrentWidget(LoginPanel);
    //Clear detail panel.
    m_detailPanel->clearUserInfo();
    //Emit logout signal.
    emit requireLogout();
    //Resize the account panel.
    setFixedHeight(270);
    //Emit resize signal.
    emit requireResize();
}

void KNAccountPanel::onActionOperateSuccess()
{
    //Update the state text.
    m_detailPanel->setStateText(KNAccountDetailPanel::OperateSuccess);
    //Check whether panel is visible.
    if(!isVisible())
    {
        //When the panel is hide, emit an notification.
        knNotification->pushOnly(
                    tr("Account Update Success"),
                    tr("The user data of %1 has been updated "
                       "successfully.").arg(
                        knAccount->accountDetails()->displayName()));
    }
}

void KNAccountPanel::onActionAvatarUpdatedFailed()
{
    //Update the state text.
    m_detailPanel->setStateText(
                KNAccountDetailPanel::FailedToUpdateAvatar);
}

void KNAccountPanel::onActionUserInfoUpdateFailed()
{
    //Update the state text.
    m_detailPanel->setStateText(
                KNAccountDetailPanel::FailedToUpdateData);
}

void KNAccountPanel::onActionUpdateInternetError()
{
    //Update the state text.
    m_detailPanel->setStateText(
                KNAccountDetailPanel::InternetConnectionError);
}

void KNAccountPanel::onActionStartAutoLogin()
{
    //Launch waiting panel animation.
    m_waitingPanel->startTicking();
    //Set current widget to looping widget.
    m_switcher->setCurrentWidget(WaitingPanel);
}
