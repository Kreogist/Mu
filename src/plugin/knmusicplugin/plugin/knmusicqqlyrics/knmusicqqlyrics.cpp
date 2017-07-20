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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
        "http://s.music.qq.com/fcgi-bin/music_search_new_platform"
        "?t=0&n=1&aggr=1&cr=1&loginUin=0&format=json&inCharset=utf-8&"
        "outCharset=utf-8&notice=0&platform=jqminiframe.json&needNewCode=0&p=1&"
        "catZhida=0&remoteplace=sizer.newclient.next_song&w=" +
        QUrl::toPercentEncoding(
            processKeywords(detailInfo.textLists[Name].toString())) + "+" +
        QUrl::toPercentEncoding(
            processKeywords(detailInfo.textLists[Artist].toString())));
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
        //In the url, the out codec has been set to UTF-8, so here it won't need
        //to transfer the codec.
        QJsonObject resultObject=QJsonDocument::fromJson(responseData).object();
        //Check the result object. it should has an value named data.
        if(!resultObject.contains("data"))
        {
            //Wrong format data.
            completeRequest(identifier);
            return;
        }
        //Pick out the data object.
        QJsonObject dataObject=resultObject.value("data").toObject();
        //Check the data object, it should has the song object.
        if(!dataObject.contains("song"))
        {
            //Wrong format data.
            completeRequest(identifier);
            return;
        }
        //Pick out the song object.
        QJsonObject songObject=dataObject.value("song").toObject();
        //Check the song object, it should contain a list named 'list'.
        if(!songObject.contains("list"))
        {
            //Wrong format data.
            completeRequest(identifier);
            return;
        }
        //Pick out the song list.
        QJsonArray songList=songObject.value("list").toArray();
        //Check the song list size.
        if(songList.isEmpty())
        {
            //No search result found, failed.
            completeRequest(identifier);
            return;
        }
        //Generate the lyrics info cache.
        QList<QMap<QString, QString>> lyricsInfoList;
        //Loop and check all the song information.
        for(auto songListItem : songList)
        {
            //Transfer the item into the object.
            QJsonObject listItem=songListItem.toObject();
            //Check the list item object.
            if(!listItem.contains("grp"))
            {
                //Check next item.
                continue;
            }
            //Pick out the group.
            QJsonArray songItemGroup=listItem.value("grp").toArray();
            //Check the group item.
            for(auto item : songItemGroup)
            {
                //Tranfer the item into the group.
                QJsonObject groupItem=item.toObject();
                //It should contain an value named "f".
                if(!groupItem.contains("f"))
                {
                    //Check the next item.
                    continue;
                }
                //Get the "f" value, split the f into value list.
                QStringList fValueList=
                        groupItem.value("f").toString().split('|');
                //Check the fValueList size.
                if(fValueList.size()<4)
                {
                    //Wrong format caught.
                    continue;
                }
                //Get the song id, title and artist data.
                QMap<QString, QString> lyricsInfo;
                lyricsInfo.insert("song_id", fValueList.at(0));
                lyricsInfo.insert("title", fValueList.at(1));
                lyricsInfo.insert("artist", fValueList.at(3));
                //Save the lyrics info to the list.
                lyricsInfoList.append(lyricsInfo);
            }
        }
        //Check the lyrics info list size.
        if(lyricsInfoList.isEmpty())
        {
            //No valid lyrics info found, mission failed.
            completeRequest(identifier);
            return;
        }
        //Loop and fetch the lyrics data.
        setReplyCount(identifier, lyricsInfoList.size());
        for(auto lyricsInfo : lyricsInfoList)
        {
            //Transfer the info map into a variant.
            QVariant lyricsData;
            lyricsData.setValue(lyricsInfo);
            //Get the song id.
            int songId=lyricsInfo.value("song_id").toInt();
            //Get the lyrics content.
            get(identifier,
                "http://music.qq.com/miniportal/static/lyric/" +
                QString::number(songId%100)+"/"+QString::number(songId)+
                ".xml",
                lyricsData);
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
            //Get the lyrics XML format data, the XML file should be like.
            /*
             * <?xml version=\"1.0\" encoding=\"GB2312\" ?>
             * <lyric>
             * <![CDATA[Lyrics Content here!]]>
             * </lyric>
             */
            //Translate the result into DomDocument, the codec should be GB2312.
            QDomDocument lyricsDocument;
            //!FIXME: the codec here should be read from the XML file.
            if(!lyricsDocument.setContent(m_gbkCodec->toUnicode(i.result)))
            {
                //Wrong XML format document.
                continue;
            }
            //Get the root element data.
            QDomElement rootElement=lyricsDocument.documentElement();
            //Check the element tag name.
            if(rootElement.tagName()!="lyric" ||
                    rootElement.childNodes().isEmpty())
            {
                //Wrong tag found.
                continue;
            }
            //Get the CDATA node text.
            QDomNode cdataNode=rootElement.firstChild();
            //Dump the CDATA node text.
            QString lyricsText;
            QTextStream stream(&lyricsText);
            cdataNode.save(stream, QDomNode::CDATASectionNode);
            //Check the lyrics text.
            if(!lyricsText.startsWith("<![CDATA[") ||
                    !lyricsText.endsWith("]>"))
            {
                //Invalid CDATA node found.
                continue;
            }
            //Remove the CDATA outter frame.
            lyricsText.remove(0, 9); // <![CDATA[
            lyricsText.remove(lyricsText.size()-2, 2); // ]>
            //Get the lyrics information.
            QMap<QString, QString> lyricsInfo=
                    i.user.value<QMap<QString, QString>>();
            //Pick out the lyrics info value.
            saveLyrics(identifier,
                       lyricsInfo.value("title"),
                       lyricsInfo.value("artist"),
                       lyricsText);
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
