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
#ifndef KNMUSICTTPODLYRICS_H
#define KNMUSICTTPODLYRICS_H

#include "../../sdk/knmusiclyricsdownloader.h"

class KNMusicTTPodLyrics : public KNMusicLyricsDownloader
{
public:
    explicit KNMusicTTPodLyrics(QObject *parent = 0);
    ~KNMusicTTPodLyrics();
    QString downloaderName()
    {
        return "TTPod";
    }
    void downloadLyrics(const KNMusicDetailInfo &detailInfo,
                        QList<KNMusicLyricsDetails> &lyricsList);

private:
    inline QString process_code(const QVariant &str);
    inline qint32 crc32(const QString &str);
    inline QString utf8HexText(const QString &original);
    QString m_table;
};

#endif // KNMUSICTTPODLYRICS_H
