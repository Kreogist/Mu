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
#ifndef KNMUSICNETEASELYRICS_H
#define KNMUSICNETEASELYRICS_H

#include "knmusiclyricsdownloader.h"

/*!
 * \brief The KNMusicNeteaseLyrics class provides the policy to download lyrics
 * from Netease Cloud server.
 */
class KNMusicNeteaseLyrics : public KNMusicLyricsDownloader
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicNeteaseLyrics object.
     * \param parent The parent object.
     */
    explicit KNMusicNeteaseLyrics(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicLyricsDownloader::downloaderName().
     */
    QString downloaderName() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLyricsDownloader::downloadLyrics().
     */
    void downloadLyrics(
            const KNMusicDetailInfo &detailInfo,
            QList<KNMusicLyricsDetails> &lyricsList) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    inline QNetworkRequest generateNeteaseRequest(const QString &url);
    inline QString getArtistNames(const QJsonObject &songData);
    inline void saveLyricsToList(const QJsonObject &lyricsObject,
                                 const QString &lyricsName,
                                 const KNMusicDetailInfo &detailInfo,
                                 KNMusicLyricsDetails &lyricsDetails,
                                 QList<KNMusicLyricsDetails> &lyricsList);
};

#endif // KNMUSICNETEASELYRICS_H
