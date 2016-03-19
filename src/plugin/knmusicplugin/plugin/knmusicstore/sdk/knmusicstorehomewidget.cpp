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

#include <QDebug>

#define ExtraSpacing 20
#define ContentSpacing 6

KNMusicStoreHomeWidget::KNMusicStoreHomeWidget(QWidget *parent) :
    QWidget(parent),
    m_newMusicList(new KNMusicStoreAlbumList(this)),
    m_hotTracks(new KNMusicStoreAlbumList(this)),
    m_backend(nullptr),
    m_listLayout(nullptr)
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
    QBoxLayout *bodyLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(bodyLayout);
    //Initial the layouts.
    m_listLayout=new QBoxLayout(QBoxLayout::TopToBottom, bodyLayout->widget());
    m_listLayout->setContentsMargins(20, 20, 20, 20);
    //Add the layout.
    bodyLayout->addLayout(m_listLayout);
    //Add widgets to layouts.
    m_listLayout->addWidget(m_blockTitle[NewMusicBlock]);
    m_listLayout->addSpacing(ContentSpacing);
    m_listLayout->addWidget(m_newMusicList);
    m_listLayout->addSpacing(ExtraSpacing);
    m_listLayout->addWidget(m_blockTitle[HotTracksBlock]);
    m_listLayout->addSpacing(ContentSpacing);
    m_listLayout->addWidget(m_hotTracks);
    m_listLayout->addStretch();
    //Initial the widget size.
    m_widgetHeight=bodyLayout->sizeHint().height();
    setMinimumHeight(m_widgetHeight);

    //Link retranslator.
    knI18n->link(this, &KNMusicStoreHomeWidget::retranslate);
    retranslate();
}

void KNMusicStoreHomeWidget::setBackend(KNMusicStoreBackend *backend)
{
    //Save the backend pointer.
    m_backend=backend;
    //Set the album model to list view.
    m_newMusicList->setModel(m_backend->newAlbumModel());
    m_hotTracks->setModel(m_backend->hotSongModel());
    //Prepare title fonts.
    QFont titleFont=font();
    titleFont.setPixelSize(18);
    //Generate list for all backend provides.
    for(int i=0; i<m_backend->listCount(); ++i)
    {
        //Initial the list title.
        QLabel *listTitle=new QLabel(this);
        //Configure the list title.
        listTitle->setFont(titleFont);
        //Save the label.
        m_listNames.append(listTitle);
        //Add the label to the list layout.
        m_listLayout->insertSpacing(m_listLayout->count()-1, ExtraSpacing);
        m_listLayout->insertWidget(m_listLayout->count()-1,
                                   listTitle);
        m_listLayout->addSpacing(ContentSpacing);
        //Initial the list view.
        KNMusicStoreAlbumList *listView=new KNMusicStoreAlbumList(this);
        //Configure the list view.
        listView->setModel(m_backend->listModel(i));
        //Add list view to layout.
        m_listLayout->insertWidget(m_listLayout->count()-1,
                                   listView);
        //Incrase the widget height.
        m_widgetHeight+=(18+ExtraSpacing+ContentSpacing+listView->height());
    }
    //Update the list title.
    updateListTitle();
    //Resize the widget.
    setFixedHeight(m_widgetHeight);
}

void KNMusicStoreHomeWidget::retranslate()
{
    //Update the title.
    m_blockTitle[NewMusicBlock]->setText(tr("New Music"));
    m_blockTitle[HotTracksBlock]->setText(tr("Hot Tracks"));
    //Update list title.
    updateListTitle();
}

void KNMusicStoreHomeWidget::updateListTitle()
{
    //Check pointer.
    if(m_backend==nullptr)
    {
        //Ignore the request.
        return;
    }
    //Update the label in the list name list.
    for(int i=0; i<m_backend->listCount(); ++i)
    {
        //Update the label.
        m_listNames.at(i)->setText(m_backend->listName(i));
    }
}

