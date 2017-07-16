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

#ifndef KNMUSICONLINELYRICSDOWNLOADER_H
#define KNMUSICONLINELYRICSDOWNLOADER_H

#include "knmusiclyricsdownloader.h"

#include <QObject>

class KNMusicOnlineLyricsDownloader : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicOnlineLyricsDownloader object with given
     * parent.
     * \param parent The parent object pointer.
     */
    explicit KNMusicOnlineLyricsDownloader(QObject *parent = 0);

    /*!
     * \brief Add a lyrics downloader to the online lyrics server.
     * \param downloader The downloader plugin pointer.
     */
    void appendDownloader(KNMusicLyricsDownloader *downloader);

    bool isRunning();

signals:
    void listContentChanged(
            QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList);
    void serverChanged(int index, int count);
    void downloadComplete();
    void downloadCancel();

public slots:
    /*!
     * \brief Download lyrics by given a detail info of a song, and write all
     * the information into lyrics list. This function works in stucked way.
     * \param detailInfo The detail info of a song.
     * \param lyricsList The lyrics list.
     */
    void downloadLyrics(const KNMusicDetailInfo &detailInfo);

    void cancelDownload();

private slots:
    void onActionDownloadFinished(
            uint identifier,
            const KNMusicDetailInfo &detailInfo,
            QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList);

private:
    QList<KNMusicLyricsDownloader *> m_downloaders;
    QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> m_lyricsList;
    KNMusicDetailInfo m_workingDetailInfo;
    uint m_identifier;
    int m_completeDownloader;
    bool m_cancelFlag, m_running;
};

#endif // KNMUSICONLINELYRICSDOWNLOADER_H
