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
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "knmusickgmusiclyrics.h"

#include <QDebug>

KNMusicKgmusicLyrics::KNMusicKgmusicLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
    //Initial the magic bytes.
    m_magicBytes[0]='k';
    m_magicBytes[1]='r';
    m_magicBytes[2]='c';
    m_magicBytes[3]='1';
    //Initial the enc key.
    m_encKey[ 0]=0x40;
    m_encKey[ 1]=0x47;
    m_encKey[ 2]=0x61;
    m_encKey[ 3]=0x77;
    m_encKey[ 4]=0x5e;
    m_encKey[ 5]=0x32;
    m_encKey[ 6]=0x74;
    m_encKey[ 7]=0x47;
    m_encKey[ 8]=0x51;
    m_encKey[ 9]=0x36;
    m_encKey[10]=0x31;
    m_encKey[11]=0x2d;
    m_encKey[12]=0xce;
    m_encKey[13]=0xd2;
    m_encKey[14]=0x6e;
    m_encKey[15]=0x69;
    //Initial the attribute header.
    m_lyricsAttributeHeader.append("ar");
    m_lyricsAttributeHeader.append("ti");
    m_lyricsAttributeHeader.append("by");
    m_lyricsAttributeHeader.append("offset");
}

QString KNMusicKgmusicLyrics::downloaderName()
{
    return "KGMusic";
}

void KNMusicKgmusicLyrics::initialStep(uint identifier,
                                       const KNMusicDetailInfo &detailInfo)
{
    //Combine the artist and name.
    QString searchCode,
            &&artist=detailInfo.textLists[Artist].toString(),
            &&title=detailInfo.textLists[Name].toString();
    //Check title text.
    if(title.isEmpty())
    {
        //We won't search anything when title is empty.
        completeRequest(identifier);
        return;
    }
    //Check out the artist.
    if(artist.isEmpty())
    {
        //Then all the data is the title.
        searchCode=title;
    }
    else
    {
        //Make it to be artist-title.
        searchCode=artist+"-"+title;
    }
    //Get the lyrics search result.
    setReplyCount(identifier, 1);
    get(identifier,
        "http://lyrics.kugou.com/search?ver=1&man=yes&client=pc&keyword="+
        QUrl::toPercentEncoding(searchCode)+
        "&duration="+
        QString::number(detailInfo.duration)+"&hash=");
}

void KNMusicKgmusicLyrics::processStep(
        uint identifier,
        int currentStep,
        const QList<KNMusicReplyData> &replyCaches)
{
    //Check the current step index.
    switch(currentStep)
    {
    // Step 1 - Parse the result, and fetch all the result back.
    case 1:
    {
        //Check the response number.
        if(replyCaches.size()!=1)
        {
            //Error happens.
            completeRequest(identifier);
            return;
        }
        //Response data should be in the following format.
        /* {
         *     "candidates":
         *     [
         *         {
         *             "accesskey":"5BBCB7BDC8E9D8D7E2C38DF447D676DD",
         *             "adjust":0,
         *             "duration":234000,
         *             "id":"19811472",
         *             "krctype":2,
         *             "language":"",
         *             "score":60,
         *             "singer":"fripside",
         *             "song":"flower of bravery",
         *             "uid":"1000000010"
         *         },
         *         ...
         *     ],
         *     "info":"OK",
         *     "keyword":"fripSide-flower of bravery",
         *     "proposal":"19811472",
         *     "status":200
         * }
         */
        //It returns a json object, parse it.
        QJsonObject resultObject=
                QJsonDocument::fromJson(replyCaches.at(0).result).object();
        //Check whether it contains data object.
        if(!resultObject.contains("candidates"))
        {
            //Failed if it doesn't contains data.
            completeRequest(identifier);
            return;
        }
        QJsonArray resultList=resultObject.value("candidates").toArray();
        //Check the result list size.
        if(resultList.isEmpty())
        {
            //Failed if it doesn't has any result.
            completeRequest(identifier);
            return;
        }
        //Translate the item of reuslt list to result object.
        setReplyCount(identifier, resultList.size());
        for(auto i : resultList)
        {
            //Get the song object.
            QJsonObject candidateObject=i.toObject();
            //Get the lyrics from server.
            get(identifier,
                "http://lyrics.kugou.com/download?ver=1&client=pc&id=" +
                candidateObject.value("id").toString()+
                "&accesskey="+
                candidateObject.value("accesskey").toString()+
                "&fmt=krc&charset=utf8",
                candidateObject);
        }
        break;
    }
    case 2:
    {
        //Check out response data.
        for(auto i : replyCaches)
        {
            //Check the empty request.
            if(i.result.isEmpty())
            {
                //Goto next item
                continue;
            }
            //Response data should be:
            /*
             * {
             *     "charset":"",
             *     "content":"(Base64 Encoded UTF-8 KRC format lyrics)",
             *     "fmt":"krc",
             *     "info":"OK",
             *     "status":200
             * }
             */
            //Parse the json object.
            QJsonObject resultObject=QJsonDocument::fromJson(i.result).object();
            //Check out whether it contains 'info' item and it should be "OK",
            //and it contains a candidates list.
            if(resultObject.value("info").toString()!="OK" ||
                    !resultObject.contains("content"))
            {
                //Try next one.
                continue;
            }
            //The content is in KRC format which is a specific format made by
            //kugoo, we have to parse it.
            //Parse the content from Base64 encoding and uncompress the data.
            //Save the current detail to the list.
            QJsonObject songInfo=i.user.toJsonObject();
            saveLyrics(identifier,
                       songInfo.value("song").toString(),
                       songInfo.value("singer").toString(),
                       krcToLrc(parseKrc(QByteArray::fromBase64(
                          resultObject.value("content").toString().toUtf8()))));
        }
        //Mission complete.
        completeRequest(identifier);
        break;
    }
    }
}

QString KNMusicKgmusicLyrics::parseKrc(const QByteArray &krcData)
{
    //Check the first four bytes in the krc data.
    char *krcRawData=(char *)krcData.data();
    //Check out the first four bytes.
    if(memcmp(krcRawData, m_magicBytes, 4)!=0)
    {
        //This is not a standard krc format lyrics.
        return QString();
    }
    //Generate the buffer.
    QByteArray buffer;
    //Move the pointer to the 4 bytes after.
    for(int i=4; i<krcData.size(); ++i)
    {
        //Get the key data.
        buffer.append((int)krcRawData[i] ^ (int)(m_encKey[(i-4)%16]));
    }
    /*
     * According to the Qt document, we have to add a unsigned 32-bit integer to
     * the front of the buffer with big-endian(natural order).
     */
    //Get buffer size.
    quint32 bufferSize=buffer.size();
    quint8 bufferSizeData[4];
    //Write data to array.
    bufferSizeData[0]=(bufferSize & 0xFF000000) >> 24;
    bufferSizeData[1]=(bufferSize & 0x00FF0000) >> 16;
    bufferSizeData[2]=(bufferSize & 0x0000FF00) >> 8;
    bufferSizeData[3]=(bufferSize & 0x000000FF);
    //Prepend the data.
    buffer.prepend((char *)bufferSizeData, 4);
    //Use zlib uncompress to extract data.
    buffer=qUncompress(buffer);
    //Give back the buffer.
    return buffer;
}

QString KNMusicKgmusicLyrics::krcToLrc(QString rawKRCContent)
{
    //Prepare the LRC format cache string.
    QString lyricsCache;
    //First remove all the <xx,xxx,xx> item in the content.
    rawKRCContent.replace(QRegExp("<\\d*,\\d*,\\d*>"), "");
    //Split the lyrics into rows.
    QStringList &&dataResult=rawKRCContent.split("\n");
    //Check each line of the data result.
    for(auto i : dataResult)
    {
        //Get the simplified result of the i, remove the \r and \n.
        QString &&lyricsLine=i.simplified();
        //Check the result.
        if(lyricsLine.length()<2)
        {
            //Ignore the empty line.
            continue;
        }
        //Check is the lyrics line coverd by a [].
        if(lyricsLine.at(0)=='[')
        {
            //Check the last character.
            if(lyricsLine.at(lyricsLine.size()-1)==']')
            {
                //Find the colon ':' in the string.
                int colonPosition=lyricsLine.indexOf(':');
                //Check colon position.
                if(colonPosition==-1)
                {
                    //Ignore the line and find next one.
                    continue;
                }
                //Then check the header.
                //If the attribute is in the data list, then throw
                //it into the lyrics cache.
                if(m_lyricsAttributeHeader.contains(
                            lyricsLine.mid(
                                1,
                                colonPosition-1).toLower()))
                {
                    //Append line.
                    lyricsCache.append(lyricsLine);
                    //Append enter char.
                    lyricsCache.append("\n");
                }
                //Go to the next line.
                continue;
            }
            //Find the ']' position.
            int rightBracketPosition=lyricsLine.indexOf(']'),
                    //Parse the inside of the bracket, find the ms pos.
                    //Find the splitter ','.
                    commaPosition=lyricsLine.indexOf(',');
            //If we cannot find the ']' or ',', then we could ignore
            //the line.
            if(rightBracketPosition==-1 || commaPosition==-1)
            {
                //Go to next line.
                continue;
            }
            //Translate the number.
            bool translateResult=false;
            //Translate to longlong.
            qint64 position=
                    lyricsLine.mid(1,commaPosition-1).toLongLong(
                        &translateResult, 10);
            //Check the result.
            if(!translateResult)
            {
                //Ignore the line.
                continue;
            }
            //The position is in ms unit, translate to LRC format
            //time.
            qint64 minuate=position/60000,
                    second=(position-minuate*60000)/1000,
                    msecond=
                    ((position-minuate*60000)-second*1000)/10;
            //Translate them back to lyrics line.
            lyricsCache.append("["+
                               (minuate<10?"0":QString())+
                               QString::number(minuate)+":"+
                               (second<10?"0":QString())+
                               QString::number(second)+":"+
                               (msecond<10?"0":QString())+
                               QString::number(msecond)+
                               lyricsLine.mid(rightBracketPosition)+"\n");
        }
    }
    return lyricsCache;
}
