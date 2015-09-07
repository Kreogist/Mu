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
#ifndef KNMUSICXIAMILYRICS_H
#define KNMUSICXIAMILYRICS_H

#include "knmusiclyricsdownloader.h"

/*!
 * \brief The KNMusicXiaMiLyrics class provide the policy to download lyrics
 * files from the server of the famous web site in China named XiaMi.
 */
class KNMusicXiaMiLyrics : public KNMusicLyricsDownloader
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicXiaMiLyrics object.
     * \param parent The parent object.
     */
    explicit KNMusicXiaMiLyrics(QObject *parent = 0);

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
};

#endif // KNMUSICXIAMILYRICS_H
