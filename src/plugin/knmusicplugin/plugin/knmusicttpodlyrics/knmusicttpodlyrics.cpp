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

KNMusicTtpodLyrics::~KNMusicTtpodLyrics()
{
    ;
}

QString KNMusicTtpodLyrics::downloaderName()
{
    return tr("TTPod");
}

void KNMusicTtpodLyrics::downloadLyrics(const KNMusicDetailInfo &detailInfo,
                                        QList<KNMusicLyricsDetails> &lyricsList)
{
    QString searchUrl="http://so.ard.iyyin.com/s/song_with_out?q=" +
                      processKeywords(detailInfo.textLists[Name].toString()) +
                      "+" +
                      processKeywords(detailInfo.textLists[Artist].toString());
    QByteArray responseData;
    //Get the response from URL.
    get(searchUrl, responseData);
    //Check the response data.
    if(responseData.isEmpty())
    {
        return;
    }
    //Get the data, and parse the json.
    QJsonDocument songInfoList=QJsonDocument::fromJson(responseData);
    if(songInfoList.isNull())
    {
        return;
    }
    //Get the 'data' to from the base object.
    QJsonArray songListData=songInfoList.object().value("data").toArray();
    for(auto i=songListData.begin(); i!=songListData.end(); ++i)
    {
        //Get the current object.
        QJsonObject currentObject=(*i).toObject();
        /*
         * The structure of the currentObject should be:
         * {
         *      "album_id":1080431,
         *      "album_name":"THE WORKS ~xxxxxx~ 5.0",
         *      "artist_flag":"0",
         *      "flag":1,
         *      "out_list":
         *      [
         *          {
         *              "logo":"http://pic.ttpod.cn/upload/new/forbidden.png",
         *              "name":"xxxx xxxx"
         *          }
         *      ],
         *      "singer_name":"xxxx",
         *      "song_id":2567205,
         *      "song_name":"flower of bravery"
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
        get(downloadURL, responseData);
        //Check the response data.
        if(!responseData.isEmpty())
        {
            QJsonDocument lyricsDocument=QJsonDocument::fromJson(responseData);
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
                //Generate the details item.
                KNMusicLyricsDetails currentDetails;
                //Save the title and artist data.
                currentDetails.title=
                        currentObject.value("song_name").toString();
                currentDetails.artist=
                        currentObject.value("singer_name").toString();
                //Save lyrics to the lyrics list.
                saveLyrics(detailInfo, lrcText, currentDetails, lyricsList);
            }
        }
    }
}
