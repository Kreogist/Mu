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
#include <QTextCodec>

#include "knmusicglobal.h"
#include "knlocalemanager.h"

#include "knmusictagid3v1.h"

#include <QDebug>

#define TitleStart 3
#define ArtistStart 33
#define AlbumStart 63
#define YearStart 93
#define CommentStart 97
#define TrackJudgeByte 125
#define TrackPosition 126
#define GenrePosition 127

KNMusicTagId3v1::KNMusicTagId3v1(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Get the locale codec.
    m_codec=knI18n->localeCodec();
}

bool KNMusicTagId3v1::parseTag(QFile &musicFile,
                               QDataStream &musicDataStream,
                               KNMusicAnalysisItem &analysisItem)
{
    //Check is the file size is enough.
    int fileSize=musicFile.size();
    //ID3v1 is 128 bytes, so if the file size is less than 128, it can't have
    //ID3v1.
    if(fileSize<128)
    {
        return false;
    }
    //Generate the tag caches.
    char rawTagData[128];
    ID3v1Struct tagData;
    //Read the raw tag data.
    musicDataStream.skipRawData(fileSize-128);
    musicDataStream.readRawData(rawTagData, 128);
    //Check is the header 'TAG':
    if(rawTagData[0]!='T' || rawTagData[1]!='A' || rawTagData[2]!='G')
    {
        return false;
    }
    //Parse the raw data to tag data.
    parseRawData(rawTagData, tagData);
    //Write the tag data to detail info structure.
    writeToDetailInfo(tagData, analysisItem.detailInfo);
    return true;
}

bool KNMusicTagId3v1::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    //Write the data according to the detail info.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Check the file is still exist or not.
    QFile musicFile(detailInfo.filePath);
    //If the file is not exist then return false.
    if(!musicFile.exists())
    {
        return false;
    }

    //Generate the tag cache.
    char rawTagData[128];
    //Write the tag header.
    rawTagData[0]='T';
    rawTagData[1]='A';
    rawTagData[2]='G';
    //Write the title data.
    setRawTagData(rawTagData+TitleStart, detailInfo.textLists[Name], 30);
    setRawTagData(rawTagData+ArtistStart, detailInfo.textLists[Artist], 30);
    setRawTagData(rawTagData+AlbumStart, detailInfo.textLists[Album], 30);
    setRawTagData(rawTagData+YearStart, detailInfo.textLists[Year], 4);
    setRawTagData(rawTagData+CommentStart, detailInfo.textLists[Comments], 30);
    //Check the track index. If track index is larger than 0, and it's in the
    //range 0-255, then will use the ID3v1.1
    int trackIndex=detailInfo.textLists[TrackNumber].toInt();
    if(trackIndex>-1 && trackIndex<255)
    {
        //Set the track flag to 0.
        rawTagData[TrackJudgeByte]=0;
        //Set the track index.
        rawTagData[TrackPosition]=(quint8)trackIndex;
    }
    //Set the genre index.
    int genreIndex=knMusicGlobal->genreIndex(
                detailInfo.textLists[Genre].toString());
    rawTagData[GenrePosition]=(genreIndex==-1)?0xFF:(quint8)genreIndex;

    //Write the tag cache into the file.
    //Open the file, if we cannot open the file in read only mode, then return
    //false.
    if(!musicFile.open(QIODevice::ReadWrite))
    {
        return false;
    }
    //Check if the file contains ID3v1 tag.
    if(musicFile.size()>128)
    {
        //Set the position to the ID3v1 proper position.
        musicFile.seek(musicFile.size()-128);
        char tagHeader[4];
        musicFile.read(tagHeader, 3);
        //If the file don't contains the header, then we have to seek to the EOF.
        if(tagHeader[0]!='T' || tagHeader[1]!='A' || tagHeader[2]!='G')
        {
            //Seek the last 125 bytes.
            musicFile.seek(125);
        }
        else
        {
            //Reset and seek to the last 128 bytes.
            musicFile.reset();
            musicFile.seek(musicFile.size()-128);
        }
    }
    else
    {
        //Seek to the end of the file.
        musicFile.seek(musicFile.size());
    }
    //Write the raw tag data to file.
    musicFile.write(rawTagData, 128);
    //Close the file.
    musicFile.close();
    //Complete.
    return true;
}

bool KNMusicTagId3v1::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //Ignore the analysis item.
    Q_UNUSED(analysisItem)
    //ID3v1 doesn't contains album art.
    return false;
}

void KNMusicTagId3v1::parseRawData(char *rawTagData,
                                   KNMusicTagId3v1::ID3v1Struct &tagData)
{
    char lastBackupData;
    int lastBackupPosition=-1;
    //ID3v1 is very simple, its data is stored in order:
    //  30 bytes Title + 30 bytes Artist + 30 bytes Album + 4 bytes Year +
    //  30 bytes Comments(+Track) + 1 bytes Genre
    //Just read them by order.
    //Why we have to backup the data.
    //Some times if the title has 30 bytes, the data will be like this:
    //  ~|29 |30 |31 |32 |33 |34....
    //  ~-------------------------...
    //  ~| l | e | A | r | t | i |...
    //  ~-------------------------...
    //We cannot get the right data. So we have to ensure the QString construct
    //function can get the data normally, use a function named backup data to
    //backup the byte in a pool with the position record. At the next time we
    //backup the next bytes, it will automatically put the original data back.
    //Title
    backupByte(rawTagData,
               ArtistStart,
               lastBackupData,
               lastBackupPosition,
               true);
    tagData.tags[0]=standardizeText(m_codec->toUnicode(rawTagData+TitleStart,
                                                       30));
    //Artist
    backupByte(rawTagData,
               AlbumStart,
               lastBackupData,
               lastBackupPosition,
               true);
    tagData.tags[1]=standardizeText(m_codec->toUnicode(rawTagData+ArtistStart,
                                                       30));
    //Album
    backupByte(rawTagData,
               YearStart,
               lastBackupData,
               lastBackupPosition,
               true);
    tagData.tags[2]=standardizeText(m_codec->toUnicode(rawTagData+AlbumStart,
                                                       30));
    //Year
    backupByte(rawTagData,
               CommentStart,
               lastBackupData,
               lastBackupPosition,
               true);
    tagData.tags[3]=standardizeText(m_codec->toUnicode(rawTagData+YearStart,
                                                       4));

    //Comment is a little complex: check the No.125 char first, if it's 0, then
    //the following char is track index.
    backupByte(rawTagData,
               GenrePosition,
               lastBackupData,
               lastBackupPosition,
               true);
    //Track flag.
    if(rawTagData[TrackJudgeByte]==0)
    {
        tagData.track=(quint8)rawTagData[TrackPosition];
    }
    //Automatically detect the length of the comment.
    tagData.tags[4]=
            standardizeText(m_codec->toUnicode(rawTagData+CommentStart));
    //Genre index. Here's a hack. The last char is stored in lastBackupData.
    //Simply copy that value.
    tagData.genreIndex=lastBackupData;
}

void KNMusicTagId3v1::writeToDetailInfo(
        const KNMusicTagId3v1::ID3v1Struct &tagData,
        KNMusicDetailInfo &detailInfo)
{
    //Set texts
    setTextData(detailInfo.textLists[Name], tagData.tags[0]);
    setTextData(detailInfo.textLists[Artist], tagData.tags[1]);
    setTextData(detailInfo.textLists[Album], tagData.tags[2]);
    setTextData(detailInfo.textLists[Year], tagData.tags[3]);
    setTextData(detailInfo.textLists[Comments], tagData.tags[4]);
    setTextData(detailInfo.textLists[Genre],
                knMusicGlobal->indexedGenre(tagData.genreIndex));
    //Set the track index.
    if(tagData.track!=-1)
    {
        setTextData(detailInfo.textLists[TrackNumber],
                    QString::number(tagData.track));
    }
}

inline void KNMusicTagId3v1::setRawTagData(char *rawTagData,
                                           const QVariant &text,
                                           const int &length)
{
    //Encode the text with the codec.
    QByteArray codecData=m_codec->fromUnicode(text.toString());
    //Simply copy the data to the char array.
    memcpy(rawTagData, codecData.data(), length);
}
