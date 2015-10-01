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
#include <QTextCodec>

#include "knlocalemanager.h"

#include "knmusictagwma.h"

QString KNMusicTagWma::m_standardFrameID[StandardFrameItemsCount]=
{
    QString("WMA_FRAMEID_TITLE"),
    QString("WMA_FRAMEID_AUTHOR"),
    QString("WMA_FRAMEID_COPYRIGHT"),
    QString("WMA_FRAMEID_DESCRIPTION"),
    QString("WMA_FRAMEID_RATING")
};

QHash<QString, int> KNMusicTagWma::m_attributesIndex=QHash<QString, int>();

unsigned char KNMusicTagWma::m_headerMark[17]=
{
    0x30, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11,
    0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C
};

unsigned char KNMusicTagWma::m_standardFrame[17]=
{
    0x33, 0x26, 0xB2, 0x75, 0x8E, 0x66, 0xCF, 0x11,
    0xA6, 0xD9, 0x00, 0xAA, 0x00, 0x62, 0xCE, 0x6C
};

unsigned char KNMusicTagWma::m_extendedFrame[17]=
{
    0x40, 0xA4, 0xD0, 0xD2, 0x07, 0xE3, 0xD2, 0x11,
    0x97, 0xF0, 0x00, 0xA0, 0xC9, 0x5E, 0xA8, 0x50
};

//Initail codeces as null.
QTextCodec *KNMusicTagWma::m_localeCodec=nullptr;
QTextCodec *KNMusicTagWma::m_isoCodec=nullptr;
QTextCodec *KNMusicTagWma::m_utf16BECodec=nullptr;
QTextCodec *KNMusicTagWma::m_utf16LECodec=nullptr;
QTextCodec *KNMusicTagWma::m_utf16Codec=nullptr;
QTextCodec *KNMusicTagWma::m_utf8Codec=nullptr;

KNMusicTagWma::KNMusicTagWma(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Check out the attributes index size.
    if(m_attributesIndex.isEmpty())
    {
        //Set the attributes index.
        m_attributesIndex.insert("WMA_FRAMEID_TITLE", Name);
        m_attributesIndex.insert("WMA_FRAMEID_AUTHOR", Artist);
        m_attributesIndex.insert("WMA_FRAMEID_DESCRIPTION", Description);
        m_attributesIndex.insert("WMA_FRAMEID_RATING", Rating);
        m_attributesIndex.insert("WM/AlbumTitle", Rating);
        m_attributesIndex.insert("WM/ParentalRating", AlbumRating);
        m_attributesIndex.insert("WM/BeatsPerMinute", BeatsPerMinuate);
        m_attributesIndex.insert("WM/ContentGroupDescription", Category);
        m_attributesIndex.insert("WM/Text", Comments);
        m_attributesIndex.insert("WM/Composer", Composer);
        m_attributesIndex.insert("WM/Genre", Genre);
        m_attributesIndex.insert("WM/TrackNumber", TrackNumber);
        m_attributesIndex.insert("WM/Year", Year);
        //Initial the codec.
        m_localeCodec=knI18n->localeCodec();
        m_isoCodec=QTextCodec::codecForName("ISO 8859-1");
        m_utf16Codec=QTextCodec::codecForName("UTF-16");
        m_utf16BECodec=QTextCodec::codecForName("UTF-16BE");
        m_utf16LECodec=QTextCodec::codecForName("UTF-16LE");
        m_utf8Codec=QTextCodec::codecForName("UTF-8");
    }
}

bool KNMusicTagWma::parseTag(QFile &musicFile,
                             QDataStream &musicDataStream,
                             KNMusicAnalysisItem &analysisItem)
{
    //If file is less than WMA header, it can't contains tag.
    if(musicFile.size()<16)
    {
        return false;
    }
    //Detect WMA header.
    char header[16];
    //Read header data from the music file.
    musicDataStream.readRawData(header, 16);
    //Compare the header data with the mark data.
    if(memcmp(header, m_headerMark, 16)!=0)
    {
        //The header is not right.
        return false;
    }
    //Get the tag size.
    char rawTagSize[14];
    //Read the raw tag size.
    musicDataStream.readRawData(rawTagSize, 14);
    //Calculate the tag size.
    quint64 tagSize=dataToInt64(rawTagSize)-30, tagDataCount=tagSize;
    //Generate tag size data field.
    char *rawTagData=new char[tagSize], *framePointer=rawTagData;
    musicDataStream.readRawData(rawTagData, tagSize);
    //Initial the map, parsed flag.
    QList<KNMusicWMAFrame> tagMap;
    bool standardParsed=false, extendParsed=false;
    //Parse all tag data, until we find parsed standard and extend data.
    while(tagDataCount!=0 || !standardParsed || !extendParsed)
    {
        //Calculate the frame size first.
        quint64 frameSize=dataToInt64(framePointer+16);
        //Ensure the frame size is not larger than tag data.
        if(frameSize>tagDataCount)
        {
            break;
        }
        //If detect standard frame, parse it.
        if(!memcmp(framePointer, m_standardFrame, 16))
        {
            //Standard parsed flag will be true.
            standardParsed=true;
            //Parsed the data.
            parseStandardFrame(framePointer+24,
                               frameSize-24,
                               tagMap);
        }
        //If detect extend frame, parse it as well.
        else if(!memcmp(framePointer, m_extendedFrame, 16))
        {
            //Extend parsed flag will be true.
            extendParsed=true;
            //Parsed the data.
            parseExtendFrame(framePointer+24,
                             frameSize-24,
                             tagMap);
        }
        //Move the pointer and reduce the data count.
        framePointer+=frameSize;
        tagDataCount-=frameSize;
    }
    //Write the map to detail info.
    //Get the detail info.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    for(auto i=tagMap.begin(); i!=tagMap.end(); ++i)
    {
        //If it's album art frame, save the image data.
        if((*i).name=="WM/Picture")
        {
            analysisItem.imageData["WMA"].append((*i).data);
            continue;
        }
        //Get the frame index.
        int frameIndex=m_attributesIndex.value((*i).name, -1);
        //Check the frame index.
        if(frameIndex!=-1)
        {
            //Set the data to detail info, use UTF-16LE to decode the data.
            setTextData(detailInfo.textLists[frameIndex],
                        m_utf16LECodec->toUnicode((*i).data));
        }
    }
    //Recover memory.
    delete[] rawTagData;
    //Mission complete.
    return true;
}

bool KNMusicTagWma::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(analysisItem)
    //Set wma as a read-only tag parser.
    return false;
}

bool KNMusicTagWma::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //Check is there contains wma data.
    if(!analysisItem.imageData.contains("WMA") ||
            analysisItem.imageData.value("WMA").isEmpty())
    {
        return false;
    }
    //Get the image data.
    QList<QByteArray> imageRawDatas=analysisItem.imageData.value("WMA");
    //These image data is just the same as ID3v2, so many of these codes are
    //copied from ID3v2 module.
    //But one WMA can only contains one image, send this image to parse.
    QImage albumArt;
    if(parseImageData(imageRawDatas.first(), albumArt))
    {
        //Save the image to the item.
        analysisItem.coverImage=albumArt;
    }
    //Parse success, finished.
    return true;
}

bool KNMusicTagWma::writable() const
{
    return false;
}

bool KNMusicTagWma::writeCoverImage() const
{
    return false;
}

inline quint64 KNMusicTagWma::dataToInt64(const char *dataArray)
{
    return (((quint64)dataArray[7]<<56) & 0xFF00000000000000)+
           (((quint64)dataArray[6]<<48) & 0x00FF000000000000)+
           (((quint64)dataArray[5]<<40) & 0x0000FF0000000000)+
           (((quint64)dataArray[4]<<32) & 0x000000FF00000000)+
           (((quint64)dataArray[3]<<24) & 0x00000000FF000000)+
           (((quint64)dataArray[2]<<16) & 0x0000000000FF0000)+
           (((quint64)dataArray[1]<<8)  & 0x000000000000FF00)+
           ( (quint64)dataArray[0]      & 0x00000000000000FF);
}

inline void KNMusicTagWma::parseStandardFrame(char *frameStart,
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
        itemSizes[i]=(((quint16)dataPointer[1]<<8) & 0xFF00)+
                     (((quint16)dataPointer[0])    & 0x00FF);
        //Calculate sum.
        sizeSum+=itemSizes[i];
        //Move the pointer.
        dataPointer+=2;
    }
    //Check the sum is the same as the record..
    if(sizeSum!=frameSize)
    {
        //When the record is different, we will stop to load it.
        return;
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
}

inline void KNMusicTagWma::parseExtendFrame(char *frameStart,
                                            quint64 frameSize,
                                            QList<KNMusicWMAFrame> &frameList)
{
    //Backup the start pointer.
    char *dataPointer=frameStart;
    //In extend frame, it starts with the number of items it contains.
    quint16 itemCounts=(((quint16)dataPointer[1]<<8) & 0xFF00)+
                       (((quint16)dataPointer[0])    & 0x00FF);
    //Remove item count bytes, move pointer.
    frameSize-=2;dataPointer+=2;
    //Read all these items to tag map.
    while(itemCounts>0 && frameSize>0)
    {
        //Generate the raw frame.
        KNMusicWMAFrame currentFrame;
        //And the first two bytes are name length, get the name.
        quint16 nameLength=(((quint16)dataPointer[1]<<8) & 0xFF00)+
                           (((quint16)dataPointer[0])    & 0x00FF);
        //Get the name from the raw data.
        currentFrame.name=m_utf16LECodec->toUnicode(QByteArray(dataPointer+2,
                                                               nameLength-2));
        //Move the pointer, skip the name length, name and 2 unkown bytes.
        dataPointer+=(nameLength+4);
        //Get the value.
        quint16 valueLength=(((quint16)dataPointer[1]<<8) & 0xFF00)+
                            (((quint16)dataPointer[0])    & 0x00FF);
        currentFrame.data=QByteArray(dataPointer+2, valueLength);
        //Add the frame to list.
        frameList.append(currentFrame);
        //Move the pointer to next position.
        dataPointer+=(valueLength+2);
        //Reduce frame size counter.
        frameSize-=(nameLength+valueLength+6);
        //Reduce count.
        itemCounts--;
    }
}

bool KNMusicTagWma::parseImageData(QByteArray imageData,
                                   QImage &albumArt)
{
    //!FIXME: Check this module once more.
    //Get mime end and description end.
    int mimeTypeEnd=imageData.indexOf('\0', 1),
        descriptionEnd=imageData.indexOf('\0', mimeTypeEnd+2);
    //We will simply ignore all the text data.
    //Get the image.
    imageData.remove(0, descriptionEnd+1);
    albumArt.loadFromData(imageData);
    //If parse the image success, add it to map.
    return !albumArt.isNull();
}
