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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREHOMELISTMODEL_H
#define KNMUSICSTOREHOMELISTMODEL_H

#include "knmusicstoreutil.h"

#include <QAbstractListModel>

using namespace MusicStoreUtil;

/*!
 * \brief The KNMusicStoreHomeListModel class provides the item list used in
 * music store home page.\n
 * Customized role data are listed following:
 *   1. Qt::UserRole + 1: Subheading text.
 *   2. Qt::UserRole + 2: Customized data, which stores the index of the item
 * that allows the backend to get the information.
 */
class KNMusicStoreHomeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreHomeListModel object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreHomeListModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractTableModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Get the album art directly from the list.
     * \param row The item row.
     * \return The pixmap data.
     */
    QPixmap albumArt(int row) const;

    /*!
     * \brief Get the album item at specific row.
     * \param row The row of the item.
     * \return The item content.
     */
    KNMusicStoreHomeItem homeItem(int row) const;

    /*!
     * \brief Append one music item to the model.
     * \param homeItem The music item.
     */
    void appendRow(const KNMusicStoreHomeItem &homeItem);

    /*!
     * \brief Get the album art pixmap size.
     * \return The size of the album art. Unit is pixel.
     */
    int albumArtSize() const;

signals:

public slots:
    /*!
     * \brief Set the size of the album art.
     * \param albumArtSize The new size of the album art. The default value is
     * 125. Unit is pixel.
     */
    void setAlbumArtSize(int albumArtSize);

    /*!
     * \brief Clear all the data of the model.
     */
    void reset();

    /*!
     * \brief Update the album artwork data.
     * \param row The album item row.
     * \param albumArt The new album art pixmap data.
     */
    void setAlbumArt(int row, const QPixmap &albumArt);

private:
    QList<KNMusicStoreHomeItem> m_itemList;
    int m_albumArtSize;
};

#endif // KNMUSICSTOREHOMELISTMODEL_H
