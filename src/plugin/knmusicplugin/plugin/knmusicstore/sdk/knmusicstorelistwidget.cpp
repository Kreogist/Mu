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

#include "knanimelabelbutton.h"
#include "knhighlightlabel.h"
#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusicglobal.h"
#include "knmusicstorebackend.h"
#include "knmusicstorealbummodel.h"
#include "knmusicstoreglobal.h"
#include "knmusicstorealbumtreeview.h"

#include "knmusicstorelistwidget.h"

using namespace MusicStoreUtil;

KNMusicStoreListWidget::KNMusicStoreListWidget(QWidget *parent) :
    KNMusicStorePanel(parent),
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
    //Link the header label.
    connect(headerLabel(), &KNAnimeLabelButton::clicked,
            this, &KNMusicStoreListWidget::onActionRefresh);

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
    //Set the resize mode.
    m_albumSongView->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_albumSongView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_albumSongView->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_albumSongView->header()->setSectionResizeMode(3, QHeaderView::Fixed);
    //Link the backend.
    connect(backend, &KNMusicStoreBackend::fetchComplete,
            this, &KNMusicStoreListWidget::onActionUpdateInfo);
    connect(m_albumSongView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this, &KNMusicStoreListWidget::onActionShowSongInfo);
}

void KNMusicStoreListWidget::retranslate()
{
    //Update the header label.
    headerLabel()->setText(fontMetrics().elidedText(
                               m_titleLabel->text(),
                               Qt::ElideRight,
                               knMusicStoreGlobal->storeHeaderLabelWidth()));
    //Update label.
    m_propertiesLabel[PropertyArtist]->setText(tr("Artist: "));
    m_propertiesLabel[PropertyReleaseDate]->setText(tr("Released: "));
    m_propertiesLabel[PropertyCompany]->setText(tr("Published: "));
}

void KNMusicStoreListWidget::onActionRefresh()
{
    //Ask to reload the album information.
    emit requireShowAlbum(
                m_albumDetailModel->albumInfo(StoreAlbumId));
    //Start network activity.
    emit startNetworkActivity();
}

void KNMusicStoreListWidget::onActionUpdateInfo(int category)
{
    //Check model pointer and category first.
    if(m_albumDetailModel==nullptr || category!=PanelList)
    {
        //Ignore invalid model settings.
        return;
    }
    //Set the album art.
    m_albumArt->setPixmap(m_albumDetailModel->albumArt().isNull() ?
                              knMusicGlobal->noAlbumArt() :
                              m_albumDetailModel->albumArt());
    //Set the label title.
    m_titleLabel->setText(m_albumDetailModel->albumInfo(StoreAlbumTitle));
    headerLabel()->setText(fontMetrics().elidedText(
                               m_titleLabel->text(),
                               Qt::ElideRight,
                               knMusicStoreGlobal->storeHeaderLabelWidth()));
    //Show header widgets.
    headerLabel()->show();
    bulletLabel()->show();
    //Set the content data.
    m_properties[PropertyArtist]->setText(
                m_albumDetailModel->albumInfo(StoreAlbumArtist));
    m_properties[PropertyReleaseDate]->setText(
                m_albumDetailModel->albumInfo(StoreAlbumReleaseTime));
    m_properties[PropertyCompany]->setText(
                m_albumDetailModel->albumInfo(StoreAlbumReleaseCompany));
    //Update treeview size.
    m_albumSongView->setMinimumHeight(StoreAlbumHeaderHeight +
                                      m_albumDetailModel->rowCount() *
                                      SongItemHeight);
    //Enable the tree view widget.
    m_albumSongView->setEnabled(true);
    //Resize current widget size.
    setMinimumHeight(sizeHint().height());
}

void KNMusicStoreListWidget::onActionShowSongInfo(
        const QModelIndex &currentIndex)
{
    //Get the current index.
    if(m_albumDetailModel==nullptr)
    {
        //Ignore the null the pointer.
        return;
    }
    //Get the index from the model.
    if(currentIndex.isValid())
    {
        //Disable the tree view widget.
        m_albumSongView->setEnabled(false);
        //Ask to show the song information.
        emit requireShowSong(m_albumDetailModel->songData(currentIndex));
        //Ask to start the network activity.
        emit startNetworkActivity();
    }
}
