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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QFormLayout>
#include <QLabel>

#include "knaccountdetails.h"
#include "knlabellineedit.h"
#include "knlocalemanager.h"
#include "kndpimanager.h"

#include "knaccountdetailbox.h"

KNAccountDetailBox::KNAccountDetailBox(QWidget *parent) :
    KNMessageBox(parent),
    m_username(new QLabel(this)),
    m_nickName(generateLineEdit()),
    m_emailAddress(generateLineEdit())
{
    //Set property.
    setTitleText("Preference");
    setCancelButtonVisible(true);
    //Configure the user name.
    m_username->setFixedHeight(m_nickName->height());

    //Initial the container.
    QWidget *container=new QWidget(this);
    //Initial the label palette.
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(157, 157, 157));
    //Initial the label.
    for(int i=0; i<EditRowCount; ++i)
    {
        //Initial the label.
        m_title[i]=new QLabel(container);
        //Set the palette.
        m_title[i]->setPalette(pal);
    }

    //Initial the layout.
    QFormLayout *containerLayout=new QFormLayout(container);
    //Configure layout.
    containerLayout->setContentsMargins(knDpi->margins(20, 15, 20, 10));
    containerLayout->setVerticalSpacing(knDpi->height(10));
    containerLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    //Set the container.
    container->setLayout(containerLayout);
    //Add rows.
    containerLayout->addRow(m_title[UserNameRow],
                            m_username);
    containerLayout->addRow(m_title[NickNameRow],
                            m_nickName);
    containerLayout->addRow(m_title[EmailAddressRow],
                            m_emailAddress);

    //Set the container to be the conent widget.
    setContentWidget(container);

    //Link translator.
    knI18n->link(this, &KNAccountDetailBox::retranslate);
    retranslate();
}

void KNAccountDetailBox::setAccountDetails(KNAccountDetails *details)
{
    //Set the data.
    m_username->setText(details->cacheUserName());
    m_nickName->setText(details->displayName());
    m_emailAddress->setText(details->data("email").toString());
}

bool KNAccountDetailBox::okayPressed()
{
    //Construct a JSON object for all these new data.
    QJsonObject updatedInfo;
    //Insert data.
    updatedInfo.insert("displayName", m_nickName->text());
    updatedInfo.insert("email", m_emailAddress->text());
    //Emit the updated object.
    emit requireUpdateInfo(updatedInfo);
    //Mission complete.
    return true;
}

void KNAccountDetailBox::retranslate()
{
    //Translate the title.
    m_title[UserNameRow]->setText(tr("Username"));
    m_title[NickNameRow]->setText(tr("Nickname"));
    m_title[EmailAddressRow]->setText(tr("E-mail"));
}

inline KNLabelLineEdit *KNAccountDetailBox::generateLineEdit()
{
    //Generate the line edit.
    KNLabelLineEdit *lineEdit=new KNLabelLineEdit(this);
    //Configure the line edit.
    lineEdit->setMinimumLightness(0xC0);
    lineEdit->setMediumLightness(0xE0);
    //Give back the line edit.
    return lineEdit;
}
