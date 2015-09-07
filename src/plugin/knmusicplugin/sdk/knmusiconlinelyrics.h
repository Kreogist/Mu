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

#ifndef KNMUSICONLINELYRICS_H
#define KNMUSICONLINELYRICS_H

#include <QLinkedList>

#include "knmusicutil.h"
#include "knmusiclyricsdownloader.h"

#include <QObject>

using namespace MusicUtil;

class KNMusicLrcParser;
class KNMusicLyricsDownloader;
class KNMusicOnlineLyrics : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicOnlineLyrics(QObject *parent = 0);
    ~KNMusicOnlineLyrics();

    void appendDownloader(KNMusicLyricsDownloader *downloader);

signals:
    void lyricsDownload(KNMusicDetailInfo detailInfo, QString content);
    void downloadNext();

public slots:
    void addToDownloadList(const KNMusicDetailInfo &detailInfo);
    void downloadLyrics(
            const KNMusicDetailInfo &detailInfo,
            QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList);

private slots:
    void onActionDownloadLyrics();

private:
    QLinkedList<KNMusicDetailInfo> m_downloadQueue;
    QList<KNMusicLyricsDownloader *> m_downloaders;
    KNMusicLrcParser *m_lrcParser;
};

#endif // KNMUSICONLINELYRICS_H
