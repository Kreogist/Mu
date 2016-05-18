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
#include <QBoxLayout>
#include <QLabel>

#include "sao/knsaosubmenu.h"
#include "knnotification.h"
#include "knimagelabel.h"
#include "knlabellineedit.h"
#include "knlocalemanager.h"
#include "knopacityanimebutton.h"
#include "knaccountdetails.h"

#include "knaccountloginpanel.h"

#define ButtonSize 36
#define AvatarSize 79

KNAccountLoginPanel::KNAccountLoginPanel(QWidget *parent) :
    QWidget(parent),
    m_kreogistLogo(QPixmap(":/public/Kreogist.png")),
    m_anonymousLogo(QPixmap("://public/anonymous.png")),
    m_okayPal(palette()),
    m_errorPal(palette()),
    m_hintPal(palette()),
    m_loginFailedTitle(QString()),
    m_subTitleText(QString()),
    m_accountDetails(nullptr),
    m_avatarLabel(new KNImageLabel(this)),
    m_title(new QLabel(this)),
    m_subTitle(new QLabel(this)),
    m_accountDisplayName(new QLabel(this)),
    m_username(new KNLabelLineEdit(this)),
    m_password(new KNLabelLineEdit(this)),
    m_login(generateButton("://public/ok.png")),
    m_relogin(generateButton("://public/ok.png")),
    m_others(generateButton("://public/others.png")),
    m_actionMenu(new KNSaoSubMenu(m_others))
{
    //Set properties.
    setFixedHeight(270);
    //Configure the avatar logo.
    m_kreogistLogo=m_kreogistLogo.scaled(AvatarSize, AvatarSize,
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
    m_anonymousLogo=m_anonymousLogo.scaled(AvatarSize, AvatarSize,
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation);
    //Configure the title label.
    m_title->setAlignment(Qt::AlignCenter);
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(18);
    m_title->setFont(titleFont);
    m_subTitle->setAlignment(Qt::AlignCenter);
    //Configure palettes.
    m_okayPal.setColor(QPalette::WindowText, QColor(0x41, 0x92, 0x57));
    m_errorPal.setColor(QPalette::WindowText, QColor(0x9a, 0x25, 0x38));
    m_hintPal.setColor(QPalette::WindowText, QColor(157, 157, 157));
    m_subTitle->setPalette(m_hintPal);
    //Configure the user name line edit.
    m_username->setMinimumLightness(0xC0);
    m_username->setMediumLightness(0xE0);
    m_username->setLabelIcon(QPixmap("://public/generate_username.png"));
    connect(m_username, &KNLabelLineEdit::textChanged,
            this, &KNAccountLoginPanel::onActionCheckInput);
    //Configure password line edit.
    m_password->setMinimumLightness(0xC0);
    m_password->setMediumLightness(0xE0);
    m_password->setLabelIcon(QPixmap("://public/generate_password.png"));
    m_password->setEchoMode(QLineEdit::Password);
    connect(m_password, &KNLabelLineEdit::textChanged,
            this, &KNAccountLoginPanel::onActionCheckInput);
    //Configure the button.
    m_login->hide();
    m_relogin->hide();
    //Initial the action.
    for(int i=0; i<AccountActionCount; ++i)
    {
        //Initial the action.
        m_menuActions[i]=new QAction(this);
        //Add action to menu.
        m_actionMenu->addAction(m_menuActions[i]);
    }
    //Set the icon to the actions.
    m_menuActions[RegisterAccount]->setIcon(QIcon("://saomenuicons/user.png"));
    m_menuActions[ForgetPassword]->setIcon(
                QIcon("://saomenuicons/question.png"));
    m_menuActions[LoginWithAnotherAccount]->setIcon(
                QIcon("://saomenuicons/user.png"));
    //Hide login with another.
    m_menuActions[LoginWithAnotherAccount]->setVisible(false);
    //Hide account display name.
    QPalette pal=m_accountDisplayName->palette();
    pal.setColor(QPalette::WindowText, QColor(157, 157, 157));
    m_accountDisplayName->setPalette(pal);
    m_accountDisplayName->setAlignment(Qt::AlignCenter);
    m_accountDisplayName->hide();
    //Link the actions.
    connect(m_menuActions[RegisterAccount], SIGNAL(triggered(bool)),
            this, SIGNAL(requireRegister()));
    connect(m_menuActions[ForgetPassword], SIGNAL(triggered(bool)),
            this, SIGNAL(requireResetPassword()));
    connect(m_menuActions[LoginWithAnotherAccount], SIGNAL(triggered(bool)),
            this, SLOT(onActionLoginWithAnotherAccount()));
    //Link the other button.
    connect(m_login, &KNOpacityAnimeButton::clicked,
            this, &KNAccountLoginPanel::requireLogin);
    connect(m_relogin, &KNOpacityAnimeButton::clicked,
            this, &KNAccountLoginPanel::requireRelogin);
    connect(m_others, &KNOpacityAnimeButton::clicked,
            [=]
            {
                //Show the action menu.
                m_actionMenu->exec();
            });

    //Initial the box layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mainLayout);
    //Initial the label.
    m_avatarLabel->setFixedSize(AvatarSize, AvatarSize);
    m_avatarLabel->setPixmap(m_kreogistLogo);
    //Add widget to layout
    mainLayout->addSpacing(5);
    mainLayout->addWidget(m_title);
    mainLayout->addSpacing(2);
    mainLayout->addWidget(m_subTitle);
    mainLayout->addSpacing(7);
    mainLayout->addStretch();
    mainLayout->addWidget(m_avatarLabel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_accountDisplayName, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addSpacing(7);
    mainLayout->addWidget(m_username);
    mainLayout->addWidget(m_password);
    //Initial button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    mainLayout->addSpacing(8);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    //Add all buttons.
    buttonLayout->addWidget(m_relogin);
    buttonLayout->addWidget(m_login);
    buttonLayout->addWidget(m_others);

    //Link the retranslator.
    knI18n->link(this, &KNAccountLoginPanel::retranslate);
    retranslate();
}

QString KNAccountLoginPanel::username() const
{
    return m_username->text();
}

QString KNAccountLoginPanel::password() const
{
    return m_password->text();
}

void KNAccountLoginPanel::clearInputData()
{
    m_username->clear();
    m_password->clear();
}

void KNAccountLoginPanel::onActionResetSuccess()
{
    //Update the hint of the panel to reset success hint.
    m_subTitle->setText(m_resetPasswordComplete);
    m_subTitle->setPalette(m_okayPal);
}

void KNAccountLoginPanel::onActionLoginError(int errorCode)
{
    //Show the only reason for login error.
    m_subTitle->setText(m_errorDescription[errorCode]);
    m_subTitle->setPalette(m_errorPal);
    //Check error code.
    switch(errorCode)
    {
    case LoginConnectionError:
        //Set focus to password.
        m_password->setFocus(Qt::MouseFocusReason);
        //Check visible.
        if(parentWidget() && !parentWidget()->isVisible())
        {
            //When panel is hidden, push a message.
            knNotification->pushOnly(
                        m_loginFailedTitle,
                        m_errorDescription[
                        LoginConnectionError]);
        }
        break;
    case InfoIncorrect:
        //Set focus to user name.
        m_username->setFocus(Qt::MouseFocusReason);
        //Check visible.
        if(parentWidget() && !parentWidget()->isVisible())
        {
            //When panel is hidden, push a message.
            knNotification->pushOnly(
                        m_loginFailedTitle,
                        m_errorDescription[InfoIncorrect]);
        }
        break;
    default:
        break;
    }
}

void KNAccountLoginPanel::onActionAutoLoginError(int errorCode)
{
    //Check error code.
    switch(errorCode)
    {
    case LoginConnectionError:
        //Show relogin button.
        m_relogin->show();
        //Show the only reason for login error.
        m_subTitle->setText(m_errorDescription[errorCode]);
        m_subTitle->setPalette(m_errorPal);
        //Hide the username and password.
        m_username->hide();
        m_password->hide();
        //Check out the detail is nullptr, set the avatar.
        m_avatarLabel->setPixmap(m_accountDetails==nullptr?
                                     m_anonymousLogo:
                                     m_accountDetails->accountAvatar());
        //Show the display name.
        m_accountDisplayName->show();
        m_accountDisplayName->setText(m_accountDetails==nullptr?
                                          "null":
                                          m_accountDetails->displayName());
        //Show login with another account action.
        m_menuActions[LoginWithAnotherAccount]->setVisible(true);
        //Check visible.
        if(parentWidget() && !parentWidget()->isVisible())
        {
            //When panel is hidden, push a message.
            knNotification->pushOnly(
                        m_loginFailedTitle,
                        m_errorDescription[LoginConnectionError]);
        }
        break;
    case InfoIncorrect:
        onActionLoginError(errorCode);
        break;
    default:
        break;
    }
}

void KNAccountLoginPanel::retranslate()
{
    //Update title text.
    m_errorDescription[LoginConnectionError]=
            tr("Connection Error. Please check Internet connection.");
    m_errorDescription[InfoIncorrect]=
            tr("Username or password incorrect.");
    m_loginFailedTitle=tr("Kreogist Account login failed.");
    m_resetPasswordComplete=tr("Check your email for a link to reset your "
                               "password.");
    //Update label.
    m_title->setText(tr("Sign in"));
    m_subTitleText=tr("with Your Kreogist Account");
    m_subTitle->setText(m_subTitleText);
    m_subTitle->setPalette(m_hintPal);
    //Set the place holder text.
    m_username->setPlaceholderText(tr("Username/E-mail"));
    m_password->setPlaceholderText(tr("Password"));
    //Initial the actions.
    m_menuActions[RegisterAccount]->setText(tr("Create an account"));
    m_menuActions[ForgetPassword]->setText(tr("Forget password?"));
    m_menuActions[LoginWithAnotherAccount]->setText(
                tr("Login with another account"));
}

void KNAccountLoginPanel::onActionForgetPassword()
{
    ;
}

void KNAccountLoginPanel::onActionLoginWithAnotherAccount()
{
    //Reset the account details.
    m_accountDetails->resetAccountDetail();
    //Hide the relogin button.
    m_relogin->hide();
    //Clear the username and password box.
    m_username->clear();
    m_password->clear();
    //Show the username and password.
    m_username->show();
    m_password->show();
    //Reset the sub title.
    m_subTitle->setText(m_subTitleText);
    m_subTitle->setPalette(m_hintPal);
    //Reset the avatar image.
    m_avatarLabel->setPixmap(m_kreogistLogo);
    //Hide login with other action.
    m_menuActions[LoginWithAnotherAccount]->setVisible(false);
    //Hide account display name widget.
    m_accountDisplayName->hide();
    //Set focus on username.
    m_username->setFocus();
}

void KNAccountLoginPanel::onActionCheckInput()
{
    //Reset the sub title.
    m_subTitle->setText(m_subTitleText);
    m_subTitle->setPalette(m_hintPal);
    //Reset the login visible.
    m_login->setVisible(!(m_username->text().isEmpty() ||
                          m_password->text().isEmpty()));
}

inline KNOpacityAnimeButton *KNAccountLoginPanel::generateButton(
        const QString &iconPath)
{
    //Initial the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    button->setIcon(QIcon(iconPath));
    button->setFixedSize(ButtonSize, ButtonSize);
    //Give back the button.
    return button;
}

void KNAccountLoginPanel::setAccountDetails(KNAccountDetails *accountDetails)
{
    m_accountDetails = accountDetails;
}
