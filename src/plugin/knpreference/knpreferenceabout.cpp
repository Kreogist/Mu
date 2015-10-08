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
#include <QScrollBar>
#include <QTextEdit>

#include "knlocalemanager.h"
#include "knthememanager.h"
#include "sao/knsaostyle.h"

#include "knpreferenceabout.h"

#include <QDebug>

KNPreferenceAbout::KNPreferenceAbout(QWidget *parent) :
    QWidget(parent),
    m_copyrightText(new QTextEdit(this))
{
    //Configure the label fonts.
    QFont labelFonts=font();
    labelFonts.setPixelSize(15);
    //Initial the labels.
    for(int i=0; i<ContentIndexCount; ++i)
    {
        //Initial the label.
        m_textContent[i]=new QLabel(this);
        //Configure the label.
        m_textContent[i]->setObjectName("PreferenceAboutLabel");
        knTheme->registerWidget(m_textContent[i]);
        m_textContent[i]->setAlignment(Qt::AlignHCenter);
        m_textContent[i]->setFont(labelFonts);
    }
    //Configure the copyright text edit.
    m_copyrightText->setObjectName("PreferenceAboutCopyright");
    m_copyrightText->setFrameStyle(QFrame::NoFrame);
    m_copyrightText->setReadOnly(true);
    m_copyrightText->viewport()->setCursor(Qt::ArrowCursor);
    knTheme->registerWidget(m_copyrightText);
    KNSaoStyle::styleVerticalScrollBar(m_copyrightText->verticalScrollBar());

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(15);
    setLayout(mainLayout);
    //Add the stretch.
    mainLayout->addStretch();
    //Add the icon to main layout.
    mainLayout->addWidget(m_textContent[0], 0, Qt::AlignHCenter);
    //Initial the content layout.
    //Initial the main layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             mainLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(8);
    mainLayout->addLayout(contentLayout);
    //Add the stretch to bottom.
    mainLayout->addStretch();
    //Add version and copyright labels to content layout.
    contentLayout->addWidget(m_textContent[Title], 0, Qt::AlignHCenter);
    contentLayout->addSpacing(8);
    contentLayout->addWidget(m_textContent[Version], 0, Qt::AlignHCenter);
    contentLayout->addWidget(m_textContent[Copyright], 0, Qt::AlignHCenter);
    contentLayout->addWidget(m_copyrightText, 0, Qt::AlignHCenter);
    contentLayout->addStretch();

    //Set the default label.
    m_textContent[Logo]->setPixmap(QPixmap("://icon/mu.png"));
    //Configure the label.
    m_textContent[Logo]->setFixedSize(16, 16);
    m_textContent[Logo]->setScaledContents(true);
    m_textContent[Title]->setText(QApplication::applicationDisplayName());

    //Link the retranslate.
    knI18n->link(this, &KNPreferenceAbout::retranslate);
    retranslate();
}

void KNPreferenceAbout::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QWidget::resizeEvent(event);
    //Calculate Icon size.
    int iconSize=(qreal)qMin(width(), height())*0.236;
    //Resize the icon.
    m_textContent[Logo]->setFixedSize(iconSize, iconSize);
    //Change the caption size.
    QFont captionFont=m_textContent[Title]->font();
    //Calculate the title size.
    iconSize=(qreal)iconSize*0.236;
    captionFont.setPixelSize(iconSize);
    //Set to caption font.
    m_textContent[Title]->setFont(captionFont);
    //Set the maximum height of the title.
    m_copyrightText->setMaximumHeight(iconSize<<2);
    //Calculate the caption size.
    iconSize=iconSize>>2;
    captionFont.setPixelSize(iconSize<13?13:iconSize);
    //Set font to those captions.
    m_textContent[Version]->setFont(captionFont);
    m_textContent[Copyright]->setFont(captionFont);
}

void KNPreferenceAbout::retranslate()
{
    //Update the content data.
    m_textContent[Version]->setText(QApplication::applicationVersion());
    m_textContent[Copyright]->setText(QString(QChar(169)) +
                                      tr(" 2013-2015 Kreogist Dev Team\n"
                                      "All rights reserved."));
    //Update the thank list.
    m_copyrightText->setPlainText(
                tr("Staffs\n"
                    "Special Thanks"));
}

