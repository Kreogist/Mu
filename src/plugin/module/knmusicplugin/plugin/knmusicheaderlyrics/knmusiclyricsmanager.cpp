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
#include <QFileInfo>

#include "plugin/knmusicxiamilyrics/knmusicxiamilyrics.h"
#include "sdk/knmusiclyricsglobal.h"
#include "knmusiclrcparser.h"

#include "knmusiclyricsmanager.h"

#include <QDebug>

KNMusicLyricsManager *KNMusicLyricsManager::m_instance=nullptr;

KNMusicLyricsManager *KNMusicLyricsManager::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicLyricsManager:m_instance;
}

int KNMusicLyricsManager::lines() const
{
    return m_positions.size();
}

bool KNMusicLyricsManager::isEmpty() const
{
    return m_positions.isEmpty();
}

qint64 KNMusicLyricsManager::positionAt(const int &index) const
{
    return m_positions.at(index);
}

QString KNMusicLyricsManager::lyricsAt(const int &index) const
{
    return m_lyricsText.at(index);
}

bool KNMusicLyricsManager::loadLyricsForFile(const KNMusicDetailInfo &detailInfo)
{
    //Clear the lyrics.
    clear();
    //Find the lyrics.
    if(!findLyricsForFile(detailInfo))
    {
        if(!downloadLyricsForFile(detailInfo))
        {
            return false;
        }
    }
    //Using parser to parse the file.
    m_lrcParser->parseFile(m_currentLyricsPath,
                           m_lyricsProperty,
                           m_positions,
                           m_lyricsText);
    return !m_positions.isEmpty();
}

void KNMusicLyricsManager::installDownloaders()
{
    installLyricsDownloader(new KNMusicXiaMiLyrics);
}

void KNMusicLyricsManager::clear()
{
    //Remove the file path.
    m_currentLyricsPath.clear();
    //Remove the old properties.
    m_lyricsProperty.clear();
    //Remove all the positions and the texts.
    m_positions.clear();
    m_lyricsText.clear();
}

void KNMusicLyricsManager::installLyricsDownloader(KNMusicLyricsDownloader *downloader)
{
    //Add the downloader to linked list.
    m_downloaders.append(downloader);
}

bool KNMusicLyricsManager::findLyricsForFile(const KNMusicDetailInfo &detailInfo)
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
            if(findRelateLyrics(KNMusicLyricsGlobal::lyricsFolderPath(), detailInfo))
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
            if(findRelateLyrics(musicInfo.absolutePath(), detailInfo))
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

bool KNMusicLyricsManager::downloadLyricsForFile(const KNMusicDetailInfo &detailInfo)
{
    //Using the downloader to download the lyrics.
    for(QLinkedList<KNMusicLyricsDownloader *>::iterator i=m_downloaders.begin();
        i!=m_downloaders.end();
        ++i)
    {
        //Try to download the file from server.
        QString downloadedFilePath=(*i)->downloadLyrics(detailInfo);
        //If download success.
        if(!downloadedFilePath.isEmpty())
        {
            //Load the file.
            if(checkLyricsFile(downloadedFilePath))
            {
                return true;
            }
            //Or else, we need to delete this file.
            //!FIXME: add delete code here.
        }
    }
    return false;
}

bool KNMusicLyricsManager::checkLyricsFile(const QString &lyricsPath)
{
    QFile testFile(lyricsPath);
    if(testFile.exists())
    {
        m_currentLyricsPath=QFileInfo(testFile).absoluteFilePath();
        return true;
    }
    return false;
}

bool KNMusicLyricsManager::findRelateLyrics(const QString &folderPath,
                                            const KNMusicDetailInfo &detailInfo)
{
    //Find the title, the artist and the title, the album and the title.
    return checkLyricsFile(folderPath+"/"+detailInfo.textLists[Name]+".lrc") ||
            checkLyricsFile(folderPath+"/"+detailInfo.textLists[Artist]+" - "+detailInfo.textLists[Name]+".lrc") ||
            checkLyricsFile(folderPath+"/"+detailInfo.textLists[Album]+" - "+detailInfo.textLists[Name]+".lrc");
}

KNMusicLyricsManager::KNMusicLyricsManager(QObject *parent) :
    QObject(parent)
{
    //Initial music global instance.
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

QString KNMusicLyricsManager::lyricsFolderPath() const
{
    return KNMusicLyricsGlobal::lyricsFolderPath();
}

void KNMusicLyricsManager::setLyricsFolderPath(const QString &lyricsFolderPath)
{
    KNMusicLyricsGlobal::setLyricsFolderPath(lyricsFolderPath);
}
