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

#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusicstorealbumlistmodel.h"
#include "knmusicstoreglobal.h"
#include "knmusicstorealbumlist.h"
#include "knmusicstorebackend.h"

#include "knmusicstorehomewidget.h"

KNMusicStoreHomeWidget::KNMusicStoreHomeWidget(QWidget *parent) :
    QWidget(parent),
    m_newMusicList(new KNMusicStoreAlbumList(this))
{
    setObjectName("MusicStoreWidget");
    //Set properties.
    setAutoFillBackground(true);
    setFixedWidth(knMusicStoreGlobal->storeContentWidth());
    //Register this widget to theme manager.
    knTheme->registerWidget(this);

    //Initial the widgets.
    // Titles
    //Prepare title fonts.
    QFont titleFont=font();
    titleFont.setPixelSize(18);
    for(int i=0; i<HomePageBlockCount; ++i)
    {
        //Generate the title label.
        m_blockTitle[i]=new QLabel(this);
        //Set the title as the music store widget.
        m_blockTitle[i]->setObjectName("MusicStoreWidget");
        knTheme->registerWidget(m_blockTitle[i]);
        //Configure the title.
        m_blockTitle[i]->setFont(titleFont);
    }
    //Initial the layouts.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    //Set the layout.
    setLayout(mainLayout);
    //Add widgets to layouts.
    mainLayout->addWidget(m_blockTitle[NewMusicBlock]);
    mainLayout->addWidget(m_newMusicList);
    mainLayout->addStretch();

    //Link retranslator.
    knI18n->link(this, &KNMusicStoreHomeWidget::retranslate);
    retranslate();
}

void KNMusicStoreHomeWidget::setBackend(KNMusicStoreBackend *backend)
{
    //Set the album model to list view.
    m_newMusicList->setModel(backend->newAlbumModel());
}

void KNMusicStoreHomeWidget::retranslate()
{
    //Update the title.
    m_blockTitle[NewMusicBlock]->setText(tr("New Music"));
}

