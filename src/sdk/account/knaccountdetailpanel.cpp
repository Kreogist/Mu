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
#include <QFileDialog>
#include <QFileInfo>
#include <QTimeLine>
#include <QTimer>

#include "knglobal.h"
#include "kndpimanager.h"
#include "kngraphicsgloweffect.h"
#include "knlocalemanager.h"
#include "knlabelbutton.h"
#include "knwaitingwheel.h"
#include "knopacityanimebutton.h"
#include "knnotification.h"

#include "knaccountdetailbox.h"
#include "knaccountpasswordbox.h"
#include "knaccountavatarbutton.h"
#include "knaccountdetails.h"

#include "knaccountdetailpanel.h"

#include <QDebug>

KNAccountDetailPanel::KNAccountDetailPanel(QWidget *parent) :
    QWidget(parent),
    m_failedColor(QColor(0x9c, 0x22, 0x39)),
    m_successColor(QColor(0x41, 0x92, 0x57)),
    m_lastDirectory(QString()),
    m_detailModify(new KNAccountDetailBox(knGlobal->mainWindow())),
    m_passwordModify(new KNAccountPasswordBox(knGlobal->mainWindow())),
    m_avatarImage(new KNAccountAvatarButton(this)),
    m_nickName(new QLabel(this)),
    m_username(new QLabel(this)),
    m_state(new QLabel(this)),
    m_waitingWheel(new KNWaitingWheel(this)),
    m_accountDetails(nullptr),
    m_fadeInAnime(new QTimeLine(200, this)),
    m_fadeOutAnime(new QTimeLine(200, this)),
    m_stateFadeTimer(new QTimer(this))
{
    //Configure the avatar image.
    int graphicsSize=knDpi->height(110), marginSize=knDpi->height(5);
    m_avatarImage->setGraphicsMargin(marginSize);
    m_avatarImage->setButtonSize(graphicsSize);
    setMinimumHeight(graphicsSize+(marginSize<<1)+(knDpi->width(10)<<1));
    KNGraphicsGlowEffect *shadowEffect=new KNGraphicsGlowEffect(m_avatarImage);
    shadowEffect->setColor(QColor(0,0,0));
    shadowEffect->setRadius(knDpi->width(10));
    m_avatarImage->setGraphicsEffect(shadowEffect);
    //Configure the nick name label.
    QFont titleFont=font();
    titleFont.setPixelSize(knDpi->height(18));
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
    m_controls[ChangePassword]->setIcon(
                QIcon("://public/account_password.png"));
    m_controls[SetAvatar]->setIcon(
                QIcon("://public/account_avatar.png"));
    m_controls[Logout]->setIcon(QIcon("://public/account_logout.png"));
    //Link controls.
    connect(m_controls[EditInformation], &KNOpacityAnimeButton::clicked,
            this, &KNAccountDetailPanel::onActionEditInformation);
    connect(m_controls[ChangePassword], &KNOpacityAnimeButton::clicked,
            this, &KNAccountDetailPanel::onActionChangePassword);
    connect(m_controls[SetAvatar], &KNOpacityAnimeButton::clicked,
            this, &KNAccountDetailPanel::onActionSelectAvatar);
    connect(m_controls[Logout], &KNOpacityAnimeButton::clicked,
            this, &KNAccountDetailPanel::requireLogout);
    //Link password editor request.
    connect(m_passwordModify, &KNAccountPasswordBox::requireUpdatePassword,
            this, &KNAccountDetailPanel::onActionAskChangePassword);
    //Link detail updater.
    connect(m_detailModify, &KNAccountDetailBox::requireUpdateInfo,
            this, &KNAccountDetailPanel::onActionAskUpdateInfo);
    //Update state property.
    QPalette pal=m_state->palette();
    pal.setColor(QPalette::WindowText, QColor(0,0,0,0));
    m_state->setPalette(pal);
    m_state->setFixedHeight(fontMetrics().height()<<1);
    m_state->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_state->setWordWrap(true);
    //Set the wating wheel.
    m_waitingWheel->hide();
    //Update user name palette.
    pal.setColor(QPalette::WindowText, QColor(0xA0, 0xA0, 0xA0));
    m_username->setPalette(pal);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setSpacing(knDpi->height(11));
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
    detailLayout->setContentsMargins(knDpi->margins(2, 4, 2, 2));
    detailLayout->setSpacing(knDpi->height(5));
    mainLayout->addLayout(detailLayout, 1);
    //Initial the layout for state.
    QBoxLayout *titleLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           detailLayout->widget());
    //Add state layout to detail layout.
    detailLayout->addLayout(titleLayout);
    //Add widget to state layout.
    titleLayout->addWidget(m_nickName, 1);
    titleLayout->addWidget(m_waitingWheel);
    //Add widget to detail layout.
    detailLayout->addWidget(m_username);
    detailLayout->addWidget(m_state);
    //Initial the control layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    buttonLayout->setSpacing(knDpi->width(5));
    detailLayout->addLayout(buttonLayout);
    //Add controls to button layout.
    for(int i=0; i<ControlButtonCount; ++i)
    {
        //Add button.
        buttonLayout->addWidget(m_controls[i]);
    }
    buttonLayout->addStretch();
    detailLayout->addStretch();

    //Configure the time line.
    m_fadeInAnime->setUpdateInterval(33);
    m_fadeInAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_fadeInAnime->setEndFrame(255);
    connect(m_fadeInAnime, &QTimeLine::frameChanged,
            this, &KNAccountDetailPanel::onActionChangeStateAlpha);
    connect(m_fadeInAnime, &QTimeLine::finished,
            m_stateFadeTimer,
            static_cast<void (QTimer::*)()>(&QTimer::start));
    m_fadeOutAnime->setUpdateInterval(33);
    m_fadeOutAnime->setEndFrame(0);
    m_fadeOutAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_fadeOutAnime, &QTimeLine::frameChanged,
            this, &KNAccountDetailPanel::onActionChangeStateAlpha);
    //Configure the timer.
    m_stateFadeTimer->setSingleShot(true);
    m_stateFadeTimer->setInterval(2000);
    connect(m_stateFadeTimer, &QTimer::timeout,
            this, &KNAccountDetailPanel::startFadeOut);

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

void KNAccountDetailPanel::setStateText(int state)
{
    //Stop the counting timer.
    m_stateFadeTimer->stop();
    //Enabled controls.
    enabledAllControls();
    //Set the lable text.
    m_state->setText(m_stateText[state]);
    //Set the state color.
    //Check whether the state is success.
    QPalette pal=m_state->palette();
    //Get alpha data.
    int textAlpha=pal.color(QPalette::WindowText).alpha();
    m_successColor.setAlpha(textAlpha);
    m_failedColor.setAlpha(textAlpha);
    pal.setColor(QPalette::WindowText,
                 (state==OperateSuccess)?m_successColor:m_failedColor);
    m_state->setPalette(pal);
    //Start animation.
    m_fadeInAnime->setStartFrame(textAlpha);
    m_fadeInAnime->start();
    //Check whether the widget is visible.
    if(!isVisible())
    {
        //For failed information.
        if(state!=OperateSuccess)
        {
            //Push notification. Check the result.
            knNotification->pushOnly(tr("Account Update Failed."),
                                     m_stateText[state]);
        }

    }
}

void KNAccountDetailPanel::retranslate()
{
    //Update the state text.
    m_stateText[OperateSuccess]=
            tr("Account update success.");
    m_stateText[InternetConnectionError]=
            tr("Cannot connect to server. Please check your Internet "
               "connection.");
    m_stateText[FailedToUpdateData]=
            tr("Failed to update account info. Try again.");
    m_stateText[FailedToUpdateAvatar]=
            tr("Failed to update new avatar. Try again.");
}

void KNAccountDetailPanel::onActionDetailUpdate()
{
    //Check out the account details.
    if(m_accountDetails==nullptr)
    {
        //Doesn't set account details before.
        return;
    }
    //Set account display name
    m_nickName->setText(m_accountDetails->displayName());
    m_username->setText(m_accountDetails->cacheUserName());
}

void KNAccountDetailPanel::onActionAvatarUpdate()
{
    //Check out the account details.
    if(m_accountDetails==nullptr)
    {
        //Doesn't set account details before.
        return;
    }
    //Get avatar from account detail.
    m_avatarImage->setAccountAvatar(m_accountDetails->accountAvatar());
}

void KNAccountDetailPanel::onActionChangeStateAlpha(int frame)
{
    //Update the state label palette alpha.
    QPalette pal=m_state->palette();
    //Change window text color alpha.
    QColor textColor=pal.color(QPalette::WindowText);
    textColor.setAlpha(frame);
    pal.setColor(QPalette::WindowText, textColor);
    //Set the palette.
    m_state->setPalette(pal);
}

void KNAccountDetailPanel::startFadeOut()
{
    //Set the fade out parameter.
    m_fadeOutAnime->setStartFrame(
                m_state->palette().color(QPalette::WindowText).alpha());
    //Start time line.
    m_fadeOutAnime->start();
}

void KNAccountDetailPanel::onActionEditInformation()
{
    //Check out the account details.
    if(m_accountDetails==nullptr)
    {
        //Doesn't set account details before.
        return;
    }
    //Set the account detail.
    m_detailModify->setAccountDetails(m_accountDetails);
    //Show the detail edit message box.
    m_detailModify->exec();
}

void KNAccountDetailPanel::onActionChangePassword()
{
    //Show the password edit message box.
    m_passwordModify->exec();
}

void KNAccountDetailPanel::onActionSelectAvatar()
{
    //Select an avatar from hard drive.
    QString targetFilePath=QFileDialog::getOpenFileName(
                this,
                tr("Select avatar file"),
                m_lastDirectory,
                knGlobal->readImageFilter());
    //Check out the target file path.
    if(targetFilePath.isEmpty())
    {
        //Ignore the invalid file path.
        return;
    }
    //Get the pixmap information.
    QImage &&scaledAvatar=
            QImage(targetFilePath).scaled(
                100, 100,
                Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation);
    scaledAvatar=scaledAvatar.copy(QRect((scaledAvatar.width()-100)>>1,
                                         (scaledAvatar.height()-100)>>1,
                                         100,
                                         100));
    //Save the image to the target file path.
    emit requireUpdateAvatar(QPixmap::fromImage(scaledAvatar));
    //Save the last dirctory.
    m_lastDirectory=QFileInfo(targetFilePath).absoluteFilePath();
    //Disable all the controls.
    disableAllControls();
}

void KNAccountDetailPanel::onActionAskChangePassword(
        const QString &encryptedPassword)
{
    //Generate setter request.
    QJsonObject passwordSetter;
    //Set the password.
    passwordSetter.insert("password", encryptedPassword);
    //Ask to update password.
    emit requireUpdateInfo(passwordSetter);
    //Disable all the controls.
    disableAllControls();
}

void KNAccountDetailPanel::onActionAskUpdateInfo(const QJsonObject &userInfo)
{
    //Ask to update info.
    emit requireUpdateInfo(userInfo);
    //Disable all the controls.
    disableAllControls();
}

inline KNOpacityAnimeButton *KNAccountDetailPanel::generateButton()
{
    //Generate the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    button->setCursor(Qt::PointingHandCursor);
    button->setFixedSize(knDpi->size(26, 26));
    //Give back the button.
    return button;
}

inline void KNAccountDetailPanel::disableAllControls()
{
    //For all the button.
    for(int i=0; i<ControlButtonCount; ++i)
    {
        //Disable all the button.
        m_controls[i]->setEnabled(false);
    }
    //Show the wheel.
    m_waitingWheel->show();
    m_waitingWheel->startTick();
}

inline void KNAccountDetailPanel::enabledAllControls()
{
    //For all the button.
    for(int i=0; i<ControlButtonCount; ++i)
    {
        //Disable all the button.
        m_controls[i]->setEnabled(true);
    }
    //Stop and hide the wheel.
    m_waitingWheel->hide();
    m_waitingWheel->stopTick();
}

void KNAccountDetailPanel::setAccountDetails(KNAccountDetails *accountDetails)
{
    //Save the account details.
    m_accountDetails = accountDetails;
    //Check pointer.
    if(m_accountDetails)
    {
        //Link detail requests.
        connect(m_accountDetails, &KNAccountDetails::accountAvatarUpdate,
                this, &KNAccountDetailPanel::onActionAvatarUpdate);
        connect(m_accountDetails, &KNAccountDetails::accountUpdate,
                this, &KNAccountDetailPanel::onActionDetailUpdate);
    }
}
