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

#include "knmusiconlinelyricsdownloader.h"

#include <QDebug>

KNMusicOnlineLyricsDownloader::KNMusicOnlineLyricsDownloader(QObject *parent) :
    QObject(parent),
    m_downloaders(QList<KNMusicLyricsDownloader *>()),
    m_identifier(qHash("OnlineLyricsDownloader")),
    m_completeDownloader(0),
    m_cancelFlag(false),
    m_running(false)
{
}

void KNMusicOnlineLyricsDownloader::appendDownloader(
        KNMusicLyricsDownloader *downloader)
{
    //Simply add downloader to list.
    m_downloaders.append(downloader);
    //Link the downloader data to this.
    connect(downloader, &KNMusicLyricsDownloader::missionComplete,
            this, &KNMusicOnlineLyricsDownloader::onActionDownloadFinished);
}

bool KNMusicOnlineLyricsDownloader::isRunning()
{
    return m_running;
}

void KNMusicOnlineLyricsDownloader::downloadLyrics(
        const KNMusicDetailInfo &detailInfo)
{
    //Check the running flag.
    if(m_running)
    {
        //Cancel the current running mission first.
        cancelDownload();
    }
    //Reset the cancel flag.
    m_cancelFlag=false;
    //Set up the running flag.
    m_running=true;
    //Save the new detail info.
    m_workingDetailInfo=detailInfo;
    //Clear the lyrics list.
    m_lyricsList=QList<KNMusicLyricsDownloader::KNMusicLyricsDetails>();
    m_completeDownloader=0;
    //Emit the empty list data right now.
    emit listContentChanged(m_lyricsList);
    //Emit the server changed signal.
    emit serverChanged(0, m_downloaders.size());
    //Download the lyrics data via all the plugins.
    for(auto i : m_downloaders)
    {
        //Before doing downloading, checking the cancel flag first.
        if(m_cancelFlag)
        {
            //Reset the running flag.
            m_running=false;
            //Emit signal.
            emit downloadCancel();
            //Won't download before calling the download.
            return;
        }
        //Try to download the lyrics from all the remote server.
        i->downloadLyrics(m_identifier, detailInfo);
    }
}

void KNMusicOnlineLyricsDownloader::cancelDownload()
{
    //Check the running state.
    if(!m_running)
    {
        //The downloader is currently not running.
        return;
    }
    //Cancel all the mission of the downloader.
    for(auto i : m_downloaders)
    {
        //Cancel the downloader mission.
        i->cancel(m_identifier);
    }
    //Set the cancel flag to true.
    m_cancelFlag=true;
}

void KNMusicOnlineLyricsDownloader::onActionDownloadFinished(
        uint identifier,
        const KNMusicDetailInfo &detailInfo,
        QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList)
{
    //Check the identifier first.
    if(identifier!=m_identifier ||
            detailInfo.textLists[Name]!=m_workingDetailInfo.textLists[Name] ||
            detailInfo.textLists[Artist]!=m_workingDetailInfo.textLists[Artist])
    {
        //This signal is not for me.
        return;
    }
    //Increase the counter of the completed downloader.
    ++m_completeDownloader;
    //Emit the server changed signal.
    emit serverChanged(m_completeDownloader, m_downloaders.size());
    //Check out the cancel flag.
    if(m_cancelFlag)
    {
        //Reset the running flag.
        m_running=false;
        //Emit signal.
        emit downloadCancel();
        //Won't download more.
        return;
    }
    //Check the lyrics list size and emit signal.
    if(!lyricsList.isEmpty())
    {
        //Increase the new lyrics list to the cached list.
        m_lyricsList.append(lyricsList);
        //Sort the lyrics list right now.
        std::sort(m_lyricsList.begin(),
                  m_lyricsList.end(),
                  KNMusicLyricsDownloader::lyricsDetailLessThan);
        //Emit the have content signal.
        emit listContentChanged(m_lyricsList);
    }
    //Check out the cancel flag.
    if(m_cancelFlag)
    {
        //Reset the running flag.
        m_running=false;
        //Emit signal.
        emit downloadCancel();
        //Won't download more.
        return;
    }
    //Check whether the completed downloader count is the same as the size of
    //the downloaders.
    if(m_completeDownloader==m_downloaders.size())
    {
        //After download all the data, emit the signal.
        emit downloadComplete();
        //Reset the running flag.
        m_running=false;
    }
}

