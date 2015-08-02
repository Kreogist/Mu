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

#include "knmusictagapev2.h"

#define APEv2HeaderSize 32
#define ID3v1Size 128
#define ID3v1nAPEv2 160

KNMusicTagApev2::KNMusicTagApev2(QObject *parent) :
    KNMusicTagParser(parent),
    m_apePreamble("APETAGEX")
{

}

bool KNMusicTagApev2::parseTag(QFile &musicFile,
                               QDataStream &musicDataStream,
                               KNMusicAnalysisItem &analysisItem)
{
    //Check the file size first.
    if(musicFile.size()<APEv2HeaderSize)
    {
        return false;
    }
    //There're several positions we have to check:
    // * The beginning of the file. (APEv2)
    // * The end of the file. (APEv1, APEv2)
    // * If there's ID3v1 tag, check the position before ID3v1. (APEv1, APEv2)
    //Generate a header structure.
    APEHeader header;
    //Initial the tag start position.
    bool foundHeader=false;
    //Check the beginning of the file.
    if(checkHeader(0,
                   musicDataStream,
                   header))
    {
        //Tag start right after the tag. So we don't need to move the position
        //of the data stream.
        //Set the header found flag.
        foundHeader=true;
    }
    //Check the end of the file.
    else if(checkHeader(musicFile.size()-APEv2HeaderSize,
                        musicDataStream,
                        header))
    {
        //Move the data stream to the right position. It should be: (APEv1)
        //...........xxxx ... xxxxAPETAGEXxxxx ... xxx|(EOF)
        //(File data)|            |                   |
        //           Tag Start    Header Start        End of File
        //For APEv2, before the tag start it has another header. Ignore it.
        //Reset the device to start.
        musicDataStream.device()->reset();
        //Skip the file data.
        musicDataStream.skipRawData(musicFile.size()-header.size);
        //Set the header found flag.
        foundHeader=true;
    }
    //Check the position before ID3v1. Some file may have both ID3v1 and APEv1/
    //APEv2.
    else if(musicFile.size()>=ID3v1nAPEv2 && //File size first.
            checkHeader(musicFile.size()-ID3v1nAPEv2,
                        musicDataStream,
                        header))
    {
        //Move the data stream to the right position. It should be: (APEv1)
        //...........xxxx ... xxxxAPETAGEXxxxx ... xxxTAGxxx ... xxx|(EOF)
        //(File data)|            |                   |             |
        //           Tag Start    Header Start        ID3v1         End of File
        //For APEv2, before the tag start it has another header. Ignore it.
        //Reset the device to start.
        musicDataStream.device()->reset();
        //Skip the file data.
        musicDataStream.skipRawData(musicFile.size()-ID3v1Size-header.size);
        //Set the header found flag.
        foundHeader=true;
    }
    //Check if we have found the header. If we didn't find any header, then
    //failed to parse the APEv1/APEv2.
    if(!foundHeader)
    {
        return false;
    }
    //Read the tag from the file.
    char *rawTagData=new char[header.size];
    musicDataStream.readRawData(rawTagData, header.size);
    //Parse the raw tag data list.
    //!FIXME: add codes here.
    //Recover the memory.
    delete[] rawTagData;
    //Write the tag list to detail info.
    //!FIXME: add codes here.
    //Parse complete.
    return true;
}

bool KNMusicTagApev2::writeTag(KNMusicAnalysisItem &analysisItem)
{
    return false;
}

bool KNMusicTagApev2::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(analysisItem)
    //APEv2 cannot contains album art.
    return false;
}

inline bool KNMusicTagApev2::checkHeader(const int &position,
                                         QDataStream &dataStream,
                                         APEHeader &header)
{
    /*
     * APE Tags Header
     * Url: http://wiki.hydrogenaud.io/index.php?title=APE_Tags_Header
     * Header and Footer are different in 1 bit in the Tags Flags to distinguish
     * between them.
     * +--------------+-------+-------------------------------------------+
     * |Preamble      |64 bits|{ 'A', 'P', 'E', 'T', 'A', 'G', 'E', 'X' } |
     * +--------------+-------+-------------------------------------------+
     * |Version Number|32 bits|1000 = Version 1.000 (old)                 |
     * |              |       |2000 = Version 2.000 (new)                 |
     * +--------------+-------+-------------------------------------------+
     * |Tag Size      |32 bits|Tag size in bytes including footer and all |
     * |              |       |tag items excluding the header.            |
     * +--------------+-------+-------------------------------------------+
     * |Item Count    |32 bits|Number of items.                           |
     * +--------------+-------+-------------------------------------------+
     * |Tags Flags    |32 bits|Global flags of all items.                 |
     * +--------------+-------+-------------------------------------------+
     * |Reserved      |64 bits|Must be zero.                              |
     * +--------------+-------+-------------------------------------------+
     */
    //Reset the input device.
    dataStream.device()->reset();
    //skip to position.
    dataStream.skipRawData(position);
    //Read 32 bytes, that's the size of the header.
    char rawData[32];
    dataStream.readRawData(rawData, 32);
    //Check the header data, compare it to the fixed header text.
    if(memcpy(rawData, m_apePreamble, 32)==0)
    {
        //Save the information of the header.
        header.version=dataToSize(rawData+8);
        header.size=dataToSize(rawData+12);
        header.itemCount=dataToSize(rawData+16);
        header.flags=dataToSize(rawData+20);
        //Parse successfully.
        return true;
    }
    return false;
}

