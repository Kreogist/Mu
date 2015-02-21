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

#include "knmusicbackend.h"
#include "knmusiclyricsitemdelegate.h"
#include "knmusiclyricsmanager.h"
#include "knmusiclyricsdownloadwidget.h"

#include "knmusiclyricsdownloaddialog.h"

KNMusicLyricsDownloadDialog::KNMusicLyricsDownloadDialog(QWidget *parent) :
    KNMusicLyricsDownloadDialogBase(parent)
{
    //Initial the lyrics manager.
    m_lyricsManager=KNMusicLyricsManager::instance();

    //Set properties.
    enableCancel();
    setTitle("Lyrics");

    //Initial the lyrics model.
    m_lyricsModel=new QStandardItemModel(this);

    //Initial the download widget, set the download widget as the content.
    m_downloadWidget=new KNMusicLyricsDownloadWidget(this);
    m_downloadWidget->setLyricsModel(m_lyricsModel);
    setContent(m_downloadWidget);
    connect(m_downloadWidget, &KNMusicLyricsDownloadWidget::requireSearchLyrics,
            this, &KNMusicLyricsDownloadDialog::onActionSearchLyrics);
    connect(m_downloadWidget, &KNMusicLyricsDownloadWidget::lyricsActivate,
            this, &KNMusicLyricsDownloadDialog::onActionLyricsActivate);
}

KNMusicLyricsDownloadDialog::~KNMusicLyricsDownloadDialog()
{
    ;
}

void KNMusicLyricsDownloadDialog::setDetailInfo(const KNMusicDetailInfo &detailInfo)
{
    //Clear the original lyrics model.
    m_lyricsModel->clear();
    //Clear the preview.
    m_downloadWidget->clearPreview();

    //Save the detail info.
    m_detailInfo=detailInfo;

    //Set text data.
    m_downloadWidget->setTitle(m_detailInfo.textLists[Name]);
    m_downloadWidget->setArtist(m_detailInfo.textLists[Artist]);
    m_downloadWidget->linkBackend();

    //Load the file to backend preview.
    m_backend->loadPreview(m_detailInfo.filePath);
    //Check if we need to set a section.
    if(m_detailInfo.startPosition!=-1)
    {
        m_backend->setPreviewSection(m_detailInfo.startPosition,
                                     m_detailInfo.duration);
    }
}

void KNMusicLyricsDownloadDialog::setBackend(KNMusicBackend *backend)
{
    m_backend=backend;
    //Set the backend.
    m_downloadWidget->setBackend(m_backend);
}

void KNMusicLyricsDownloadDialog::onActionSearchComplete()
{
    //Switch to lyrics list.
    m_downloadWidget->showLyricsList();
}

bool KNMusicLyricsDownloadDialog::onActionOkayClose()
{
    //Cut down the links.
    m_downloadWidget->resetPreviewPlayer();
    //Check whether there's any select item.
    QModelIndex currentLyricsIndex=m_downloadWidget->currentLyricsIndex();
    if(currentLyricsIndex.isValid())
    {
        QStandardItem *lyricsItem=m_lyricsModel->itemFromIndex(currentLyricsIndex);
        if(lyricsItem!=nullptr)
        {
            //Save the lyrics to the current data.
            QString lyricsFilePath=m_lyricsManager->saveLyrics(m_detailInfo,
                                                               lyricsItem->data(LyricsTextRole).toString());
            //Check the lyircs manager's current is playing the file or not.
            if(m_lyricsManager->musicDetailInfo().filePath==m_detailInfo.filePath
                    && m_lyricsManager->musicDetailInfo().trackFilePath==m_detailInfo.trackFilePath
                    && m_lyricsManager->musicDetailInfo().trackIndex==m_detailInfo.trackIndex)
            {
                m_lyricsManager->loadLyricsFile(lyricsFilePath);
                //Ask to update lyrics for datas.
                m_lyricsManager->lyricsUpdate();
            }
        }
    }
    return true;
}

void KNMusicLyricsDownloadDialog::onActionCancelClose()
{
    //Cut down the links.
    m_downloadWidget->resetPreviewPlayer();
}

void KNMusicLyricsDownloadDialog::onActionSearchLyrics()
{
    //Switch to loading wheel.
    m_downloadWidget->showLoadingWheel();
    //Generate a new detail info, set the title and artist.
    KNMusicDetailInfo lyricsDetailInfo=m_detailInfo;
    lyricsDetailInfo.textLists[Name]=m_downloadWidget->title();
    lyricsDetailInfo.textLists[Artist]=m_downloadWidget->artist();
    //Ask to update the lyrics model.
    emit requireSearchLyrics(lyricsDetailInfo, m_lyricsModel);
}

void KNMusicLyricsDownloadDialog::onActionLyricsActivate(const QModelIndex &index)
{
    //Give out the lyrics item.
    m_downloadWidget->showLyricsItem(m_lyricsModel->item(index.row(), index.column()));
}
