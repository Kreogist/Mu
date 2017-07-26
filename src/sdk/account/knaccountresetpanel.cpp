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
#include <QLabel>
#include <QBoxLayout>
#include <QRegExp>

#include "kndpimanager.h"
#include "knimagelabel.h"
#include "knlabellineedit.h"
#include "knlocalemanager.h"
#include "knopacityanimebutton.h"

#include "knaccountresetpanel.h"

#define ButtonSize 36
#define LogoSize 79

KNAccountResetPanel::KNAccountResetPanel(QWidget *parent) :
    QWidget(parent),
    m_emailExp(QRegExp("[\\w\\._-]+@(\\w)+((\\.(\\w)+)+)$")),
    m_resetLogo(new KNImageLabel(this)),
    m_title(new QLabel(this)),
    m_hintLabel(new QLabel(this)),
    m_emailAddress(new KNLabelLineEdit(this)),
    m_okay(generateButton("://public/ok.png")),
    m_cancel(generateButton("://public/cancel.png"))
{
    //Set properties.
    setFixedHeight(knDpi->height(270));

    //Configure the hint and title lable.
    m_title->setAlignment(Qt::AlignCenter);
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(knDpi->height(18));
    m_title->setFont(titleFont);
    //Configure the logo label.
    m_resetLogo->setFixedSize(knDpi->size(LogoSize, LogoSize));
    m_resetLogo->setPixmap(QPixmap("://public/reset-password.png"));
    //Configure the hint label.
    m_hintLabel->setWordWrap(true);
    QPalette hintPal=palette();
    hintPal.setColor(QPalette::WindowText, QColor(157, 157, 157));
    m_hintLabel->setPalette(hintPal);
    //Configure the E-mail line edit.
    m_emailAddress->setMinimumLightness(0xC0);
    m_emailAddress->setMediumLightness(0xE0);
    m_emailAddress->setLabelIcon(QPixmap("://public/generate_email.png"));
    connect(m_emailAddress, &KNLabelLineEdit::textChanged,
            this, &KNAccountResetPanel::onActionCheckEmail);
    //Configure okay button.
    m_okay->hide();
    connect(m_okay, &KNOpacityAnimeButton::clicked,
            this, &KNAccountResetPanel::onActionSentEmail);
    //Configure the button.
    connect(m_cancel, &KNOpacityAnimeButton::clicked,
            this, &KNAccountResetPanel::requireCancel);
    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mainLayout);
    //Add widget to layout
    mainLayout->addStretch();
    mainLayout->addWidget(m_resetLogo, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(knDpi->height(7));
    mainLayout->addWidget(m_title);
    mainLayout->addWidget(m_hintLabel);
    mainLayout->addSpacing(knDpi->height(14));
    mainLayout->addWidget(m_emailAddress);
    //Initial button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    mainLayout->addSpacing(knDpi->height(14));
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    //Add all buttons.
    buttonLayout->addWidget(m_okay);
    buttonLayout->addWidget(m_cancel);

    //Link the retranslate part.
    knI18n->link(this, &KNAccountResetPanel::retranslate);
    retranslate();
}

void KNAccountResetPanel::onActionSentSuccess()
{
    //Reset the E-mail address text line box.
    m_emailAddress->clear();
    //Release the E-mail input box.
    m_emailAddress->setDisabled(false);
    //Release the buttons.
    m_okay->setEnabled(true);
    m_cancel->setEnabled(true);
}

void KNAccountResetPanel::showEvent(QShowEvent *event)
{
    //Do original show event.
    QWidget::showEvent(event);
    //Check the email address state.
    if(m_emailAddress->isEnabled())
    {
        //This means the email address is not working.
        //Reset the E-mail address text line box.
        m_emailAddress->clear();
    }
}

void KNAccountResetPanel::retranslate()
{
    //Update the title label text.
    m_title->setText(tr("Reset your password"));
    //Update the hint text.
    m_hintText=tr("Enter your email address and we will send you a link to "
                  "reset your password.");
    //Update the error hint text.
    m_errorMessage[ResetConnectionError]=tr("Can't connect to Internet, please "
                                            "check your Internet connections.");
    m_errorMessage[ResetCannotFindEmail]=tr("Can't find that email, please "
                                            "check the E-mail address again.");
    m_errorMessage[ResetUnknownError]=tr("Unknown error happens, please try it "
                                         "again.");
    //Update the hint text.
    m_hintLabel->setText(m_hintText);
    //Update place holder text.
    m_emailAddress->setPlaceholderText(tr("Enter your E-mail address"));
}

void KNAccountResetPanel::onActionCheckEmail(const QString &content)
{
    //Reset the hint label text.
    m_hintLabel->setText(m_hintText);
    //Change the okay button visible.
    m_okay->setVisible(m_emailExp.exactMatch(content));
}

void KNAccountResetPanel::onActionSentEmail()
{
    //Emit the reset signal.
    emit requireSendEmail(m_emailAddress->text());
    //Disable the line edit.
    m_emailAddress->setDisabled(true);
    //Disable the buttons.
    m_okay->setEnabled(false);
    m_cancel->setEnabled(false);
}

void KNAccountResetPanel::onActionSentFailed(int errorCode)
{
    //Update the hint with the error code.
    m_hintLabel->setText(m_errorMessage[errorCode]);
    //Release the E-mail input box.
    m_emailAddress->setDisabled(false);
    //Select the email address.
    m_emailAddress->selectAll();
    //Set focus on e-mail.
    m_emailAddress->setFocus();
    //Release the buttons.
    m_okay->setEnabled(true);
    m_cancel->setEnabled(true);
}

inline KNOpacityAnimeButton *KNAccountResetPanel::generateButton(
        const QString &iconPath)
{
    //Initial the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    button->setIcon(QIcon(iconPath));
    button->setFixedSize(knDpi->size(ButtonSize, ButtonSize));
    //Give back the button.
    return button;
}
