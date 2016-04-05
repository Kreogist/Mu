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
#ifndef KNMUSICSTOREALBUMMODEL_H
#define KNMUSICSTOREALBUMMODEL_H

#include <QMap>

#include "knmusicstoresonglistmodel.h"

/*!
 * \brief The KNMusicStoreAlbumModel class provides a model which could be used
 * to describe an album.
 */
class KNMusicStoreAlbumModel : public KNMusicStoreSongListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreAlbumModel object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreAlbumModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicStoreSongListModel::clear().
     */
    void clear() Q_DECL_OVERRIDE;

    /*!
     * \brief Get the album detail info.
     * \param albumInfoField The info type.
     * \return The album information.
     */
    QString albumInfo(KNMusicStoreUtil::StoreAlbumDetail albumInfoField) const;

    /*!
     * \brief Get the album art image.
     * \return The album art pixmap.
     */
    QPixmap albumArt() const;

signals:

public slots:
    /*!
     * \brief Set the album art of the album.
     * \param albumArt The album art image.
     */
    void setAlbumArt(const QPixmap &albumArt);

    /*!
     * \brief Set the album information.
     * \param albumInfoField Album information type.
     * \param albumInfoData The album information.
     */
    void setAlbumInfo(KNMusicStoreUtil::StoreAlbumDetail albumInfoField,
                      const QString &albumInfoData);

private:
    QString m_albumDetail[KNMusicStoreUtil::StoreAlbumDetailCount];
    QPixmap m_albumArt;
};

#endif // KNMUSICSTOREALBUMMODEL_H
