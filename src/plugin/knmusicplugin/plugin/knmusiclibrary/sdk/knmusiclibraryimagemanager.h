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

#ifndef KNMUSICLIBRARYIMAGEMANAGER_H
#define KNMUSICLIBRARYIMAGEMANAGER_H

#include <QLinkedList>
#include <QPersistentModelIndex>

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

class KNMusicLibraryImageManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLibraryImageManager(QObject *parent = 0);

    QHash<QString, QVariant> *hashAlbumArt() const;
    void setHashAlbumArt(QHash<QString, QVariant> *hashAlbumArt);

    QString imageFolderPath() const;
    void setImageFolderPath(const QString &imageFolderPath);

signals:
    void requireSaveImage(QString imageHashKey);
    void requireAnalysisNext();
    void requireUpdateRow(int row, KNMusicDetailInfo detailInfo);
    void recoverImageComplete();

public slots:
    void analysisAlbumArt(QPersistentModelIndex itemIndex,
                          KNMusicAnalysisItem item);
    void recoverAlbumArt(const QStringList &hashList);
    QString insertHashImage(const QImage &image);
    void removeHashImage(const QString &hashKey);

private slots:
    void analysisNext();

private:
    struct AnalysisQueueItem
    {
        QPersistentModelIndex itemIndex;
        KNMusicAnalysisItem item;
    };
    QLinkedList<AnalysisQueueItem> m_analysisQueue;
    QString m_imageFolderPath;
    QHash<QString, QVariant> *m_hashAlbumArt;
};

#endif // KNMUSICLIBRARYIMAGEMANAGER_H
