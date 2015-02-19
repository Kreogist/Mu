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
#include <QNetworkCookie>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrl>

#include "knmusicneteaselyrics.h"

#include <QDebug>

KNMusicNeteaseLyrics::KNMusicNeteaseLyrics(QObject *parent):
    KNMusicLyricsDownloader(parent)
{
    ;
}

void KNMusicNeteaseLyrics::downloadLyrics(const KNMusicDetailInfo &detailInfo,
                                        QList<KNMusicLyricsDetails> &lyricsList)
{
    //Generate cookies.
    QNetworkCookie neteaseCookie;
    neteaseCookie.setName("appver");
    neteaseCookie.setValue("2.0.2;");
    QVariant neteaseCookieData;
    neteaseCookieData.setValue(neteaseCookie);
    //Generate search url.
    QString musicSearchURL="http://music.163.com/api/search/get/";
    QByteArray responseData, parameter;
    parameter.append("limit=20&offset=0&type=1&s="+
                     QUrl::toPercentEncoding(detailInfo.textLists[Name]));
    post(musicSearchURL,
         responseData,
         parameter,
         neteaseCookieData,
         "http://music.163.com/");
    //Parse the json data.
    QJsonObject songListObject=QJsonDocument::fromJson(responseData).object();
    if(!songListObject.contains("result"))
    {
        return;
    }
    //Get the result list.
    QJsonObject resultObject=songListObject.value("result").toObject();
    if(resultObject.value("songCount")==0)
    {
        return;
    }
    QJsonArray resultList=resultObject.value("songs").toArray();
    for(QJsonArray::iterator i=resultList.begin();
        i!=resultList.end();
        ++i)
    {
        //Each song contains the name, artist and id.
        QJsonObject currentSong=(*i).toObject();
        //Generate the lyrics URL.
        QString lyricsURL="http://music.163.com/api/song/lyric?os=pc&id="+
                           QString::number(currentSong.value("id").toInt())+
                           "&lv=-1&tv=-1";
        //Get the lyrics.
        get(lyricsURL,
            responseData,
            neteaseCookieData,
            "http://music.163.com");
        if(responseData.isEmpty())
        {
            continue;
        }
        //Parse the response data as json object.
        QJsonObject lyricsObject=QJsonDocument::fromJson(responseData).object();
        //Check the code, 200 means success.
        if(lyricsObject.value("code")==200)
        {
            QJsonObject lrcObject=lyricsObject.value("lrc").toObject();
            //Generate the lyrics detail.
            KNMusicLyricsDetails currentLyrics;
            currentLyrics.title=currentSong.value("name").toString();
            QJsonArray artistList=currentSong.value("artists").toArray();
            for(QJsonArray::iterator j=artistList.begin();
                j!=artistList.end();
                ++j)
            {
                QJsonObject artistObject=(*j).toObject();
                currentLyrics.artist+=(currentLyrics.artist.isEmpty()?",":"")+
                            artistObject.value("name").toString();
            }
            saveLyrics(detailInfo,
                       lrcObject.value("lyric").toString(),
                       currentLyrics,
                       lyricsList);
        }
    }
}
