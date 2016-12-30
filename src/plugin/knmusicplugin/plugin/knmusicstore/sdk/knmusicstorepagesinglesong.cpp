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
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>

#include "knlocalemanager.h"
#include "knthememanager.h"
#include "knanimelabelbutton.h"
#include "knopacityanimetextbutton.h"

#include "knmusicstorebackend.h"
#include "knmusicalbumlabel.h"

#include "knmusicstorepagesinglesong.h"

#include <QDebug>

#define MinLabelBrightness 0xB0
#define MaxLabelBrightness 0xFF

KNMusicStorePageSingleSong::KNMusicStorePageSingleSong(QWidget *parent) :
    KNMusicStorePage(parent),
    m_artistHintText(QString()),
    m_artistsHintText(QString()),
    m_albumMetadata(QString()),
    m_songUrl(QString()),
    m_highUrl(QString()),
    m_titleLabel(new QLabel(this)),
    m_subheadingLabel(new QLabel(this)),
    m_lyrics(new QLabel(this)),
    m_artistLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_albumLabel(new KNAnimeLabelButton(this)),
    m_download(new KNOpacityAnimeTextButton(this)),
    m_albumArt(new KNMusicAlbumLabel(this))
{
    updateObjectName("MusicStorePage");
    //Configure the header.
    QFont headerFont=font();
    headerFont.setPixelSize(21);
    headerFont.setBold(true);
    m_titleLabel->setFont(headerFont);
    //Configure the subheading.
    m_subheadingLabel->setContentsMargins(0, 0, 0, 10);
    m_subheadingLabel->setObjectName("MusicStorePageLabel");
    knTheme->registerWidget(m_subheadingLabel);
    //Configure the album label.
    m_albumLabel->setCursor(Qt::PointingHandCursor);
    m_albumLabel->setRange(MinLabelBrightness, MaxLabelBrightness);
    connect(m_albumLabel, &KNAnimeLabelButton::clicked,
            this, &KNMusicStorePageSingleSong::onAlbumClicked);
    //Configure the album art label.
    m_albumArt->setFixedSize(200, 200);
    //Configure the artist layout.
    m_artistLayout->setContentsMargins(0, 0, 0, 0);
    m_artistLayout->setSpacing(0);
    m_artistLayout->addStretch();
    //Configure the download button.
    connect(m_download, &KNOpacityAnimeTextButton::clicked,
            this, &KNMusicStorePageSingleSong::onActionDownloadSong);
    //Configure the lyrics display widget.
    m_lyrics->setContentsMargins(0, 20, 0, 0);
    m_lyrics->setObjectName("MusicStorePageText");
    knTheme->registerWidget(m_lyrics);
    //Initial hint labels.
    for(int i=0; i<LabelHintsCount; ++i)
    {
        //Initial the hint label.
        m_labelHints[i]=new QLabel(this);
        //Configure the label.
        m_labelHints[i]->setObjectName("MusicStorePageLabel");
        m_labelHints[i]->setAlignment(Qt::AlignRight | Qt::AlignTop);
        m_labelHints[i]->setContentsMargins(0, 0, 7, 0);
        //Add widget to theme manager.
        knTheme->registerWidget(m_labelHints[i]);
    }

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,
                                   KNMusicStoreUtil::headerHeight() + 36,
                                   0,
                                   KNMusicStoreUtil::headerHeight() + 36);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Initial the content layout.
    QBoxLayout *albumLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    albumLayout->setContentsMargins(30, 0, 30, 0);
    albumLayout->setSpacing(10);
    mainLayout->addLayout(albumLayout);
    //Add labels.
    albumLayout->addWidget(m_albumArt, 0, Qt::AlignLeft | Qt::AlignTop);
    //Initial metadata layout.
    QGridLayout *metadataLayout=new QGridLayout();
    metadataLayout->setContentsMargins(0, 0, 0, 0);
    metadataLayout->setSpacing(7);
    metadataLayout->setColumnStretch(1, 1);
    metadataLayout->setRowStretch(4, 1);
    albumLayout->addLayout(metadataLayout, 1);
    //Add widgets.
    // Name of the song.
    metadataLayout->addWidget(m_titleLabel, 0, 0, 1, 2);
    // Subheading of the song.
    metadataLayout->addWidget(m_subheadingLabel, 1, 0, 1, 2);
    // Artist of the song.
    metadataLayout->addWidget(m_labelHints[HintArtists], 2, 0, 1, 1);
    metadataLayout->addLayout(m_artistLayout, 2, 1, 1, 1);
    // Album of the song.
    metadataLayout->addWidget(m_labelHints[HintAlbum], 3, 0, 1, 1);
    metadataLayout->addWidget(m_albumLabel, 3, 1, 1, 1);
    //Initial the operation button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    metadataLayout->addLayout(buttonLayout, 4, 0, 1, 2);
    // Operation buttons.
    buttonLayout->addWidget(m_download);
    buttonLayout->addStretch();
    // Lyrics of the song.
    metadataLayout->addWidget(m_lyrics, 5, 0, 1, 2);
    //Add the other widgets.
    mainLayout->addStretch();

    //Add translate handler.
    knI18n->link(this, &KNMusicStorePageSingleSong::retranslate);
    retranslate();
}

void KNMusicStorePageSingleSong::setPageLabel(int labelIndex,
                                              const QVariant &value)
{
    //Check the label index.
    switch(labelIndex)
    {
    case SingleMetadata:
    {
        // All the metadata of the single song, this includes most of the text
        //data.
        QJsonObject metadata=value.toJsonObject();
        //The metadata contains the following value:
        // Song url, the lossless quality and the normal url.
        m_highUrl=metadata.value("lossless_url").toString();
        m_songUrl=metadata.value("normal_url").toString();
        // Song name, the value should be a string type value.
        m_titleLabel->setText(metadata.value("name").toString());
        // Song subheading, the value should be a string type value.
        m_subheadingLabel->setText(metadata.value("subheading").toString());
        // Song album, the value should be a string type value.
        m_albumLabel->setText(metadata.value("album").toString());
        m_albumMetadata=metadata.value("album_meta").toString();
        // Song artist list, the value should a string list.
        //Clear the artist layout widgets.
        clearArtistList();
        //Translate the artist list to a json list.
        QJsonArray artistList=metadata.value("artist").toArray();
        //Clear the string list.
        m_artistList.clear();
        //Check the size of the artist list.
        if(artistList.isEmpty())
        {
            //Construct a label with no artist.
            m_artistLayout->insertWidget(0, new QLabel("N/A", this));
        }
        else
        {
            //Clear the cache list.
            m_artistLabels.clear();
            m_artistSplitLabels.clear();
            //Update the artist list size.
            m_artistList.reserve(artistList.size());
            //Get the artist metadata.
            QJsonArray artistMetadata=metadata.value("artist_meta").toArray();
            //For each artist in the list, construct the mouse sense label.
            //The last one need to be process specially.
            while(artistList.size() > 1)
            {
                //Get the current artist position.
                int artistPosition=artistList.size()-1;
                //Access the artist in the list, construct the artist name label.
                insertArtistLabel(artistList.takeAt(artistPosition).toString(),
                                  artistMetadata.takeAt(
                                      artistPosition).toString());
                //Construct the sperate label.
                QLabel *sperateLabel=new QLabel(" / ", this);
                //Add the split label to the list.
                m_artistSplitLabels.append(sperateLabel);
                //Insert the sperate label.
                m_artistLayout->insertWidget(0, sperateLabel);
            }
            //For the last one, we only need to the construct the label button.
            //Access the artist in the list, insert the artist name label.
            insertArtistLabel(artistList.first().toString(),
                              artistMetadata.first().toString());
            //Update the artist hint label.
            updateArtistHintLabel();
            //Update all the label palette.
            updateArtistLabelPalette();
        }
        break;
    }
    case SingleLyrics:
        // Song lyrics, the value should be a string type value.
        m_lyrics->setText(value.toString());
        //Show the page.
        emit requireShowPage();
        emit requireSetNavigatorItem(PageSingleSong, m_titleLabel->text());
        //Update the page size.
        emit requireUpdateHeight();
        break;
    case SingleAlbumArt:
        //Parse the value as pixmap.
        m_albumArt->setPixmap(value.value<QPixmap>());
        break;
    }
}

void KNMusicStorePageSingleSong::reset()
{
    //Clear the data in the label.
    m_titleLabel->clear();
    m_subheadingLabel->clear();
    m_albumLabel->clear();
    m_albumArt->setPixmap(QPixmap(":/plugin/music/public/noalbum.png"));
    m_lyrics->clear();
    //Clear the artist list.
    clearArtistList();
}

void KNMusicStorePageSingleSong::setBackend(KNMusicStoreBackend *backend)
{
    //Do the parent set backend.
    KNMusicStorePage::setBackend(backend);
    //Break all the previous connections.
    m_backendConnection.disconnectAll();
    //Link the backend to this page.
    m_backendConnection.append(
                connect(backend, &KNMusicStoreBackend::requireSetSingleSong,
                        this, &KNMusicStorePageSingleSong::setPageLabel));
    //Reset the current page.
    reset();
}

void KNMusicStorePageSingleSong::showEvent(QShowEvent *event)
{
    //Show the widget.
    KNMusicStorePage::showEvent(event);
}

void KNMusicStorePageSingleSong::onPaletteChanged()
{
    //Do the original palette changed slot.
    KNMusicStorePage::onPaletteChanged();
    //Update all the artist label object palette.
    updateArtistLabelPalette();
    //Update the album label palette.
    m_albumLabel->updateObjectName("MusicStorePageText");
}

void KNMusicStorePageSingleSong::retranslate()
{
    //Translate the labels.
    m_artistHintText=tr("Artist");
    m_artistsHintText=tr("Artists");
    m_labelHints[HintAlbum]->setText(tr("Album"));
    //Update button.
    m_download->setText("  "+tr("Download")+"  ");
    //Update the artist hint label.
    updateArtistHintLabel();
}

void KNMusicStorePageSingleSong::onAlbumClicked()
{
    //Ask to show the album.
    emit requireShowAlbum(m_albumMetadata);
}

void KNMusicStorePageSingleSong::onActionDownloadSong()
{
    //Construct the artist list.
    QString artists;
    for(auto i : m_artistLabels)
    {
        //Update the item.
        if(!artists.isEmpty())
        {
            //Append the comma.
            artists.append(", ");
        }
        //Append the artist name.
        artists.append(i->text());
    }
    //Download the song.
    emit requireDownload(m_highUrl,
                         artists+" - "+m_titleLabel->text(),
                         m_titleLabel->text());
}

inline void KNMusicStorePageSingleSong::clearArtistList()
{
    //Clear the all the widget in the artist layout.
    while(m_artistLayout->count()>1)
    {
        //Take the item at position 0.
        QLayoutItem *widgetItem=m_artistLayout->takeAt(0);
        //All (except the last) items should be the widget item.
        //Get the widget.
        QWidget *removedLabel=widgetItem->widget();
        //Reset the parent.
        removedLabel->setParent(nullptr);
        //Delete the widget.
        removedLabel->deleteLater();
        //Remove the widget item.
        delete widgetItem;
    }
}

inline void KNMusicStorePageSingleSong::insertArtistLabel(
        const QString &artistName, const QString &artistMetadata)
{
    //Insert the data to the list,
    m_artistList.prepend(artistMetadata);
    //Construct a label.
    KNAnimeLabelButton *artistLabel=new KNAnimeLabelButton(this);
    //Set the text.
    artistLabel->setText(artistName);
    //Set artist label property.
    artistLabel->setCursor(Qt::PointingHandCursor);
    artistLabel->setRange(MinLabelBrightness, MaxLabelBrightness);
    //Insert the label to the layout.
    m_artistLayout->insertWidget(0, artistLabel);
    //Append the artist label.
    m_artistLabels.append(artistLabel);
}

inline void KNMusicStorePageSingleSong::updateArtistHintLabel()
{
    //Check the size of the list.
    m_labelHints[HintArtists]->setText(m_artistLayout->count()<3?
                                           //For only 1 artist
                                           m_artistHintText:
                                           //For more than 1 artist
                                           m_artistsHintText);
}

inline void KNMusicStorePageSingleSong::updateArtistLabelPalette()
{
    //Update all the artist label item.
    for(auto i : m_artistLabels)
    {
        //Update the item.
        i->updateObjectName("MusicStorePageText");
    }
    //Get the label palette.
    QPalette labelPalette=knTheme->getPalette("MusicStorePageLabel");
    //For all the item in the label list.
    for(auto i : m_artistSplitLabels)
    {
        //Get the list.
        i->setPalette(labelPalette);
    }
}
