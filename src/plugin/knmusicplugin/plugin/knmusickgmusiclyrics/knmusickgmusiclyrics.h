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

#ifndef KNMUSICKGMUSICLYRICS_H
#define KNMUSICKGMUSICLYRICS_H

#include "knmusiclyricsdownloader.h"

/*!
 * \brief The KNMusicKgmusicLyrics class can download lyrics from the KuGou
 * server. And parse the krc format lyrics to lrc format.\n
 * Thanks for the script from Anonymous. The script is provided by miaolapd.\n
 * This module is not simply translate all the raw javascript. It should works
 * much better than the script. It will check the JSON strcuture before using
 * it.
 */
class KNMusicKgmusicLyrics : public KNMusicLyricsDownloader
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNMusicKgmusicLyrics object with specific parent object.
     * \param parent The parent object.
     */
    explicit KNMusicKgmusicLyrics(QObject *parent = 0);

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
    QString parseKRC(const QByteArray &krcData);
    unsigned char m_magicBytes[4], m_encKey[16];
    QStringList m_lyricsAttributeHeader;
};

#endif // KNMUSICKGMUSICLYRICS_H
