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

/*!
 * \brief The KNMusicLibraryImageManager class provides a black box image hash
 * map management interface. Give the hash map object to image manager, and set
 * a library image.
 */
class KNMusicLibraryImageManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryImageManager object.
     * \param parent The parent object.
     */
    explicit KNMusicLibraryImageManager(QObject *parent = 0);

    /*!
     * \brief Get the scaled album art hash map. This will automatically scaled
     * the parsed image into a smaller one for album art view to paint up.
     * \return The scaled hash map pointer. It will return a nullptr if you
     * never set it before.
     */
    QHash<QString, QVariant> *scaledHashAlbumArt() const;

    /*!
     * \brief Set the album art hash map for manager to manage.
     * \param scaledHashAlbumArt The scaled image hash map. It will store the
     * scaled image hash map.
     */
    void setHashAlbumArt(QHash<QString, QVariant> *scaledHashAlbumArt);

    /*!
     * \brief Get the image folder path.
     * \return The image folder path. It will be empty if you never set it.
     */
    QString imageFolderPath() const;

    /*!
     * \brief Set the image folder path.
     * \param imageFolderPath The folder which saved all the images. The name of
     * the image file should be its MD4 hash result.
     */
    void setImageFolderPath(const QString &imageFolderPath);

    /*!
     * \brief Get the raw artwork via hash key from the image folder.
     * \param hashKey The artwork hash key.
     * \return The artwork image in QPixmap.
     */
    QPixmap artwork(const QString &hashKey);

    /*!
     * \brief Insert album art image to image manager.
     * \param image The album art image.
     * \return The album art image hash key.
     */
    QString insertArtwork(const QImage &image);

    /*!
     * \brief Get whether the image manager is working for saving artworks and
     * maintaining hash map.
     * \return If the image manager is working, then return true.
     */
    bool isWorking() const;

signals:
    /*!
     * \brief This signal is actually private, it is used for inner processing
     * loop.
     */
    void requireAnalysisNext();

    /*!
     * \brief This signal is asking the music model to update the specific row
     * with the new image hash data.
     * \param row The row of the detail info in the music model.
     * \param detailInfo The new detail info with image hash data.
     */
    void requireUpdateRow(int row, KNMusicDetailInfo detailInfo);

    /*!
     * \brief When all the image has been recover from the image folder, this
     * signal will be emitted.
     */
    void recoverImageComplete();

    /*!
     * \brief When there's a new image is inserted, this signal will be emitted.
     * \param hashKey The new image hash key.
     */
    void imageInserted(const QString &hashKey);

public slots:
    /*!
     * \brief When there's a new item finished analysised, this slot will be
     * called to add the item into parsing list. The album art will be parsed
     * and add to hash map.
     * \param itemIndex The analysis item index.
     * \param item The analysis item.
     */
    void analysisAlbumArt(QPersistentModelIndex itemIndex,
                          KNMusicAnalysisItem item);

    /*!
     * \brief Reload the album art image from the specific folder.
     * \param hashList Loading file name list.
     */
    void recoverAlbumArt(const QStringList &hashList);

    /*!
     * \brief Remove the album art via hash key.
     * \param hashKey The hash key of the removed image.
     */
    void removeHashImage(const QString &hashKey);

private slots:
    void analysisNext();

private:
    struct AnalysisQueueItem
    {
        QPersistentModelIndex itemIndex;
        KNMusicAnalysisItem item;
    };
    inline void insertImage(const QString &hashKey, const QPixmap &pixmap);
    QLinkedList<AnalysisQueueItem> m_analysisQueue;
    QString m_imageFolderPath;
    QHash<QString, QVariant> *m_hashAlbumArt, *m_scaledHashAlbumArt;
    bool m_isWorking;
};

#endif // KNMUSICLIBRARYIMAGEMANAGER_H
