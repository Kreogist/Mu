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
    return tr("KGMusic");
}

void KNMusicKgmusicLyrics::downloadLyrics(
        const KNMusicDetailInfo &detailInfo,
        QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList)
{
    //Combine the artist and name.
    QString searchCode,
            &&artist=detailInfo.textLists[Artist].toString(),
            &&title=detailInfo.textLists[Name].toString();
    //Check title text.
    if(title.isEmpty())
    {
        //We won't search anything when title is empty.
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
    QByteArray responseData;
    get("http://lib9.service.kugou.com/websearch/index.php?"
        "page=1&cmd=100&keyword="+QUrl::toPercentEncoding(searchCode)+
        "&pagesize=3",
        responseData);
    //Check out the response data.
    if(responseData.isEmpty())
    {
        //Mission complete.
        return;
    }
    /*
     * The response data should be the following structure:
     *  {
     *      "status":1,
     *      "data":
     *      {
     *          "total":3,
     *          "songs":
     *          [
     *              {
     *                  "singername":"Fripside",
     *                  "songname":"Flower Of Bravery",
     *                  "filename":"fripside - flower of bravery",
     *                  "hash":"1C62244B29E7CBA931163B68E5A153A5",
     *                  "filesize":3757465,
     *                  "bitrate":128,
     *                  "timelength":234000,
     *                  "extname":"mp3",
     *                  "ownercount":1390
     *              },
     *              {
     *                  ...
     *              },
     *              {
     *                  ...
     *              }
     *          ]
     *      },
     *      "error":""
     *  }
     */
    //Prepare a json list.
    QJsonArray resultList;
    //It returns a json object, parse it.
    QJsonObject resultObject=QJsonDocument::fromJson(responseData).object();
    //Check whether it contains data object.
    if(!resultObject.contains("data"))
    {
        //Failed if it doesn't contains data.
        return;
    }
    //Get the data item.
    resultObject=resultObject.value("data").toObject();
    //Get the songs list.
    resultList=resultObject.value("songs").toArray();
    //Check out the result object.
    if(resultObject.value("total").toInt()!=resultList.size())
    {
        //Failed to parse the information.
        return;
    }
    //Translate the item of reuslt list to result object.
    for(auto i=resultList.begin(); i!=resultList.end(); ++i)
    {
        //Get the song object.
        resultObject=(*i).toObject();
        //Get the lyrics from server.
        get("http://lyrics.kugou.com/search?ver=1&man=yes&client=pc&keyword=" +
            QUrl::toPercentEncoding(
                resultObject.value("singername").toString()) +
            "-"+
            QUrl::toPercentEncoding(resultObject.value("songname").toString())+
            "&duration="+
            QString::number(resultObject.value("timelength").toInt())+
            "&hash="+
            resultObject.value("hash").toString(),
            responseData);
        //Check out response data.
        if(responseData.isEmpty())
        {
            //Goto next item
            continue;
        }
        /*
         * The response data structure should be
         *  {
         *      "candidates":
         *      [
         *          {
         *              "accesskey":"1F4053C63399083E798E8A00269E099A",
         *              "adjust":0,
         *              "duration":234000,
         *              "id":"16617584",
         *              "score":60,
         *              "singer":"fripSide",
         *              "song":"flower of bravery",
         *              "uid":"1000000010"
         *          },
         *          {
         *              ...
         *          },
         *          ...
         *      ],
         *      "info":"OK",
         *      "keyword":"Fripside-Flower Of Bravery",
         *      "proposal":"16617584",
         *      "status":200
         *  }
         */
        //Parse the json object.
        resultObject=QJsonDocument::fromJson(responseData).object();
        //Check out whether it contains 'info' item and it should be "OK", and
        //it contains a candidates list.
        if(resultObject.value("info").toString()!="OK" ||
                !resultObject.contains("candidates"))
        {
            //Try next one.
            continue;
        }
        //Save the title and artist information.
        KNMusicLyricsDetails currentDetails;
        //Parse the list.
        QJsonArray &&candidateList=resultObject.value("candidates").toArray();
        //Check out each item.
        for(auto j=candidateList.begin(); j!=candidateList.end(); ++j)
        {
            //Translate j to item.
            resultObject=(*j).toObject();
            //Save the title and artist data.
            currentDetails.title=resultObject.value("song").toString();
            currentDetails.artist=resultObject.value("singer").toString();
            //Get lyrics according to the item candidates.
            get("http://lyrics.kugou.com/download?ver=1&client=pc&id=" +
                resultObject.value("id").toString() +
                "&accesskey=" +
                resultObject.value("accesskey").toString() +
                "&fmt=krc&charset=utf8",
                responseData);
            /*
             * The structure of response data is
             *  {
             *      "charset":"",
             *      "content":"Base 64 Encoded content",
             *      "fmt":"krc",
             *      "info":"OK",
             *      "status":200
             *  }
             */
            //Parse the response data.
            resultObject=QJsonDocument::fromJson(responseData).object();
            //Check whether it contains content or not.
            if(resultObject.contains("content"))
            {
                //The content is in KRC format which is a specific format made
                //by kugoo, we have to parse it.
                //Parse the content from Base64 encoding and uncompress the
                //data.
                QString rawKRCContent=
                        parseKRC(QByteArray::fromBase64(
                                     resultObject.value("content").toString(
                                         ).toUtf8())),
                        lyricsCache;
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
                                lyricsLine.mid(1,
                                               commaPosition-1).toLongLong(
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
                //Save the current detail to the list.
                saveLyrics(detailInfo, lyricsCache, currentDetails, lyricsList);
            }
        }
    }
}

QString KNMusicKgmusicLyrics::parseKRC(const QByteArray &krcData)
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
