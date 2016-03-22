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
#include <QHeaderView>

#include "knhighlightlabel.h"
#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusicglobal.h"
#include "knmusicstorebackend.h"
#include "knmusicstorealbummodel.h"
#include "knmusicstoreglobal.h"
#include "knmusicstorealbumtreeview.h"

#include "knmusicstorelistwidget.h"

KNMusicStoreListWidget::KNMusicStoreListWidget(QWidget *parent) :
    QWidget(parent),
    m_titleLabel(new QLabel(this)),
    m_albumArt(new KNHighLightLabel(this)),
    m_albumDetailModel(nullptr),
    m_albumSongView(new KNMusicStoreAlbumTreeView(this))
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
        m_properties[i]=new QLabel(this);
        m_propertiesLabel[i]=new QLabel(this);
        //Set object name.
        m_propertiesLabel[i]->setObjectName("MusicStoreWidget");
        knTheme->registerWidget(m_propertiesLabel[i]);
        m_properties[i]->setObjectName("MusicStoreTextLabel");
        knTheme->registerWidget(m_properties[i]);
        //Configure label.
        m_propertiesLabel[i]->setAlignment(Qt::AlignRight);
        m_propertiesLabel[i]->setFont(labelFont);
        m_properties[i]->setFont(labelFont);
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
    //Configure the song view.
    m_albumSongView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_albumSongView->updateObjectName("MusicStoreAlbumView");

    //Initial the global layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(40, 47, 40, 47);
    setLayout(mainLayout);
    //Initial the top layouts.
    QBoxLayout *infoLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          mainLayout->widget());
    infoLayout->setSpacing(31);
    mainLayout->addLayout(infoLayout);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(m_albumSongView, 2);
    //Add widget to infor layout.
    infoLayout->addWidget(m_albumArt);
    //Initial the meta data layout.
    QBoxLayout *metaDataLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                              mainLayout->widget());
    metaDataLayout->setSpacing(0);
    infoLayout->addLayout(metaDataLayout, 1);
    //Add title label first.
    metaDataLayout->addWidget(m_titleLabel);
    metaDataLayout->addSpacing(20);
    //Add the form layout
    QFormLayout *metaLayout=new QFormLayout(infoLayout->widget());
    metaDataLayout->addLayout(metaLayout);
    metaDataLayout->addStretch();
    //Configure meta layout.
    metaLayout->setLabelAlignment(Qt::AlignTop | Qt::AlignRight);
    metaLayout->setVerticalSpacing(11);
    //Add widget to layout.
    metaLayout->addRow(m_propertiesLabel[PropertyArtist],
                       m_properties[PropertyArtist]);
    metaLayout->addRow(m_propertiesLabel[PropertyReleaseDate],
                       m_properties[PropertyReleaseDate]);
    metaLayout->addRow(m_propertiesLabel[PropertyCompany],
                       m_properties[PropertyCompany]);

    //Link the locale manager.
    knI18n->link(this, &KNMusicStoreListWidget::retranslate);
    retranslate();
}

void KNMusicStoreListWidget::setBackend(KNMusicStoreBackend *backend)
{
    //Check backend pointer.
    if(backend==nullptr)
    {
        //Ignore request.
        return;
    }
    //Save detail model.
    m_albumDetailModel=backend->albumDetailModel();
    //Set the model to tree view.
    m_albumSongView->setModel(m_albumDetailModel);
    //Link the backend.
    connect(m_albumDetailModel, &KNMusicStoreAlbumModel::albumDetailUpdated,
            this, &KNMusicStoreListWidget::onActionUpdateInfo);
}

void KNMusicStoreListWidget::retranslate()
{
    m_propertiesLabel[PropertyArtist]->setText(tr("Artist: "));
    m_propertiesLabel[PropertyReleaseDate]->setText(tr("Released: "));
    m_propertiesLabel[PropertyCompany]->setText(tr("Published: "));
}

void KNMusicStoreListWidget::onActionUpdateInfo()
{
    //Check model pointer first.
    if(m_albumDetailModel==nullptr)
    {
        //Ignore invalid model settings.
        return;
    }
    //Set the album art.
    m_albumArt->setPixmap(m_albumDetailModel->albumArt().isNull() ?
                              knMusicGlobal->noAlbumArt() :
                              m_albumDetailModel->albumArt());
    //Set the label title.
    m_titleLabel->setText(
                m_albumDetailModel->albumInfo(KNMusicStoreUtil::AlbumTitle));
    //Set the content data.
    m_properties[PropertyArtist]->setText(
                m_albumDetailModel->albumInfo(KNMusicStoreUtil::AlbumArtist));
    m_properties[PropertyReleaseDate]->setText(
                m_albumDetailModel->albumInfo(
                    KNMusicStoreUtil::AlbumReleaseTime));
    m_properties[PropertyCompany]->setText(
                m_albumDetailModel->albumInfo(
                    KNMusicStoreUtil::AlbumReleaseCompany));
    //Update treeview size.
    m_albumSongView->setMinimumHeight(m_albumSongView->header()->height() +
                                      m_albumDetailModel->rowCount() *
                                      AlbumSongItemHeight);
    //Resize current widget size.
    setMinimumHeight(sizeHint().height());
}
