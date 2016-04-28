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

#ifndef KNMUSICLYRICSDETAILLISTMODEL_H
#define KNMUSICLYRICSDETAILLISTMODEL_H

#include "knmusiclyricsdownloader.h"

#include <QAbstractListModel>

/*!
 * \brief The KNMusicLyricsDetailListModel class provides a managing model for
 * lyrics list. It will be used in the lyrics download dialog.
 */
class KNMusicLyricsDetailListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicLyricsDetailListModel object.
     * \param parent The parent object.
     */
    explicit KNMusicLyricsDetailListModel(QObject *parent = 0);

    /*!
     * \brief Get the current lyrics list.
     * \return The managed lyrics list.
     */
    QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList() const;

    /*!
     * \brief Set the lyrics list information to the model.
     * \param lyricsList The new lyrics list information.
     */
    void setLyricsList(
        const QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList);

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index,
                  int role=Qt::DisplayRole) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> m_lyricsList;
    QVariant m_nullData;
};

#endif // KNMUSICLYRICSDETAILLISTMODEL_H
