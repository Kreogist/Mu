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
#include <QLinkedList>
#include <QDomDocument>

#include "knmusicqqlyrics.h"

#include <QDebug>

KNMusicQQLyrics::KNMusicQQLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
    //Initial the GBK codec.
    m_gbkCodec=QTextCodec::codecForName("GBK");
}

QString KNMusicQQLyrics::downloaderName()
{
    return tr("QQMusic");
}

void KNMusicQQLyrics::initialStep(uint identifier,
                                  const KNMusicDetailInfo &detailInfo)
{
    // Step 0 - Start to search on the server.
    //Update the reply count.
    setReplyCount(identifier, 1);
    //Generate the url and get the data from the url.
    get(identifier,
        "http://qqmusic.qq.com/fcgi-bin/qm_getLyricId.fcg?name="+
        processKeywordsToGBK(detailInfo.textLists[Name].toString())+
        "&singer="+
        processKeywordsToGBK(detailInfo.textLists[Artist].toString())+
        "&from=qqplayer");
}

void KNMusicQQLyrics::processStep(uint identifier,
                                  int currentStep,
                                  const QList<KNMusicReplyData> &replyCaches)
{
    //Check the current step index.
    switch(currentStep)
    {
    // Step 1 - Parse the result data. Fetch the lyrics file.
    case 1:
    {
        //Check the size of the reply caches.
        if(replyCaches.size()!=1)
        {
            //Mission failed.
            completeRequest(identifier);
            return;
        }
        //Get the response data.
        const QByteArray &responseData=replyCaches.at(0).result;
        //Tencent use GBK as default codec, translate the data to UTF-8, parse
        //it with DomDocument.
        QDomDocument xmlDoc;
        xmlDoc.setContent(m_gbkCodec->toUnicode(responseData));
        //To find whether it contains song info.
        QDomNodeList nameList=xmlDoc.elementsByTagName("name"),
                singerNameList=xmlDoc.elementsByTagName("singername"),
                songInfoList=xmlDoc.elementsByTagName("songinfo");
        if(songInfoList.isEmpty())
        {
            //No result find.
            completeRequest(identifier);
            return;
        }
        //Get the song id from the song info.
        QStringList songIDList;
        QLinkedList<QMap<QString, QString>> lyricsInfoList;
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
                //Save the title and artist information.
                QMap<QString, QString> lyricsInfo;
                lyricsInfo.insert("title",
                                  QUrl::fromPercentEncoding(
                                      nameList.at(i).toElement(
                                          ).text().toUtf8()));
                lyricsInfo.insert("artist",
                                  QUrl::fromPercentEncoding(
                                      singerNameList.at(i).toElement(
                                          ).text().toUtf8()));
                //Write to the list.
                songIDList.append(currentID);
                lyricsInfoList.append(lyricsInfo);
            }
        }
        //Check if the song id is empty.
        if(songIDList.isEmpty())
        {
            //No result find.
            completeRequest(identifier);
            return;
        }
        //Get the detail data for each song.
        setReplyCount(identifier, songIDList.size());
        for(auto i : songIDList)
        {
            //Pick out the first item from the lyrics info list.
            QVariant lyricsInfo;
            lyricsInfo.setValue(lyricsInfoList.takeFirst());
            //Get the information form song ID.
            get(identifier, generateRequestString(i), lyricsInfo);
        }
        //Mission complete.
        break;
    }
    // Step 2 - Save the lrc file data.
    case 2:
    {
        //Get the detail data for each song.
        for(auto i : replyCaches)
        {
            //Get the lyrics information.
            QMap<QString, QString> lyricsInfo=
                    i.user.value<QMap<QString, QString>>();
            //Get the response data.
            const QByteArray &responseData=i.result;
            //Check the response data is empty or not.
            if(responseData.isEmpty())
            {
                //Ignore the current item.
                continue;
            }
            //Parse the response data.
            QString xml_text=m_gbkCodec->toUnicode(responseData);
            QDomDocument xmlDoc;
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
            //Save the lyrics data and calculate the similarity.
            saveLyrics(identifier,
                       lyricsInfo.value("title"),
                       lyricsInfo.value("artist"),
                       lyricsContent);
        }
        //Mission complete.
        completeRequest(identifier);
        break;
    }
    default:
        //It should never goes here.
        break;
    }
}

inline QString KNMusicQQLyrics::processKeywordsToGBK(const QString &keywords)
{
    //Use GBK codec to parse the encoded data.
    return m_gbkCodec->fromUnicode(
                processKeywords(keywords)).toPercentEncoding();
}

inline QString KNMusicQQLyrics::generateRequestString(const QString &id)
{
    return "http://music.qq.com/miniportal/static/lyric/" +
            QString::number(id.toLongLong()%100) +
            "/" +
            id +
            ".xml";
}
