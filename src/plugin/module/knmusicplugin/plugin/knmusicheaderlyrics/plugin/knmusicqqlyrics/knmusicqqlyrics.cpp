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
#include <QUrl>
#include <QTextCodec>
#include <QDomDocument>

#include "knmusicqqlyrics.h"

#include <QDebug>

KNMusicQQLyrics::KNMusicQQLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
    //Initial the GBK codec.
    m_gbkCodec=QTextCodec::codecForName("GBK");
}

QString KNMusicQQLyrics::downloadLyrics(const KNMusicDetailInfo &detailInfo)
{
    //Generate the url and get the data from the url.
    QString url="http://qqmusic.qq.com/fcgi-bin/qm_getLyricId.fcg?name="+
            processKeywordsToGBK(detailInfo.textLists[Name])+"&singer="+
            processKeywordsToGBK(detailInfo.textLists[Artist])+"&from=qqplayer";
    QByteArray responseData;
    get(url, responseData);
    //Check the response.
    if(responseData.isEmpty())
    {
        return QString();
    }
    //Tencent use GBK as default codec, translate the data to UTF-8, parse it
    //with DomDocument.
    QDomDocument xmlDoc;
    xmlDoc.setContent(m_gbkCodec->toUnicode(responseData));
    //To find whether it contains song info.
    QDomNodeList songInfoList=xmlDoc.elementsByTagName("songinfo");
    if(songInfoList.isEmpty())
    {
        return QString();
    }
    //Get the song id from the song info.
    QStringList songIDList;
    for(int i=0; i<songInfoList.length(); i++)
    {
        //Ensure the song info is available.
        QDomElement currentSongInfo=songInfoList.at(i).toElement();
        if(currentSongInfo.isNull())
        {
            continue;
        }
        //Ensure the id is not empty.
        QString currentID=currentSongInfo.attribute("id");
        if(!currentID.isEmpty())
        {
            songIDList.append(currentID);
        }
    }
    //Check if the song id is empty.
    if(songIDList.isEmpty())
    {
        return QString();
    }
    //Get the detail data for each song.
    for(QStringList::iterator i=songIDList.begin();
        i!=songIDList.end();
        ++i)
    {
        get(generateRequestString(*i), responseData);
        //Check the response data is empty or not.
        if(responseData.isEmpty())
        {
            continue;
        }
        //Parse the response data.
        QString xml_text=m_gbkCodec->toUnicode(responseData);
        xmlDoc.setContent(xml_text);
        //Find the lyrics.
        QDomNodeList lr=xmlDoc.elementsByTagName("lyric");
        if(lr.isEmpty())
        {
            continue;
        }
        QString lyricsContent=lr.at(0).childNodes().at(0).toText().data();
        if(lyricsContent.isEmpty())
        {
            continue;
        }
        //Write the lyrics and return the file name.
        return writeLyricsFile(detailInfo, lyricsContent);
    }
    return QString();
}

QString KNMusicQQLyrics::processKeywordsToGBK(const QString &keywords)
{
    return m_gbkCodec->fromUnicode(processKeywords(keywords)).toPercentEncoding();
}

QString KNMusicQQLyrics::generateRequestString(const QString &id)
{
    return "http://music.qq.com/miniportal/static/lyric/" +
            QString::number(id.toLongLong()%100) +
            "/" +
            id +
            ".xml";
}
