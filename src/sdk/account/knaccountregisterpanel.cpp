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

#define ButtonSize 36

KNAccountRegisterPanel::KNAccountRegisterPanel(QWidget *parent) :
    QWidget(parent),
    m_title(new QLabel(this)),
    m_emailHint(new QLabel(this)),
    m_passwordHint(new QLabel(this)),
    m_agreeLicense(new QCheckBox(this)),
    m_username(new KNLabelLineEdit(this)),
    m_password(new KNLabelLineEdit(this)),
    m_email(new KNLabelLineEdit(this)),
    m_login(generateButton("://public/ok.png")),
    m_cancel(generateButton("://public/cancel.png"))
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
    //Configure password line edit.
    m_password->setMinimumLightness(0xC0);
    m_password->setMediumLightness(0xE0);
    m_password->setEchoMode(QLineEdit::Password);
    //Configure the E-mail.
    m_email->setMinimumLightness(0xC0);
    m_email->setMediumLightness(0xE0);
    //Configure the hint label.
    QPalette hintPal=palette();
    hintPal.setColor(QPalette::WindowText, QColor(138, 155, 168));
    m_emailHint->setPalette(hintPal);
    m_passwordHint->setPalette(hintPal);

    //Initial the box layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mainLayout);
    //Add widget to layout
    mainLayout->addSpacing(5);
    mainLayout->addWidget(m_title);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(m_username);
    mainLayout->addWidget(m_password);
    mainLayout->addWidget(m_passwordHint);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(m_email);
    mainLayout->addWidget(m_emailHint);
    mainLayout->addWidget(m_agreeLicense, 0, Qt::AlignHCenter);
    //Initial button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    mainLayout->addSpacing(8);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    //Add all buttons.
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_login);
    buttonLayout->addSpacing(45);
    buttonLayout->addWidget(m_cancel);
    buttonLayout->addStretch();

    //Link the retranslator.
    knI18n->link(this, &KNAccountRegisterPanel::retranslate);
    retranslate();
}

void KNAccountRegisterPanel::hideEvent(QHideEvent *event)
{
    //Hide the widget first.
    QWidget::hideEvent(event);
    //CLear the data.
    m_username->clear();
    m_password->clear();
}

void KNAccountRegisterPanel::retranslate()
{
    //Update label.
    m_title->setText(tr("Create Kreogist Account"));
    //Update hint.
    m_passwordHint->setText(tr("Passwords must be at least 6 characters.\n"
                               "  1. Must include a number.\n"
                               "  2. Must include an upper and lower case "
                               "letter."));
    m_emailHint->setText(tr("Your E-mail will be verified."));
    //Set the text edit.
    m_username->setPlaceholderText(tr("Username"));
    m_password->setPlaceholderText(tr("Password"));
    m_email->setPlaceholderText(tr("E-mail"));
    //Set the check box.
    m_agreeLicense->setText(tr("I agree to the Terms of Service."));
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
