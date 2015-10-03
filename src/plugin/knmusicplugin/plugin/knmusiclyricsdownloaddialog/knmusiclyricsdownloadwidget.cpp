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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QPushButton>

#include "knlabellineedit.h"
#include "knlocalemanager.h"

#include "knmusicbackend.h"
#include "knmusiclyricsdownloadlist.h"
#include "knmusiclyricsmanager.h"
#include "knmusiconlinelyricsdownloader.h"
#include "knmusicglobal.h"

#include "knmusiclyricsdownloadwidget.h"

#include <QDebug>

KNMusicLyricsDownloadWidget::KNMusicLyricsDownloadWidget(QWidget *parent) :
    QWidget(parent),
    m_detailInfo(KNMusicDetailInfo()),
    m_title(generateLineEdit(":/plugin/music/lyrics/title.png")),
    m_artist(generateLineEdit(":/plugin/music/lyrics/artist.png")),
    m_searchLyrics(new QPushButton(this)),
    m_searchTextLayout(nullptr),
    m_downloadedLyrics(new KNMusicLyricsDownloadList(this)),
    m_onlineDownloader(knMusicGlobal->lyricsManager()->onlineLyricsDownloader())
{
    //Set properties.
    setContentsMargins(15, 15, 15, 15);

    //Configure the title line edit.
    setTabOrder(m_title, m_artist);
    //Configure the search button.
    setTabOrder(m_artist, m_searchLyrics);
    m_searchLyrics->setDefault(true);
    // Set the palette for the search lyrics button.
    QPalette buttonPalette=m_searchLyrics->palette();
    buttonPalette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    m_searchLyrics->setPalette(buttonPalette);
    // Link the click signal.
    connect(m_searchLyrics, &QPushButton::clicked,
            this, &KNMusicLyricsDownloadWidget::onActionSearch);
    //Configure the download list.
    m_downloadedLyrics->setVisible(false);
    connect(m_downloadedLyrics, &KNMusicLyricsDownloadList::requireShowOkay,
            this, &KNMusicLyricsDownloadWidget::requireShowOkayButton);
    connect(m_downloadedLyrics, &KNMusicLyricsDownloadList::requireHideOkay,
            this, &KNMusicLyricsDownloadWidget::requireHideOkayButton);
    //Link the online lyrics downloader.
    connect(this, &KNMusicLyricsDownloadWidget::requireDownloadLyrics,
            m_onlineDownloader,
            &KNMusicOnlineLyricsDownloader::downloadLyrics);
    connect(m_onlineDownloader, &KNMusicOnlineLyricsDownloader::serverChanged,
            m_downloadedLyrics,
            &KNMusicLyricsDownloadList::setDownloadServerText);
    connect(m_onlineDownloader,
            &KNMusicOnlineLyricsDownloader::downloadComplete,
            this, &KNMusicLyricsDownloadWidget::onActionDownloadComplete);
    connect(m_onlineDownloader,
            &KNMusicOnlineLyricsDownloader::listContentChanged,
            m_downloadedLyrics, &KNMusicLyricsDownloadList::setLyricsList);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    //Initial the search text layout.
    m_searchTextLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                      mainLayout->widget());
    m_searchTextLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(m_searchTextLayout);
    //Initial the text layout.
    QBoxLayout *textLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          m_searchTextLayout->widget());
    textLayout->setContentsMargins(0,0,0,0);
    m_searchTextLayout->addLayout(textLayout, 1);
    //Add widget to text layout.
    textLayout->addWidget(m_title);
    textLayout->addWidget(m_artist);
    //Add search lyrics button to search text layout.
    m_searchTextLayout->addWidget(m_searchLyrics);
    //Add download lyrics to the main layout.
    mainLayout->addWidget(m_downloadedLyrics);

    //Link the retranslate link.
    knI18n->link(this, &KNMusicLyricsDownloadWidget::retranslate);
    retranslate();
}

void KNMusicLyricsDownloadWidget::setDetailInfo(
        const KNMusicDetailInfo &detailInfo)
{
    //Save the detail info.
    m_detailInfo=detailInfo;
    //Set the title and artist text.
    m_title->setText(m_detailInfo.textLists[Name].toString());
    m_artist->setText(m_detailInfo.textLists[Artist].toString());
    //Once we do this, means we have to show up in a new instance.
    //Enabled search edits.
    enableSearchEdit();
    //Hide the download list.
    m_downloadedLyrics->setVisible(false);
    //Hide all the widgets in the download lyrics.
    m_downloadedLyrics->hideAllWidgets();
    //Resize the widget.
    resize(255, 76);
    //Get the backend.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the backend pointer.
    if(backend)
    {
        //Load the music in the preview threads.
        backend->previewLoadMusic(detailInfo.filePath,
                                  detailInfo.startPosition,
                                  detailInfo.duration);
    }
}

void KNMusicLyricsDownloadWidget::saveSelectLyrics()
{
    //Save the lyrics file by calling the lyrics manager.
    knMusicGlobal->lyricsManager()->saveLyricsAndUpdateBackend(
                m_detailInfo,
                m_downloadedLyrics->currentLyricsData());
}

void KNMusicLyricsDownloadWidget::retranslate()
{
    //Update search lyrics.
    m_searchLyrics->setText(tr("Search"));
    //Update the place holder text.
    m_title->setPlaceholderText(tr("Name"));
    m_artist->setPlaceholderText(tr("Artist"));
}

void KNMusicLyricsDownloadWidget::onActionSearch()
{
    //Expand the dialog.
    emit requireExpand();
    //Show up the download list.
    m_downloadedLyrics->setVisible(true);
    //Hide the search button.
    m_searchLyrics->hide();
    //Disabled the text edit.
    m_title->setEnabled(false);
    m_artist->setEnabled(false);
    //Show the download widget.
    m_downloadedLyrics->showDownloadWidgets();
    //Get a deep copy of the detail info.
    KNMusicDetailInfo detailInfo=m_detailInfo;
    //Update the detail info.
    detailInfo.textLists[Name]=m_title->text();
    detailInfo.textLists[Artist]=m_artist->text();
    //Ask to download the lyrics.
    emit requireDownloadLyrics(detailInfo);
}

void KNMusicLyricsDownloadWidget::hideEvent(QHideEvent *event)
{
    //Do the hide first.
    QWidget::hideEvent(event);
    //Hide all the widget of the download lyrics list.
    m_downloadedLyrics->hideAllWidgets();
    //Check out the online downloader status.
    if(m_onlineDownloader->isRunning())
    {
        //Stop the online downloader.
        m_onlineDownloader->cancelDownload();
    }
}

void KNMusicLyricsDownloadWidget::onActionDownloadCancel()
{
    ;
}

void KNMusicLyricsDownloadWidget::onActionDownloadComplete()
{
    //Hide the download widgets.
    m_downloadedLyrics->hideDownloadWidgets();
    //Enabled the search edit.
    enableSearchEdit();
}

inline void KNMusicLyricsDownloadWidget::enableSearchEdit()
{
    //Enabled text edits.
    m_title->setEnabled(true);
    m_artist->setEnabled(true);
    //Show the search button.
    m_searchLyrics->show();
}

inline KNLabelLineEdit *KNMusicLyricsDownloadWidget::generateLineEdit(
        const QString &pixmapPath)
{
    //Generate the widget.
    KNLabelLineEdit *lineEdit=new KNLabelLineEdit(this);
    //Set properties.
    lineEdit->setMinimumLightness(0xC0);
    lineEdit->setMediumLightness(0xE0);
    //Set the pixmap.
    lineEdit->setLabelIcon(QPixmap(pixmapPath));
    //Give back the line edit widget.
    return lineEdit;
}
