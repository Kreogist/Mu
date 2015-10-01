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

#include "knmusictagwav.h"

QHash<QString, int> KNMusicTagWav::m_listKeyIndex=QHash<QString, int>();

char KNMusicTagWav::m_id32Chunk[4]={'i', 'd', '3', (char)0x20};

KNMusicTagWav::KNMusicTagWav(QObject *parent) :
    KNMusicTagId3v2(parent),
    m_riffHeader("RIFF"),
    m_waveHeader("WAVE"),
    m_listChunk("LIST"),
    m_listInfoHeader("INFO")
{
    //Check whether the global data has been initialized or not.
    if(m_listKeyIndex.isEmpty())
    {
        m_listKeyIndex.insert("IART", Artist);
        m_listKeyIndex.insert("IPRO", Album);
        m_listKeyIndex.insert("IBPM", BeatsPerMinuate);
        m_listKeyIndex.insert("ICMT", Comments);
        m_listKeyIndex.insert("IGNR", Genre);
        m_listKeyIndex.insert("INAM", Name);
        m_listKeyIndex.insert("ITRK", TrackNumber);
        m_listKeyIndex.insert("ICRD", Year);
    }
}

bool KNMusicTagWav::parseTag(QFile &musicFile,
                             QDataStream &musicDataStream,
                             KNMusicAnalysisItem &analysisItem)
{
    //Check file size.
    if(musicFile.size()<12)
    {
        //It cannot be a wav format file, until a valid one.
        return false;
    }
    //Gernerate the header cache.
    char rawHeader[12];
    //Read the header.
    musicDataStream.readRawData(rawHeader, 12);
    //Check the riff header and the wave header.
    if(memcmp(rawHeader, m_riffHeader, 4)!=0 ||
            memcmp(rawHeader+8, m_waveHeader, 4)!=0)
    {
        //This is not a wav file.
        return false;
    }
    /*
     * WAV file is a combination of several chunks.
     * Read all the chunks, and find LIST and id32 chunk.
     */
    //Get the music file size.
    qint64 fileSize=musicFile.size();
    //Generate the chunk header cache.
    char chunkHeader[8];
    //Initial the chunk found flag.
    bool listFound=false, id32Found=false;
    //We have to prepare the list here and write the data after parse all the
    //data, we want the id32 chunk has a higher priority than list chunk.
    //Prepare the wav item list for list chunk.
    QList<WAVItem> listData;
    //Generate the raw frame linked list for id32 chunk.
    QLinkedList<ID3v2Frame> frames;
    //Generate the id3v2 frame function set for id32 chunk.
    ID3v2FunctionSet functionSet;
    //Start finding the chunk.
    while(musicDataStream.device()->pos()<fileSize && !listFound && !id32Found)
    {
        //Read chunk head.
        musicDataStream.readRawData(chunkHeader, 8);
        //Calculate the chunk size.
        quint32 chunkSize=KNMusicUtil::inverseCharToInt32(chunkHeader+4);
        //Check if it's list chunk
        if(memcmp(chunkHeader, m_listChunk, 4)==0)
        {
            //Set list chunk found flag to true.
            listFound=true;
            //Generate chunk size cache.
            char *listRawData=new char[chunkSize];
            //Read the raw data.
            musicDataStream.readRawData(listRawData, chunkSize);
            //Parse list chunk.
            parseListChunk(listRawData, chunkSize, listData);
            //Recover memory.
            delete[] listRawData;
        }
        //Check if it's id32 chunk.
        else if(memcmp(chunkHeader, m_id32Chunk, 4)==0)
        {
            //Generate ID3v2 header cache.
            char rawHeader[10];
            //Generate ID3v2 header structure.
            ID3v2Header header;
            //Read ID3v2 header data.
            musicDataStream.readRawData(rawHeader, 10);
            //Parse the ID3v2 header.
            //and then Check is chunk size smaller than tag size.
            if(!parseID3v2Header(rawHeader, header) ||
                    chunkSize<(header.size+10))
            {
                //If we cannot parse it, skip the whole chunk.
                musicDataStream.skipRawData(chunkSize-10);
                //Continue to next chunk.
                continue;
            }
            //Generate the raw tag data field.
            char *rawTagData=new char[header.size];
            //Read the raw tag data.
            musicDataStream.readRawData(rawTagData, header.size);
            //Get the function set according to the minor version of the header.
            getId3v2FunctionSet(header.major, functionSet);
            //Parse the raw data.
            parseID3v2RawData(rawTagData, header, functionSet, frames);
            //Recover the memory.
            delete[] rawTagData;
            //Set the id32 chunk find flag to true.
            id32Found=true;
        }
        //For all the other chunks.
        else
        {
            //Skip the data.
            musicDataStream.skipRawData(chunkSize);
        }
    }
    //Check if the list data is not empty, first.
    //The data can be overwrite by id32 chunk data.
    if(!listData.isEmpty())
    {
        //Check all the data in the wav item list.
        for(auto i : listData)
        {
            //Get the index of current item.
            int chunkIndex=m_listKeyIndex.value(i.key, -1);
            //Check the validation of chunk index.
            if(chunkIndex==-1)
            {
                //Abandon the current index.
                continue;
            }
            //Set the data.
            analysisItem.detailInfo.textLists[chunkIndex]=
                    QVariant(i.value);
        }
    }
    //Check id32 chunk data then.
    if(!frames.isEmpty())
    {
        //Write the tag to analysis info.
        writeFrameToDetails(frames, functionSet, analysisItem);
    }
    //Mission complete.
    return true;
}

bool KNMusicTagWav::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    //This is a read-only parser.
    Q_UNUSED(analysisItem)
    return false;
}

bool KNMusicTagWav::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //WAV doesn't contains album art, but the ID3v2 might contains data.
    //And ID3v2 parser will parse the image for it.
    Q_UNUSED(analysisItem)
    return false;
}

bool KNMusicTagWav::writable() const
{
    return false;
}

bool KNMusicTagWav::writeCoverImage() const
{
    return false;
}

inline void KNMusicTagWav::parseListChunk(char *rawData,
                                          quint32 dataSize,
                                          QList<WAVItem> &listData)
{
    //Check the header of the chunk data.
    if(memcmp(m_listInfoHeader, rawData, 4)!=0)
    {
        //Ignore the invalid list chunk.
        return;
    }
    //Initial the pointer and the size counter.
    rawData+=4;dataSize-=4;
    //Read all the data from the chunk data.
    while(dataSize>0)
    {
        //Calculate the frame size.
        quint32 frameSize=KNMusicUtil::inverseCharToInt32(rawData+4);
        //Check out the frame size is still valid or not.
        if(frameSize>dataSize)
        {
            break;
        }
        //Generate a wav item.
        WAVItem currentItem;
        //Set the data.
        currentItem.key=QByteArray(rawData, 4);
        currentItem.value=QByteArray(rawData+8, frameSize);
        //Add to list.
        listData.append(currentItem);
        //Move the pointer and reduce counter.
        rawData+=frameSize+8;dataSize-=(frameSize+8);
        //Skip the unused 0 bytes.
        while(rawData[0]==0 && dataSize>0)
        {
            //Skip the data.
            rawData++;dataSize--;
        }
    }
}

