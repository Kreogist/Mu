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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "knmusicttpodlyrics.h"

#include <QDebug>

KNMusicTtpodLyrics::KNMusicTtpodLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
}

QString KNMusicTtpodLyrics::downloaderName()
{
    return "TTPod";
}

void KNMusicTtpodLyrics::initialStep(uint identifier,
                                     const KNMusicDetailInfo &detailInfo)
{
    //Step 0 - send the search request.
    //Update the reply count.
    setReplyCount(identifier, 1);
    //Get the response from URL.
    get(identifier, "http://so.ard.iyyin.com/s/song_with_out?q=" +
        processKeywords(detailInfo.textLists[Name].toString()) +
        "+" +
        processKeywords(detailInfo.textLists[Artist].toString()));
}

void KNMusicTtpodLyrics::processStep(uint identifier,
                                     int currentStep,
                                     const QList<KNMusicReplyData> &replyCaches)
{
    //Check the current step index.
    switch(currentStep)
    {
    // Step 1 - Parse the result, and fetch all the result back.
    case 1:
    {
        //Check the reply caches.
        if(replyCaches.size()!=1)
        {
            //Mission failed.
            completeRequest(identifier);
            return;
        }
        //Get the data, and parse the json.
        QJsonDocument songInfoList=
                QJsonDocument::fromJson(replyCaches.at(0).result);
        if(songInfoList.isNull())
        {
            //Mission failed.
            completeRequest(identifier);
            return;
        }
        //Get the 'data' to from the base object.
        QJsonArray songListData=songInfoList.object().value("data").toArray();
        //Update the reply count size.
        setReplyCount(identifier, songListData.size());
        for(auto i : songListData)
        {
            //Get the current object.
            QJsonObject currentObject=i.toObject();
            /*
             * The structure of the currentObject should be:
             * {
             *   "album_id":1080431,
             *   "album_name":"THE WORKS ~xxxxxx~ 5.0",
             *   "artist_flag":"0",
             *   "flag":1,
             *   "out_list":
             *   [
             *       {
             *           "logo":"http://pic.ttpod.cn/upload/new/forbidden.png",
             *           "name":"xxxx xxxx"
             *       }
             *   ],
             *   "singer_name":"xxxx",
             *   "song_id":2567205,
             *   "song_name":"flower of bravery"
             * }
             */
            //Generate the URL.
            QString downloadURL=
                    "http://lp.music.ttpod.com/lrc/down?lrcid=&artist=" +
                    currentObject.value("singer_name").toString() +
                    "&title=" +
                    currentObject.value("song_name").toString() +
                    "&song_id=" +
                    currentObject.value("song_id").toString();
            //Get the data from the download URL.
            get(identifier, downloadURL, currentObject);
        }
        break;
    }
    // Step 2 - Parse all the response data.
    case 2:
    {
        // Check all the data in the reply caches.
        for(auto i : replyCaches)
        {
            //Check the response data.
            if(i.result.isEmpty())
            {
                //Ignore the empty request.
                continue;
            }
            //Load the data to json object.
            QJsonDocument lyricsDocument=QJsonDocument::fromJson(i.result);
            QJsonObject currentObject=i.user.toJsonObject();
            //Get the data object from the document.
            QJsonObject lyricsObject=lyricsDocument.object();
            /*
             * The structure of lyricsObject should be:
             * {
             *      "code":1,
             *      "data":
             *      {
             *          "lrc":"[ti:flower of bravery]..."
             *      }
             * }
             */
            if(lyricsObject.isEmpty())
            {
                continue;
            }
            //Get the lrc in the data object in the lyrics object.
            QJsonObject dataObject=lyricsObject.value("data").toObject();
            if(dataObject.isEmpty())
            {
                continue;
            }
            QString lrcText=dataObject.value("lrc").toString();
            if(!lrcText.isEmpty())
            {
                //Save the lyrics.
                saveLyrics(identifier,
                           currentObject.value("song_name").toString(),
                           currentObject.value("singer_name").toString(),
                           lrcText);
            }
        }
        //Mission complete.
        completeRequest(identifier);
        break;
    }
    default:
        //Should never arrived here.
        break;
    }
}
