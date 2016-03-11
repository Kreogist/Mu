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

#ifndef KNMUSICLYRICSMANAGER_H
#define KNMUSICLYRICSMANAGER_H

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

class KNMusicLyricsDownloader;
class KNMusicLyricsBackend;
class KNMusicLrcParser;
class KNMusicOnlineLyrics;
class KNMusicOnlineLyricsDownloader;
/*!
 * \brief The KNMusicLyricsManager class provides a seamless local and online
 * lyrics loader and downloader management.
 */
class KNMusicLyricsManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLyricsManager class.
     * \param parent The parent object.
     */
    explicit KNMusicLyricsManager(QObject *parent = 0);
    ~KNMusicLyricsManager();

    /*!
     * \brief Get the backend of lyrics manager provides.
     * \return The default lyrics backend, the lyrics will be sync to the loaded
     * lyrics.
     */
    KNMusicLyricsBackend *backend();

    /*!
     * \brief Get the path of lyrics directory.
     * \return The lyrics directory path.
     */
    QString lyricsDirectory() const;

    /*!
     * \brief Append a downloader to the online downloader engine.
     * \param downloader The online downloader engine.
     */
    void appendDownloader(KNMusicLyricsDownloader *downloader);

    /*!
     * \brief Get the online lyrics downloader object pointer.
     * \return The object pointer of the online lyrics downloader.
     */
    KNMusicOnlineLyricsDownloader *onlineLyricsDownloader();

signals:
    /*!
     * \brief When the lyrics manager need to download lyrics from Internet,
     * this signal will be emitted for asking download the lyrics.
     * \param detailInfo The detail info structure of the song.
     */
    void requireDownloadLyrics(KNMusicDetailInfo detailInfo);

public slots:
    /*!
     * \brief Set the lyrics directory path.
     * \param lyricsDir The new path of lyrics directory.
     */
    void setLyricsDirectory(const QString &lyricsDir);

    /*!
     * \brief Load the lyrics according to a item. This slot should be connect
     * to the nowPlayingChanged signal from a now playing object.
     * \param analysisItem The current playing item.
     */
    void loadLyrics(const KNMusicAnalysisItem &analysisItem);

    /*!
     * \brief Load lyrics from a local file.
     * \param lyricsPath The lyrics path.
     * \return If load the file successfully, return true.
     */
    bool loadLyricsFile(const QString &lyricsPath);

    /*!
     * \brief Reset the lyrics backend, clear all the data of the backend.
     */
    void resetBackend();

    /*!
     * \brief Save the lyrics data to the lyrics directory. Named as "Artist-
     * Name.lrc".
     * \param detailInfo The detail info of the song for the lyrics.
     * \param content Lyrics file content.
     */
    void saveLyrics(const KNMusicDetailInfo &detailInfo,
                    const QString &content);

    /*!
     * \brief Save the lyrics data and update the main backend data.
     * \param detailInfo The detail info of the lyrics.
     * \param content The lyrics data.
     */
    void saveLyricsAndUpdateBackend(const KNMusicDetailInfo &detailInfo,
                                    const QString &content);

private slots:
    void checkAndSaveLyrics(const KNMusicDetailInfo &detailInfo,
                            const QString &content);

private:
    enum SearchPolicy
    {
        SameNameInLyricsDir,
        RelateNameInLyricsDir,
        SameNameInMusicDir,
        RelateNameInMusicDir
    };
    enum RelateNameFindPolicy
    {
        LyricsNamedTitle,
        LyricsNamedArtistHyphonTitle,
        LyricsNamedAlbumHyphonTitle
    };
    inline QString generateLyricsPath(const KNMusicDetailInfo &detailInfo);
    inline bool loadLocalLyrics(const KNMusicDetailInfo &detailInfo);
    inline bool loadRelatedLyrics(const QString &dirPath,
                                  const KNMusicDetailInfo &detailInfo);

    //Policy list.
    QList<int> m_policyList;
    QList<int> m_relateNamePolicyList;
    //Current state.
    KNMusicDetailInfo m_detailInfo;
    //Lyrics directory path.
    QString m_lyricsDir;
    //Support objects.
    KNMusicOnlineLyrics *m_onlineLyrics;
    KNMusicOnlineLyricsDownloader *m_onlineLyricsDownloader;
    KNMusicLyricsBackend *m_backend;
    KNMusicLrcParser *m_parser;
    //Working thread for the downloader.
    QThread *m_onlineThread;
};

#endif // KNMUSICLYRICSMANAGER_H
