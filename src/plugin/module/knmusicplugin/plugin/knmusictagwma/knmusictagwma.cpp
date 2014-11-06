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
#include "knmusictagwma.h"

#include <QDebug>

KNMusicTagWMA::KNMusicTagWMA(QObject *parent) :
    KNMusicTagParser(parent)
{
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
                                              frameSize-24);
        }
        else if(isExtendFrame(framePointer)) //Check if it's extend frame.
        {
            extendParsed=parseExtendFrame(framePointer+24,
                                          frameSize-24);
        }
        //Move the pointer and reduce the data count.
        framePointer+=frameSize;
        tagDataCount-=frameSize;
    }

    delete[] rawTagData;
    return true;
}

bool KNMusicTagWMA::parseAlbumArt(KNMusicDetailInfo &detailInfo)
{
    ;
}

bool KNMusicTagWMA::isStandardFrame(char *frame)
{
    return !memcmp(frame, m_standardFrame, 16);
}

bool KNMusicTagWMA::isExtendFrame(char *frame)
{
    return !memcmp(frame, m_extendedFrame, 16);
}

bool KNMusicTagWMA::parseStandardFrame(char *frameStart,
                                       const quint64 &frameSize)
{
    return true;
}

bool KNMusicTagWMA::parseExtendFrame(char *frameStart,
                                     const quint64 &frameSize)
{
    return true;
}
