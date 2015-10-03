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

class KNMusicLyricsDetailListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDetailListModel(QObject *parent = 0);

    /*!
     * \brief lyricsList
     * \return
     */
    QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList() const;

    /*!
     * \brief setLyricsList
     * \param lyricsList
     */
    void setLyricsList(
        const QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList);

    QVariant data(const QModelIndex &index, int role) const;

    int rowCount(const QModelIndex &parent) const;

signals:

public slots:

private:
    QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> m_lyricsList;
    QVariant m_nullData;
};

#endif // KNMUSICLYRICSDETAILLISTMODEL_H
