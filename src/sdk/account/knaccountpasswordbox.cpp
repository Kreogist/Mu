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

#include "knhwidgetswitcher.h"
#include "knlabellineedit.h"
#include "knlocalemanager.h"

#include "knaccount.h"
#include "knaccountdetails.h"
#include "knaccountpasswordverification.h"
#include "knaccountpasswordbox.h"

#include <QDebug>

KNAccountPasswordBox::KNAccountPasswordBox(QWidget *parent) :
    KNMessageBox(parent),
    m_hintText(QString()),
    m_validColor(QColor(0x41, 0x92, 0x57)),
    m_invalidColor(QColor(0x9a, 0x25, 0x38)),
    m_emptyColor(QColor(0x9D, 0x9D, 0x9D)),
    m_resetTitle(new QLabel(this)),
    m_resetHint(new QLabel(this)),
    m_passwordContainer(new KNHWidgetSwitcher(this)),
    m_passwordVerification(new KNAccountPasswordVerification(this)),
    m_password1(generatePassword()),
    m_password2(generatePassword()),
    m_validPassword(false)
{
    //Set property.
    setCancelButtonVisible(true);
    setOkayButtonVisible(false);
    setTitleText("Password");
    //Configure the password container.
    m_passwordContainer->setFixedSize(342, 200);
    //Configure the verification widget.
    m_passwordVerification->setContentsMargins(5, 0, 5, 0);
    connect(m_passwordVerification,
            &KNAccountPasswordVerification::passwordCorrect,
            this, &KNAccountPasswordBox::onActionAcceptPassword);
    //Generate the label palette.
    QPalette labelPalette=palette();
    labelPalette.setColor(QPalette::WindowText, m_emptyColor);
    //Initial the hint label.
    for(int i=0; i<PasswordHintTypeCount; ++i)
    {
        //Initial the password hint.
        m_passwordHint[i]=new QLabel(this);
        //Set the palette.
        m_passwordHint[i]->setPalette(labelPalette);
    }
    //Configure title label.
    QFont titleFont=m_resetTitle->font();
    titleFont.setPixelSize((titleFont.pixelSize()>>2)+titleFont.pixelSize());
    titleFont.setBold(true);
    m_resetTitle->setFont(titleFont);
    m_resetTitle->setWordWrap(true);
    //Configure hint label.
    m_resetHint->setWordWrap(true);
    //Configure line edit.
    connect(m_password1, &KNLabelLineEdit::textChanged,
            this, &KNAccountPasswordBox::onActionPassword1Change);
    connect(m_password2, &KNLabelLineEdit::textChanged,
            this, &KNAccountPasswordBox::checkOkayButton);

    //Add verification widget to container.
    m_passwordContainer->addWidget(m_passwordVerification);
    //Initial the new password container.
    QWidget *changeContainer=new QWidget(this);
    //Add to password container.
    m_passwordContainer->addWidget(changeContainer);
    //Initial the layout.
    QBoxLayout *changeLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            changeContainer);
    //Configure and set layout.
    changeLayout->setContentsMargins(20, 10, 20, 0);
    changeLayout->setSpacing(0);
    changeContainer->setLayout(changeLayout);
    //Add widget to main layout.
    changeLayout->addWidget(m_resetTitle);
    changeLayout->addWidget(m_resetHint);
    changeLayout->addStretch();
    changeLayout->addWidget(m_password1);
    changeLayout->addSpacing(6);
    changeLayout->addWidget(m_password2);
    changeLayout->addSpacing(12);
    //Initial the label.
    for(int i=0; i<PasswordHintTypeCount; ++i)
    {
        //Add widget to main layout.
        changeLayout->addWidget(m_passwordHint[i]);
        //Add spacing between hints.
        changeLayout->addSpacing(2);
    }
    changeLayout->addStretch();
    //Set the content widget.
    setContentWidget(m_passwordContainer);

    //Add password box to retranslate list.
    knI18n->link(this, &KNAccountPasswordBox::retranslate);
    retranslate();
}

void KNAccountPasswordBox::showEvent(QShowEvent *event)
{
    //Show the message box.
    KNMessageBox::showEvent(event);
    //Reset the current index.
    m_passwordContainer->setCurrentWidget(0);
    //Update reset hint text.
    m_resetHint->setText(m_hintText.arg(
                             knAccount->accountDetails()->displayName()));
    //Clear the password box.
    m_password1->clear();
    m_password2->clear();
    //Reset valid password.
    m_validPassword=false;
}

bool KNAccountPasswordBox::okayPressed()
{
    //Emit the change signal.
    emit requireUpdatePassword(KNAccount::accessPassword(m_password1->text()));
    //Complete.
    return true;
}

void KNAccountPasswordBox::retranslate()
{
    //Set the title and hint text.
    m_resetTitle->setText(tr("Set new password"));
    m_hintText=tr("Input your new password for %1.");
    m_resetHint->setText(m_hintText.arg(
                             knAccount->accountDetails()->displayName()));
    //Reset the password1 and password2 place holder text.
    m_password1->setPlaceholderText(tr("New Password"));
    m_password2->setPlaceholderText(tr("Input New Password Again"));
    //Set password hint.
    m_passwordHint[LengthRequest]->setText(
                tr("Password must be at least 6 characters."));
    m_passwordHint[NumberRequest]->setText(
                tr("Password must include a number."));
    m_passwordHint[LetterRequest]->setText(
                tr("Password must include an upper and lower case letter."));
}

void KNAccountPasswordBox::onActionAcceptPassword()
{
    //Move to new password set interface.
    m_passwordContainer->setCurrentIndex(1);
}

void KNAccountPasswordBox::onActionPassword1Change(const QString &password)
{
    //Check whether the password is empty.
    if(password.isEmpty())
    {
        //Of course failed.
        m_validPassword=false;
        //Get the palette.
        QPalette pal=m_passwordHint[0]->palette();
        pal.setColor(QPalette::WindowText, m_emptyColor);
        //Set the result to label.
        for(int i=0; i<PasswordHintTypeCount; ++i)
        {
            //Set the palette.
            m_passwordHint[i]->setPalette(pal);
        }
        //Check okay button.
        checkOkayButton();
        return;
    }
    //Check password validation.
    bool passwordValidation[PasswordHintTypeCount];
    //Get the validation result, save the result.
    m_validPassword=
            KNAccountUtil::isPasswordValid(password, passwordValidation);
    //Get the palette.
    QPalette pal=m_passwordHint[0]->palette();
    //Set the result to label.
    for(int i=0; i<PasswordHintTypeCount; ++i)
    {
        //Configure the pal.
        pal.setColor(QPalette::WindowText,
                     passwordValidation[i] ? m_validColor : m_invalidColor);
        //Set the palette.
        m_passwordHint[i]->setPalette(pal);
    }
    //Check okay button.
    checkOkayButton();
}

inline KNLabelLineEdit *KNAccountPasswordBox::generatePassword()
{
    //Generate line edit.
    KNLabelLineEdit *passwordEdit=new KNLabelLineEdit(this);
    //Set properties.
    passwordEdit->setEchoMode(QLineEdit::Password);
    //Configure the password line edit.
    passwordEdit->setMinimumLightness(0xC0);
    passwordEdit->setMediumLightness(0xE0);
    //Give back the pointer.
    return passwordEdit;
}

void KNAccountPasswordBox::checkOkayButton()
{
    //Check total validation and the same.
    //Show the okay button.
    setButtonVisible(true,
                     m_validPassword &&
                     (m_password1->text()==m_password2->text()));
}
