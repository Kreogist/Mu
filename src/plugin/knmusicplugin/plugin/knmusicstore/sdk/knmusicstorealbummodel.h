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

#include <QList>
#include <QMap>

#include "knmusicstoreutil.h"

#include <QAbstractTableModel>

/*!
 * \brief The KNMusicStoreAlbumModel class provides a model which could be used
 * to describe an album.
 */
class KNMusicStoreAlbumModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreAlbumModel object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreAlbumModel(QObject *parent = 0);

    /*!
     * \brief Append a song item to the end of the model.
     * \param item The item of the song.
     */
    void appendItem(const KNMusicStoreUtil::StoreSongItem &item);

    /*!
     * \brief Clear all the data.
     */
    void clear();

    /*!
     * \brief Reimplemented from QAbstractItemModel::rowCount().
     */
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemModel::columnCount().
     */
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemModel::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

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
    /*!
     * \brief This signal will be emitted when album detail updated completely.
     */
    void albumDetailUpdated();

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

private slots:
    void retranslate();

private:
    QString m_titles[4];
    QMap<KNMusicStoreUtil::StoreAlbumDetail, QString> m_albumDetail;
    QList<KNMusicStoreUtil::StoreSongItem> m_songList;
    QPixmap m_albumArt;
};

#endif // KNMUSICSTOREALBUMMODEL_H
