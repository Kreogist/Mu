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
#include <QHeaderView>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>

#include "knhighlightlabel.h"
#include "knscrolllabel.h"
#include "knlocalemanager.h"

#include "knmusicstoreutil.h"
#include "knmusicstoreglobal.h"
#include "knmusicstorelistview.h"
#include "knmusicstorealbummodel.h"
#include "knmusicstorebackend.h"

#include "knmusicstorepagealbum.h"

#include <QDebug>

using namespace MusicStoreUtil;

KNMusicStorePageAlbum::KNMusicStorePageAlbum(QWidget *parent) :
    KNMusicStorePage(parent),
    m_albumTitle(new KNScrollLabel(this)),
    m_albumArtistLabel(new QLabel(this)),
    m_releaseDateLabel(new QLabel(this)),
    m_publishByLabel(new QLabel(this)),
    m_albumArt(new KNHighLightLabel(this)),
    m_albumView(new KNMusicStoreListView(this))
{
    updateObjectName("MusicStorePage");
    //Configure the album art.
    m_albumArt->setFixedSize(200, 200);
    //Configure the title label font.
    QFont titleFont=font();
    titleFont.setPixelSize(23);
    titleFont.setBold(true);
    m_albumTitle->setFont(titleFont);
    //Configure the content margin.
    m_albumArtistLabel->setContentsMargins(0, 10, 0, 0);
    //Configure the publish text.
    QByteArray publishText;
    publishText.append(0xE2);
    publishText.append(0x84);
    publishText.append(0x97);
    m_publishByText.append(publishText);
    m_publishByText.append(" %1");
    //Configure the album model.
    KNMusicStoreAlbumModel *albumModel=knMusicStoreGlobal->albumModel();
    connect(albumModel, &KNMusicStoreAlbumModel::rowCountChanged,
            this, &KNMusicStorePageAlbum::onAlbumRowCountChanged);
    //Configure the album view.
    m_albumView->setModel(albumModel);
    QHeaderView *albumHeader=m_albumView->header();
    albumHeader->setSectionResizeMode(KNMusicStoreAlbumModel::AlbumModelIndex,
                                      QHeaderView::Fixed);
    albumHeader->setSectionResizeMode(KNMusicStoreAlbumModel::AlbumModelName,
                                      QHeaderView::Stretch);
    albumHeader->setSectionResizeMode(KNMusicStoreAlbumModel::AlbumModelArtist,
                                      QHeaderView::Stretch);
    albumHeader->setSectionResizeMode(
                KNMusicStoreAlbumModel::AlbumModelDuration,
                QHeaderView::Fixed);
    albumHeader->setStretchLastSection(false);
    m_albumView->setColumnWidth(KNMusicStoreAlbumModel::AlbumModelIndex,
                                30);
    m_albumView->setColumnWidth(KNMusicStoreAlbumModel::AlbumModelDuration,
                                60);
    m_albumView->updateObjectName("MusicStoreAlbumTreeView");
    connect(m_albumView, &KNMusicStoreListView::clicked,
            this, &KNMusicStorePageAlbum::onViewIndexClicked);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,
                                   KNMusicStoreUtil::headerHeight() + 55,
                                   0,
                                   KNMusicStoreUtil::headerHeight() + 36);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add the content left and right layout.
    QBoxLayout *albumLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    albumLayout->setContentsMargins(30, 0, 30, 0);
    albumLayout->setSpacing(0);
    mainLayout->addLayout(albumLayout, 1);
    //Add widget to the layout.
    QBoxLayout *metadataLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    metadataLayout->addWidget(m_albumArt);
    metadataLayout->addSpacing(10);
    metadataLayout->addWidget(m_releaseDateLabel);
    metadataLayout->addWidget(m_publishByLabel);
    metadataLayout->addStretch();
    albumLayout->addLayout(metadataLayout);
    //Initial the song list layout.
    QBoxLayout *songListLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    songListLayout->setContentsMargins(20, 0, 0, 0);
    songListLayout->setSpacing(0);
    albumLayout->addLayout(songListLayout, 1);
    //Add widget to layout.
    songListLayout->addWidget(m_albumTitle);
    songListLayout->addWidget(m_albumArtistLabel);
    songListLayout->addSpacing(30);
    songListLayout->addWidget(m_albumView);
    songListLayout->addStretch();

    //Link retranslator.
    knI18n->link(this, &KNMusicStorePageAlbum::retranslate);
    retranslate();
}

void KNMusicStorePageAlbum::reset()
{
    //Reset the album art data.
    m_albumArt->setPixmap(QPixmap(":/plugin/music/public/noalbum.png"));
}

void KNMusicStorePageAlbum::setPageLabel(int labelIndex, const QVariant &value)
{
    //Check the lable index.
    switch(labelIndex)
    {
    case AlbumMetadata:
    {
        //This contains the basic data of the album. It is a pack of JSON type
        //object.
        QJsonObject metadata=value.toJsonObject();
        // Album name, it should be a string type value.
        m_albumTitle->setText(metadata.value("name").toString());
        // Album artist, it should be a string type value.
        m_albumArtistLabel->setText(metadata.value("artist").toString());
        // Release date, it should be a long long type. UNIX timestamp.
        quint64 releaseTime=(quint64)metadata.value("release").toDouble();
        m_releaseTime=QDateTime::fromMSecsSinceEpoch(releaseTime).date();
        // Publish by, it should be a string type value.
        m_publishByLabel->setText(m_publishByText.arg(
                                      metadata.value("publish").toString()));
        // Song list, it is a JSON array.
        //Get the album model
        KNMusicStoreAlbumModel *albumModel=knMusicStoreGlobal->albumModel();
        //Clear current data.
        albumModel->clear();
        //Get the song list.
        QJsonArray songList=metadata.value("songs").toArray();
        //Reset the size.
        albumModel->reserve(songList.size());
        //Get over the list, construct the item.
        for(int i=0; i<songList.size(); ++i)
        {
            //Get the object.
            QJsonObject songItem=songList.at(i).toObject();
            //Parse the item.
            KNMusicStoreSongInfo songInfo;
            songInfo.index=songItem.value("index").toString();
            songInfo.name=songItem.value("name").toString();
            songInfo.artist=songItem.value("artist").toString();
            songInfo.artistId=songItem.value("artistId").toString();
            songInfo.duration=songItem.value("duration").toString();
            songInfo.customData=songItem.value("custom").toString();
            //Set the song info to model.
            albumModel->replace(i, songInfo);
        }
        //Update the metadata.
        updateMetadata();
        //Ask for show the page.
        emit requireShowPage();
        emit requireSetNavigatorItem(PageAlbum, m_albumTitle->text());
        //When this part is down.
        break;
    }
    case AlbumArt:
        //Parse the value as pixmap.
        m_albumArt->setPixmap(value.value<QPixmap>());
        break;
    }
}

void KNMusicStorePageAlbum::setBackend(KNMusicStoreBackend *backend)
{
    //Call the parent set backend.
    KNMusicStorePage::setBackend(backend);
    //Break all the previous connections.
    m_backendConnection.disconnectAll();
    //Link the backend to this page.
    m_backendConnection.append(
                connect(backend, &KNMusicStoreBackend::requireSetAlbum,
                        this, &KNMusicStorePageAlbum::setPageLabel));
    //Reset the current page.
    reset();
}

void KNMusicStorePageAlbum::showEvent(QShowEvent *event)
{
    //Do the parent show event.
    KNMusicStorePage::showEvent(event);
}

void KNMusicStorePageAlbum::retranslate()
{
    //Update release text.
    m_releaseText=tr("Release: %1");
    //Update metadata.
    updateMetadata();
}

void KNMusicStorePageAlbum::onAlbumRowCountChanged(int row)
{
    //Resize the album tree view.
    m_albumView->setFixedHeight(m_albumView->fixedRowHeight()*(row+1));
}

void KNMusicStorePageAlbum::onViewIndexClicked(const QModelIndex &index)
{
    //Check the index validation.
    if(!index.isValid())
    {
        //Ignore the invalid index.
        return;
    }
    //Get the model.
    KNMusicStoreAlbumModel *albumModel=knMusicStoreGlobal->albumModel();
    //Check the index column.
    switch(index.column())
    {
    case KNMusicStoreAlbumModel::AlbumModelName:
        //Ask the backend to show the song information.
        emit requireShowSingleSong(albumModel->metadata(index.row()));
        break;
    default:
        return;
    }
}

inline void KNMusicStorePageAlbum::updateMetadata()
{
    //Update the release text.
    m_releaseDateLabel->setText(m_releaseText.arg(
                                    m_releaseTime.toString(
                                        Qt::SystemLocaleLongDate)));
}
