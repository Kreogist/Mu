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
#include <QLinkedList>
#include <QDomDocument>
#include <QRegularExpression>

#include <QNetworkAccessManager>

#include "knmusicxiamilyrics.h"

#include <QDebug>

KNMusicXiamiLyrics::KNMusicXiamiLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
}

QString KNMusicXiamiLyrics::downloaderName()
{
    return "XiaMi Music";
}

void KNMusicXiamiLyrics::initialStep(uint identifier,
                                     const KNMusicDetailInfo &detailInfo)
{
    // Step 0 - Start to search on the server.
    //Update the reply count.
    setReplyCount(identifier, 1);
    //Get the data from the url.
    get(identifier,
        "http://www.xiami.com/search/song-lyric?key=" +
        processKeywords(detailInfo.textLists[Name].toString()),
        QVariant());
}

void KNMusicXiamiLyrics::processStep(uint identifier,
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
        //Get the data from the reply caches.
        const QByteArray &responseData=replyCaches.at(0).result;
        //Check the response data.
        if(responseData.isEmpty())
        {
            //Mission failed.
            completeRequest(identifier);
            return;
        }
        QString xmlhttpText=responseData;
        //Parse the data.
        QStringList songIdList;
        QRegularExpression rex("<a.*?href=\".*?/song/(\\d+).*?><b.*?key_red");
        QRegularExpressionMatchIterator i=rex.globalMatch(xmlhttpText);
        while(i.hasNext())
        {
            QRegularExpressionMatch match=i.next();
            songIdList.append(match.captured(1));
        }
        //Check the song id list size.
        if(songIdList.isEmpty())
        {
            //Failed to find any song.
            completeRequest(identifier);
            return;
        }
        //Update the reply count.
        setReplyCount(identifier, songIdList.size());
        for(QStringList::iterator i=songIdList.begin();
            i!=songIdList.end();
            ++i)
        {
            get(identifier, "http://www.xiami.com/song/playlist/id/"+(*i),
                QVariant());
        }
        //Complete.
        break;
    }
    case 2:
    {
        //Prepare the url cache and lyrics data caches.
        QLinkedList<QMap<QString, QString>> lyricsDataList;
        QStringList lyricsUrlList;
        //Loop and check the response data.
        for(auto replyData : replyCaches)
        {
            //Check the response data.
            const QByteArray &responseData=replyData.result;
            if(!responseData.isEmpty())
            {
                //Parse the document.
                QDomDocument lyricsDocument;
                lyricsDocument.setContent(responseData);
                //Get the tracklist.
                QDomElement root=lyricsDocument.documentElement();
                QDomNodeList trackList=root.elementsByTagName("trackList");
                //Get the information from the tracklist.
                for(int j=0; j<trackList.size(); j++)
                {
                    QDomElement currentTrack=trackList.at(j).toElement();
                    //Find lyrics url.
                    QDomNodeList lyricUrlList=
                            currentTrack.elementsByTagName("lyric");
                    if(!lyricUrlList.isEmpty())
                    {
                        QDomNodeList lyricsUrl=
                                lyricUrlList.at(0).toElement().childNodes();
                        if(!lyricsUrl.isEmpty())
                        {
                            //Save the lyrics information.
                            QMap<QString, QString> lyricsData;
                            lyricsData.insert("title",
                                              getContentText(&currentTrack,
                                                             "title"));
                            lyricsData.insert("artist",
                                              getContentText(&currentTrack,
                                                             "artist"));
                            //Add the lyrics information and url to the list.
                            lyricsDataList.append(lyricsData);
                            lyricsUrlList.append(lyricsUrl.at(0).nodeValue());
                        }
                    }
                }
            }
        }
        //Check the lyrics url list.
        if(lyricsUrlList.isEmpty())
        {
            //Theoretically, if it comes here, the list shouldn't be 0.
            //Failed to find any song.
            completeRequest(identifier);
            return;
        }
        //Set the total count.
        setReplyCount(identifier, lyricsUrlList.size());
        //Try to download the lyrics.
        for(QStringList::iterator i=lyricsUrlList.begin();
            i!=lyricsUrlList.end();
            ++i)
        {
            //Prepare the user data.
            QVariant lyricsUser;
            lyricsUser.setValue(lyricsDataList.takeFirst());
            //Get the data from the url.
            get(identifier, (*i), lyricsUser);
        }
        //Complete.
        break;
    }
    case 3:
    {
        //Loop and check the response data.
        for(auto i : replyCaches)
        {
            //Get the response data.
            const QByteArray &responseData=i.result;
            //Get the lyrics data.
            QMap<QString, QString> lyricsData=
                    i.user.value<QMap<QString, QString>>();
            //This is the lyrics file! Add to the lyrics list.
            saveLyrics(identifier,
                       lyricsData.value("title"),
                       lyricsData.value("artist"),
                       responseData);
        }
        //All the mission is complete.
        completeRequest(identifier);
        break;
    }
    default:
        //Shouldn't come here.
        break;
    }
}

inline QString KNMusicXiamiLyrics::getContentText(QDomElement *currentTrack,
                                                  const QString &tagName)
{
    //Get the tag name element.
    QDomNodeList &&elementList=currentTrack->elementsByTagName(tagName);
    //Check element list.
    if(elementList.isEmpty())
    {
        //There should be only 1 element in the list.
        return QString();
    }
    //Translate the first one into element, and get the child nodes.
    elementList=elementList.at(0).toElement().childNodes();
    //Check the element list once more.
    if(elementList.isEmpty())
    {
        //There should be only 1 element in the list.
        return QString();
    }
    //Get the element node value.
    return elementList.at(0).nodeValue();
}
