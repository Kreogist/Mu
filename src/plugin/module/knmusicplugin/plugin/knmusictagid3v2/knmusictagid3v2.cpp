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

#include "knmusictagid3v2.h"

#include <QDebug>

KNMusicTagID3v2::KNMusicTagID3v2(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Initial the codec.
    m_isoCodec=QTextCodec::codecForName("ISO 8859-16");
    m_utf16Codec=QTextCodec::codecForName("UTF-16");
    m_utf16BECodec=QTextCodec::codecForName("UTF-16BE");
    m_utf16LECodec=QTextCodec::codecForName("UTF-16LE");
    m_utf8Codec=QTextCodec::codecForName("UTF-8");
    m_localeCodec=KNGlobal::localeDefaultCodec();

    //Initial music global.
    m_musicGlobal=KNMusicGlobal::instance();

    //Initial frame ID Index.
    m_frameIDIndex["TIT2"]=Name;
    m_frameIDIndex["TPE1"]=Artist;
    m_frameIDIndex["TALB"]=Album;
    m_frameIDIndex["TPE2"]=AlbumArtist;
    m_frameIDIndex["TBPM"]=BeatsPerMinuate;
    m_frameIDIndex["TIT1"]=Category;
    m_frameIDIndex["COMM"]=Comments;
    m_frameIDIndex["TCOM"]=Composer;
    m_frameIDIndex["TIT3"]=Description;
    m_frameIDIndex["TCON"]=Genre;
    m_frameIDIndex["TRCK"]=TrackNumber;
    m_frameIDIndex["TPOS"]=DiscNumber;
    m_frameIDIndex["POPM"]=Rating;
    m_frameIDIndex["TYER"]=Year;

    m_frameIDIndex["TT2"]=Name;
    m_frameIDIndex["TP1"]=Artist;
    m_frameIDIndex["TAL"]=Album;
    m_frameIDIndex["TP2"]=AlbumArtist;
    m_frameIDIndex["TBP"]=BeatsPerMinuate;
    m_frameIDIndex["TT1"]=Category;
    m_frameIDIndex["COM"]=Comments;
    m_frameIDIndex["TCM"]=Composer;
    m_frameIDIndex["TT3"]=Description;
    m_frameIDIndex["TCO"]=Genre;
    m_frameIDIndex["TRK"]=TrackNumber;
    m_frameIDIndex["TPA"]=DiscNumber;
    m_frameIDIndex["POP"]=Rating;
    m_frameIDIndex["TYE"]=Year;
}

bool KNMusicTagID3v2::praseTag(QFile &musicFile,
                               QDataStream &musicDataStream,
                               KNMusicDetailInfo &detailInfo)
{
    //Initial datas.
    char rawHeader[10];
    ID3v2Header header;
    //If file is less than ID3v2 header, it can't contains ID3v2 tag.
    if(musicFile.size()<10)
    {
        return false;
    }
    //Detect ID3v2 header.
    musicDataStream.readRawData(rawHeader, 10);
    if(!parseHeader(rawHeader, header))
    {
        return false;
    }
    //Check is file's size smaller than tag size.
    if(musicFile.size()<(header.size+10))
    {
        //File is smaller than the tag says, failed to get.
        return false;
    }
    //Read the raw tag data.
    char *rawTagData=new char[header.size];
    musicDataStream.readRawData(rawTagData, header.size);
    //Parse these raw data.
    QLinkedList<ID3v2Frame> frames;
    ID3v2MinorProperty property;
    generateProperty(header.minor, property);
    parseRawData(rawTagData, header, property, frames);
    //Write the tag to details.
    if(!frames.isEmpty())
    {
        writeFramesToDetails(frames, property, detailInfo);
    }
    //Recover the memory.
    delete[] rawTagData;
    return true;
}

bool KNMusicTagID3v2::parseAlbumArt(KNMusicDetailInfo &detailInfo)
{
    ;
}

quint32 KNMusicTagID3v2::minor2Size(char *rawTagData)
{
    return (((quint32)rawTagData[0]<<16)&0b00000000111111110000000000000000)+
           (((quint32)rawTagData[1]<<8) &0b00000000000000001111111100000000)+
           ( (quint32)rawTagData[2]     &0b00000000000000000000000011111111);
}

quint32 KNMusicTagID3v2::minor3Size(char *rawTagData)
{
    return (((quint32)rawTagData[0]<<24)&0b11111111000000000000000000000000)+
           (((quint32)rawTagData[1]<<16)&0b00000000111111110000000000000000)+
           (((quint32)rawTagData[2]<<8 )&0b00000000000000001111111100000000)+
           ( (quint32)rawTagData[3]     &0b00000000000000000000000011111111);
}

quint32 KNMusicTagID3v2::minor4Size(char *rawTagData)
{
    return (((quint32)rawTagData[0]<<21)&0b00001111111000000000000000000000)+
           (((quint32)rawTagData[1]<<14)&0b00000000000111111100000000000000)+
           (((quint32)rawTagData[2]<<7 )&0b00000000000000000011111110000000)+
           ( (quint32)rawTagData[3]     &0b00000000000000000000000001111111);
}

void KNMusicTagID3v2::saveFlag(char *rawTagData, ID3v2Frame &frameData)
{
    frameData.flags[0]=rawTagData[8];
    frameData.flags[1]=rawTagData[9];
}

QString KNMusicTagID3v2::frameToText(QByteArray content)
{
    //Remove all the '\0' byte at the end of the array.
    int realContentSize=content.size();
    while(content.at(realContentSize-1)==0)
    {
        realContentSize--;
    }
    content.resize(realContentSize);
    //Get the codec.
    //The first char of the ID3v2 text is the encoding of the current text.
    quint8 encoding=(quint8)(content.at(0));
    content.remove(0, 1);
    switch(encoding)
    {
    case 0: //0 = ISO-8859-1
        return m_isoCodec->toUnicode(content).simplified();
    case 1: //1 = UTF-16 LE/BE (Treat other as no BOM UTF-16)
        if((quint8)content.at(0)==0xFE && (quint8)content.at(1)==0xFF)
        {
            return m_utf16BECodec->toUnicode(content).simplified();
        }
        if((quint8)content.at(0)==0xFF && (quint8)content.at(1)==0xFE)
        {
            return m_utf16LECodec->toUnicode(content).simplified();
        }
        return m_utf16Codec->toUnicode(content).simplified();
    case 2: //2 = UTF-16 BE without BOM
        return m_utf16Codec->toUnicode(content).simplified();
    case 3: //3 = UTF-8
        return m_utf8Codec->toUnicode(content).simplified();
    default://Use locale codec.
        return m_localeCodec->toUnicode(content).simplified();
    }
}

bool KNMusicTagID3v2::parseHeader(char *rawHeader,
                                  ID3v2Header &header)
{
    //Check 'ID3' from the very beginning.
    if(rawHeader[0]!='I' || rawHeader[1]!='D' || rawHeader[2]!='3')
    {
        return false;
    }
    //Get the version of the tag.
    header.major=(quint8)rawHeader[3];
    header.minor=(quint8)rawHeader[3];
    //Get the flag.
    header.flag=(quint8)rawHeader[5];
    //Calculate tag size.
    header.size=(((quint32)rawHeader[6]<<21)&0b00001111111000000000000000000000)+
                (((quint32)rawHeader[7]<<14)&0b00000000000111111100000000000000)+
                (((quint32)rawHeader[8]<<7) &0b00000000000000000011111110000000)+
                ( (quint32)rawHeader[9]     &0b00000000000000000000000001111111);
    return true;
}

bool KNMusicTagID3v2::parseRawData(char *rawTagData,
                                   const ID3v2Header &header,
                                   const ID3v2MinorProperty &property,
                                   QLinkedList<ID3v2Frame> &frameList)
{
    char *rawPosition=rawTagData;
    quint32 rawTagDataSurplus=header.size;
    while(rawTagDataSurplus>0)
    {
        //If no tags, means behind of these datas are all '\0'.
        if(rawPosition[0]==0)
        {
            break;
        }
        //Calculate the size first.
        quint32 frameSize=((*(property.toSize))(rawPosition+property.frameIDSize));
        //Check the frame size.
        if(frameSize<=0 || frameSize>rawTagDataSurplus)
        {
            break;
        }
        ID3v2Frame currentFrame;
        //Set the frame ID.
        strncpy(currentFrame.frameID, rawPosition, property.frameIDSize);
        //Set the start position and size.
        currentFrame.start=rawPosition+property.frameHeaderSize;
        currentFrame.size=frameSize;
        //Process the flag.
        if(property.saveFlag!=nullptr)
        {
            (*(property.saveFlag))(rawPosition, currentFrame);
        }
        //Append the frame to the list.
        frameList.append(currentFrame);
        //Reduce the surplus, jump to the next position.
        int frameContentSize=property.frameHeaderSize+currentFrame.size;
        rawTagDataSurplus-=frameContentSize;
        rawPosition=rawPosition+frameContentSize;
    }
    return true;
}

void KNMusicTagID3v2::generateProperty(const quint8 &minor,
                                       ID3v2MinorProperty &property)
{
    //Because the ID3v2 has so many version, we have to use different calculate
    //function to process these frames.
    switch(minor)
    {
    case 0:
    case 1:
    case 2:
        property.frameIDSize=3;
        property.frameHeaderSize=6;
        property.toSize=KNMusicTagID3v2::minor2Size;
        property.saveFlag=nullptr;
        break;
    case 3:
        property.frameIDSize=4;
        property.frameHeaderSize=10;
        property.toSize=KNMusicTagID3v2::minor3Size;
        property.saveFlag=KNMusicTagID3v2::saveFlag;
        break;
    case 4:
        property.frameIDSize=4;
        property.frameHeaderSize=10;
        property.toSize=KNMusicTagID3v2::minor4Size;
        property.saveFlag=KNMusicTagID3v2::saveFlag;
        break;
    default:
        break;
    }
}

void KNMusicTagID3v2::writeFramesToDetails(const QLinkedList<ID3v2Frame> &frames,
                                           const ID3v2MinorProperty &property,
                                           KNMusicDetailInfo &detailInfo)
{
    int imageCounter=0;
    for(QLinkedList<ID3v2Frame>::const_iterator i=frames.begin();
        i!=frames.end();
        ++i)
    {
        //Process the data according to the flag before we use it.
        QByteArray frameData;
        //Check if it contains a data length indicator.
        if((*i).flags[1] & FrameDataLengthIndicator)
        {
            frameData=QByteArray((*i).start+4,
                                 ((*(property.toSize))((*i).start)));
        }
        else
        {
            frameData=QByteArray((*i).start, (*i).size);
        }
        //Get the frame Index.
        QString frameID=QString((*i).frameID).toUpper();
        if(frameID=="APIC" || frameID=="PIC")
        {
            QString imageID="ID3v2"+QString::number(imageCounter++);
            detailInfo.containsImage=true;
            detailInfo.imageData[imageID].append(frameData);
            continue;
        }
        if(!m_frameIDIndex.contains((*i).frameID))
        {
            continue;
        }
        int frameIndex=m_frameIDIndex[(*i).frameID],
            diagonalPosition, rightBracketsPosition;
        QString frameText;
        switch(frameIndex)
        {
        case DiscNumber:
            //Get the text of the frame.
            frameText=frameToText(frameData);
            //Search the character '/', check whether the format is like 1/9
            diagonalPosition=frameText.indexOf('/');
            //If there's no diagonal, means it only contains disc number.
            if(diagonalPosition==-1)
            {
                setTextData(detailInfo.textLists[DiscNumber], frameText);
            }
            else
            {
                setTextData(detailInfo.textLists[DiscNumber],
                            frameText.left(diagonalPosition));
                setTextData(detailInfo.textLists[DiscCount],
                            frameText.mid(diagonalPosition+1));
            }
            break;
        case TrackNumber:
            //Get the text of the frame.
            frameText=frameToText(frameData);
            //Search the character '/', check whether the format is like 1/9
            diagonalPosition=frameText.indexOf('/');
            //If there's no diagonal, means it only contains track number.
            if(diagonalPosition==-1)
            {
                setTextData(detailInfo.textLists[TrackNumber], frameText);
            }
            else
            {
                setTextData(detailInfo.textLists[TrackNumber], frameText.left(diagonalPosition));
                setTextData(detailInfo.textLists[TrackCount], frameText.mid(diagonalPosition+1));
            }
            break;
        case Genre:
            //Get the text of the frame.
            frameText=frameToText(frameData);
            //Check is the frame text is format like: (9) or (9) XXX
            if(frameText[0]=='(')
            {
                //Check is there any ')'.
                rightBracketsPosition=frameText.indexOf(')');
                if(rightBracketsPosition==-1)
                {
                    //This text is like '(XXXXX', WTF, remove first '('.
                    frameText.remove(0, 1);
                    bool indexTranslateOk=false;
                    //Try to translate this into a number.
                    int attemptIndex=frameText.toInt(&indexTranslateOk);
                    //If it's a number, set the indexed genre, or set the raw text.
                    setTextData(detailInfo.textLists[Genre],
                                indexTranslateOk?
                                    m_musicGlobal->indexedGenre(attemptIndex):
                                    frameText);
                }
                else
                {
                    //We find ')', check is it the last char.
                    if(rightBracketsPosition==frameText.size()-1)
                    {
                        //Remove the first and the last.
                        frameText.resize(frameText.size()-1);
                        frameText.remove(0, 1);
                        //Try to translate this into a number.
                        bool indexTranslateOk=false;
                        int attemptIndex=frameText.toInt(&indexTranslateOk);
                        //If it's a number, set the indexed genre, or set the raw text.
                        setTextData(detailInfo.textLists[Genre],
                                    indexTranslateOk?
                                        m_musicGlobal->indexedGenre(attemptIndex):
                                        frameText);
                    }
                    else
                    {
                        //There still something after the genre.
                        //That will be the genre text, set it to them.
                        setTextData(detailInfo.textLists[Genre],
                                    frameText.mid(rightBracketsPosition+1));
                    }
                }
            }
            else
            {
                //Treat the whole text as genre.
                setTextData(detailInfo.textLists[Genre],
                            frameText);
            }
            break;
        default:
            setTextData(detailInfo.textLists[frameIndex],
                        frameToText(frameData));
        }
    }
}
