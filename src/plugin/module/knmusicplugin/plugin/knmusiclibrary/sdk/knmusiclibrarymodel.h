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
#ifndef KNMUSICLIBRARYMODEL_H
#define KNMUSICLIBRARYMODEL_H

#include <QLinkedList>

#include "knmusiccategorymodel.h"

#include "knmusicmodel.h"

class KNHashPixmapList;
class KNMusicLibraryAnalysisExtend;
class KNMusicLibraryModel : public KNMusicModel
{
    Q_OBJECT
public:
    explicit KNMusicLibraryModel(QObject *parent = 0);
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QPixmap artwork(const QString &key);
    int playingItemColumn();
    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent);
    void installCategoryModel(KNMusicCategoryModel *model);

signals:

public slots:
    void retranslate();
    void addFiles(const QStringList &fileList);
    void appendMusicRow(const QList<QStandardItem *> &musicRow);
    void updateCoverImage(const KNMusicDetailInfo &detailInfo);
    void removeMusicRow(const int &row);

private:
    void initialHeader();
    QLinkedList<KNMusicCategoryModel *> m_categoryModels;
    KNMusicGlobal *m_musicGlobal;
    KNMusicLibraryAnalysisExtend *m_analysisExtend;
    KNHashPixmapList *m_coverImageList;
};

#endif // KNMUSICLIBRARYMODEL_H
