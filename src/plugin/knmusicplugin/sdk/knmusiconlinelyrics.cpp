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
#include "knmusiclrcparser.h"

#include "knmusiconlinelyrics.h"

#include <QDebug>

KNMusicOnlineLyrics::KNMusicOnlineLyrics(QObject *parent) :
    QObject(parent),
    m_lrcParser(new KNMusicLrcParser(this)),
    m_identifier(qHash("OnlineLyrics")),
    m_finishedDownloader(0),
    m_isWorking(false)
{
    //Link the download signal and slot.
    connect(this, &KNMusicOnlineLyrics::downloadNext,
            this, &KNMusicOnlineLyrics::onActionDownloadLyrics,
            Qt::QueuedConnection);
}

KNMusicOnlineLyrics::~KNMusicOnlineLyrics()
{
    //Remove all the downloaders.
    qDeleteAll(m_downloaders);
}

void KNMusicOnlineLyrics::appendDownloader(KNMusicLyricsDownloader *downloader)
{
    //Move the downloader to the current thread.
    downloader->setWorkingThread(thread());
    //Link the downloader to current function.
    connect(downloader, &KNMusicLyricsDownloader::missionComplete,
            this, &KNMusicOnlineLyrics::onActionDownloadFinished);
    //Add downloader to list.
    m_downloaders.append(downloader);
}

void KNMusicOnlineLyrics::addToDownloadList(const KNMusicDetailInfo &detailInfo)
{
    //Check detail info first, if the detail info is not in the list.
    if(!m_downloadQueue.contains(detailInfo))
    {
        //Add the detail info to the download queue.
        m_downloadQueue.append(detailInfo);
    }
    //Check the working state.
    if(!m_isWorking)
    {
        //Execute the download list.
        emit downloadNext();
    }
}

void KNMusicOnlineLyrics::onActionDownloadLyrics()
{
    //Check the download queue.
    if(m_downloadQueue.isEmpty())
    {
        //Lock the state.
        m_workingLock.lock();
        //Reset the working state to be false.
        m_isWorking=false;
        //Release the lock.
        m_workingLock.unlock();
        //We won't process anything for empty queue.
        return;
    }
    else
    {
        //Update working state, lock the working state.
        m_workingLock.lock();
        //Reset the working state to be false.
        m_isWorking=true;
        //Release the lock.
        m_workingLock.unlock();
    }
    //Get the last item in the download queue.
    KNMusicDetailInfo detailInfo=m_downloadQueue.takeLast();
    //Reset the download counter.
    m_finishedDownloader=0;
    //Download the lyrics data via all the plugins.
    for(auto i=m_downloaders.begin(); i!=m_downloaders.end(); ++i)
    {
        //Try to download the lyrics from all the remote server.
        (*i)->downloadLyrics(m_identifier, detailInfo);
    }
}

void KNMusicOnlineLyrics::onActionDownloadFinished(uint identifier,
        const KNMusicDetailInfo &detailInfo,
        QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList)
{
    //Check the identifier.
    if(identifier!=m_identifier)
    {
        //This signal is not for me.
        return;
    }
    //Check the lyrics list.
    if(!lyricsList.isEmpty())
    {
        //Sort the list according to the similarity of the lyrics, if the list
        //is not empty.
        std::sort(lyricsList.begin(),
                  lyricsList.end(),
                  KNMusicLyricsDownloader::lyricsDetailLessThan);
        //Generate the position and text list of the lyrics.
        QList<qint64> positionList;
        QStringList textList;
        //Parse all the data from the top to the bottom, save the first lyrics
        //which can be parsed.
        for(auto i=lyricsList.begin(); i!=lyricsList.end(); ++i)
        {
            //Parse the lyrics data.
            if(m_lrcParser->parseText((*i).lyricsData, positionList, textList))
            {
                //Emit the download success information.
                emit lyricsDownload(detailInfo, (*i).lyricsData);
                //Mission complete.
                break;
            }
        }
    }
    //Parse the next item.
    emit downloadNext();
}

