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

#include "kngraphicsgloweffect.h"
#include "knlocalemanager.h"
#include "knlabelbutton.h"
#include "knopacityanimebutton.h"

#include "knaccountavatarbutton.h"
#include "knaccountdetails.h"
#include "knaccount.h"

#include "knaccountdetailpanel.h"

KNAccountDetailPanel::KNAccountDetailPanel(QWidget *parent) :
    QWidget(parent),
    m_avatarImage(new KNAccountAvatarButton(this)),
    m_nickName(new QLabel(this)),
    m_accountDetails(knAccount->accountDetails())
{
    //Configure the avatar image.
    m_avatarImage->setGraphicsMargin(5);
    m_avatarImage->setButtonSize(110);
    KNGraphicsGlowEffect *shadowEffect=new KNGraphicsGlowEffect(m_avatarImage);
    shadowEffect->setColor(QColor(0,0,0));
    shadowEffect->setRadius(10.0);
    m_avatarImage->setGraphicsEffect(shadowEffect);
    //Configure the nick name label.
    QFont titleFont=font();
    titleFont.setPixelSize(18);
    titleFont.setBold(true);
    m_nickName->setFont(titleFont);
    //Initial the controls.
    for(int i=0; i<ControlButtonCount; ++i)
    {
        //Generate the button.
        m_controls[i]=generateButton();
    }
    //Configure controls.
    m_controls[EditInformation]->setIcon(
                QIcon("://public/account_edit_info.png"));
    m_controls[Logout]->setIcon(QIcon("://public/account_logout.png"));
    //Link controls.
    connect(m_controls[Logout], &KNOpacityAnimeButton::clicked,
            this, &KNAccountDetailPanel::requireLogout);
    //Initial the label button palette.
    QPalette labelButtonPal=palette();
    labelButtonPal.setColor(QPalette::WindowText, QColor(0x44, 0x66, 0xbb));
    //Initial the label buttons.
    for(int i=0; i<LabelControlButtonCount; ++i)
    {
        //Generate the label button.
        m_labelButton[i]=generateLabelButton();
        //Set the label button.
        m_labelButton[i]->setPalette(labelButtonPal);
    }
    //Link detail requests.
    connect(m_accountDetails, &KNAccountDetails::accountAvatarUpdate,
            this, &KNAccountDetailPanel::onActionAvatarUpdate);
    connect(m_accountDetails, &KNAccountDetails::accountUpdate,
            this, &KNAccountDetailPanel::onActionDetailUpdate);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setSpacing(11);
    //Set layout to panel.
    setLayout(mainLayout);
    //Initial the avatar layout.
    QBoxLayout *avatarLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    mainLayout->addLayout(avatarLayout);
    //Add widget to avatar layout.
    avatarLayout->addWidget(m_avatarImage);
    avatarLayout->addStretch();
    //Initial the detail layout.
    QBoxLayout *detailLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    detailLayout->setContentsMargins(2, 4, 2, 2);
    mainLayout->addLayout(detailLayout, 1);
    //Add widget to detail layout.
    detailLayout->addWidget(m_nickName);
    //Initial the control layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    buttonLayout->setSpacing(5);
    detailLayout->addLayout(buttonLayout);
    //Add controls to button layout.
    buttonLayout->addStretch();
    for(int i=0; i<ControlButtonCount; ++i)
    {
        //Add button.
        buttonLayout->addWidget(m_controls[i]);
    }
    //Add label controls.
    for(int i=0; i<LabelControlButtonCount; ++i)
    {
        //Add label button.
        detailLayout->addWidget(m_labelButton[i]);
    }
    detailLayout->addStretch();

    //Link retranslator.
    knI18n->link(this, &KNAccountDetailPanel::retranslate);
    retranslate();
}

void KNAccountDetailPanel::clearUserInfo()
{
    //Clear label.
    m_nickName->clear();
    //Clear avatar.
    m_avatarImage->resetAccountAvatar();
}

void KNAccountDetailPanel::retranslate()
{
    m_labelButton[ChangePassword]->setText(tr("Change password"));
}

void KNAccountDetailPanel::onActionDetailUpdate()
{
    //Set account display name
    m_nickName->setText(m_accountDetails->displayName());
}

void KNAccountDetailPanel::onActionAvatarUpdate()
{
    //Get avatar from account detail.
    m_avatarImage->setAccountAvatar(m_accountDetails->accountAvatar());
}

inline KNOpacityAnimeButton *KNAccountDetailPanel::generateButton()
{
    //Generate the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(26, 26);
    //Give back the button.
    return button;
}

inline KNLabelButton *KNAccountDetailPanel::generateLabelButton()
{
    //Generate the button.
    KNLabelButton *labelButton=new KNLabelButton(this);
    //Give back the button.
    return labelButton;
}
