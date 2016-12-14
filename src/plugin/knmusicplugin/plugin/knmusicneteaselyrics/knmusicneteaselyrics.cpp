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
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "knmusicneteaselyrics.h"

#include <QDebug>

KNMusicNeteaseLyrics::KNMusicNeteaseLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{

}

QString KNMusicNeteaseLyrics::downloaderName()
{
    return tr("Netease Cloud Music");
}

void KNMusicNeteaseLyrics::downloadLyrics(
        const KNMusicDetailInfo &detailInfo,
        QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList)
{
    //Generate the search text.
    QString &&artist=detailInfo.textLists[Artist].toString(),
            &&title=detailInfo.textLists[Name].toString();
    //Generate the response cache.
    QByteArray responseData;
    //Do the search post request.
    if(post(generateNeteaseRequest("http://music.163.com/api/search/get/"
                                   "?s="+artist+"-"+title+"&"
                                   "limit=20&" +
                                   "type=1&offset=0"),
            QByteArray(),
            responseData)==200)
    {
        //Parse the data.
        QJsonObject dataObject=QJsonDocument::fromJson(responseData).object();
        //Get the result.
        if(!dataObject.contains("result"))
        {
            //The data object is failed.
            return;
        }
        //Get the result object.
        dataObject=dataObject.value("result").toObject();
        //Get the songs items array.
        QJsonArray searchItemList=dataObject.value("songs").toArray();
        //The format of search item list should be:
        /*[
         *    {
         *        "album":
         *            {
         *                "artist":{...},
         *                "copyrightId":0,
         *                "id":71074,
         *                "name":"only my railgun",
         *                "picId":3254554427603674,
         *                "publishTime":1257264000007,
         *                "size":4,
         *                "status":1
         *            },
         *        "artists":
         *            [
         *                {
         *                    "albumSize":0,
         *                    "alias":[],
         *                    "id":19395,
         *                    "img1v1":0,
         *                    "img1v1Url":"{Image URL}",
         *                    "name":"fripSide",
         *                    "picId":0,
         *                    "picUrl":null,
         *                    "trans":null
         *                }
         *            ],
         *        "copyrightId":0,
         *        "duration":257093,
         *        "fee":0,
         *        "ftype":0,
         *        "id":725692,
         *        "mvid":320126,
         *        "name":"only my railgun",
         *        "rUrl":null,
         *        "rtype":0,
         *        "status":-1
         *    },
         *    ...
         *]
         */
        //Get the lyrics data for all the objects in the list.
        for(auto i=searchItemList.begin(); i!=searchItemList.end(); ++i)
        {
            //Translate the first object.
            dataObject=(*i).toObject();
            //Prepare a lyrics detail object.
            KNMusicLyricsDetails currentDetails;
            //Set data to current details.
            currentDetails.title=dataObject.value("name").toString();
            currentDetails.artist=getArtistNames(dataObject);
            //Get the lyrics for the data object.
            if(get(generateNeteaseRequest(
                       "http://music.163.com/api/song/lyric?os=osx&id="+
                       QString::number(
                           qint64(dataObject.value("id").toDouble()))+
                       "&lv=-1&kv=-1&tv=-1"),
                   responseData)==200)
            {
                //Translate the response data to JSON object.
                dataObject=QJsonDocument::fromJson(responseData).object();
                //The lyrics data should be the following format.
                /* {
                 *     "code":200,
                 *     "klyric":
                 *     {
                 *         "lyric":null,
                 *         "version":10
                 *     },
                 *     "lrc":
                 *     {
                 *         "lyric":"(UTF-8 LRC Original language)",
                 *         "version":29
                 *     },
                 *     "lyricUser":
                 *     {
                 *         "demand":0,
                 *         "id":xxxxx,
                 *         "nickname":"xxxxx",
                 *         "status":0,
                 *         "uptime":xxxxx,
                 *         "userid":xxxxx
                 *     },
                 *     "qfy":false,
                 *     "sfy":true,
                 *     "sgc":true,
                 *     "tlyric":
                 *     {
                 *         "lyric":"(UTF-8 LRC Translate)",
                 *         "version":2
                 *     }
                 * }
                 */
                //Save the original lyrics, lrc.
                saveLyricsToList(dataObject, "lrc", detailInfo,
                                 currentDetails, lyricsList);
                //Save the translate lyrics when it contains "tlyric".
                saveLyricsToList(dataObject, "tlyric", detailInfo,
                                 currentDetails, lyricsList);
            }
        }
    }
}

inline QNetworkRequest KNMusicNeteaseLyrics::generateNeteaseRequest(
        const QString &url)
{
    //Generate the network request, initial it with the url.
    QNetworkRequest request;
    //Set default content type header.
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    //Set url of the request.
    request.setUrl(QUrl(url));
    //Set to header.
    request.setRawHeader("Cookie", "appver=2.0.2");
    //Set the "Referer" and "Origin".
    request.setRawHeader("Referer", "http://music.163.com/search/");
    request.setRawHeader("Original", "http://music.163.com");
    //Update the connext and host header.
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Host", "music.163.com");
    //Fake User-Agent, use Safari user-agent.
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) "
                         "AppleWebKit/537.36 (KHTML, like Gecko) "
                         "Chrome/33.0.1750.152 "
                         "Safari/537.36");
    //Give back the request.
    return request;
}

inline QString KNMusicNeteaseLyrics::getArtistNames(const QJsonObject &songData)
{
    //Get the artist list.
    const QJsonArray &artists=songData.value("artists").toArray();
    //Generate artist names list.
    QStringList artistsNames;
    //Get the artist names.
    for(int j=0; j<artists.size(); ++j)
    {
        //Add name to artist names.
        artistsNames.append(artists.at(j).toObject().value("name").toString());
    }
    //Give back the join data.
    return artistsNames.join(", ");
}

inline void KNMusicNeteaseLyrics::saveLyricsToList(
        const QJsonObject &lyricsObject,
        const QString &lyricsName,
        const KNMusicDetailInfo &detailInfo,
        KNMusicLyricsDownloader::KNMusicLyricsDetails &lyricsDetails,
        QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList)
{
    //Save the lyrics when it contains name of the lyrics.
    if(!lyricsObject.contains(lyricsName))
    {
        //When the lyrics
        return;
    }
    //Get the lyrics data.
    QString lyricsData=
            lyricsObject.value(lyricsName).toObject().value("lyric").toString();
    //Save the lyrics data from lrc object.
    saveLyrics(detailInfo, lyricsData, lyricsDetails, lyricsList);
}
