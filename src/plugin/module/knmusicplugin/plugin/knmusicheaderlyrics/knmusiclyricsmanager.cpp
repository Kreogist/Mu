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
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "knglobal.h"

#include "plugin/knmusicttpodlyrics/knmusicttpodlyrics.h"
#include "plugin/knmusicxiamilyrics/knmusicxiamilyrics.h"
#include "plugin/knmusicqqlyrics/knmusicqqlyrics.h"
#include "plugin/knmusicneteaselyrics/knmusicneteaselyrics.h"
#include "plugin/knmusicttplayerlyrics/knmusicttplayerlyrics.h"
#include "knmusiclyricsglobal.h"
#include "knmusiclrcparser.h"

#include "knmusiclyricsmanager.h"

#include <QDebug>

KNMusicLyricsManager *KNMusicLyricsManager::m_instance=nullptr;

KNMusicLyricsManager *KNMusicLyricsManager::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicLyricsManager:m_instance;
}

bool KNMusicLyricsManager::loadLyricsForFile(const KNMusicDetailInfo &detailInfo,
                                             QList<qint64> &positions,
                                             QStringList &lyricsText)
{
    //Clear the lyrics.
    clear();
    //Find the lyrics.
    if(!findLyricsForFile(detailInfo))
    {
        if(m_downloadLyrics && !downloadLyricsForFile(detailInfo))
        {
            return false;
        }
    }
    //Using parser to parse the file.
    m_lrcParser->parseFile(m_currentLyricsPath,
                           m_lyricsProperty,
                           positions,
                           lyricsText);
    return !positions.isEmpty();
}

inline void KNMusicLyricsManager::installDownloaders()
{
    installLyricsDownloader(new KNMusicTTPodLyrics);
    installLyricsDownloader(new KNMusicQQLyrics);
    installLyricsDownloader(new KNMusicTTPlayerLyrics);
    installLyricsDownloader(new KNMusicXiaMiLyrics);
//    installLyricsDownloader(new KNMusicNeteaseLyrics);
}

void KNMusicLyricsManager::clear()
{
    //Remove the file path.
    m_currentLyricsPath.clear();
    //Remove the old properties.
    m_lyricsProperty.clear();
}

void KNMusicLyricsManager::installLyricsDownloader(KNMusicLyricsDownloader *downloader)
{
    //Add the downloader to linked list.
    m_downloaders.append(downloader);
}

inline bool KNMusicLyricsManager::findLyricsForFile(const KNMusicDetailInfo &detailInfo)
{
    //Clear the old path.
    m_currentLyricsPath.clear();
    //Get the file info of the music.
    QFileInfo musicInfo(detailInfo.filePath);
    //Search the lyrics using the policy.
    int currentPolicy=0;
    while(currentPolicy<m_policyList.size())
    {
        switch (m_policyList.at(currentPolicy))
        {
        case SameNameInLyricsDir:
            if(checkLyricsFile(KNMusicLyricsGlobal::lyricsFolderPath() + "/" +
                               musicInfo.completeBaseName()+".lrc"))
            {
                return true;
            }
            break;
        case RelateNameInLyricsDir:
            if(findRelateLyrics(KNMusicLyricsGlobal::lyricsFolderPath(),
                                detailInfo))
            {
                return true;
            }
            break;
        case SameNameInMusicDir:
            if(checkLyricsFile(musicInfo.absolutePath() + "/" +
                               musicInfo.completeBaseName()+".lrc"))
            {
                return true;
            }
            break;
        case RelateNameInMusicDir:
            if(findRelateLyrics(musicInfo.absolutePath(),
                                detailInfo))
            {
                return true;
            }
            break;
        default:
            break;
        }
        currentPolicy++;
    }
    return false;
}

inline bool KNMusicLyricsManager::downloadLyricsForFile(const KNMusicDetailInfo &detailInfo)
{
    //Prepare the lyrics detail list.
    QList<KNMusicLyricsDetails> lyricsList;
    //Using the downloader to download the lyrics.
    for(QLinkedList<KNMusicLyricsDownloader *>::iterator i=m_downloaders.begin();
        i!=m_downloaders.end();
        ++i)
    {
        //Try to download the file from all the server.
        (*i)->downloadLyrics(detailInfo, lyricsList);
    }
    //Check if we need to process lyrics list.
    if(lyricsList.isEmpty())
    {
        return false;
    }
    //Sort the list according to the similarity.
    qSort(lyricsList.begin(), lyricsList.end(), lyricsDetailLessThan);
    //Now the first data is the best matching data we can find,
    //Save it to a file.
    QString lyricsFilePath=
            writeLyricsFile(detailInfo, lyricsList.first().lyricsData);
    return lyricsFilePath.isEmpty()?
                false:
                checkLyricsFile(lyricsFilePath);
}

inline bool KNMusicLyricsManager::checkLyricsFile(const QString &lyricsPath)
{
    QFile testFile(lyricsPath);
    if(testFile.exists())
    {
        m_currentLyricsPath=QFileInfo(testFile).absoluteFilePath();
        return true;
    }
    return false;
}

inline bool KNMusicLyricsManager::findRelateLyrics(const QString &folderPath,
                                                   const KNMusicDetailInfo &detailInfo)
{
    //Find the title, the artist and the title, the album and the title.
    return checkLyricsFile(folderPath+"/"+detailInfo.textLists[Name]+".lrc") ||
            checkLyricsFile(folderPath+"/"+m_global->legalFileName(detailInfo.textLists[Artist]+" - "+detailInfo.textLists[Name]+".lrc")) ||
            checkLyricsFile(folderPath+"/"+m_global->legalFileName(detailInfo.textLists[Album]+" - "+detailInfo.textLists[Name]+".lrc"));
}

inline QString KNMusicLyricsManager::writeLyricsFile(const KNMusicDetailInfo &detailInfo,
                                                     const QString &content)
{
    //Generate the lyrics file path
    QString lyricsFilePath=KNMusicLyricsGlobal::lyricsFolderPath() + "/" +
            m_global->legalFileName(detailInfo.textLists[Artist]+" - "+detailInfo.textLists[Name]+".lrc");
    QFile lyricsFile(lyricsFilePath);
    //Try to open the file.
    if(lyricsFile.open(QIODevice::WriteOnly))
    {
        //Write the data to the file.
        QTextStream lyricsStream(&lyricsFile);
        lyricsStream << content << flush;
        //Close the file.
        lyricsFile.close();
        //Return the file path.
        return lyricsFilePath;
    }
    return QString();
}

bool KNMusicLyricsManager::lyricsDetailLessThan(const KNMusicLyricsDetails &lyricsDetailLeft,
                                                const KNMusicLyricsDetails &lyricsDetailRight)
{
    return (lyricsDetailLeft.titleSimilarity==lyricsDetailRight.titleSimilarity)?
                lyricsDetailLeft.artistSimilarity<lyricsDetailRight.artistSimilarity:
                lyricsDetailLeft.titleSimilarity<lyricsDetailRight.titleSimilarity;
}

KNMusicLyricsManager::KNMusicLyricsManager(QObject *parent) :
    QObject(parent)
{
    //Initial global instances.
    m_global=KNGlobal::instance();
    m_musicGlobal=KNMusicGlobal::instance();

    //Get the lyrics folder path.
    KNMusicLyricsGlobal::setLyricsFolderPath(KNMusicGlobal::musicLibraryPath()+"/Lyrics");
    //Set the default loading policy.
    m_policyList.append(SameNameInLyricsDir);
    m_policyList.append(RelateNameInLyricsDir);
    m_policyList.append(SameNameInMusicDir);
    m_policyList.append(RelateNameInMusicDir);

    //Initial the LRC file parser.
    m_lrcParser=new KNMusicLRCParser(this);

    //Install the downloaders.
    installDownloaders();
}

bool KNMusicLyricsManager::downloadLyrics() const
{
    return m_downloadLyrics;
}

void KNMusicLyricsManager::setDownloadLyrics(bool downloadLyrics)
{
    m_downloadLyrics = downloadLyrics;
}

QString KNMusicLyricsManager::lyricsFolderPath() const
{
    return KNMusicLyricsGlobal::lyricsFolderPath();
}

void KNMusicLyricsManager::setLyricsFolderPath(const QString &lyricsFolderPath)
{
    KNMusicLyricsGlobal::setLyricsFolderPath(lyricsFolderPath);
}
