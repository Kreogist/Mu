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
#include <QGridLayout>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>

#include "knlocalemanager.h"

#include "knmusicstoreutil.h"
#include "knmusicstorebackend.h"
#include "knmusicstorehomealbumview.h"
#include "knmusicstorehomesongview.h"
#include "knmusicstorehomelistmodel.h"

#include "knmusicstorepagehome.h"

using namespace MusicStoreUtil;

KNMusicStorePageHome::KNMusicStorePageHome(QWidget *parent) :
    KNMusicStorePage(parent),
    m_newAlbumView(new KNMusicStoreHomeAlbumView(this)),
    m_newSongView(new KNMusicStoreHomeSongView(this))
{
    updateObjectName("MusicStorePage");
    //Initial title label fonts.
    QFont titleFont=font();
    titleFont.setPixelSize(16);
    //Initial title labels and models.
    for(int i=0; i<HomeSongListCount; ++i)
    {
        //Initial the label.
        m_titleLabel[i]=new QLabel(this);
        //Set properties.
        m_titleLabel[i]->setFont(titleFont);
        //Initial the model.
        m_homeListModel[i]=new KNMusicStoreHomeListModel(this);
    }
    //Set the view.
    m_newAlbumView->setModel(m_homeListModel[ListNewAlbum]);
    m_newSongView->setModel(m_homeListModel[ListNewSongs]);
    //Configure the label.
    m_titleLabel[ListTopSongs]->setFixedWidth(220);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0, KNMusicStoreUtil::headerHeight() + 76,
                                   0, KNMusicStoreUtil::headerHeight() + 36);
    setLayout(mainLayout);
    //Initial the rank list layout.
    QBoxLayout *listLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    listLayout->setContentsMargins(30, 0, 30, 0);
    listLayout->setSpacing(40);
    mainLayout->addLayout(listLayout);
    //Initial the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    listLayout->addLayout(contentLayout, 1);
    //Add widgets.
    contentLayout->addWidget(m_titleLabel[ListNewAlbum]);
    contentLayout->addSpacing(4);
    contentLayout->addWidget(m_newAlbumView);
    contentLayout->addSpacing(13);
    contentLayout->addWidget(m_titleLabel[ListNewSongs]);
    contentLayout->addSpacing(2);
    contentLayout->addWidget(m_newSongView);
    //Initial the ranking layout.
    QGridLayout *rankLayout=new QGridLayout();
    contentLayout->addLayout(rankLayout);
    //Add ranks.
    rankLayout->addWidget(m_titleLabel[ListBillboard], 0, 0, 1, 1);
    rankLayout->addWidget(m_titleLabel[ListOricon], 0, 1, 1, 1);
    rankLayout->addWidget(m_titleLabel[ListItunes], 0, 2, 1, 1);
    //Add free stretch.
    contentLayout->addStretch();
    //Add sidebar layout.
    QBoxLayout *sidebarLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    sidebarLayout->addWidget(m_titleLabel[ListTopSongs]);
    sidebarLayout->addStretch();
    listLayout->addLayout(sidebarLayout);

    //Link the retranslator.
    knI18n->link(this, &KNMusicStorePageHome::retranslate);
    retranslate();
}

void KNMusicStorePageHome::reset()
{
    //Home page special reset.
}

void KNMusicStorePageHome::setPageLabel(int labelIndex, const QVariant &value)
{
    switch(labelIndex)
    {
    case HomeNewAlbumData:
    {
        //Reset the model.
        m_homeListModel[ListNewAlbum]->reset();
        //Translate the value to json array.
        QJsonArray albumDataList=value.toJsonArray();
        //Start to insert data.
        for(auto i : albumDataList)
        {
            //Cast i as object.
            QJsonObject albumData=i.toObject();
            //Construct the item.
            KNMusicStoreHomeItem albumItem;
            //Set the album data.
            albumItem.title=albumData.value("name").toString();
            albumItem.subheading=albumData.value("artist").toString();
            albumItem.customData=albumData.value("custom").toString();
            //Insert the album to model.
            m_homeListModel[ListNewAlbum]->appendRow(albumItem);
        }
        break;
    }
    case HomeNewAlbumArt:
    case HomeNewSongArt:
    {
        //Translate the data to structure.
        KNMusicStoreHomeUpdateArtwork homeArtworkData=
                value.value<KNMusicStoreHomeUpdateArtwork>();
        //Get update the model.
        m_homeListModel[labelIndex==HomeNewAlbumArt?
                                ListNewAlbum:
                                ListNewSongs]->setAlbumArt(
                        homeArtworkData.index,
                        homeArtworkData.artwork);
        //Update complete.
        break;
    }
    case HomeNewSongData:
    {
        //Reset the model.
        m_homeListModel[ListNewSongs]->reset();
        //Translate the value to json array.
        QJsonArray songDataList=value.toJsonArray();
        //Start to insert data.
        for(auto i : songDataList)
        {
            //Cast i as object.
            QJsonObject songData=i.toObject();
            //Construct the item.
            KNMusicStoreHomeItem songItem;
            //Set the album data.
            songItem.title=songData.value("name").toString();
            songItem.subheading=songData.value("artist-album").toString();
            songItem.customData=songData.value("custom").toString();
            //Insert the album to model.
            m_homeListModel[ListNewSongs]->appendRow(songItem);
        }
        break;
    }
    }
}

void KNMusicStorePageHome::setBackend(KNMusicStoreBackend *backend)
{
    //Call the parent set backend.
    KNMusicStorePage::setBackend(backend);
    //Break all the previous connections.
    m_backendConnection.disconnectAll();
    //Link the backend to this page.
    m_backendConnection.append(
                connect(backend, &KNMusicStoreBackend::requireSetHome,
                        this, &KNMusicStorePageHome::setPageLabel));
    //Reset the current page.
    reset();
}

void KNMusicStorePageHome::retranslate()
{
    m_titleLabel[ListNewAlbum]->setText(tr("New Album"));
    m_titleLabel[ListNewSongs]->setText(tr("New Music"));
    m_titleLabel[ListBillboard]->setText(tr("Billboard Rank"));
    m_titleLabel[ListOricon]->setText(tr("Oricon Rank"));
    m_titleLabel[ListItunes]->setText(tr("iTunes Rank"));
    m_titleLabel[ListTopSongs]->setText(tr("Top Songs"));
}
