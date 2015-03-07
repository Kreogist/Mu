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
#include "knmusictagwav.h"

#include <QDebug>

KNMusicTagWAV::KNMusicTagWAV(QObject *parent) :
    KNMusicTagID3v2(parent)
{
    //Initial id3 chunk
    m_id32Chunk[0]='i';
    m_id32Chunk[1]='d';
    m_id32Chunk[2]='3';
    m_id32Chunk[3]=0x20;
    //Initial the map.
    m_listKeyIndex["IART"]=Artist;
    m_listKeyIndex["IPRO"]=Album;
    m_listKeyIndex["IBPM"]=BeatsPerMinuate;
    m_listKeyIndex["ICMT"]=Comments;
    m_listKeyIndex["IGNR"]=Genre;
    m_listKeyIndex["INAM"]=Name;
    m_listKeyIndex["ITRK"]=TrackNumber;
    m_listKeyIndex["ICRD"]=Year;
}

bool KNMusicTagWAV::praseTag(QFile &musicFile,
                             QDataStream &musicDataStream,
                             KNMusicAnalysisItem &analysisItem)
{
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Check file size.
    if(musicFile.size()<12)
    {
        return false;
    }
    //Read the header.
    char rawHeader[12];
    musicDataStream.readRawData(rawHeader, 12);
    if(memcmp(rawHeader, m_riffHeader, 4)!=0 ||
            memcmp(rawHeader+8, m_waveHeader, 4)!=0)
    {
        return false;
    }
    /*
     * WAV file is a combination of several chunks.
     * Read all the chunks, and find LIST and id32 chunk.
     */
    qint64 fileSize=musicFile.size();
    bool listFound=false, id32Found=false;
    char chunkHeader[8];
    QList<WAVItem> listData;
    QLinkedList<ID3v2Frame> frames;
    ID3v2MinorProperty property;
    while(musicDataStream.device()->pos()<fileSize && !listFound && !id32Found)
    {
        //Read chunk head.
        musicDataStream.readRawData(chunkHeader, 8);
        //Calculate the truck size.
        quint32 chunkSize=(((quint32)chunkHeader[7]<<24) & 0xFF000000)+
                          (((quint32)chunkHeader[6]<<16) & 0x00FF0000)+
                          (((quint32)chunkHeader[5]<<8 ) & 0x0000FF00)+
                          ( (quint32)chunkHeader[4]      & 0x000000FF);
        if(memcmp(chunkHeader, m_listChunk, 4)==0)
        {
            //Read the raw data.
            char *listRawData=new char[chunkSize];
            musicDataStream.readRawData(listRawData, chunkSize);
            //Parse list chunk.
            parseListChunk(listRawData, chunkSize, listData);
            //Recover memory.
            delete[] listRawData;
            //Set flag.
            listFound=true;
        }
        else if(memcmp(chunkHeader, m_id32Chunk, 4)==0)
        {
            //Initial datas.
            char rawHeader[10];
            ID3v2Header header;
            //Detect ID3v2 header.
            musicDataStream.readRawData(rawHeader, 10);
            if(!parseID3v2Header(rawHeader, header))
            {
                musicDataStream.skipRawData(chunkSize-10);
                continue;
            }
            //Check is chunk size smaller than tag size.
            if(chunkSize<(header.size+10))
            {
                //File is smaller than the tag says, failed to get.
                musicDataStream.skipRawData(chunkSize-10);
                continue;
            }
            //Read the raw tag data.
            char *rawTagData=new char[header.size];
            musicDataStream.readRawData(rawTagData, header.size);
            //Parse these raw data.
            generateID3v2Property(header.minor, property);
            parseID3v2RawData(rawTagData, header, property, frames);
            //Recover the memory.
            delete[] rawTagData;
            //Set flag.
            id32Found=true;
        }
        else
        {
            //Skip the data.
            musicDataStream.skipRawData(chunkSize);
        }
    }
    //Write list data to detail info.
    if(!listData.isEmpty())
    {
        writeListDataToDetailInfo(listData, detailInfo);
    }
    //Write the id3 tag to details.
    if(!frames.isEmpty())
    {
        writeID3v2ToDetails(frames, property, analysisItem);
    }
    return true;
}

bool KNMusicTagWAV::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //WAV doesn't contains album art, but the ID3v2 might contains data.
    Q_UNUSED(analysisItem)
    return false;
}

inline void KNMusicTagWAV::parseListChunk(char *rawData,
                                   quint32 dataSize,
                                   QList<WAVItem> &listData)
{
    //Check the header of the chunk data.
    if(memcmp(m_listInfoHeader, rawData, 4)!=0)
    {
        return;
    }
    //Initial the pointer and the size counter.
    rawData+=4;
    dataSize-=4;
    //Read all the data from the chunk data.
    while(dataSize>0)
    {
        //Calculate the frame size.
        quint32 frameSize=(((quint32)rawData[7]<<24) & 0xFF000000)+
                          (((quint32)rawData[6]<<16) & 0x00FF0000)+
                          (((quint32)rawData[5]<<8 ) & 0x0000FF00)+
                          ( (quint32)rawData[4]      & 0x000000FF);
        if(frameSize>dataSize)
        {
            break;
        }
        //Set the data.
        WAVItem currentItem;
        currentItem.key=QByteArray(rawData, 4);
        currentItem.value=QByteArray(rawData+8, frameSize);
        //Add to list.
        listData.append(currentItem);
        //Move the pointer and reduce counter.
        rawData+=frameSize+8;
        dataSize-=(frameSize+8);
        //Magic, don't touch it.
        while(rawData[0]==0 && dataSize>0)
        {
            rawData++;
            dataSize--;
        }
    }
}

inline void KNMusicTagWAV::writeListDataToDetailInfo(const QList<WAVItem> &listData,
                                                     KNMusicDetailInfo &detailInfo)
{
    for(QList<WAVItem>::const_iterator i=listData.begin();
        i!=listData.end();
        ++i)
    {
        //Get the index of current item.
        if(m_listKeyIndex.contains((*i).key))
        {
            int truckKeyIndex=m_listKeyIndex[(*i).key];
            switch(truckKeyIndex)
            {
            default:
                detailInfo.textLists[truckKeyIndex]=(*i).value;
            }
        }
    }
}
