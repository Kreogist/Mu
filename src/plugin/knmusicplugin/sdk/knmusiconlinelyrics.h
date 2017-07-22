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

#include <QMutex>

#include "knmusicutil.h"
#include "knmusiclyricsdownloader.h"

#include <QObject>

using namespace MusicUtil;

class KNMusicLrcParser;
class KNMusicLyricsDownloader;
/*!
 * \brief The KNMusicOnlineLyrics class provides you an online lyrics download
 * interface. It won't work until there's a downloader plugin is added.\n
 * The online lyrics holds all the online downloader. When there's a download
 * request in, we will called all the downloaders to download the lyrics.\n
 * When they we finished, we will sort the all the download lyrics items
 * according to their similarity.
 */
class KNMusicOnlineLyrics : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicOnlineLyrics object.
     * \param parent The parent object.
     */
    explicit KNMusicOnlineLyrics(QObject *parent = 0);
    ~KNMusicOnlineLyrics();

    /*!
     * \brief Add a lyrics downloader to the online lyrics server.
     * \param downloader The downloader plugin pointer.
     */
    void appendDownloader(KNMusicLyricsDownloader *downloader);

signals:
    /*!
     * \brief When a new lyrics is hit and download successfully, this signal
     * will be emitted.
     * \param detailInfo The detail info of the original song.
     * \param content The lyrics content data.
     */
    void lyricsDownload(KNMusicDetailInfo detailInfo, QString content);

    /*!
     * \brief This signal is used to download the next lyrics. Do NOT connect
     * this signal to do anything else.
     */
    void downloadNext();

public slots:
    /*!
     * \brief Add a new song information to the download list of the lyrics.
     * \param detailInfo The detail information of a song.
     */
    void addToDownloadList(const KNMusicDetailInfo &detailInfo);

private slots:
    void onActionDownloadLyrics();
    void onActionDownloadFinished(uint identifier,
            const KNMusicDetailInfo &detailInfo,
            QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList);

private:
    QList<KNMusicDetailInfo> m_downloadQueue;
    QList<KNMusicLyricsDownloader *> m_downloaders;
    QMutex m_workingLock;
    KNMusicLrcParser *m_lrcParser;
    uint m_identifier;
    int m_finishedDownloader;
    bool m_isWorking;
};

#endif // KNMUSICONLINELYRICS_H
