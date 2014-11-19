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
#include "knhashpixmaplist.h"
#include "knmusicparser.h"
#include "knmusicmodelassist.h"

#include "knmusiclibraryanalysisextend.h"

#include <QDebug>

KNMusicLibraryAnalysisExtend::KNMusicLibraryAnalysisExtend(QObject *parent) :
    KNMusicAnalysisExtend(parent)
{
    //Establish the analysis loop
    connect(this, &KNMusicLibraryAnalysisExtend::requireParseNextImage,
            this, &KNMusicLibraryAnalysisExtend::onActionParseNextImage);
}

void KNMusicLibraryAnalysisExtend::onActionAnalysisComplete(const KNMusicDetailInfo &detailInfo)
{
    //Generate the row.
    QList<QStandardItem *> songRow=KNMusicModelAssist::generateRow(detailInfo);
    //Generate a item row.
    AlbumArtItem currentItem;
    currentItem.item=songRow.at(Name);
    currentItem.detailInfo=detailInfo;
    //Add the item to analysis queue.
    m_analysisQueue.append(currentItem);
    //Append the music row.
    emit requireAppendRow(songRow);
    //And of course, ask to analysis the next item.
    emit requireParseNextImage();
}

void KNMusicLibraryAnalysisExtend::onActionParseNextImage()
{
    //Check is there no item in the queue.
    if(m_analysisQueue.isEmpty())
    {
        return;
    }
    //Analysis the first item in the queue.
    AlbumArtItem currentItem=m_analysisQueue.takeFirst();
    KNMusicGlobal::parser()->parseAlbumArt(currentItem.detailInfo);
    if(!currentItem.detailInfo.coverImage.isNull())
    {
        //Add the image data in the hash pixmap list, get the hash key.
        currentItem.detailInfo.coverImageHash=
                m_coverImageList->appendImage(currentItem.detailInfo.coverImage);
        //Set the hash key to the item.
        currentItem.item->setData(currentItem.detailInfo.coverImageHash,
                                  ArtworkKeyRole);
        //We should ask to update the data.
        emit requireUpdateAlbumArt(currentItem.detailInfo);
    }
    //Ask to analysis next item.
    emit requireParseNextImage();
}

KNHashPixmapList *KNMusicLibraryAnalysisExtend::coverImageList() const
{
    return m_coverImageList;
}

void KNMusicLibraryAnalysisExtend::setCoverImageList(KNHashPixmapList *coverImageList)
{
    m_coverImageList = coverImageList;
}
