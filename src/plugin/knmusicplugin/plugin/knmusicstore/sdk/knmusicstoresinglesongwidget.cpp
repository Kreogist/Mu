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
#include <QFormLayout>

#include "knhighlightlabel.h"
#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knanimelabelbutton.h"
#include "knlabelbutton.h"

#include "knmusicstoreglobal.h"
#include "knmusicstorebackend.h"
#include "knmusicstoresongdetailinfo.h"

#include "knmusicstoresinglesongwidget.h"

#include <QDebug>

KNMusicStoreSingleSongWidget::KNMusicStoreSingleSongWidget(QWidget *parent) :
    KNMusicStorePanel(parent),
    m_titleLabel(new QLabel(this)),
    m_lyricsLabel(new QLabel(this)),
    m_albumArt(new KNHighLightLabel(this)),
    m_songDetail(nullptr)
{
    setObjectName("MusicStoreWidget");
    //Set properties.
    setAutoFillBackground(true);
    setFixedWidth(knMusicStoreGlobal->storeContentWidth());
    //Register this widget to theme manager.
    knTheme->registerWidget(this);
    //Configure the widget.
    m_albumArt->setFixedSize(219, 219);
    //Configure the label text font.
    QFont labelFont=font();
    labelFont.setPixelSize(12);
    //Initial the properties labels.
    for(int i=0; i<PropertyCount; ++i)
    {
        //Initial the labels.
        m_properties[i]=new KNLabelButton(this);
        m_propertiesLabel[i]=new QLabel(this);
        //Set object name.
        // Property hint label.
        m_propertiesLabel[i]->setObjectName("MusicStoreWidget");
        knTheme->registerWidget(m_propertiesLabel[i]);
        // Property value label.
        m_properties[i]->setObjectName("MusicStoreTextLabel");
        knTheme->registerWidget(m_properties[i]);
        //Configure label.
        // Property hint label.
        m_propertiesLabel[i]->setAlignment(Qt::AlignRight);
        m_propertiesLabel[i]->setFont(labelFont);
        // Property value label.
        m_properties[i]->setFont(labelFont);
        m_properties[i]->setCursor(Qt::PointingHandCursor);
    }
    //Change the font size of title label.
    QFont titleFont=m_titleLabel->font();
    titleFont.setPixelSize(21);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    //Configure title label.
    m_titleLabel->setObjectName("MusicStoreTextLabel");
    knTheme->registerWidget(m_titleLabel);
    m_titleLabel->setWordWrap(true);
    //Configure the lyrics label.
    m_lyricsLabel->setWordWrap(true);
    m_lyricsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    //Link the header label.
    connect(headerLabel(), &KNAnimeLabelButton::clicked,
            this, &KNMusicStoreSingleSongWidget::onActionRefresh);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(40, 47, 40, 47);
    mainLayout->setSpacing(40);
    setLayout(mainLayout);
    //Add widget to main layout.
    mainLayout->addWidget(m_albumArt, 0, Qt::AlignLeft | Qt::AlignTop);
    //Initial the meta data layout.
    QBoxLayout *metaDataLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                              mainLayout->widget());
    metaDataLayout->setContentsMargins(0, 0, 0, 0);
    metaDataLayout->setSpacing(20);
    mainLayout->addLayout(metaDataLayout, 1);
    //Add title label first.
    metaDataLayout->addWidget(m_titleLabel);
    //Add the form layout
    QFormLayout *metaLayout=new QFormLayout(mainLayout->widget());
    metaLayout->setContentsMargins(0,0,0,0);
    metaLayout->setLabelAlignment(Qt::AlignTop | Qt::AlignRight);
    metaLayout->setHorizontalSpacing(0);
    metaLayout->setVerticalSpacing(11);
    metaDataLayout->addLayout(metaLayout);
    //Configure meta layout.
    //Add widget to layout.
    metaLayout->addRow(m_propertiesLabel[PropertyArtist],
                       m_properties[PropertyArtist]);
    metaLayout->addRow(m_propertiesLabel[PropertyAlbum],
                       m_properties[PropertyAlbum]);
    //Link the album as one button.
    connect(m_properties[PropertyAlbum], &KNLabelButton::clicked,
            this, &KNMusicStoreSingleSongWidget::onActionShowAlbum);
    //Add lyrics label.
    metaDataLayout->addWidget(m_lyricsLabel, 3);

    //Link retranslator.
    knI18n->link(this, &KNMusicStoreSingleSongWidget::retranslate);
    retranslate();
}

void KNMusicStoreSingleSongWidget::setBackend(KNMusicStoreBackend *backend)
{
    //Get the song detail object.
    m_songDetail=backend->getSongDetail();
    //Check pointer.
    if(!m_songDetail)
    {
        return;
    }
    //Link the update information.
    connect(backend, &KNMusicStoreBackend::fetchComplete,
            this, &KNMusicStoreSingleSongWidget::onActionDataUpdate);
}

void KNMusicStoreSingleSongWidget::retranslate()
{
    //Retranslate the song label.
    m_propertiesLabel[PropertyArtist]->setText(tr("Artist: "));
    m_propertiesLabel[PropertyAlbum]->setText(tr("Album: "));
}

void KNMusicStoreSingleSongWidget::onActionRefresh()
{
    //Update the reload the current information.
    emit requireShowSong(
                m_songDetail->songData(KNMusicStoreSongDetailInfo::SongId));
    //Start the Internet activity.
    emit startNetworkActivity();
}

void KNMusicStoreSingleSongWidget::onActionDataUpdate(int category)
{
    //Check category.
    if(category!=KNMusicStoreUtil::PanelSong)
    {
        //Ignore the category if the data is not mine.
        return;
    }
    //Update the title.
    m_titleLabel->setText(m_songDetail->songData(
                              KNMusicStoreSongDetailInfo::Name));
    //Update the header label text.
    headerLabel()->setText(fontMetrics().elidedText(
                               m_titleLabel->text(),
                               Qt::ElideRight,
                               knMusicStoreGlobal->storeHeaderLabelWidth()));
    //Show header widget.
    headerLabel()->show();
    bulletLabel()->show();
    //Update the album art.
    m_albumArt->setPixmap(m_songDetail->albumArt());
    //Update the content data.
    m_properties[PropertyArtist]->setText(m_songDetail->artists().join(
                                              tr(", ")));
    m_properties[PropertyAlbum]->setText(
                m_songDetail->songData(
                    KNMusicStoreSongDetailInfo::AlbumName));
    //Update the lyrics.
    QString lyricsText=m_songDetail->songData(
                KNMusicStoreSongDetailInfo::Lyrics);
    //Remove tags.
    lyricsText.remove(QRegExp("\\[[^\\]]*\\]"));
    m_lyricsLabel->setText(lyricsText);
    //Resize the widget.
    setMinimumHeight(m_titleLabel->height() +
                     m_properties[PropertyArtist]->height() +
                     m_properties[PropertyAlbum]->height() +
                     m_lyricsLabel->sizeHint().height() +
                     217);
}

void KNMusicStoreSingleSongWidget::onActionShowAlbum()
{
    //Check detail pointer first.
    if(!m_songDetail)
    {
        //Failed.
        return;
    }
    //Emit the start signal.
    emit startNetworkActivity();
    //Emit the show album signal.
    emit requireShowAlbum(m_songDetail->songData(
                              KNMusicStoreSongDetailInfo::AlbumId));
}
