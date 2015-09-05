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

#include "knmusictagflac.h"

#include <QDebug>

QHash<QString, int> KNMusicTagFLAC::m_fieldNameIndex=QHash<QString, int>();

KNMusicTagFLAC::KNMusicTagFLAC(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Check the field name hash has been built before.
    if(m_fieldNameIndex.isEmpty())
    {
        m_fieldNameIndex.insert("title", Name);
        m_fieldNameIndex.insert("artist", Artist);
        m_fieldNameIndex.insert("album", Album);
        m_fieldNameIndex.insert("genre", Genre);
        m_fieldNameIndex.insert("description", Description);
        m_fieldNameIndex.insert("composer", Composer);
        m_fieldNameIndex.insert("totaldiscs", DiscCount);
        m_fieldNameIndex.insert("discnumber", DiscNumber);
        m_fieldNameIndex.insert("comment", Comments);
        m_fieldNameIndex.insert("albumartist", AlbumArtist);
        m_fieldNameIndex.insert("tracktotal", TrackCount);
        m_fieldNameIndex.insert("date", Year);
        m_fieldNameIndex.insert("tracknumber", TrackNumber);
    }
}

bool KNMusicTagFLAC::parseTag(QFile &musicFile,
                              QDataStream &musicDataStream,
                              KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(musicFile)
    //Generate the header cache.
    char blockHeader[5];
    //Check the header of the music file.
    //Read the header.
    musicDataStream.readRawData(blockHeader, 4);
    //Check the header, it must be 'fLaC'(66 4C 61 43).
    if(blockHeader[0]!=0x66 || blockHeader[1]!=0x4C || blockHeader[2]!=0x61 ||
            blockHeader[3]!=0x43)
    {
        //This file is not a flac format file.
        return false;
    }
    //Prepare some temporary data.
    bool lastMetadataBlock=false;       //The last metadata block flag.
    char *rawTagData;                   //Tag data cache.
    QLinkedList<VorbisFrame> tagMap;    //Vorbis tag frame linked list.
    //Read the metadata until it's the last block.
    while(!lastMetadataBlock)
    {
        //Read the METADATA block header.
        musicDataStream.readRawData(blockHeader, 4);
        //Parse the header.
        //Check whether the current block is the last one. the first bit is 1 if
        //it's the last block, or else 0.
        lastMetadataBlock=((quint8)blockHeader[0]>>7)==1;
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
        //Calculate the block type.
        quint8 blockType=(quint8)blockHeader[0] & 0x7F;
        //The last 3 bytes are the size of this block expect the header.
        quint32 blockSize=(((quint32)blockHeader[1]<<16) & 0x00FF0000) +
                          (((quint32)blockHeader[2]<<8)  & 0x0000FF00) +
                          ( (quint32)blockHeader[3]      & 0x000000FF);
        //Check the type of the block, if it's not 4 or 6, then ignore.
        if(blockType!=4 && blockType!=6)
        {
            //Skip the data.
            musicDataStream.skipRawData(blockSize);
            //Go to the next block.
            continue;
        }
        //Read the raw metadata block data.
        rawTagData=new char[blockSize];
        musicDataStream.readRawData(rawTagData, blockSize);
        //Deep copy the block data.
        QByteArray blockData=QByteArray(rawTagData, blockSize);
        //Recover the memory.
        delete[] rawTagData;

        //Parse the block data according to the block type.
        if(blockType==4) //Vorbis Comment.
        {
            //Parse the vorbis comment.
            parseVorbisComment(blockData, tagMap);
            //Get the detail info.
            KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
            //Write tag to the detail info.
            //Try all the tags, check whether it's usable in text list.
            for(auto i=tagMap.begin(); i!=tagMap.end(); ++i)
            {
                //Check whether the field name is contains in the field name
                //index hash list.
                int fieldNameIndex=m_fieldNameIndex.value((*i).fieldName, -1);
                if(fieldNameIndex!=-1)
                {
                    //Set the text data.
                    setTextData(detailInfo.textLists[fieldNameIndex],
                                (*i).data);
                }
            }
        }
        else //Block type should be 6, image.
        {
            //Insert the flac block data.
            analysisItem.imageData["FLAC"].append(blockData);
        }
    }
    //Mission complete.
    return true;
}

bool KNMusicTagFLAC::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(analysisItem)
    return false;
}

bool KNMusicTagFLAC::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //Check whether the image data contains the FLAC format album data.
    if(!analysisItem.imageData.contains("FLAC") ||
            analysisItem.imageData.value("FLAC").isEmpty())
    {
        return false;
    }
    //Prepare the image map.
    QHash<int, PictureFrame> imageMap;
    //Parse all the picture.
    parsePictureList(analysisItem.imageData.value("FLAC"), imageMap);
    //If there's a album art image after parse all the album art, set.
    if(imageMap.contains(3))
    {
        analysisItem.coverImage=imageMap.value(3).image;
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
                                               QLinkedList<VorbisFrame> &tagMap)
{
    //There's a string like 'Lavf53.24.0' at the begin, should jump over.
    //It's a 4-byte length+string data+4-byte unknown data.
    quint32 stringStart=KNMusicUtil::inverseCharToInt32(blockData.data())+8,
            dataSize=blockData.size();
    //If the current string start position is at the end of data, exit.
    while(stringStart<dataSize)
    {
        //This string is a PASCAL-liked string, start with four bytes length.
        quint32 stringLength=
                KNMusicUtil::inverseCharToInt32(blockData.data()+stringStart);
        stringStart+=4;
        //Now, get the raw comment data.
        QString rawCommentData=
                blockData.mid(stringStart, stringLength);
        //Parse the raw comment data:
        //Vorbis Comment is a string like: Key=Value
        int equalPosition=rawCommentData.indexOf('=');
        VorbisFrame frame;
        frame.fieldName=rawCommentData.left(equalPosition).toLower();
        frame.data=rawCommentData.mid(equalPosition+1);
        tagMap.append(frame);
        //Move pointer.
        stringStart+=stringLength;
    }
}

inline void KNMusicTagFLAC::parsePictureList(const QList<QByteArray> &blocks,
                                             QHash<int, PictureFrame> &imageMap)
{
    //Parse all the picture list.
    for(auto i=blocks.constBegin(); i!=blocks.constEnd(); ++i)
    {
        //Parse the picture.
        //Picture metadata block start with 4-bytes type.
        //Following a 4-bytes length mime type discription string size.
        quint32 dataSize=KNMusicUtil::charToInt32((*i).data()+4),
                dataPointer=dataSize+8;
        //Prepare the picture frame.
        PictureFrame frame;
        //Read the mime type string.
        frame.mimeType=QByteArray((*i).mid(8, dataSize));
        //Then is a 4-bytes description length followed by description.
        dataSize=KNMusicUtil::charToInt32((*i).data()+dataPointer);
        frame.description=QByteArray((*i).mid(dataPointer+4, dataSize));
        //Here should read these 4-bytes data: width, height, depth, index color
        //num and the size of image, but Qt is so powerful that we don't need to
        //do these.
        //I love you! Qt! Daisuki!
        frame.image.loadFromData((*i).mid(dataPointer+dataSize+24));
        //Set the frame to the hash.
        imageMap.insert(KNMusicUtil::charToInt32((*i).data()), frame);
    }
}
