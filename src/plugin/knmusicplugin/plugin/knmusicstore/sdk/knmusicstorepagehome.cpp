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

#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusicstoreutil.h"
#include "knmusicstorebackend.h"
#include "knmusicstorehomealbumview.h"
#include "knmusicstorehomesongview.h"
#include "knmusicstorehomelistview.h"
#include "knmusicstorehomelistmodel.h"

#include "knmusicstorepagehome.h"

#include <QDebug>

using namespace MusicStoreUtil;

KNMusicStorePageHome::KNMusicStorePageHome(QWidget *parent) :
    KNMusicStorePage(parent),
    m_newAlbumView(new KNMusicStoreHomeAlbumView(this)),
    m_newSongView(new KNMusicStoreHomeSongView(this)),
    m_homeContentCounter(0),
    m_homeCounterClear(true)
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
        m_titleLabel[i]->setObjectName("MusicStorePage");
        knTheme->registerWidget(m_titleLabel[i]);
        //Initial the model.
        m_homeListModel[i]=new KNMusicStoreHomeListModel(this);
    }
    //Inital views.
    for(int i=0; i<RankingListViewCount; ++i)
    {
        //Initial the view.
        m_rankingList[i]=new KNMusicStoreHomeListView(this);
        //Configure the list view size.
        m_rankingList[i]->tweakHeight(10);  //Max 10 items.
        m_rankingList[i]->setObjectName("MusicStoreHomeListView");
        knTheme->registerWidget(m_rankingList[i]);
    }
    //Tweak the top song to be 20.
    m_rankingList[ViewTopSongs]->tweakHeight(20);
    //Set the model to view.
    m_newAlbumView->setModel(m_homeListModel[ListNewAlbum]);
    m_newSongView->setModel(m_homeListModel[ListNewSongs]);
    m_rankingList[ViewBillboard]->setModel(m_homeListModel[ListBillboard]);
    m_rankingList[ViewOricon]->setModel(m_homeListModel[ListOricon]);
    m_rankingList[ViewItunes]->setModel(m_homeListModel[ListItunes]);
    m_rankingList[ViewTopSongs]->setModel(m_homeListModel[ListTopSongs]);
    //Configure the view.
    connect(m_newAlbumView, &KNMusicStoreHomeAlbumView::clicked,
            this, &KNMusicStorePageHome::onNewAlbumViewClicked);
    connect(m_newSongView, &KNMusicStoreHomeSongView::clicked,
            this, &KNMusicStorePageHome::onSongViewClicked);
    connect(m_rankingList[ViewBillboard], &KNMusicStoreHomeListView::clicked,
            this, &KNMusicStorePageHome::onSongViewClicked);
    connect(m_rankingList[ViewOricon], &KNMusicStoreHomeListView::clicked,
            this, &KNMusicStorePageHome::onSongViewClicked);
    connect(m_rankingList[ViewItunes], &KNMusicStoreHomeListView::clicked,
            this, &KNMusicStorePageHome::onSongViewClicked);
    connect(m_rankingList[ViewTopSongs], &KNMusicStoreHomeListView::clicked,
            this, &KNMusicStorePageHome::onSongViewClicked);
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
    contentLayout->addSpacing(13);
    //Initial the ranking layout.
    QGridLayout *rankLayout=new QGridLayout();
    rankLayout->setHorizontalSpacing(20);
    rankLayout->setVerticalSpacing(2);
    contentLayout->addLayout(rankLayout);
    //Add ranks.
    rankLayout->addWidget(m_titleLabel[ListBillboard], 0, 0, 1, 1);
    rankLayout->addWidget(m_rankingList[ViewBillboard], 1, 0, 1, 1);
    rankLayout->addWidget(m_titleLabel[ListOricon], 0, 1, 1, 1);
    rankLayout->addWidget(m_rankingList[ViewOricon], 1, 1, 1, 1);
    rankLayout->addWidget(m_titleLabel[ListItunes], 0, 2, 1, 1);
    rankLayout->addWidget(m_rankingList[ViewItunes], 1, 2, 1, 1);
    //Add free stretch.
    contentLayout->addStretch();
    //Add sidebar layout.
    QBoxLayout *sidebarLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    sidebarLayout->setSpacing(0);
    sidebarLayout->addWidget(m_titleLabel[ListTopSongs]);
    sidebarLayout->addSpacing(7);
    sidebarLayout->addWidget(m_rankingList[ViewTopSongs]);
    sidebarLayout->addStretch();
    listLayout->addLayout(sidebarLayout);

    //Link the retranslator.
    knI18n->link(this, &KNMusicStorePageHome::retranslate);
    retranslate();
}

void KNMusicStorePageHome::reset()
{
    //Home page special reset.
    m_homeContentCounter=6;
    //Reset the counter flag.
    m_homeCounterClear=true;
    //Clear the home model.
    m_homeListModel[ListNewAlbum]->reset();
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
        //Decrease the counter.
        --m_homeContentCounter;
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
        //Set the song data to the list model.
        setListModelData(value, m_homeListModel[ListNewSongs]);
        break;
    case HomeBillboardList:
        setListModelData(value, m_homeListModel[ListBillboard]);
        break;
    case HomeOriconList:
        setListModelData(value, m_homeListModel[ListOricon]);
        break;
    case HomeItunesList:
        setListModelData(value, m_homeListModel[ListItunes]);
        break;
    case HomeTopSongsList:
        setListModelData(value, m_homeListModel[ListTopSongs]);
        break;
    }
    //Check the home content counter.
    if(m_homeCounterClear && (m_homeContentCounter==0))
    {
        //Emit require show signal.
        emit requireShowPage();
        //Clear the flag.
        m_homeCounterClear=false;
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

void KNMusicStorePageHome::onNewAlbumViewClicked(const QModelIndex &albumIndex)
{
    //Check album index is valid or not.
    if(!albumIndex.isValid())
    {
        //Ignore the request.
        return;
    }
    //Ask to display the album.
    emit requireShowAlbum(albumIndex.data(Qt::UserRole+2).toString());
}

void KNMusicStorePageHome::onSongViewClicked(const QModelIndex &songIndex)
{
    //Check the index is valid or not.
    if(!songIndex.isValid())
    {
        //Ignore the request.
        return;
    }
    //Ask to display the song.
    emit requireShowSingleSong(songIndex.data(Qt::UserRole+2).toString());
}

inline void KNMusicStorePageHome::setListModelData(
        const QVariant &value,
        KNMusicStoreHomeListModel *model)
{
    //Reset the model.
    model->reset();
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
        model->appendRow(songItem);
    }
    //Decrease the counter.
    --m_homeContentCounter;
}
