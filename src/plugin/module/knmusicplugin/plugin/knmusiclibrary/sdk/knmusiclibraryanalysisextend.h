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
#ifndef KNMUSICLIBRARYANALYSISEXTEND_H
#define KNMUSICLIBRARYANALYSISEXTEND_H

#include <QLinkedList>

#include "knmusicglobal.h"

#include "knmusicanalysisextend.h"

namespace KNMusicLibraryAlbumArt
{
struct AlbumArtItem
{
    QStandardItem *item=nullptr;
    KNMusicAnalysisItem analysisItem;
};
}

using namespace KNMusic;
using namespace KNMusicLibraryAlbumArt;

class KNHashPixmapList;
class KNMusicLibraryAnalysisExtend : public KNMusicAnalysisExtend
{
    Q_OBJECT
public:
    explicit KNMusicLibraryAnalysisExtend(QObject *parent = 0);
    KNHashPixmapList *coverImageList() const;
    void setCoverImageList(KNHashPixmapList *coverImageList);

signals:
    void requireParseNextImage();
    void requireAppendLibraryRow(QList<QStandardItem *> musicRow,
                                 KNMusicAnalysisItem analysisItem);
    void requireUpdateImage(int row,
                            KNMusicAnalysisItem analysisItem);

public slots:
    void onActionAnalysisComplete(const KNMusicAnalysisItem &analysisItem);
    void onActionAnalysisAlbumArt(QStandardItem *item,
                                  const KNMusicAnalysisItem &analysisItem);

private slots:
    void onActionParseNextImage();

private:
    QLinkedList<AlbumArtItem> m_analysisQueue;
    KNHashPixmapList *m_coverImageList;
};

#endif // KNMUSICLIBRARYANALYSISEXTEND_H
