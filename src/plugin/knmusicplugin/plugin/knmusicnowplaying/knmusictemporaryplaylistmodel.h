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

#ifndef KNMUSICTEMPORARYPLAYLISTMODEL_H
#define KNMUSICTEMPORARYPLAYLISTMODEL_H

#include "knmusicmodel.h"

/*!
 * \brief The KNMusicTemporaryPlaylistModel class provides a simple playlist for
 * temporary playing.\n
 * This model should be use only for now playing module.
 */
class KNMusicTemporaryPlaylistModel : public KNMusicModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTemporaryPlaylistModel object.
     * \param parent The parent object.
     */
    explicit KNMusicTemporaryPlaylistModel(QObject *parent = 0);

    /*!
     * \brief Append local music files to the model.
     * \param filePaths The file paths.
     */
    void appendTemporaryFiles(QStringList filePaths);

    /*!
     * \brief Append an online URL item to the temporary model.
     * \param url The analysis item which contains a URL item.
     */
    void appendOnlineUrl(KNMusicAnalysisItem urlItem);

    /*!
     * \brief Append several online URL items to the temporary model.
     * \param urlList The analysis item list which contains a URL item.
     */
    void appendOnlineUrls(QList<KNMusicAnalysisItem> urlList);


signals:

public slots:
    /*!
     * \brief Set the online URL to the model.
     * \param item The url analysis item.
     */
    void setOnlineUrl(KNMusicAnalysisItem item);

    /*!
     * \brief Set the online URL list to the model.
     * \param urlList The url lists.
     */
    void setOnlineUrls(QList<KNMusicAnalysisItem> urlList);

private:
    QList<QPixmap> m_artworkLists;
};

#endif // KNMUSICTEMPORARYPLAYLISTMODEL_H
