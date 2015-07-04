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
#include <QApplication>
#include <QBoxLayout>
#include <QLabel>

#include "knlocalemanager.h"

#include "knpreferenceabout.h"

#include <QDebug>

KNPreferenceAbout::KNPreferenceAbout(QWidget *parent) :
    QWidget(parent)
{
    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(18,18,18,18);
    mainLayout->setSpacing(15);
    setLayout(mainLayout);

    //Initial the labels.
    for(int i=0; i<ContentIndexCount; i++)
    {
        //Initial the label.
        m_textContent[i]=new QLabel(this);
        //Configure the label.
        m_textContent[i]->setAlignment(Qt::AlignCenter);
        //Add the label to main layout.
        mainLayout->addWidget(m_textContent[i]);
    }
    mainLayout->addStretch();


    //Set the default label.
    m_textContent[Logo]->setPixmap(
                QPixmap("://icon/mu.png").scaled(128,
                                                 128,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
    m_textContent[Title]->setText(QApplication::applicationDisplayName());

    //Link the retranslate.
    knI18n->link(this, &KNPreferenceAbout::retranslate);
    retranslate();
}

void KNPreferenceAbout::retranslate()
{
    //Update the content data.
    m_textContent[Version]->setText(QApplication::applicationVersion());
    m_textContent[Copyright]->setText(QString(QChar(169)) +
                                      tr(" 2013-2015 Kreogist Dev Team\n"
                                      "All rights reserved."));
}

