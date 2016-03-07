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

#include "knaccountloginpanel.h"

#define ButtonSize 36

KNAccountLoginPanel::KNAccountLoginPanel(QWidget *parent) :
    QWidget(parent),
    m_title(new QLabel(this)),
    m_subTitle(new QLabel(this)),
    m_username(new KNLabelLineEdit(this)),
    m_password(new KNLabelLineEdit(this)),
    m_login(generateButton("://public/ok.png")),
    m_others(generateButton("://public/others.png")),
    m_actionMenu(new KNSaoSubMenu(m_others))
{
    //Set properties.
    setFixedHeight(270);
    //Configure the title label.
    m_title->setAlignment(Qt::AlignCenter);
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(18);
    m_title->setFont(titleFont);
    m_subTitle->setAlignment(Qt::AlignCenter);
    m_errorPal=palette();
    m_errorPal.setColor(QPalette::WindowText, QColor(0x9a, 0x25, 0x38));
    //Configure the user name line edit.
    m_username->setMinimumLightness(0xC0);
    m_username->setMediumLightness(0xE0);
    connect(m_username, &KNLabelLineEdit::textChanged,
            this, &KNAccountLoginPanel::onActionCheckInput);
    //Configure password line edit.
    m_password->setMinimumLightness(0xC0);
    m_password->setMediumLightness(0xE0);
    m_password->setEchoMode(QLineEdit::Password);
    connect(m_password, &KNLabelLineEdit::textChanged,
            this, &KNAccountLoginPanel::onActionCheckInput);
    //Configure the button.
    m_login->hide();
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
    //Link the actions.
    connect(m_menuActions[RegisterAccount], SIGNAL(triggered(bool)),
            this, SIGNAL(requireRegister()));
    //Link the other button.
    connect(m_login, &KNOpacityAnimeButton::clicked,
            this, &KNAccountLoginPanel::requireLogin);
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
    KNImageLabel *iconLabel=new KNImageLabel(this);
    iconLabel->setFixedSize(79, 79);
    iconLabel->setPixmap(QPixmap(":/public/Kreogist.png"));
    //Add widget to layout
    mainLayout->addSpacing(5);
    mainLayout->addWidget(m_title);
    mainLayout->addSpacing(2);
    mainLayout->addWidget(m_subTitle);
    mainLayout->addSpacing(7);
    mainLayout->addWidget(iconLabel, 0, Qt::AlignHCenter);
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

void KNAccountLoginPanel::onActionLoginError(int errorCode)
{
    //Show the only reason for login error.
    m_subTitle->setText(m_errorDescription[errorCode]);
    m_subTitle->setPalette(m_errorPal);
    //Check error code.
    switch(errorCode)
    {
    case KNAccountUtil::InfoIncorrect:
        //Select all of the user name.
        m_username->selectAll();
        m_password->selectAll();
        m_username->setFocus(Qt::MouseFocusReason);
        //Check visible.
        if(!isVisible())
        {
            //When panel is hidden, push a message.
            knNotification->pushOnly(
                        tr("Kreogiat Account login failed."),
                        m_errorDescription[KNAccountUtil::InfoIncorrect]);
        }
        break;
    default:
        break;
    }
}

void KNAccountLoginPanel::retranslate()
{
    //Update title text.
    m_errorDescription[KNAccountUtil::LoginConnectionError]=
            tr("Cannot connect to server. Please check your Internet "
               "connection.");
    m_errorDescription[KNAccountUtil::InfoIncorrect]=
            tr("Username or password incorrect.");
    //Update label.
    m_title->setText(tr("Sign in"));
    m_subTitleText=tr("with Your Kreogist Account");
    m_subTitle->setText(m_subTitleText);
    //Set the place holder text.
    m_username->setPlaceholderText(tr("Username/E-mail"));
    m_password->setPlaceholderText(tr("Password"));
    //Initial the actions.
    m_menuActions[RegisterAccount]->setText(tr("Create an account"));
    m_menuActions[ForgetPassword]->setText(tr("Forget password?"));
}

void KNAccountLoginPanel::onActionForgetPassword()
{
    ;
}

void KNAccountLoginPanel::onActionCheckInput()
{
    //Reset the sub title.
    m_subTitle->setText(m_subTitleText);
    m_subTitle->setPalette(palette());
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
