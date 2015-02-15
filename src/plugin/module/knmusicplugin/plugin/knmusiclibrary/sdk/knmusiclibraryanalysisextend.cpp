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

void KNMusicLibraryAnalysisExtend::onActionParseNextImage()
{
    //Check is there no item in the queue.
    if(m_analysisQueue.isEmpty())
    {
        return;
    }
    //Analysis the first item in the queue.
    AlbumArtItem currentItem=m_analysisQueue.takeFirst();
    KNMusicGlobal::parser()->parseAlbumArt(currentItem.analysisItem);
    if(!currentItem.analysisItem.coverImage.isNull())
    {
        //Add the image data in the hash pixmap list, get the hash key.
        currentItem.analysisItem.detailInfo.coverImageHash=
                m_coverImageList->appendImage(currentItem.analysisItem.coverImage);
        //Require update the row.
        if(currentItem.itemIndex.isValid())
        {
            emit requireUpdateImage(currentItem.itemIndex.row(),
                                    currentItem.analysisItem);
        }
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
    m_coverImageList=coverImageList;
}

void KNMusicLibraryAnalysisExtend::onActionAnalysisComplete(
        const KNMusicAnalysisItem &analysisItem)
{
    emit requireAppendLibraryRow(KNMusicModelAssist::generateRow(analysisItem.detailInfo),
                                 analysisItem);
}

void KNMusicLibraryAnalysisExtend::onActionAnalysisAlbumArt(QStandardItem *item,
                                                            const KNMusicAnalysisItem &analysisItem)
{
    //Generate a item row.
    AlbumArtItem currentItem;
    currentItem.itemIndex=item->index();
    currentItem.analysisItem=analysisItem;
    //Add the item to analysis queue.
    m_analysisQueue.append(currentItem);
    //And of course, ask to analysis the next item.
    emit requireParseNextImage();
}
