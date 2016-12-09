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

#include "knlocalemanager.h"
#include "knthememanager.h"
#include "knanimelabelbutton.h"

#include "knmusicalbumlabel.h"

#include "knmusicstorepagesinglesong.h"

#define MinLabelBrightness 0xB0
#define MaxLabelBrightness 0xFF

KNMusicStorePageSingleSong::KNMusicStorePageSingleSong(QWidget *parent) :
    KNMusicStorePage(parent),
    m_artistHintText(QString()),
    m_artistsHintText(QString()),
    m_headerLabel(new QLabel(this)),
    m_subheadingLabel(new QLabel(this)),
    m_lyrics(new QLabel(this)),
    m_artistLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_albumLabel(new KNAnimeLabelButton(this)),
    m_albumArt(new KNMusicAlbumLabel(this))
{
    updateObjectName("MusicStoreSingleSong");
    //Configure the header.
    QFont headerFont=font();
    headerFont.setPixelSize(21);
    headerFont.setBold(true);
    m_headerLabel->setFont(headerFont);
    //Configure the subheading.
    m_subheadingLabel->setContentsMargins(0, 0, 0, 10);
    m_subheadingLabel->setObjectName("MusicStoreSingleSongLabel");
    knTheme->registerWidget(m_subheadingLabel);
    //Configure the album label.
    m_albumLabel->setCursor(Qt::PointingHandCursor);
    m_albumLabel->setRange(MinLabelBrightness, MaxLabelBrightness);
    //Configure the album art label.
    m_albumArt->setFixedSize(200, 200);
    m_albumArt->setArtwork(QPixmap(":/plugin/music/public/noalbum.png"));
    //Configure the artist layout.
    m_artistLayout->setContentsMargins(0, 0, 0, 0);
    m_artistLayout->setSpacing(0);
    m_artistLayout->addStretch();
    //Configure the lyrics display widget.
    m_lyrics->setContentsMargins(0, 20, 0, 0);
    m_lyrics->setObjectName("MusicStoreSingleSongText");
    knTheme->registerWidget(m_lyrics);
    //Initial hint labels.
    for(int i=0; i<LabelHintsCount; ++i)
    {
        //Initial the hint label.
        m_labelHints[i]=new QLabel(this);
        //Configure the label.
        m_labelHints[i]->setObjectName("MusicStoreSingleSongLabel");
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
                                   0, 0);
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
    metadataLayout->addWidget(m_headerLabel, 0, 0, 1, 2);
    // Subheading of the song.
    metadataLayout->addWidget(m_subheadingLabel, 1, 0, 1, 2);
    // Artist of the song.
    metadataLayout->addWidget(m_labelHints[HintArtists], 2, 0, 1, 1);
    metadataLayout->addLayout(m_artistLayout, 2, 1, 1, 1);
    // Album of the song.
    metadataLayout->addWidget(m_labelHints[HintAlbum], 3, 0, 1, 1);
    metadataLayout->addWidget(m_albumLabel, 3, 1, 1, 1);
    // Lyrics of the song.
    metadataLayout->addWidget(m_lyrics, 4, 0, 1, 2);
    //Add the other widgets.
    mainLayout->addStretch();

    //Add translate handler.
    knI18n->link(this, &KNMusicStorePageSingleSong::retranslate);
    retranslate();

    //Debug
    setSingleSong(SingleName, "カチューシャ");
    setSingleSong(SingleSubheading, "《少女与战车》插曲");
    QStringList artistList;
    artistList << "a" << "b" << "c" << "d" << "e";
    setSingleSong(SingleArtist, artistList);
    artistList.clear();
    artistList << "上坂すみれ" << "金元寿子";
    setSingleSong(SingleArtist, artistList);
    setSingleSong(SingleAlbum, "TVアニメ ガールズ&パンツァー オリジナルサウンドトラック");
    setSingleSong(SingleLyrics,
                  "Расцветали яблони и груши\n"
                  "Поплыли туманы над рекой\n"
                  "Выходила на берег Катюша\n"
                  "На высокий берег，на крутой.\n"
                  "Выходила на берег Катюша\n"
                  "На высокий берег，на крутой.\n"
                  "Выходила,песню заводила\n"
                  "Про степного，сизого орла,\n"
                  "Про того，которого любила,\n"
                  "Про того，чьи письма берегла.\n"
                  "Про того，которого любила,\n"
                  "Про того，чьи письма берегла.\n"
                  "Ой,ты песня，песенка девичья,\n"
                  "Ты лети за ясным солнцем вслед，\n"
                  "И бойцу на дальнем пограничье\n"
                  "От Катюши передай привет.\n"
                  "И бойцу на дальнем пограничье\n"
                  "От Катюши передай привет.\n"
                  "\n"
                  "Пусть он вспомнит девушку простую,\n"
                  "Пусть услышит，как она поёт,\n"
                  "Пусть он землю бережёт родную，\n"
                  "А любовь Катюша сбережёт.\n"
                  "Пусть он землю бережёт родную，\n"
                  "А любовь Катюша сбережёт.\n"
                  "Расцветали яблони и груши,\n"
                  "Поплыли туманы над рекой\n"
                  "Выходила на берег Катюша\n"
                  "На высокий берег，на крутой.\n"
                  "Выходила на берег Катюша\n"
                  "На высокий берег，на крутой.\n");
}

void KNMusicStorePageSingleSong::setSingleSong(int labelIndex,
                                               const QVariant &value)
{
    //Check the label index.
    switch(labelIndex)
    {
    case SingleName:
        // Song name, the value should be a string type value.
        m_headerLabel->setText(value.toString());
        break;
    case SingleSubheading:
        // Song subheading, the value should be a string type value.
        m_subheadingLabel->setText(value.toString());
        break;
    case SingleArtist:
    {
        // Song artist list, the value should a string list.
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
        //Translate the artist list to a string list.
        QStringList artistList=value.toStringList();
        //Check the size of the artist list.
        if(artistList.isEmpty())
        {
            //Construct a label with no artist.
            m_artistLayout->insertWidget(0, new QLabel("N/A", this));
            //Get out of the switch.
            break;
        }
        //Clear the cache list.
        m_artistLabels.clear();
        //For each artist in the list, construct the mouse sense label.
        //The last one need to be process specially.
        while(artistList.size() > 1)
        {
            //Access the artist in the list, construct the artist name label.
            insertArtistLabel(artistList.takeLast());
            //Construct the sperate label.
            QLabel *sperateLabel=new QLabel(" / ", this);
            //Add the split label to the list.
            m_artistSplitLabels.append(sperateLabel);
            //Insert the sperate label.
            m_artistLayout->insertWidget(0, sperateLabel);
        }
        //For the last one, we only need to the construct the label button.
        //Access the artist in the list, insert the artist name label.
        insertArtistLabel(artistList.at(0));
        //Update the artist hint label.
        updateArtistHintLabel();
        //Update all the label palette.
        updateArtistLabelPalette();
        break;
    }
    case SingleAlbum:
        // Song album, the value should be a string type value.
        m_albumLabel->setText(value.toString());
        break;
    case SingleLyrics:
        // Song lyrics, the value should be a string type value.
        m_lyrics->setText(value.toString());
        break;
    }
}

void KNMusicStorePageSingleSong::onPaletteChanged()
{
    //Do the original palette changed slot.
    KNMusicStorePage::onPaletteChanged();
    //Update all the artist label object palette.
    updateArtistLabelPalette();
    //Update the album label palette.
    m_albumLabel->updateObjectName("MusicStoreSingleSongText");
}

void KNMusicStorePageSingleSong::retranslate()
{
    //Translate the labels.
    m_artistHintText=tr("Artist");
    m_artistsHintText=tr("Artists");
    m_labelHints[HintAlbum]->setText(tr("Album"));
    //Update the artist hint label.
    updateArtistHintLabel();
}

inline void KNMusicStorePageSingleSong::insertArtistLabel(
        const QString &artistName)
{
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
    m_labelHints[HintArtists]->setText(m_artistHintText);
}

inline void KNMusicStorePageSingleSong::updateArtistLabelPalette()
{
    //Update all the artist label item.
    for(auto i : m_artistLabels)
    {
        //Update the item.
        i->updateObjectName("MusicStoreSingleSongText");
    }
    //Get the label palette.
    QPalette labelPalette=knTheme->getPalette("MusicStoreSingleSongLabel");
    //For all the item in the label list.
    for(auto i : m_artistSplitLabels)
    {
        //Get the list.
        i->setPalette(labelPalette);
    }
}
