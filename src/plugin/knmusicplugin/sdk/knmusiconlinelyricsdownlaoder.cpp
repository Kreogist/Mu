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

#include "knmusiconlinelyricsdownlaoder.h"

KNMusicOnlineLyricsDownloader::KNMusicOnlineLyricsDownloader(QObject *parent) :
    QObject(parent),
    m_downloaders(QList<KNMusicLyricsDownloader *>()),
    m_cancelFlag(false),
    m_running(false)
{
}

void KNMusicOnlineLyricsDownloader::appendDownloader(
        KNMusicLyricsDownloader *downloader)
{
    //Simply add downloader to list.
    m_downloaders.append(downloader);
}

bool KNMusicOnlineLyricsDownloader::isRunning()
{
    return m_running;
}

void KNMusicOnlineLyricsDownloader::downloadLyrics(
        const KNMusicDetailInfo &detailInfo)
{
    //Reset the cancel flag.
    m_cancelFlag=false;
    //Set up the running flag.
    m_running=true;
    //Generate the lyrics list.
    QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> lyricsList;
    //Emit the empty list data right now.
    emit listContentChanged(lyricsList);
    //Get the lyrics size.
    int lyricsListSize=0;
    //Download the lyrics data via all the plugins.
    for(int i=0; i<m_downloaders.size(); ++i)
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
        //Emit the server changed signal.
        emit serverChanged(i+1, m_downloaders.size());
        //Try to download the lyrics from all the remote server.
        m_downloaders.at(i)->downloadLyrics(detailInfo, lyricsList);
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
        if(lyricsList.size() > lyricsListSize)
        {
            //Save the new lyrics list size.
            lyricsListSize=lyricsList.size();
            //Sort the lyrics list right now.
            std::sort(lyricsList.begin(),
                      lyricsList.end(),
                      KNMusicLyricsDownloader::lyricsDetailLessThan);
            //Emit the have content signal.
            emit listContentChanged(lyricsList);
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
    }
    //After download all the data, emit the signal.
    emit downloadComplete();
    //Reset the running flag.
    m_running=false;
}

void KNMusicOnlineLyricsDownloader::cancelDownload()
{
    //Set the cancel flag to true.
    m_cancelFlag=true;
}

