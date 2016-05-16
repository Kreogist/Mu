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
#include <QCheckBox>

#include "knlabellineedit.h"
#include "knlocalemanager.h"
#include "knopacityanimebutton.h"

#include "knaccountregisterpanel.h"

#include <QDebug>
#define ButtonSize 36

KNAccountRegisterPanel::KNAccountRegisterPanel(QWidget *parent) :
    QWidget(parent),
    m_validColor(QColor(0x41, 0x92, 0x57)),
    m_invalidColor(QColor(0x9a, 0x25, 0x38)),
    m_emptyColor(QColor(0x9D, 0x9D, 0x9D)),
    m_title(new QLabel(this)),
    m_emailHint(new QLabel(this)),
    m_errorHint(new QLabel(this)),
    m_agreeLicense(new QCheckBox(this)),
    m_username(new KNLabelLineEdit(this)),
    m_password(new KNLabelLineEdit(this)),
    m_email(new KNLabelLineEdit(this)),
    m_okay(generateButton("://public/ok.png")),
    m_cancel(generateButton("://public/cancel.png")),
    m_errorCode(-1)
{
    //Set properties.
    setFixedHeight(270);
    //Configure the title label.
    m_title->setAlignment(Qt::AlignCenter);
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(18);
    m_title->setFont(titleFont);
    //Configure the user name line edit.
    m_username->setMinimumLightness(0xC0);
    m_username->setMediumLightness(0xE0);
    m_username->setLabelIcon(QPixmap("://public/generate_username.png"));
    connect(m_username, &KNLabelLineEdit::textChanged,
            this, &KNAccountRegisterPanel::onActionCheckValid);
    //Configure password line edit.
    m_password->setMinimumLightness(0xC0);
    m_password->setMediumLightness(0xE0);
    m_password->setLabelIcon(QPixmap("://public/generate_password.png"));
    m_password->setEchoMode(QLineEdit::Password);
    connect(m_password, &KNLabelLineEdit::textChanged,
            this, &KNAccountRegisterPanel::onActionCheckValid);
    //Configure the E-mail.
    m_email->setMinimumLightness(0xC0);
    m_email->setMediumLightness(0xE0);
    m_email->setLabelIcon(QPixmap("://public/generate_email.png"));
    connect(m_email, &KNLabelLineEdit::textChanged,
            this, &KNAccountRegisterPanel::onActionCheckValid);
    //Configure the check box.
    connect(m_agreeLicense, &QCheckBox::stateChanged,
            this, &KNAccountRegisterPanel::onActionCheckValid);
    //Configure the hint label.
    QPalette hintPal=palette();
    hintPal.setColor(QPalette::WindowText, QColor(157, 157, 157));
    // E-mail Hint.
    m_emailHint->setPalette(hintPal);
    m_emailHint->setContentsMargins(5, 0, 0, 0);
    // Error Hint.
    m_errorHint->setAlignment(Qt::AlignCenter);
    // Error Hint.
    QPalette errorPal=m_errorHint->palette();
    errorPal.setColor(QPalette::WindowText, QColor(0x9a, 0x25, 0x38));
    m_errorHint->setPalette(errorPal);
    //Okay buttons.
    m_okay->hide();
    //Connect buttons.
    connect(m_okay, SIGNAL(clicked(bool)), this, SIGNAL(requireRegister()));
    connect(m_cancel, SIGNAL(clicked(bool)), this, SLOT(onActionCancel()));

    //Initial the box layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add widget to layout
    mainLayout->addSpacing(7);
    mainLayout->addWidget(m_title);
    mainLayout->addSpacing(3);
    mainLayout->addWidget(m_errorHint);
    mainLayout->addSpacing(3);
    mainLayout->addWidget(m_username);
    mainLayout->addSpacing(3);
    mainLayout->addWidget(m_password);
    mainLayout->addSpacing(2);
    //Generate the label palette.
    QPalette labelPalette=palette();
    labelPalette.setColor(QPalette::WindowText, m_emptyColor);
    //Initial the hint label.
    for(int i=0; i<PasswordHintTypeCount; ++i)
    {
        //Initial the password hint.
        m_passwordHint[i]=new QLabel(this);
        //Set porperties.
        m_passwordHint[i]->setContentsMargins(5, 0, 0, 0);
        m_passwordHint[i]->setWordWrap(true);
        //Set the palette.
        m_passwordHint[i]->setPalette(labelPalette);
        //Add hint to main layout.
        mainLayout->addWidget(m_passwordHint[i]);
    }
    mainLayout->addSpacing(5);
    mainLayout->addWidget(m_email);
    mainLayout->addSpacing(2);
    mainLayout->addWidget(m_emailHint);
    mainLayout->addSpacing(3);
    mainLayout->addWidget(m_agreeLicense, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(1);

    //Initial button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    buttonLayout->setContentsMargins(1,3,1,3);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    //Add all buttons.
    buttonLayout->addWidget(m_okay);
    buttonLayout->addWidget(m_cancel);

    //Link the retranslator.
    knI18n->link(this, &KNAccountRegisterPanel::retranslate);
    retranslate();
}

QString KNAccountRegisterPanel::username() const
{
    return m_username->text();
}

QString KNAccountRegisterPanel::password() const
{
    return m_password->text();
}

QString KNAccountRegisterPanel::email() const
{
    return m_email->text();
}

void KNAccountRegisterPanel::retranslate()
{
    //Update label.
    m_title->setText(tr("Create Kreogist Account"));
    m_emailHint->setText(tr("Your E-mail will be verified."));
    //Update the password label.
    m_passwordHint[LengthRequest]->setText(
                tr("Password must be at least 6 characters."));
    m_passwordHint[NumberRequest]->setText(
                tr("Password must include a number."));
    m_passwordHint[LetterRequest]->setText(
                tr("Password must include an upper and lower case letter."));
    //Set the text edit.
    m_username->setPlaceholderText(tr("Username"));
    m_password->setPlaceholderText(tr("Password"));
    m_email->setPlaceholderText(tr("E-mail"));
    //Set the check box.
    m_agreeLicense->setText(tr("I agree to the Terms of Service."));
    //Set error description.
    m_errorDescription[UnknownRegisterError]=
            tr("Unknown error. Please check your Internet connection.");
    m_errorDescription[UserNameAlreadyTaken]=
            tr("Username is already taken, try another one.");
    m_errorDescription[EmailAlreadyTaken]=
            tr("E-mail is already taken, use another E-mail address.");
    //Check error code.
    if(m_errorCode!=-1)
    {
        //Update error description.
        m_errorHint->setText(m_errorDescription[m_errorCode]);
    }
}

void KNAccountRegisterPanel::onActionCancel()
{
    //Clear the user data.
    clearInputData();
    //Emit signal.
    emit cancelRegister();
}

void KNAccountRegisterPanel::onActionCheckValid()
{
    //Check the validation and set the okay button to be the visible.
    m_okay->setVisible(isInformationValid());
}

void KNAccountRegisterPanel::clearInputData()
{
    //Clear all the text box.
    m_username->clear();
    m_password->clear();
    m_email->clear();
    //Clear the check box.
    m_agreeLicense->setChecked(false);
    //Clear the error information.
    m_errorHint->clear();
    m_errorCode=-1;
}

void KNAccountRegisterPanel::onActionRegisterError(int errorCode)
{
    //Save error code.
    m_errorCode=errorCode;
    //Set the error hint text.
    m_errorHint->setText(m_errorDescription[m_errorCode]);
    //Check error code.
    switch(m_errorCode)
    {
    case UserNameAlreadyTaken:
        //Set focus on user name line edit.
        m_username->selectAll();
        m_username->setFocus(Qt::MouseFocusReason);
        break;
    case EmailAlreadyTaken:
        //Set focus on E-mail.
        m_email->selectAll();
        m_email->setFocus(Qt::MouseFocusReason);
        break;
    default:
        break;
    }
}

inline bool KNAccountRegisterPanel::isInformationValid()
{
    //Get the password.
    QString &&password=m_password->text();
    //Check whether the password is empty.
    if(password.isEmpty())
    {
        //Of course we couldn't accept it.
        //Get the palette.
        QPalette pal=m_passwordHint[0]->palette();
        pal.setColor(QPalette::WindowText, m_emptyColor);
        //Set the result to label.
        for(int i=0; i<PasswordHintTypeCount; ++i)
        {
            //Set the palette.
            m_passwordHint[i]->setPalette(pal);
        }
        //Failed to use.
        return false;
    }
    //Check password validation.
    bool passwordValidation[PasswordHintTypeCount],
        //Get the password validation check result.
        validPassword=KNAccountUtil::isPasswordValid(password,
                                                     passwordValidation);
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
    //Check password validation.
    if(!validPassword)
    {
        //Failed to pass to validation check.
        return false;
    }

    //Check E-mail first, because it's complex.
    if(m_email->text().isEmpty())
    {
        //No E-mail address, failed.
        return false;
    }
    {
        //Find first '@'
        int firstAt=m_email->text().indexOf('@');
        //There should be only one @.
        if(firstAt!=m_email->text().lastIndexOf('@') || firstAt==-1)
        {
            //More than one @.
            return false;
        }
        //Check whether it contains . after @.
        int lastDot=m_email->text().lastIndexOf('.');
        if(lastDot < firstAt ||
                //There's nothing after the dot.
                lastDot==m_email->text().length()-1 ||
                //There's nothing between the dot.
                lastDot==firstAt+1)
        {
            //Failed when there's no dot after @.
            return false;
        }
    }
            //User Name cannot be empty.
    return (!m_username->text().isEmpty()) &&
            //User agree the license.
            m_agreeLicense->isChecked();
}

inline KNOpacityAnimeButton *KNAccountRegisterPanel::generateButton(
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
