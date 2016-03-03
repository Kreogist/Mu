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

#include "knlabellineedit.h"
#include "knlocalemanager.h"

#include "knaccountpanel.h"

KNAccountPanel::KNAccountPanel(QWidget *parent) :
    QWidget(parent),
    m_username(new KNLabelLineEdit(this)),
    m_password(new KNLabelLineEdit(this))
{
    //Configure the user name line edit.
    m_username->setMinimumLightness(0xC0);
    m_username->setMediumLightness(0xE0);
    //Configure password line edit.
    m_password->setMinimumLightness(0xC0);
    m_password->setMediumLightness(0xE0);
    m_password->setEchoMode(QLineEdit::Password);

    //Initial the box layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mainLayout);
    //Add widget to layout
    mainLayout->addWidget(m_username);
    mainLayout->addWidget(m_password);

    //Link the retranslator.
    knI18n->link(this, &KNAccountPanel::retranslate);
    retranslate();
}

void KNAccountPanel::retranslate()
{
    m_username->setPlaceholderText(tr("User Name/E-mail"));
    m_password->setPlaceholderText(tr("Password"));
}

