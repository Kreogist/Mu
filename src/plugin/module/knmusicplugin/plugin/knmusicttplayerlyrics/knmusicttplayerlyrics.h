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
#ifndef KNMUSICTTPLAYERLYRICS_H
#define KNMUSICTTPLAYERLYRICS_H

#include "knmusiclyricsdownloader.h"

class KNMusicTTPlayerLyrics : public KNMusicLyricsDownloader
{
    Q_OBJECT
public:
    explicit KNMusicTTPlayerLyrics(QObject *parent=0);
    ~KNMusicTTPlayerLyrics();
    QString downloaderName()
    {
        return "TTPlayer";
    }
    void downloadLyrics(const KNMusicDetailInfo &detailInfo,
                        QList<KNMusicLyricsDetails> &lyricsList);

private:
    struct lrcInfo
    {
        QString id;
        QString artist;
        QString title;
    };
    inline QString generateCode(const QHash<QString, QString> &info);
    inline QString utf8HexText(const QString &original);
    inline QString utf16LEHex(const QString &original);
    inline qint64 conv(qint64 i);
    QTextCodec *m_utf16LE;
};

#endif // KNMUSICTTPLAYERLYRICS_H
