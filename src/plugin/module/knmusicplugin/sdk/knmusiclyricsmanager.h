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

#include <QList>
#include <QLinkedList>

#include "knmusicglobal.h"
#include "knmusiclyricsdownloader.h"

#include <QObject>

using namespace KNMusic;

class KNMusicLRCLyricsParser;
class KNMusicLyricsManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLyricsManager(QObject *parent = 0);
    ~KNMusicLyricsManager();
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying);
    void installLyricsDownloader(KNMusicLyricsDownloader *downloader);
    QStringList downloaderNames() const;

    //Lyrics data.
    QList<qint64> positionList() const;
    QStringList textList() const;
    QString lyricsFilePath() const;
    KNMusicDetailInfo musicDetailInfo() const;

    //Configure.
    QString lyricsDir() const;
    void setLyricsDir(const QString &lyricsDir);
    bool enableOnlineLyrics() const;
    void setEnableOnlineLyrics(bool enableOnlineLyrics);

signals:
    void lyricsReset();
    void lyricsUpdate();
    void lyricsSearchedComplete();

public slots:
    void loadLyrics(const KNMusicAnalysisItem &analysisItem);
    void downloadLyrics(const KNMusicDetailInfo &detailInfo);
    void searchLyrics(const KNMusicDetailInfo &detailInfo,
                      QStandardItemModel *lyricsModel);

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

    inline void getOnlineLyrics(const KNMusicDetailInfo &detailInfo,
                                QList<KNMusicLyricsDetails> &lyricsList);
    inline void clearCurrentData();
    inline bool findLocalLyricsFile(const KNMusicDetailInfo &detailInfo);

    //Tried to find and load the lyrics file, if the lyrics can be loaded,
    //then returns true.
    inline bool triedLyricsFile(const QString &lyricsPath);

    //Tried to find and load the related named lyrics file, it will calling
    //triedLyricsFile() functions.
    inline bool triedRelatedNameLyricsFile(const QString &dirPath,
                                           const KNMusicDetailInfo &detailInfo);

    inline void saveLyrics(const KNMusicDetailInfo &detailInfo,
                           const QString &content);
    static bool lyricsDetailLessThan(const KNMusicLyricsDetails &lyricsDetailLeft,
                                     const KNMusicLyricsDetails &lyricsDetailRight);

    //Global instances.
    KNGlobal *m_global;
    KNMusicLRCLyricsParser *m_parser;
    QTextCodec *m_utf8Codec;

    //Lyrics data.
    QList<qint64> m_positionList;
    QStringList m_textList;
    QString m_lyricsFilePath;
    KNMusicDetailInfo m_musicDetailInfo;

    //Lyrics downlaoders.
    QLinkedList<KNMusicLyricsDownloader *> m_downloaders;
    QStringList m_downloaderNames;

    //Lyrics directory path.
    QString m_lyricsDir;
    //Lyrics finding policy list.
    QList<int> m_policyList;
    QList<int> m_relateNamePolicyList;
    //Online lyrics search switch.
    bool m_enableOnlineLyrics=true;
};

#endif // KNMUSICLYRICSMANAGER_H
