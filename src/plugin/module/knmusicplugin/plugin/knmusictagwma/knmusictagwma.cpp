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

#include "knglobal.h"
#include "knmusictagwma.h"

#include <QDebug>

KNMusicTagWMA::KNMusicTagWMA(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Initial the standard items attributes.
    m_standardFrameID[ItemTitle]="WMA_FRAMEID_TITLE";
    m_standardFrameID[ItemAuthor]="WMA_FRAMEID_AUTHOR";
    m_standardFrameID[ItemCopyright]="WMA_FRAMEID_COPYRIGHT";
    m_standardFrameID[ItemDescription]="WMA_FRAMEID_DESCRIPTION";
    m_standardFrameID[ItemRating]="WMA_FRAMEID_RATING";

    //Initial attributes index.
    m_attributesIndex["WMA_FRAMEID_TITLE"]=Name;
    m_attributesIndex["WMA_FRAMEID_AUTHOR"]=Artist;
    m_attributesIndex["WMA_FRAMEID_DESCRIPTION"]=Description;
    m_attributesIndex["WMA_FRAMEID_RATING"]=Rating;
    m_attributesIndex["WM/AlbumTitle"]=Rating;
    m_attributesIndex["WM/ParentalRating"]=AlbumRating;
    m_attributesIndex["WM/BeatsPerMinute"]=BeatsPerMinuate;
    m_attributesIndex["WM/ContentGroupDescription"]=Category;
    m_attributesIndex["WM/Text"]=Comments;
    m_attributesIndex["WM/Composer"]=Composer;
    m_attributesIndex["WM/Genre"]=Genre;
    m_attributesIndex["WM/TrackNumber"]=TrackNumber;
    m_attributesIndex["WM/Year"]=Year;

    //Initial the codec.
    m_isoCodec=QTextCodec::codecForName("ISO 8859-1");
    m_utf16Codec=QTextCodec::codecForName("UTF-16");
    m_utf16BECodec=QTextCodec::codecForName("UTF-16BE");
    m_utf16LECodec=QTextCodec::codecForName("UTF-16LE");
    m_utf8Codec=QTextCodec::codecForName("UTF-8");
    m_localeCodec=KNGlobal::localeDefaultCodec();
}

bool KNMusicTagWMA::praseTag(QFile &musicFile,
                             QDataStream &musicDataStream,
                             KNMusicDetailInfo &detailInfo)
{
    //If file is less than WMA header, it can't contains tag.
    if(musicFile.size()<16)
    {
        return false;
    }
    //Detect WMA header.
    char header[16], rawTagSize[14];
    musicDataStream.readRawData(header, 16);
    if(memcmp(header, m_headerMark, 16)!=0)
    {
        return false;
    }
    //Get the tag size.
    musicDataStream.readRawData(rawTagSize, 14);
    quint64 tagSize=(((quint64)rawTagSize[7]<<56)&0b1111111100000000000000000000000000000000000000000000000000000000)+
                    (((quint64)rawTagSize[6]<<48)&0b0000000011111111000000000000000000000000000000000000000000000000)+
                    (((quint64)rawTagSize[5]<<40)&0b0000000000000000111111110000000000000000000000000000000000000000)+
                    (((quint64)rawTagSize[4]<<32)&0b0000000000000000000000001111111100000000000000000000000000000000)+
                    (((quint64)rawTagSize[3]<<24)&0b0000000000000000000000000000000011111111000000000000000000000000)+
                    (((quint64)rawTagSize[2]<<16)&0b0000000000000000000000000000000000000000111111110000000000000000)+
                    (((quint64)rawTagSize[1]<<8) &0b0000000000000000000000000000000000000000000000001111111100000000)+
                    ( (quint64)rawTagSize[0]     &0b0000000000000000000000000000000000000000000000000000000011111111)-30,
            tagDataCount=tagSize;
    //Get all the tag data.
    char *rawTagData=new char[tagSize],
         *framePointer=rawTagData;
    bool standardParsed=false, extendParsed=false;
    musicDataStream.readRawData(rawTagData, tagSize);
    //Initial the map.
    QList<KNMusicWMAFrame> tagMap;
    //Parse tag data.
    while(tagDataCount>0 || !standardParsed || !extendParsed)
    {
        //Calculate the frame size first.
        quint64 frameSize=(((quint64)framePointer[23]<<56)&0b1111111100000000000000000000000000000000000000000000000000000000)+
                          (((quint64)framePointer[22]<<48)&0b0000000011111111000000000000000000000000000000000000000000000000)+
                          (((quint64)framePointer[21]<<40)&0b0000000000000000111111110000000000000000000000000000000000000000)+
                          (((quint64)framePointer[20]<<32)&0b0000000000000000000000001111111100000000000000000000000000000000)+
                          (((quint64)framePointer[19]<<24)&0b0000000000000000000000000000000011111111000000000000000000000000)+
                          (((quint64)framePointer[18]<<16)&0b0000000000000000000000000000000000000000111111110000000000000000)+
                          (((quint64)framePointer[17]<<8) &0b0000000000000000000000000000000000000000000000001111111100000000)+
                          ( (quint64)framePointer[16]     &0b0000000000000000000000000000000000000000000000000000000011111111);
        //If detect standard frame, parse it.
        if(isStandardFrame(framePointer))
        {
            standardParsed=parseStandardFrame(framePointer+24,
                                              frameSize-24,
                                              tagMap);
        }
        else if(isExtendFrame(framePointer)) //Check if it's extend frame.
        {
            extendParsed=parseExtendFrame(framePointer+24,
                                          frameSize-24,
                                          tagMap);
        }
        //Move the pointer and reduce the data count.
        framePointer+=frameSize;
        tagDataCount-=frameSize;
    }
    //Write the map to detail info.
    writeTagMapToDetailInfo(tagMap, detailInfo);
    delete[] rawTagData;
    return true;
}

bool KNMusicTagWMA::parseAlbumArt(KNMusicDetailInfo &detailInfo)
{
    //Check is there contains wma data.
    if(!detailInfo.imageData.contains("WMA"))
    {
        return false;
    }
    //Get the image data.
    QList<QByteArray> imageRawDatas=detailInfo.imageData["WMA"];
    //These image data is just the same as ID3v2, so many of these codes are
    //copied from ID3v2 module.
    //But one WMA can only contains one image, send this image to parse.
    WMAPicture albumArt;
    if(parseImageData(imageRawDatas.first(), albumArt))
    {
        detailInfo.coverImage=albumArt.image;
        return true;
    }
    return false;
}

QString KNMusicTagWMA::frameToText(QByteArray content)
{
    //Check is content empty.
    if(content.isEmpty())
    {
        return QString();
    }
    //Get the codec.
    //The first char of the ID3v2 text is the encoding of the current text.
    quint8 encoding=(quint8)(content.at(0));
    content.remove(0, 1);
    switch(encoding)
    {
    case 0: //0 = ISO-8859-1
        return m_isoCodec->toUnicode(content).simplified().remove(QChar('\0'));
    case 1: //1 = UTF-16 LE/BE (Treat other as no BOM UTF-16)
        if((quint8)content.at(0)==0xFE && (quint8)content.at(1)==0xFF)
        {
            return m_utf16BECodec->toUnicode(content).simplified().remove(QChar('\0'));
        }
        if((quint8)content.at(0)==0xFF && (quint8)content.at(1)==0xFE)
        {
            return m_utf16LECodec->toUnicode(content).simplified().remove(QChar('\0'));
        }
        return m_utf16Codec->toUnicode(content).simplified().remove(QChar('\0'));
    case 2: //2 = UTF-16 BE without BOM
        return m_utf16Codec->toUnicode(content).simplified().remove(QChar('\0'));
    case 3: //3 = UTF-8
        return m_utf8Codec->toUnicode(content).simplified().remove(QChar('\0'));
    default://Use locale codec.
        return m_localeCodec->toUnicode(content).simplified().remove(QChar('\0'));
    }
}

bool KNMusicTagWMA::parseStandardFrame(char *frameStart,
                                       quint64 frameSize,
                                       QList<KNMusicWMAFrame> &frameList)
{
    //In the standard frame, it starts with five 2-byte values of item size:
    //Title Length, Author Length, Copyright Length, Description Length and
    //Rating Length. Check the sum of these sizes is frameSize or not.
    char *dataPointer=frameStart;
    //Size sum starts at 10, include the these sizes bytes.
    quint16 itemSizes[StandardFrameItemsCount],
            sizeSum=10;
    for(int i=0; i<StandardFrameItemsCount; i++)
    {
        //Get the size.
        itemSizes[i]=(((quint16)dataPointer[1]<<8)&0b1111111100000000)+
                     (((quint16)dataPointer[0])   &0b0000000011111111);
        //Calculate sum.
        sizeSum+=itemSizes[i];
        //Move the pointer.
        dataPointer+=2;
    }
    //Check the sum.
    if(sizeSum!=frameSize)
    {
        return false;
    }
    //Get the item data and set it to map.
    for(int i=0; i<StandardFrameItemsCount; i++)
    {
        //Get the frame.
        KNMusicWMAFrame currentFrame;
        currentFrame.name=m_standardFrameID[i];
        currentFrame.data=QByteArray(dataPointer, itemSizes[i]);
        //Add to list.
        frameList.append(currentFrame);
        //Move pointer.
        dataPointer+=itemSizes[i];
    }
    return true;
}

bool KNMusicTagWMA::parseExtendFrame(char *frameStart,
                                     quint64 frameSize,
                                     QList<KNMusicWMAFrame> &frameList)
{
    char *dataPointer=frameStart;
    //In extend frame, it starts with the number of items it contains.
    quint16 itemCounts=(((quint16)dataPointer[1]<<8)&0b1111111100000000)+
                       (((quint16)dataPointer[0])   &0b0000000011111111);
    //Remove item count bytes, move pointer.
    frameSize-=2;dataPointer+=2;
    //Read all these items to tag map.
    while(itemCounts>0 && frameSize>0)
    {
        KNMusicWMAFrame currentFrame;
        //And the first two bytes are name length, get the name.
        quint16 nameLength=(((quint16)dataPointer[1]<<8)&0b1111111100000000)+
                           (((quint16)dataPointer[0])   &0b0000000011111111);
        currentFrame.name=m_utf16LECodec->toUnicode(QByteArray(dataPointer+2, nameLength-2));
        //Move the pointer, skip the name length, name and 2 unkown bytes.
        dataPointer+=(nameLength+4);
        //Get the value.
        quint16 valueLength=(((quint16)dataPointer[1]<<8)&0b1111111100000000)+
                            (((quint16)dataPointer[0])   &0b0000000011111111);
        currentFrame.data=QByteArray(dataPointer+2, valueLength);
        //Add the frame to list.
        frameList.append(currentFrame);
        //Reduce count and frame size counter, move pointer.
        dataPointer+=(valueLength+2);
        frameSize-=(nameLength+valueLength+6);
        itemCounts--;
    }
    return true;
}

bool KNMusicTagWMA::parseImageData(QByteArray imageData,
                                   WMAPicture &albumArt)
{
    //!FIXME: Check this module once more.
    //Get mime end and description end.
    int mimeTypeEnd=imageData.indexOf('\0', 1),
        descriptionEnd=imageData.indexOf('\0', mimeTypeEnd+2);
    //Backup the text encording and get the picture type.
    quint8 textEncoding=imageData.at(0),
           pictureType=imageData.at(mimeTypeEnd+1);
    //Get the mime type text.
    albumArt.mimeType=frameToText(imageData.left(mimeTypeEnd));
    //Get the description text.
    QByteArray descriptionText=imageData.mid(mimeTypeEnd+2,
                                             descriptionEnd-(mimeTypeEnd+2));
    descriptionText.insert(0, textEncoding);
    albumArt.description=frameToText(descriptionText);
    //Get the image.
    imageData.remove(0, descriptionEnd+1);
    albumArt.image.loadFromData(imageData);
    //If parse the image success, add it to map.
    return !albumArt.image.isNull();
}

void KNMusicTagWMA::writeTagMapToDetailInfo(const QList<KNMusicWMAFrame> &frameList,
                                            KNMusicDetailInfo &detailInfo)
{
    for(QList<KNMusicWMAFrame>::const_iterator i=frameList.begin();
        i!=frameList.end();
        ++i)
    {
        //If it's album art frame, save the image data.
        if((*i).name=="WM/Picture")
        {
            detailInfo.imageData["WMA"].append((*i).data);
            continue;
        }
        //Check the index first.
        if(m_attributesIndex.contains((*i).name))
        {
            int frameIndex=m_attributesIndex[(*i).name];
            switch(frameIndex)
            {
            default:
                setTextData(detailInfo.textLists[frameIndex],
                            m_utf16LECodec->toUnicode((*i).data));
                break;
            }
        }
    }
}
