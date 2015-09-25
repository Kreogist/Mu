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
#include <QThread>

#include "knutil.h"

#include "knmusiclyricsbackend.h"
#include "knmusiclrcparser.h"
#include "knmusicglobal.h"
#include "knmusiconlinelyrics.h"

#include "knmusiclyricsmanager.h"

#include <QDebug>

KNMusicLyricsManager::KNMusicLyricsManager(QObject *parent) :
    QObject(parent),
    m_detailInfo(KNMusicDetailInfo()),
    m_onlineLyrics(new KNMusicOnlineLyrics),
    m_backend(new KNMusicLyricsBackend(this)),
    m_parser(new KNMusicLrcParser(this)),
    m_onlineThread(new QThread(this)),
    m_lyricsDir(QString())
{
    //Set the default loading policy.
    m_policyList << SameNameInLyricsDir << RelateNameInLyricsDir
                 << SameNameInMusicDir << RelateNameInMusicDir;
    //Set the default relate finding policy list.
    m_relateNamePolicyList << LyricsNamedArtistHyphonTitle
                           << LyricsNamedTitle
                           << LyricsNamedAlbumHyphonTitle;
    //Move the online lyrics to online thread.
    m_onlineLyrics->moveToThread(m_onlineThread);
    //Link the online lyrics.
    connect(m_onlineLyrics, &KNMusicOnlineLyrics::lyricsDownload,
            this, &KNMusicLyricsManager::onActionLyricsDownloaded,
            Qt::QueuedConnection);
    //Start online thread.
    m_onlineThread->start();
}

KNMusicLyricsManager::~KNMusicLyricsManager()
{
    //Quit the online thread.
    m_onlineThread->quit();
    m_onlineThread->wait();
    //Delete online lyrics.
    m_onlineLyrics->deleteLater();
}

KNMusicLyricsBackend *KNMusicLyricsManager::backend()
{
    return m_backend;
}

void KNMusicLyricsManager::loadLyrics(const KNMusicAnalysisItem &analysisItem)
{
    //Save the detail info for current lyrics info.
    m_detailInfo=analysisItem.detailInfo;
    //Reset the backend.
    m_backend->reset();
    //Load the local lyrics first.
    if(loadLocalLyrics(m_detailInfo))
    {
        //Lyrics load complete.
        return;
    }
    //Or else we need to download the lyrics.
    m_onlineLyrics->addToDownloadList(m_detailInfo);
}

bool KNMusicLyricsManager::loadLyricsFile(const QString &lyricsPath)
{
    //Check if the lyrics is exist.
    if(!QFileInfo::exists(lyricsPath))
    {
        //Failed to load if the lyrics is not exist.
        return false;
    }
    //Generate the data cache.
    QList<qint64> timeList;
    QStringList textList;
    //Use the parser to parse the file.
    if(m_parser->parseFile(lyricsPath, timeList, textList))
    {
        //Set the data to backend.
        m_backend->setLyricsData(timeList, textList);
        //Load the file successfully.
        return true;
    }
    //Failed to load the lrc file.
    return false;
}

void KNMusicLyricsManager::onActionLyricsDownloaded(
        const KNMusicDetailInfo &detailInfo,
        const QString &content)
{
    //Ensure the lyrics directory is exist.
    KNUtil::ensurePathValid(m_lyricsDir);
    //Save the lyrics content.
    KNUtil::saveTextToFile(
                //Generate the file path.
                m_lyricsDir + "/" + KNUtil::legalFileName(
                            detailInfo.textLists[Artist].toString() + " - " +
                            detailInfo.textLists[Name].toString() + ".lrc"),
                //Lyrics content.
                content);
    //Check whether the detail info is still the current one.
    if(detailInfo.filePath==m_detailInfo.filePath)
    {
        //Generate the data cache.
        QList<qint64> timeList;
        QStringList textList;
        //Use the parser to parse the file.
        if(m_parser->parseText(content, timeList, textList))
        {
            //Set the data to backend.
            m_backend->setLyricsData(timeList, textList);
        }
    }
}

inline bool KNMusicLyricsManager::loadLocalLyrics(
        const KNMusicDetailInfo &detailInfo)
{
    //Generate the same file name as the music.
    QFileInfo musicFileInfo(detailInfo.filePath);
    QString sameNameLyrics=musicFileInfo.completeBaseName()+".lrc";
    //Search the lyrics according the lyrics finding policy.
    for(int i=0; i<m_policyList.size(); i++)
    {
        //Load the lyrics according to the policy index in the policy list.
        switch(m_policyList.at(i))
        {
        case SameNameInLyricsDir:
            if(loadLyricsFile(m_lyricsDir+"/"+sameNameLyrics))
            {
                return true;
            }
            break;
        case RelateNameInLyricsDir:
            if(loadRelatedLyrics(m_lyricsDir, detailInfo))
            {
                return true;
            }
            break;
        case SameNameInMusicDir:
            if(loadLyricsFile(musicFileInfo.absolutePath()+"/"+
                              sameNameLyrics))
            {
                return true;
            }
            break;
        case RelateNameInMusicDir:
            if(loadRelatedLyrics(musicFileInfo.absolutePath(), detailInfo))
            {
                return true;
            }
            break;
        }
    }
    return false;
}

bool KNMusicLyricsManager::loadRelatedLyrics(
        const QString &dirPath,
        const KNMusicDetailInfo &detailInfo)
{
    //Find the lyrics named with the following.
    for(int i=0; i<m_relateNamePolicyList.size(); i++)
    {
        //Load the related lyrics according to the policy index in the policy
        //list.
        switch(m_relateNamePolicyList.at(i))
        {
        case LyricsNamedArtistHyphonTitle:
            if(loadLyricsFile(
                        dirPath+"/"+
                        KNUtil::legalFileName(
                            detailInfo.textLists[Artist].toString() + " - " +
                            detailInfo.textLists[Name].toString() + ".lrc")))
            {
                return true;
            }
            break;
        case LyricsNamedTitle:
            if(loadLyricsFile(
                        dirPath+"/"+
                        KNUtil::legalFileName(
                            detailInfo.textLists[Name].toString() + ".lrc")))
            {
                return true;
            }
            break;
        case LyricsNamedAlbumHyphonTitle:
            if(loadLyricsFile(
                        dirPath+"/"+
                        KNUtil::legalFileName(
                            detailInfo.textLists[Album].toString() + " - " +
                            detailInfo.textLists[Name].toString() + ".lrc")))
            {
                return true;
            }
            break;
        }
    }
    return false;
}

QString KNMusicLyricsManager::lyricsDirectory() const
{
    return m_lyricsDir;
}

void KNMusicLyricsManager::appendDownloader(KNMusicLyricsDownloader *downloader)
{
    m_onlineLyrics->appendDownloader(downloader);
}

void KNMusicLyricsManager::setLyricsDirectory(const QString &lyricsDir)
{
    m_lyricsDir = lyricsDir;
}
