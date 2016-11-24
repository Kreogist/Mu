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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QLabel>

#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusicstoreloadingdimmer.h"

KNMusicStoreLoadingDimmer::KNMusicStoreLoadingDimmer(QWidget *parent) :
    QWidget(parent),
    m_title(new QLabel(this))
{
    setObjectName("MusicStoreLoadingDimmer");
    //Set properties.
    // Title label
    QFont titleFont=m_title->font();
    titleFont.setBold(true);
    titleFont.setPixelSize(40);
    m_title->setFont(titleFont);
    m_title->setText(QString(QChar(0x03BC))+"'sic Store");

    //Prepare the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);
    setLayout(mainLayout);
    //Add widgets to layout.
    mainLayout->addWidget(m_title, 0, Qt::AlignHCenter);

    //Add to theme manager.
    knTheme->registerWidget(this);

    //Link translator.
    knI18n->link(this, &KNMusicStoreLoadingDimmer::retranslate);
    retranslate();
}

void KNMusicStoreLoadingDimmer::retranslate()
{
    //Update title.
}
