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

#include <QFile>
#include <QMap>
#include <QLinkedList>
#include <QStringList>

#include "knmusiclyricsdownloader.h"

#include "knmusicglobal.h"

#include <QObject>

namespace KNMusicLyrics
{
enum SearchPolicy
{
    SameNameInLyricsDir,
    RelateNameInLyricsDir,
    SameNameInMusicDir,
    RelateNameInMusicDir
};
}

using namespace KNMusic;
using namespace KNMusicLyrics;

class KNGlobal;
class KNPreferenceItemBase;
class KNPreferenceItemGlobal;
class KNMusicGlobal;
class KNMusicLRCParser;
class KNMusicLyricsManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLyricsManager(QObject *parent = 0);
    QString lyricsFolderPath() const;
    void setLyricsFolderPath(const QString &lyricsFolderPath);
    void clear();
    void installLyricsDownloader(KNMusicLyricsDownloader *downloader);
    bool downloadLyrics() const;
    void setDownloadLyrics(bool downloadLyrics);

signals:

public slots:
    bool loadLyricsForFile(const KNMusicDetailInfo &detailInfo,
                           QList<qint64> &positions,
                           QStringList &lyricsText);

private slots:

private:
    inline bool findLyricsForFile(const KNMusicDetailInfo &detailInfo);
    inline bool downloadLyricsForFile(const KNMusicDetailInfo &detailInfo);
    inline bool checkLyricsFile(const QString &lyricsPath);
    inline bool findRelateLyrics(const QString &folderPath,
                                 const KNMusicDetailInfo &detailInfo);
    inline QString writeLyricsFile(const KNMusicDetailInfo &detailInfo,
                                   const QString &content);
    static bool lyricsDetailLessThan(const KNMusicLyricsDetails &lyricsDetailLeft,
                                     const KNMusicLyricsDetails &lyricsDetailRight);

    KNGlobal *m_global;
    KNMusicGlobal *m_musicGlobal;
    KNMusicLRCParser *m_lrcParser;
    QString m_currentLyricsPath;
    QFile m_lyricsFile;
    QList<int> m_policyList;
    QMap<int, QString> m_lyricsProperty;
    QLinkedList<KNMusicLyricsDownloader *> m_downloaders;

    bool m_downloadLyrics=true;
};

#endif // KNMUSICLYRICSMANAGER_H
