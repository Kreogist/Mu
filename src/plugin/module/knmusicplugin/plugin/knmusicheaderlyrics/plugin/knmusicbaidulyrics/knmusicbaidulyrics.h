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
#ifndef KNMUSICBAIDULYRICS_H
#define KNMUSICBAIDULYRICS_H

#include "../../sdk/knmusiclyricsdownloader.h"

class KNMusicBaiduLyrics : public KNMusicLyricsDownloader
{
    Q_OBJECT
public:
    explicit KNMusicBaiduLyrics(QObject *parent = 0);
    QString downloaderName()
    {
        return "Baidu Music";
    }
    void downloadLyrics(const KNMusicDetailInfo &detailInfo,
                        QList<KNMusicLyricsDetails> &lyricsList);

private:
    QTextCodec *m_gbkCodec;
};

#endif // KNMUSICBAIDULYRICS_H
