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
#include "knmusictagflac.h"

#include <QDebug>

KNMusicTagFLAC::KNMusicTagFLAC(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Initial flac frames.
    m_fieldNameIndex["title"]=Name;
    m_fieldNameIndex["artist"]=Artist;
    m_fieldNameIndex["album"]=Album;
    m_fieldNameIndex["genre"]=Genre;
    m_fieldNameIndex["description"]=Description;
    m_fieldNameIndex["composer"]=Composer;
    m_fieldNameIndex["totaldiscs"]=DiscCount;
    m_fieldNameIndex["discnumber"]=DiscNumber;
    m_fieldNameIndex["comment"]=Comments;
    m_fieldNameIndex["albumartist"]=AlbumArtist;
    m_fieldNameIndex["tracktotal"]=TrackCount;
    m_fieldNameIndex["date"]=Year;
    m_fieldNameIndex["tracknumber"]=TrackNumber;
}

bool KNMusicTagFLAC::praseTag(QFile &musicFile,
                              QDataStream &musicDataStream,
                              KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(musicFile)
    char rawHeader[5];
    //Check the header of file, it must be 'fLaC'(66 4C 61 43).
    musicDataStream.readRawData(rawHeader, 4);
    if(rawHeader[0]!=0x66 || rawHeader[1]!=0x4C ||
            rawHeader[2]!=0x61 || rawHeader[3]!=0x43)
    {
        return false;
    }
    //Prepare some temporary data.
    bool lastMetadataBlock=false;
    char *rawTagData;
    QLinkedList<VorbisCommentFrame> tagMap;
    //Read the metadata until it's the last block.
    while(!lastMetadataBlock)
    {
        //Read the METADATA BLOCK HEADER.
        musicDataStream.readRawData(rawHeader, 4);

        //Parse the header.
        //If the current block is the last one, the first bit is 1, or else 0.
        lastMetadataBlock=((quint8)rawHeader[0]>>7)==1;
        //the 2-8 bit is the block type:
        /*
            0 : STREAMINFO
            1 : PADDING
            2 : APPLICATION
            3 : SEEKTABLE
            4 : VORBIS_COMMENT
            5 : CUESHEET
            6 : PICTURE
            7-126 : reserved
            127 : Invalid.
        */
        quint8 blockType=(quint8)rawHeader[0]&0b01111111;
        //The last 3 bytes are the size of this block expect the header.
        quint32 blockSize=(((quint32)rawHeader[1]<<16) & 0b00000000111111110000000000000000) +
                (((quint32)rawHeader[2]<<8)  & 0b00000000000000001111111100000000) +
                ( (quint32)rawHeader[3]      & 0b00000000000000000000000011111111);

        //Read the raw metadata block data.
        rawTagData=new char[blockSize];
        musicDataStream.readRawData(rawTagData, blockSize);
        QByteArray blockData=QByteArray(rawTagData, blockSize);
        delete[] rawTagData;

        //Parse the block data according to the block type.
        switch(blockType)
        {
        case 4:
            parseVorbisComment(blockData, tagMap);
            writeTagToDetails(tagMap, analysisItem.detailInfo);
            break;
        case 6:
            analysisItem.imageData["FLAC"].append(blockData);
            break;
        default:
            break;
        }
    }
    return true;
}

bool KNMusicTagFLAC::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    if(!analysisItem.imageData.contains("FLAC") ||
            analysisItem.imageData["FLAC"].isEmpty())
    {
        return false;
    }
    QHash<int, PictureFrame> imageMap;
    //Parse all the picture.
    parsePictureList(analysisItem.imageData["FLAC"], imageMap);
    //If there's a album art image after parse all the album art, set.
    if(imageMap.contains(3))
    {
        analysisItem.coverImage=imageMap[3].image;
    }
    else
    {
        //Or else use the first image.
        if(!imageMap.isEmpty())
        {
            analysisItem.coverImage=imageMap.begin().value().image;
        }
    }
    return true;
}

inline void KNMusicTagFLAC::parseVorbisComment(QByteArray &blockData,
                                               QLinkedList<VorbisCommentFrame> &tagMap)
{
    //There's a string like 'Lavf53.24.0' at the begin, should jump over.
    //It's a 4-byte length+string data+4-byte unknown data.
    quint32 stringStart=
            KNMusicGlobal::inverseCharToInt32(blockData.data())+8,
            dataSize=blockData.size();
    //If the current string start position is at the end of data, exit.
    while(stringStart<dataSize)
    {
        //This string is a PASCAL-liked string, start with four bytes length.
        quint32 stringLength=
                KNMusicGlobal::inverseCharToInt32(blockData.data()+stringStart);
        stringStart+=4;
        //Now, get the raw comment data.
        QString rawCommentData=
                QByteArray(blockData.mid(stringStart, stringLength));
        //Parse the raw comment data:
        //Vorbis Comment is a string like: Key=Value
        int equalPosition=rawCommentData.indexOf('=');
        VorbisCommentFrame frame;
        frame.fieldName=rawCommentData.left(equalPosition).toLower();
        frame.data=rawCommentData.mid(equalPosition+1);
        tagMap.append(frame);
        //Move pointer.
        stringStart+=stringLength;
    }
}

inline void KNMusicTagFLAC::parsePictureList(QList<QByteArray> &blocks,
                                             QHash<int, PictureFrame> &imageMap)
{
    for(auto i=blocks.begin();
        i!=blocks.end();
        ++i)
    {
        parsePicture(*i, imageMap);
    }
}

inline void KNMusicTagFLAC::parsePicture(QByteArray &blockData,
                                         QHash<int, PictureFrame> &imageMap)
{
    PictureFrame frame;
    //Picture metadata block start with 4-bytes type.
    //Following a 4-bytes length mime type discription string size.
    quint32 imageType=KNMusicGlobal::charToInt32(blockData.data()),
            dataSize=KNMusicGlobal::charToInt32(blockData.data()+4),
            dataPointer=dataSize+8;
    //Read the mime type string.
    frame.mimeType=QByteArray(blockData.mid(8, dataSize));
    //Then is a 4-bytes description.
    dataSize=KNMusicGlobal::charToInt32(blockData.data()+dataPointer);
    frame.description=QByteArray(blockData.mid(dataPointer+4, dataSize));
    //Here should read these 4-bytes data: width, height, depth, index color num
    //and the size of image, but Qt is so powerful that we don't need to do these.
    //I love you! Qt! Daisuki!
    frame.image.loadFromData(blockData.mid(dataPointer+dataSize+24));
    //Set the frame to the hash.
    imageMap[imageType]=frame;
}

inline void KNMusicTagFLAC::writeTagToDetails(QLinkedList<VorbisCommentFrame> &tagMap,
                                              KNMusicDetailInfo &detailInfo)
{
    //Try all the tags, check whether it's usable in text list.
    for(auto i=tagMap.begin();
        i!=tagMap.end();
        ++i)
    {
        if(m_fieldNameIndex.contains((*i).fieldName))
        {
            setTextData(detailInfo.textLists[m_fieldNameIndex[(*i).fieldName]],
                    (*i).data);
        }
    }
}
