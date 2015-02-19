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
#include <QStandardItemModel>

#include "knmusiclyricsitemdelegate.h"
#include "knmusiclyricsmanager.h"
#include "knmusiclyricsdownloadwidget.h"

#include "knmusiclyricsdownloaddialog.h"

KNMusicLyricsDownloadDialog::KNMusicLyricsDownloadDialog(QWidget *parent) :
    KNMusicLyricsDownloadDialogBase(parent)
{
    //Initial the lyrics manager.
    m_lyricsManager=KNMusicGlobal::instance()->lyricsManager();

    //Set properties.
    setTitle("Lyrics");

    //Initial the lyrics model.
    m_lyricsModel=new QStandardItemModel(this);

    //Initial the download widget, set the download widget as the content.
    m_downloadWidget=new KNMusicLyricsDownloadWidget(this);
    m_downloadWidget->setLyricsModel(m_lyricsModel);
    setContent(m_downloadWidget);
    connect(m_downloadWidget, &KNMusicLyricsDownloadWidget::requireSearchLyrics,
            this, &KNMusicLyricsDownloadDialog::onActionSearchLyrics);
}

KNMusicLyricsDownloadDialog::~KNMusicLyricsDownloadDialog()
{
    ;
}

void KNMusicLyricsDownloadDialog::setDetailInfo(const KNMusicDetailInfo &detailInfo)
{
    //Clear the original lyrics model.
    m_lyricsModel->clear();
    //Save the detail info.
    m_detailInfo=detailInfo;

    //Set text data.
    m_downloadWidget->setTitle(m_detailInfo.textLists[Name]);
    m_downloadWidget->setArtist(m_detailInfo.textLists[Artist]);
}

void KNMusicLyricsDownloadDialog::onActionSearchLyrics()
{
    //Clear the lyrics model.
    m_lyricsModel->clear();
    //Generate a new detail info, set the title and artist.
    KNMusicDetailInfo lyricsDetailInfo=m_detailInfo;
    lyricsDetailInfo.textLists[Name]=m_downloadWidget->title();
    lyricsDetailInfo.textLists[Artist]=m_downloadWidget->artist();
    //Get all the lyrics from Internet.
    QList<KNMusicLyricsDetails> lyricsList;
    m_lyricsManager->getOnlineLyrics(lyricsDetailInfo, lyricsList);
    //Set the lyrics data to the model.
    for(QList<KNMusicLyricsDetails>::iterator i=lyricsList.begin();
        i!=lyricsList.end();
        ++i)
    {
        QStandardItem *lyricsItem=new QStandardItem((*i).title);
        lyricsItem->setData((*i).artist, LyricsArtistRole);
        lyricsItem->setData((*i).titleSimilarity, LyricsTitleSimilarityRole);
        lyricsItem->setData((*i).artistSimilarity, LyricsArtistSimilarityRole);
        m_lyricsModel->appendRow(lyricsItem);
    }
}
