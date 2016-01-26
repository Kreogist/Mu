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

#ifndef KNMUSICGENREMODEL_H
#define KNMUSICGENREMODEL_H

#include "knmusiccategorymodel.h"

class KNMusicGenreModel : public KNMusicCategoryModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicGenreModel object.
     * \param parent The parent object.
     */
    explicit KNMusicGenreModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicCategoryModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicCategoryModel::onCategoryAdd().
     */
    void onCategoryAdd(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModel::onCategoryUpdate().
     */
    void onCategoryUpdate(const KNMusicDetailInfo &before,
                          const KNMusicDetailInfo &after) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from
     * KNMusicCategoryModel::onCategoryAlbumArtUpdate().
     */
    void onCategoryAlbumArtUpdate(const KNMusicDetailInfo &detailInfo)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from
     * KNMusicCategoryModel::onActionImageRecoverComplete().
     */
    void onActionImageRecoverComplete() Q_DECL_OVERRIDE;

private:
    inline void loadGenreIcons(const QString &folderPath);
    QHash<QString, QVariant> m_genreIconMap;
};

#endif // KNMUSICGENREMODEL_H
