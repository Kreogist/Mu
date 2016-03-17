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
#ifndef KNMUSICSTOREALBUMLISTMODEL_H
#define KNMUSICSTOREALBUMLISTMODEL_H

#include <QList>

#include "knmusicstoreutil.h"

#include <QAbstractListModel>

/*!
 * \brief The KNMusicAlbumListModel class provides the model for albums which
 * could be stored the informations of albums.
 */
class KNMusicStoreAlbumListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAlbumListModel object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreAlbumListModel(QObject *parent = 0);

    /*!
     * \brief Append an album item to the end of the model.
     * \param item The item of the album.
     */
    void appendItem(const KNMusicStoreUtil::StoreAlbumListItem &item);

    /*!
     * \brief Clear all the data.
     */
    void clear();

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Get the album art image.
     * \param row The album index row.
     * \return The album art cover image in pixmap format.
     */
    QPixmap albumArt(int row);

    /*!
     * \brief This is an override function. Get the album art image.
     * \param index The album index.
     * \return The album art cover image in pixmap format.
     */
    QPixmap albumArt(const QModelIndex &index)
    {
        //Give the index album back.
        return albumArt(index.row());
    }

    /*!
     * \brief Get album title via row.
     * \param row The album index row.
     * \return The album title text.
     */
    QString albumTitle(int row);

    /*!
     * \brief Get album artist name via row.
     * \param row The album index row.
     * \return The album artist name text.
     */
    QString artist(int row);

signals:

public slots:

private:
    QList<KNMusicStoreUtil::StoreAlbumListItem> m_albumList;
};

#endif // KNMUSICSTOREALBUMLISTMODEL_H
