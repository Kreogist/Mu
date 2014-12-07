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
#include "knmusictagapev2.h"

#include <QDebug>

KNMusicTagAPEv2::KNMusicTagAPEv2(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Initial key index.
    m_keyIndex["TITLE"]=Name;
    m_keyIndex["ARTIST"]=Artist;
    m_keyIndex["ALBUM"]=Album;
    m_keyIndex["TRACK"]=TrackNumber;
    m_keyIndex["COMPOSER"]=Composer;
    m_keyIndex["COMMENT"]=Comments;
    m_keyIndex["YEAR"]=Year;
    m_keyIndex["ALBUM ARTIST"]=AlbumArtist;
    m_keyIndex["GENRE"]=Genre;
}

bool KNMusicTagAPEv2::praseTag(QFile &musicFile,
                               QDataStream &musicDataStream,
                               KNMusicDetailInfo &detailInfo)
{
    //Check the file size.
    if(musicFile.size()<32)
    {
        return false;
    }
    //There're several positions we have to check:
    // * The beginning of the file.
    // * The end of the file.
    // * If there's ID3v1 tag, check the position before ID3v1.
    APEHeader header;
    char *rawTagData;
    int tagDataStart=-1;
    if(checkHeader(0,                        //Check the beginning of the file.
                   musicDataStream,
                   header))
    {
        tagDataStart=32;
    }
    else if(checkHeader(musicFile.size()-32, //Check the end of the file.
                        musicDataStream,
                        header))
    {
        //Get the header raw data.
        musicDataStream.device()->reset();
        tagDataStart=musicFile.size()-header.size;
    }
    else if(musicFile.size()>=160 &&          //Check the position before ID3v1.
            checkHeader(musicFile.size()-160,
                        musicDataStream,
                        header))
    {
        //Get the header raw data.
        musicDataStream.device()->reset();
        tagDataStart=musicFile.size()-160-header.size;
    }
    //If we didn't find any header.
    if(tagDataStart==-1)
    {
        return false;
    }
    //Parse the raw tag data.
    rawTagData=new char[header.size];
    musicDataStream.skipRawData(tagDataStart);
    musicDataStream.readRawData(rawTagData, header.size);
    QList<APETagItem> tagList;
    parseRawData(rawTagData, header, tagList);
    delete[] rawTagData;
    //Write the tag list to detail info.
    writeTagListToDetailInfo(tagList, detailInfo);
    return true;
}

bool KNMusicTagAPEv2::parseAlbumArt(KNMusicDetailInfo &detailInfo)
{
    //APEv2 cannot contains album art.
    Q_UNUSED(detailInfo)
    return false;
}

bool KNMusicTagAPEv2::checkHeader(const int &position,
                                  QDataStream &musicDataStream,
                                  APEHeader &header)
{
    //Set the datastream to the position.
    musicDataStream.device()->reset();
    musicDataStream.skipRawData(position);
    //Try to get the header.
    char rawHeaderData[32];
    musicDataStream.readRawData(rawHeaderData, 32);
    //Check the header data.
    if(memcmp(m_apePreamble, rawHeaderData, 8)==0)
    {
        //Parse the header data to header struct.
        header.version=(((quint32)rawHeaderData[11]<<24)&0b11111111000000000000000000000000)+
                       (((quint32)rawHeaderData[10]<<16)&0b00000000111111110000000000000000)+
                       (((quint32)rawHeaderData[9] <<8) &0b00000000000000001111111100000000)+
                       ( (quint32)rawHeaderData[8]      &0b00000000000000000000000011111111);
        header.size=(((quint32)rawHeaderData[15]<<24)&0b11111111000000000000000000000000)+
                    (((quint32)rawHeaderData[14]<<16)&0b00000000111111110000000000000000)+
                    (((quint32)rawHeaderData[13]<<8) &0b00000000000000001111111100000000)+
                    ( (quint32)rawHeaderData[12]     &0b00000000000000000000000011111111);
        header.itemCount=(((quint32)rawHeaderData[19]<<24)&0b11111111000000000000000000000000)+
                         (((quint32)rawHeaderData[18]<<16)&0b00000000111111110000000000000000)+
                         (((quint32)rawHeaderData[17]<<8) &0b00000000000000001111111100000000)+
                         ( (quint32)rawHeaderData[16]     &0b00000000000000000000000011111111);
        header.flags=(((quint32)rawHeaderData[23]<<24)&0b11111111000000000000000000000000)+
                     (((quint32)rawHeaderData[22]<<16)&0b00000000111111110000000000000000)+
                     (((quint32)rawHeaderData[21]<<8) &0b00000000000000001111111100000000)+
                     ( (quint32)rawHeaderData[20]     &0b00000000000000000000000011111111);
        return true;
    }
    return false;
}

void KNMusicTagAPEv2::parseRawData(char *rawData,
                                   APEHeader &header,
                                   QList<APETagItem> &tagList)
{
    //The struct of the APEv2 frame is:
    /*
    ----
    Size of the Item Value, Bits 0...7
    Size of the Item Value, Bits 8...15
    Size of the Item Value, Bits 16...23
    Size of the Item Value, Bits 24...31
    ----
    Item Flags, Bits 0...7
    Item Flags, Bits 8...15
    Item Flags, Bits 16...23
    Item Flags, Bits 24...31
    ----
    Item Key: Can contain ASCII characters from 0x20 (Space) up to 0x7E (Tilde)
    ----
    0x00: Item key terminator
    ----
    Item Value: can be binary data or UTF-8 string
    */
    quint32 sizeSurplus=header.size, itemSurplus=header.itemCount;
    char *dataPointer=rawData;
    while(sizeSurplus>0 && itemSurplus>0)
    {
        //Calculate the frame size.
        quint32 currentFrameSize=(((quint32)dataPointer[3]<<24)&0b11111111000000000000000000000000)+
                                 (((quint32)dataPointer[2]<<16)&0b00000000111111110000000000000000)+
                                 (((quint32)dataPointer[1]<<8) &0b00000000000000001111111100000000)+
                                 ( (quint32)dataPointer[0]     &0b00000000000000000000000011111111);
        //Check is frame size available.
        if(currentFrameSize>sizeSurplus)
        {
            break;
        }
        //Generate item according to the frame size.
        APETagItem currentItem;
        currentItem.flag=(((quint32)dataPointer[7]<<24)&0b11111111000000000000000000000000)+
                         (((quint32)dataPointer[6]<<16)&0b00000000111111110000000000000000)+
                         (((quint32)dataPointer[5]<<8) &0b00000000000000001111111100000000)+
                         ( (quint32)dataPointer[4]     &0b00000000000000000000000011111111);
        //Here is a magic:
        //QString can automatically terminate at the 0x00, so we don't need to
        //find 0x00 byte manually. Just use the key length to find the 0x00.
        currentItem.key=QString(dataPointer+8).toUpper();
        currentItem.value=QByteArray(dataPointer+9+currentItem.key.size(),
                                     currentFrameSize);
        //Add the frame to list.
        tagList.append(currentItem);
        //Add the key size, flag size and size number size to the frame size.
        currentFrameSize+=(currentItem.key.size()+9);
        //Move the data pointer, reduce item and size surplus.
        dataPointer+=currentFrameSize;
        sizeSurplus-=currentFrameSize;
        itemSurplus--;
    }
}

void KNMusicTagAPEv2::writeTagListToDetailInfo(const QList<APETagItem> &tagList,
                                               KNMusicDetailInfo &detailInfo)
{
    QString trackText;
    int splitterIndex;
    //Parse each tag list.
    for(QList<APETagItem>::const_iterator i=tagList.begin();
        i!=tagList.end();
        ++i)
    {
        if(m_keyIndex.contains((*i).key))
        {
            int frameIndex=m_keyIndex[(*i).key];
            switch(frameIndex)
            {
            case Name:
                //Overwrite the text.
                detailInfo.textLists[Name]=QString((*i).value);
                break;
            case TrackNumber:
                //Find the '/' char.
                trackText=QString((*i).value);
                splitterIndex=trackText.indexOf('/');
                if(splitterIndex==-1)
                {
                    //It only contains track number.
                    detailInfo.textLists[TrackNumber]=trackText;
                }
                else
                {
                    //It contains track number and track count.
                    detailInfo.textLists[TrackNumber]=trackText.left(splitterIndex);
                    detailInfo.textLists[TrackCount]=trackText.mid(splitterIndex+1);
                }
                break;
            default:
                detailInfo.textLists[frameIndex]=QString((*i).value);
            }
        }
    }
}
