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

#include "knopacityanimebutton.h"
#include "knlocalemanager.h"

#include "knmusicstoredownloadlist.h"

KNMusicStoreDownloadList::KNMusicStoreDownloadList(QWidget *parent) :
    QWidget(parent),
    m_button(new KNOpacityAnimeButton(this)),
    m_title(new QLabel(this))
{
    //Configure the button.
    m_button->setIcon(QPixmap(":/plugin/music/store/download_list.png"));
    m_button->setCursor(Qt::PointingHandCursor);
    m_button->setFixedSize(13, 13);
    //Configure the title label button.
    QFont labelFont=m_title->font();
    labelFont.setBold(true);
    labelFont.setPixelSize(17);
    m_title->setFont(labelFont);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_title);
    mainLayout->addStretch();

    //Link the local manager.
    knI18n->link(this, &KNMusicStoreDownloadList::retranslate);
    retranslate();
}

KNOpacityAnimeButton *KNMusicStoreDownloadList::iconButton()
{
    return m_button;
}

void KNMusicStoreDownloadList::retranslate()
{
    m_title->setText(tr("Download"));
}
