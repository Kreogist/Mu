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
#include <QTimeLine>
#include <QLabel>
#include <QTimer>

#include "knopacityanimebutton.h"
#include "knlabellineedit.h"
#include "knlocalemanager.h"
#include "kndpimanager.h"

#include "knaccount.h"
#include "knaccountutil.h"
#include "knaccountdetails.h"

#include "knaccountpasswordverification.h"

#define ErrorColor 0x9a,0x25,0x38

KNAccountPasswordVerification::KNAccountPasswordVerification(QWidget *parent) :
    QWidget(parent),
    m_authTitle(new QLabel(this)),
    m_authHint(new QLabel(this)),
    m_errorHint(new QLabel(this)),
    m_passwordEdit(new KNLabelLineEdit(this)),
    m_checkButton(new KNOpacityAnimeButton(this)),
    m_errorFadeIn(generateTimeLine()),
    m_errorFadeOut(generateTimeLine()),
    m_displayDelay(new QTimer(this))
{
    //Set properties.
    //Configure title label.
    QFont titleFont=m_authTitle->font();
    titleFont.setPixelSize((titleFont.pixelSize()>>2)+titleFont.pixelSize());
    titleFont.setBold(true);
    m_authTitle->setFont(titleFont);
    m_authTitle->setWordWrap(true);
    //Configure hint label.
    m_authHint->setWordWrap(true);
    //Configure the hint label.
    m_errorHint->setAlignment(Qt::AlignCenter);
    QPalette pal=m_errorHint->palette();
    pal.setColor(QPalette::WindowText, QColor(ErrorColor, 0));
    m_errorHint->setPalette(pal);
    //Configure the check button.
    m_checkButton->setIcon(QIcon("://public/next.png"));
    m_checkButton->setFixedSize(knDpi->size(32, 32));
    //Set password edit properties.
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setLabelIcon(QPixmap("://public/generate_password.png"));
    //Configure the password line edit.
    m_passwordEdit->setMinimumLightness(0xC0);
    m_passwordEdit->setMediumLightness(0xE0);
    //Configure animation.
    m_displayDelay->setInterval(3000);
    m_displayDelay->setSingleShot(true);
    m_errorFadeIn->setFrameRange(0, 255);
    m_errorFadeOut->setFrameRange(255, 0);
    connect(m_errorFadeIn, &QTimeLine::finished,
            m_displayDelay, static_cast<void (QTimer::*)()>(&QTimer::start));
    connect(m_displayDelay, &QTimer::timeout,
            m_errorFadeOut, &QTimeLine::start);
    //Link button.
    connect(m_checkButton, &KNOpacityAnimeButton::clicked,
            this, &KNAccountPasswordVerification::onActionCheckPassword);

    //Construct layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(knDpi->margins(15, 15, 15, 0));
    //Set the layout to widget.
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_authTitle);
    mainLayout->addWidget(m_authHint);
    mainLayout->addWidget(m_errorHint);
    //Add password box.
    mainLayout->addWidget(m_passwordEdit);
    //Add check button.
    mainLayout->addSpacing(knDpi->height(2));
    mainLayout->addWidget(m_checkButton, 0, Qt::AlignCenter);
    mainLayout->addSpacing(fontMetrics().height());

    //Link locale translate.
    knI18n->link(this, &KNAccountPasswordVerification::retranslate);
    retranslate();
}

void KNAccountPasswordVerification::showEvent(QShowEvent *event)
{
    //Clear password box.
    m_passwordEdit->clear();
    //Set fous on password edit.
    m_passwordEdit->setFocus(Qt::MouseFocusReason);
    //Show the widget.
    QWidget::showEvent(event);
}

void KNAccountPasswordVerification::retranslate()
{
    //Set the title.
    m_authTitle->setText(tr("Enter your password to perform administrative "
                            "tasks"));
    //Set the hint.
    m_authHint->setText(tr("An action is attempting to perform an action that"
                           " requires privileges. Authentication is required"
                           " to perform this action."));
    //Set the error text.
    m_errorHint->setText(tr("Password is not correct, try again."));
}

void KNAccountPasswordVerification::onActionUpdateAlpha(int frame)
{
    //Update the palette.
    QPalette pal=m_errorHint->palette();
    pal.setColor(QPalette::WindowText, QColor(ErrorColor, frame));
    //Set the palette.
    m_errorHint->setPalette(pal);
}

void KNAccountPasswordVerification::onActionCheckPassword()
{
    //Check if the password is emtpy, ignore.
    if(m_passwordEdit->text().isEmpty())
    {
        //Give back the focus.
        m_passwordEdit->setFocus(Qt::MouseFocusReason);
        //Ignore the focus.
        return;
    }
    //Stop all animation.
    m_displayDelay->stop();
    m_errorFadeIn->stop();
    m_errorFadeOut->stop();
    //Reset the alpha.
    onActionUpdateAlpha(0);
    //Check password.
    if(KNAccountUtil::accessPassword(m_passwordEdit->text())
            ==knAccount->accountDetails()->cachePassword())
    {
        //Emit password is correct signal.
        emit passwordCorrect();
        //Complete.
        return;
    }
    //Emit error signal.
    emit passwordWrong();
    //Start the error text.
    m_errorFadeIn->start();
    //Select all the password.
    m_passwordEdit->selectAll();
    m_passwordEdit->setFocus(Qt::MouseFocusReason);
}

inline QTimeLine *KNAccountPasswordVerification::generateTimeLine()
{
    //Generate the time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configrue the time line.
    timeLine->setUpdateInterval(33);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    //Link time line.
    connect(timeLine, &QTimeLine::frameChanged, this,
            &KNAccountPasswordVerification::onActionUpdateAlpha);
    //Give back the time line.
    return timeLine;
}

