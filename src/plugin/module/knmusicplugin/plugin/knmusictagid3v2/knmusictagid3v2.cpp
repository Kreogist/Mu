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
    m_isoCodec=QTextCodec::codecForName("ISO 8859-1");
    m_utf16Codec=QTextCodec::codecForName("UTF-16");
    m_utf16BECodec=QTextCodec::codecForName("UTF-16BE");
    m_utf16LECodec=QTextCodec::codecForName("UTF-16LE");
    m_utf8Codec=QTextCodec::codecForName("UTF-8");
    m_localeCodec=KNGlobal::localeDefaultCodec();

    //Initial music global.
    m_musicGlobal=KNMusicGlobal::instance();

    //Initial unsynchronisation data.
    //Using forced conversation to ignore the ambiguous calling.
    m_unsynchronisationRaw.append((char)0xff);
    m_unsynchronisationRaw.append((char)0x00);
    m_unsynchronisationTo.append((char)0xff);

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
    //If file is less than ID3v2 header, it can't contains ID3v2 tag.
    if(musicFile.size()<10)
    {
        return false;
    }
    //Initial datas.
    char rawHeader[10];
    ID3v2Header header;
    //Detect ID3v2 header.
    musicDataStream.readRawData(rawHeader, 10);
    if(!parseID3v2Header(rawHeader, header))
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
    generateID3v2Property(header.minor, property);
    parseID3v2RawData(rawTagData, header, property, frames);
    //Write the tag to details.
    if(!frames.isEmpty())
    {
        writeID3v2ToDetails(frames, property, detailInfo);
    }
    //Recover the memory.
    delete[] rawTagData;
    return true;
}

bool KNMusicTagID3v2::parseAlbumArt(KNMusicDetailInfo &detailInfo)
{
    if(!detailInfo.imageData.contains("ID3v2"))
    {
        return false;
    }
    //Get the total size of images.
    QByteArray imageTypes=detailInfo.imageData["ID3v2"].takeLast();
    int imageCount=imageTypes.size();
    QHash<int, ID3v2PictureFrame> imageMap;
    for(int i=0; i<imageCount; i++)
    {
        //Check the flag is "APIC" or "PIC"
        if(imageTypes.at(i)==1)
        {
            parseAPICImageData(detailInfo.imageData["ID3v2_Images"].takeFirst(),
                               imageMap);
        }
        else
        {
            parsePICImageData(detailInfo.imageData["ID3v2_Images"].takeFirst(),
                              imageMap);
        }
    }
    //If there's a album art image after parse all the album art, set.
    if(imageMap.contains(3))
    {
        detailInfo.coverImage=imageMap[3].image;
    }
    else
    {
        //Or else use the first image.
        if(!imageMap.isEmpty())
        {
            detailInfo.coverImage=imageMap.begin().value().image;
        }
    }
    return true;
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
    //Check is content empty.
    if(content.isEmpty())
    {
        return QString();
    }
    //Get the codec according to the first char.
    //The first char of the ID3v2 text is the encoding of the current text.
    //Using a state to catch the convert state.
    quint8 encoding=(quint8)(content.at(0));
    content.remove(0, 1);
    switch(encoding)
    {
    case EncodeISO: //0 = ISO-8859-1
        //Use unicode codec to translate.
        return m_usingDefaultCodec?
                    m_localeCodec->toUnicode(content).simplified().remove(QChar('\0')):
                    m_isoCodec->toUnicode(content).simplified().remove(QChar('\0'));
    case EncodeUTF16BELE: //1 = UTF-16 LE/BE (Treat other as no BOM UTF-16)
        //Decode via first two bytes.
        if((quint8)content.at(0)==0xFE && (quint8)content.at(1)==0xFF)
        {
            return m_utf16BECodec->toUnicode(content).simplified().remove(QChar('\0'));
        }
        if((quint8)content.at(0)==0xFF && (quint8)content.at(1)==0xFE)
        {
            return m_utf16LECodec->toUnicode(content).simplified().remove(QChar('\0'));
        }
        return m_utf16Codec->toUnicode(content).simplified().remove(QChar('\0'));
    case EncodeUTF16: //2 = UTF-16 BE without BOM
        //Decode with UTF-16
        return m_utf16Codec->toUnicode(content).simplified().remove(QChar('\0'));
    case EncodeUTF8: //3 = UTF-8
        //Use UTF-8 to decode it.
        return m_utf8Codec->toUnicode(content).simplified().remove(QChar('\0'));
    default://Use locale codec.
        return m_localeCodec->toUnicode(content).simplified().remove(QChar('\0'));
    }
}

bool KNMusicTagID3v2::parseID3v2Header(char *rawHeader,
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

bool KNMusicTagID3v2::parseID3v2RawData(char *rawTagData,
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

void KNMusicTagID3v2::generateID3v2Property(const quint8 &minor,
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

void KNMusicTagID3v2::writeID3v2ToDetails(const QLinkedList<ID3v2Frame> &frames,
                                          const ID3v2MinorProperty &property,
                                          KNMusicDetailInfo &detailInfo)
{
    QByteArray imageTypeList;
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
        //Check if the frame is unsynchronisation.
        if((*i).flags[1] & FrameUnsynchronisation)
        {
            frameData.replace(m_unsynchronisationRaw,
                              m_unsynchronisationTo);
        }
        //Get the frame Index.
        QString frameID=QString((*i).frameID).toUpper();
        if(frameID=="APIC" || frameID=="PIC")
        {
            //Here is a hack:
            //Using "ID3v2" as a counter, the size of the byte array is the
            //number of how many images contains in ID3v2.
            //If the frameID is "APIC", add a 1 to the "ID3v2" array, or else
            //add a 0.
            imageTypeList.append((int)(frameID=="APIC"));
            detailInfo.imageData["ID3v2_Images"].append(frameData);
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
                    //Try to translate the content in the brackets.
                    bool indexTranslateOk=false;
                    int attemptIndex=frameText.mid(1, rightBracketsPosition-1).toInt(&indexTranslateOk);
                    //If it's a number, set the indexed genre, or set the raw text.
                    setTextData(detailInfo.textLists[Genre],
                                indexTranslateOk?
                                    m_musicGlobal->indexedGenre(attemptIndex):
                                    frameText);
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
    //If there's any data in type list, add it to image data.
    if(!imageTypeList.isEmpty())
    {
        detailInfo.imageData["ID3v2"].append(imageTypeList);
    }
}

void KNMusicTagID3v2::parseAPICImageData(QByteArray imageData,
                                         QHash<int, ID3v2PictureFrame> &imageMap)
{
    //APIC contains:
    /*
       Text encoding      $xx
       MIME type          <text string> $00
       Picture type       $xx
       Description        <text string according to encoding> $00 (00)
       Picture data       <binary data>
     */
    //In the official document above, (00) in the description means there's a
    //00 byte after the $00, I finally understand what this mean.
    ID3v2PictureFrame currentFrame;
    //Get mime end and description end.
    int mimeTypeEnd=imageData.indexOf('\0', 1),
        descriptionEnd=mimeTypeEnd+2;
    //Backup the text encording and get the picture type.
    quint8 textEncoding=imageData.at(0),
           pictureType=imageData.at(mimeTypeEnd+1);
    switch (textEncoding)
    {
    case EncodeISO:
    case EncodeUTF8:
        descriptionEnd=imageData.indexOf('\0', mimeTypeEnd+2);
        break;
    case EncodeUTF16BELE:
    case EncodeUTF16:
        while(imageData.at(descriptionEnd)!='\0' ||
              imageData.at(descriptionEnd+1)!='\0')
        {
            descriptionEnd+=2;
        }
        break;
    default:
        break;
    }
    //Get the mime type text.
    currentFrame.mimeType=frameToText(imageData.left(mimeTypeEnd));
    //Get the description text.
    QByteArray descriptionText=imageData.mid(mimeTypeEnd+2,
                                             descriptionEnd-(mimeTypeEnd+2));
    descriptionText.insert(0, textEncoding);
    currentFrame.description=frameToText(descriptionText);
    //Get the image.
    switch (textEncoding)
    {
    case EncodeISO:
    case EncodeUTF8:
        imageData.remove(0, descriptionEnd+1);
        break;
    case EncodeUTF16BELE:
    case EncodeUTF16:
        imageData.remove(0, descriptionEnd+2);
        break;
    default:
        break;
    }
    currentFrame.image.loadFromData(imageData);
    //If parse the image success, add it to map.
    if(!currentFrame.image.isNull())
    {
        imageMap[pictureType]=currentFrame;
    }
}

void KNMusicTagID3v2::parsePICImageData(QByteArray imageData,
                                        QHash<int, ID3v2PictureFrame> &imageMap)
{
    //PIC contains:
    /*
      Text encoding      $xx
      Image format       $xx xx xx
      Picture type       $xx
      Description        <textstring> $00 (00)
      Picture data       <binary data>
    */
    //In the official document above, the (00) in description is the same as APIC.
    ID3v2PictureFrame currentFrame;
    //Get the mime type text.
    currentFrame.mimeType=imageData.mid(1, 3);
    //Backup the text encording and get the picture type.
    quint8 textEncoding=imageData.at(0), pictureType=imageData.at(4);
    //Get description end.
    int descriptionEnd=5;
    switch (textEncoding)
    {
    case EncodeISO:
    case EncodeUTF8:
        descriptionEnd=imageData.indexOf('\0', 5);
        break;
    case EncodeUTF16BELE:
    case EncodeUTF16:
        while(imageData.at(descriptionEnd)!='\0' ||
              imageData.at(descriptionEnd+1)!='\0')
        {
            descriptionEnd+=2;
        }
        break;
    default:
        break;
    }
    //Get the description.
    QByteArray descriptionText=imageData.mid(5, descriptionEnd-5);
    descriptionText.append(textEncoding);
    currentFrame.description=frameToText(descriptionText);
    //Get the image.
    switch (textEncoding)
    {
    case EncodeISO:
    case EncodeUTF8:
        imageData.remove(0, descriptionEnd+1);
        break;
    case EncodeUTF16BELE:
    case EncodeUTF16:
        imageData.remove(0, descriptionEnd+2);
        break;
    default:
        break;
    }
    currentFrame.image.loadFromData(imageData);
    //If parse the image success, add it to map.
    if(!currentFrame.image.isNull())
    {
        imageMap[pictureType]=currentFrame;
    }
}
bool KNMusicTagID3v2::usingDefaultCodec() const
{
    return m_usingDefaultCodec;
}

void KNMusicTagID3v2::setUsingDefaultCodec(bool usingDefaultCodec)
{
    m_usingDefaultCodec = usingDefaultCodec;
}

