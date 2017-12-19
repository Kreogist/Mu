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
#ifndef KNMUSICALBUMLISTPROXYMODEL_H
#define KNMUSICALBUMLISTPROXYMODEL_H

#include "knmusicproxymodel.h"

/*!
 * \brief The KNMusicAlbumListProxyModel class provides the ability for sorting
 * the album song items.
 * It first sort by the disc number, then sort by the tracking number.
 * For all the number which doesn't has the tracking number or the disc number,
 * it will be treat as -1, display at the very beginning.
 */
class KNMusicAlbumListProxyModel : public KNMusicProxyModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAlbumListProxyModel object.
     * \param parent The parent object.
     */
    explicit KNMusicAlbumListProxyModel(QObject *parent = nullptr);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMusicProxyModel::lessThan().
     */
    bool lessThan(const QModelIndex &source_left,
                  const QModelIndex &source_right) const Q_DECL_OVERRIDE;

};

#endif // KNMUSICALBUMLISTPROXYMODEL_H
