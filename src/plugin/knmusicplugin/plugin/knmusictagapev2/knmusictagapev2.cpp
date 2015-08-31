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
#include <QTemporaryFile>

#include "knmusictagapev2.h"

#include <QDebug>

#define APEv2HeaderSize 32
#define ID3v1Size 128
#define ID3v1nAPEv2 160
//1MB music data cache copy size.
#define DataCacheSize 1048576

QHash<QString, int> KNMusicTagApev2::m_keyIndex=QHash<QString, int>();
QHash<int, QString> KNMusicTagApev2::m_indexKey=QHash<int, QString>();

KNMusicTagApev2::KNMusicTagApev2(QObject *parent) :
    KNMusicTagParser(parent),
    m_apePreamble("APETAGEX")
{
    //Initial the index-key map.
    if(m_keyIndex.isEmpty())
    {
        //Insert the string and index to key-index map hash group.
        m_keyIndex.insert("TITLE", Name);
        m_keyIndex.insert("ARTIST", Artist);
        m_keyIndex.insert("ALBUM", Album);
        m_keyIndex.insert("TRACK", TrackNumber);
        m_keyIndex.insert("COMPOSER", Composer);
        m_keyIndex.insert("COMMENT", Comments);
        m_keyIndex.insert("YEAR", Year);
        m_keyIndex.insert("ALBUM ARTIST", AlbumArtist);
        m_keyIndex.insert("GENRE", Genre);

        //Insert the string and key to index-key map hash group.
        m_indexKey.insert(Name, "TITLE");
        m_indexKey.insert(Artist, "ARTIST");
        m_indexKey.insert(Album, "ALBUM");
        m_indexKey.insert(TrackNumber, "TRACK");
        m_indexKey.insert(Composer, "COMPOSER");
        m_indexKey.insert(Comments, "COMMENT");
        m_indexKey.insert(Year, "YEAR");
        m_indexKey.insert(AlbumArtist, "ALBUM ARTIST");
        m_indexKey.insert(Genre, "GENRE");
    }
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
    QList<APETagItem> tagList;
    parseRawData(rawTagData, header, tagList);
    //Recover the memory.
    delete[] rawTagData;
    //Write the tag list to analysis item.
    //Get the detail info.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Parse each tag list.
    for(auto i=tagList.constBegin(); i!=tagList.constEnd(); ++i)
    {
        //Get the frame index from the hash list.
        int frameIndex=m_keyIndex.value((*i).key, -1);
        //If we cannot map the key to the index, then ignore the current frame.
        if(frameIndex==-1)
        {
            continue;
        }
        switch(frameIndex)
        {
        case TrackNumber:
        {
            //Get the track string data.
            QString trackText=QString((*i).value);
            //Find the '/' char.
            int splitterIndex=trackText.indexOf('/');
            //If we cannot find the splitter,
            if(splitterIndex==-1)
            {
                //means it only contains track number.
                detailInfo.textLists[TrackNumber]=QVariant(trackText);
            }
            else
            {
                //Or else, it contains track number and track count data.
                //Treat the left side as track number, and the right side as
                //track count.
                detailInfo.textLists[TrackNumber]=trackText.left(splitterIndex);
                detailInfo.textLists[TrackCount]=trackText.mid(splitterIndex+1);
            }
        }
        default:
            //For default cases, because it's UTF-8 plain text, just write the
            //data to the text list.
            detailInfo.textLists[frameIndex]=QString((*i).value);
        }
    }
    //Parse complete.
    return true;
}

bool KNMusicTagApev2::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    //Write the data according to the detail info.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Check the file is still exist or not.
    QFile musicFile(detailInfo.filePath);
    //If the file is not exist then return false.
    if(!musicFile.exists())
    {
        return false;
    }

    //Find the original tag data.
    //ID3v1 header flag.
    bool hasId3v1=false;
    //Generate a header structure.
    APEHeader header;
    //Generate the raw tag data list.
    QList<APETagItem> itemList;
    //Initial the tag start position.
    qint64 tagDataStart=-1, tagDataLength=-1;
    //Open the file first.
    if(musicFile.open(QIODevice::ReadOnly))
    {
        //Generate a data stream.
        QDataStream musicDataStream(&musicFile);
        if(musicFile.size() > 128)
        {
            //ID3v1 header cache.
            char id3v1Header[3];
            //Check whether there's ID3v1 tag in the music file.
            musicDataStream.skipRawData(musicFile.size()-128);
            //Read ID3v1 header.
            musicDataStream.readRawData(id3v1Header, 3);
            //Check the header, and we can know that whether there's ID3v1
            //header.
            hasId3v1=(id3v1Header[0]=='T' &&
                        id3v1Header[1]=='A' &&
                          id3v1Header[2]=='G');
        }
        //Check the beginning of the file.
        if(checkHeader(0,
                       musicDataStream,
                       header))
        {
            //Set the tag data start.
            tagDataStart=0;
            //Set the tag length to header length.
            tagDataLength=header.size;
            //Check whether is a footer in the tag.
            if(header.size > APEv2HeaderSize)
            {
                //Check the footer.
                APEHeader footer;
                //Tried to parse the footer.
                if(checkHeader(header.size,
                               musicDataStream,
                               footer))
                {
                    //Update the tag length.
                    tagDataLength+=APEv2HeaderSize;
                }
            }
            //Reset the device to start.
            musicDataStream.device()->reset();
            //Skip the file data.
            musicDataStream.skipRawData(APEv2HeaderSize);
        }
        //Check the end of the file.
        else if(checkHeader(musicFile.size()-APEv2HeaderSize,
                            musicDataStream,
                            header))
        {
            //Save the tag start data.
            int tagContentStart=musicFile.size()-header.size;
            //Check the footer.
            APEHeader footer;
            //Check whether there's a header in the tag.
            if(checkHeader(tagContentStart-APEv2HeaderSize,
                           musicDataStream,
                           footer))
            {
                //Save the tag data start position as the header start position.
                //This is APEv2 tag.
                tagDataStart=tagContentStart-APEv2HeaderSize;
                //The tag length will be a header size + tag size.
                tagDataLength=APEv2HeaderSize + header.size;
            }
            else
            {
                //This is APEv1 tag.
                tagDataStart=tagContentStart;
                //The tag length will be tag size.
                tagDataLength=header.size;
            }
            //Reset the device to start.
            musicDataStream.device()->reset();
            //Skip the file data.
            musicDataStream.skipRawData(tagContentStart);
        }
        //Check the position before ID3v1. Some file may have both ID3v1 and
        //APEv1/APEv2.
        else if(musicFile.size()>=(ID3v1nAPEv2 + APEv2HeaderSize) &&
                                                            //File size first.
                checkHeader(musicFile.size()-ID3v1nAPEv2,
                            musicDataStream,
                            header))
        {
            //Save the tag start position.
            int tagContentStart=musicFile.size()-ID3v1Size-header.size;
            //Check the footer.
            APEHeader footer;
            //Check whether there's a header in the tag.
            if(checkHeader(tagContentStart-APEv2HeaderSize,
                           musicDataStream,
                           footer))
            {
                //Save the tag data start position as the header start position.
                //This is APEv2 tag.
                tagDataStart=tagContentStart-APEv2HeaderSize;
                //The tag length will be a header size + tag size.
                tagDataLength=APEv2HeaderSize + header.size;
            }
            else
            {
                //This is APEv1 tag.
                tagDataStart=tagContentStart;
                //The tag length will be tag size.
                tagDataLength=header.size;
            }
            //Reset the device to start.
            musicDataStream.device()->reset();
            //Skip the file data.
            musicDataStream.skipRawData(tagContentStart);
        }
        //Parse the data if we find the header.
        if(tagDataStart!=-1)
        {
            //Read the tag from the file.
            char *rawTagData=new char[header.size];
            musicDataStream.readRawData(rawTagData, header.size);
            //Parse the raw tag data list.
            parseRawData(rawTagData, header, itemList);
            //Recover the memory.
            delete[] rawTagData;
        }
        //Close the music file.
        musicFile.close();
    }
    //Add all the text labels to detail frames if it's not empty.
    for(int i=0; i<MusicDataCount; i++)
    {
        //Check if the text is empty.
        if(detailInfo.textLists[i].toString().isEmpty())
        {
            continue;
        }
        //Get the frame key from hash group.
        QString key=m_indexKey.value(i, QString());
        //If the key is empty, means you cannot write this data to APEv2.
        if(key.isEmpty())
        {
            continue;
        }
        //Generate a data frame.
        APETagItem item;
        //Save the key.
        item.key=key;
        //According to the frame, generate the item.
        switch(i)
        {
        case DiscNumber:
            //If disc count isn't empty, then add disc count to disc number
            //data.
            item.value=detailInfo.textLists[DiscCount].toString().isEmpty()?
                        detailInfo.textLists[DiscNumber].toString().toUtf8():
                        (detailInfo.textLists[DiscNumber].toString()+"/"+
                         detailInfo.textLists[DiscCount].toString()).toUtf8();
        default:
            //Add the whole data to the item.
            item.value=detailInfo.textLists[i].toString().toUtf8();
        }
        //Remove the all the original item.
        //We have to check the key from the back to the first, and we won't get
        //mess to the index.
        for(int i=itemList.size()-1; i>-1; i--)
        {
            //If the key is the same as current key,
            if(itemList.at(i).key==key)
            {
                //Remove it.
                itemList.removeAt(i);
            }
        }
        //Add current key to item list.
        itemList.append(item);
    }
    //Now translate the frame structure data to the raw data.
    QByteArray contentData;
    //Prepare the cache size.
    char numberCache[4];
    //Simply transfered the APETagItem to content data.
    for(auto i=itemList.constBegin(); i!=itemList.constEnd(); ++i)
    {
        //Get the item size.
        quint32 size=(*i).value.size();
        //First transfer item size to raw data into cache.
        numberToData(size, numberCache);
        //Add item size to content data.
        contentData.append(numberCache, 4);
        //Transfer the flag to raw data into cache.
        numberToData((*i).flag, numberCache);
        //Add flag data to content data.
        contentData.append(numberCache, 4);
        //Add item key to content data.
        contentData.append((*i).key.toUtf8());
        //Add 0x00 for item key terminator.
        contentData.append('\0');
        //Add item value.
        contentData.append((*i).value);
    }

    //Update the header data.
    header.size=contentData.size()+32;
    header.itemCount=itemList.size();

    //We will write the APEv2 data to the end part of the file. Just before the
    //ID3v1.
    //Check the header data.
    //Open the music file again.
    if(!musicFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Generate a temporary file, write the new data to the temporary file.
    QTemporaryFile updatedTagFile;
    //Open the temporary file, if we cannot open the temporary file it will be
    //failed to write the tag.
    if(!updatedTagFile.open())
    {
        //Close the opened music file.
        musicFile.close();
        return false;
    }
    //Initial the file size.
    qint64 dataSurplusSize=musicFile.size();
    /*
     * Algorithm:
     * We treat the file as these two kinds of format:
     *          APEv2 | xxxx (Content) (| ID3v1)
     * or
     *          xxxx (Content) | APEv2 (| ID3v1)
     * So we have to process the ID3v1 at first. Then the file should be like
     * the following:
     *                 APEv2 | xxxx (Content)
     * or
     *                 xxxx (Content) | APEv2
     * And now, we only have to check if the APEv2 is at the beginning or the
     * end of the content.
     */
    //If we have ID3v1, then remove the tag from the copying bytes.
    if(hasId3v1)
    {
        //Reduce the ID3v1 tag size.
        dataSurplusSize-=128;
    }
    //Check whether we have original APEv2 header.
    if(tagDataStart!=-1)
    {
        //Reduce the tag size from the file size.
        dataSurplusSize-=tagDataLength;
        //Check whether the header is at the start of the music file.
        if(tagDataStart==0)
        {
            //It's at the beginning of the file.
            //Skip the Original APEv2 tag at the beginning of the file.
            musicFile.seek(tagDataLength);
        }
    }
    //Generate the music data cache.
    char fileCache[DataCacheSize];
    int bytesRead;
    //Now copy all the content from the original file to temporary file.
    while(dataSurplusSize>0)
    {
        //Read the original data.
        bytesRead=musicFile.read(fileCache,
                                 (DataCacheSize < dataSurplusSize ?
                                      DataCacheSize : dataSurplusSize));
        //Write the cache to temporary file.
        updatedTagFile.write(fileCache, bytesRead);
        //Reduce the surplus size.
        dataSurplusSize-=bytesRead;
    }
    //According to the hydrogenaud.io, we have to put the APEv2 at the end of
    //the file.
    //Write new APEv2 tag to the file.
    /*
     * From http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags:
     * Bit 29:
     * 0: This is the footer, not the header
     * 1: This is the header, not the footer
     */
    //First set the item flag to header in header data bytearray.
    updatedTagFile.write(generateHeaderData(header, true));
    //Then, write the content data.
    updatedTagFile.write(contentData);
    //Last, write the footer data.
    updatedTagFile.write(generateHeaderData(header, false));
    //If there's ID3v1 tag, then copy the ID3v1 data from the original file.
    if(hasId3v1)
    {
        //Seek to the ID3v1 tag start.
        musicFile.seek(musicFile.size()-128);
        //Read 128 bytes ID3v1 tag.
        musicFile.read(fileCache, 128);
        //Write the cache to temporary file.
        updatedTagFile.write(fileCache, 128);
    }
    //Close the music file.
    musicFile.close();
    //Reset the temporary file.
    updatedTagFile.reset();
    //Reopen the music file as write only mode, write all the udpated tag file
    //data to the music file.
    if(!musicFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    //Copy data from temporary file to music file.
    bytesRead=updatedTagFile.read(fileCache, DataCacheSize);
    while(bytesRead>0)
    {
        //Write the cache to music file.
        musicFile.write(fileCache, bytesRead);
        //Read new data from the original file to cache.
        bytesRead=updatedTagFile.read(fileCache, DataCacheSize);
    }
    //Close the music file and temporary file.
    musicFile.close();
    updatedTagFile.close();
    //The tag rewrite is finished.
    return true;
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
    if(memcmp(rawData, m_apePreamble, 8)==0)
    {
        //Save the information of the header.
        header.version=dataToNumber(rawData+8);
        header.size=dataToNumber(rawData+12);
        header.itemCount=dataToNumber(rawData+16);
        header.flags=dataToNumber(rawData+20);
        //Parse successfully.
        return true;
    }
    return false;
}

inline void KNMusicTagApev2::parseRawData(char *rawData,
                                          APEHeader &header,
                                          QList<APETagItem> &tagList)
{
    //The struct of the APEv2 frame is:
    /*
     * ----
     * Size of the Item Value, Bits 0...7
     * Size of the Item Value, Bits 8...15
     * Size of the Item Value, Bits 16...23
     * Size of the Item Value, Bits 24...31
     * ----
     * Item Flags, Bits 0...7
     * Item Flags, Bits 8...15
     * Item Flags, Bits 16...23
     * Item Flags, Bits 24...31
     * ----
     * Item Key: Can contain ASCII characters from 0x20 (Space) up to 0x7E
     * (Tilde).
     * ----
     * 0x00: Item key terminator.
     * ----
     * Item Value: can be binary data or UTF-8 string
     */
    //Backup size of the content and item count.
    quint32 sizeSurplus=header.size,
            itemSurplus=header.itemCount;
    //Process until no item or no surplus content.
    while(sizeSurplus>0 && itemSurplus>0)
    {
        //Calculate current item size.
        quint32 frameSize=dataToNumber(rawData);
        //Check whether the frame size is overflow.
        if(frameSize>sizeSurplus)
        {
            break;
        }
        //Generate item according to the frame size.
        APETagItem item;
        //Copy the flag to the item.
        item.flag=dataToNumber(rawData+4);
        //Here is a magic:
        //QString can automatically terminate at the 0x00, so we don't need to
        //find 0x00 byte manually. Just use the key length to find the 0x00.
        item.key=QString(rawData+8).toUpper();
        //And now the value part will be the surplus data of the item.
        item.value=QByteArray(rawData+9+item.key.size(), frameSize);
        //Add the frame to list.
        tagList.append(item);

        //Add the item key size and the flag size to the frame size.
        frameSize+=(item.key.size()+9);
        //Move the raw data pointer, reduce the size and item surplus.
        rawData+=frameSize;
        sizeSurplus-=frameSize;
        itemSurplus--;
    }
}

inline QByteArray KNMusicTagApev2::generateHeaderData(const APEHeader &header,
                                                      bool isHeader)
{
    //Prepare the cache size.
    char numberCache[4];
    //Generate the header.
    QByteArray headerData;
    //--Preamble--
    //Set the preamble first.
    headerData.append(m_apePreamble);
    //--Version--
    //Add the version. Fixed APEv2(2.000).
    headerData.append((char)0x20);
    headerData.append((char)0x00);
    headerData.append((char)0x00);
    headerData.append((char)0x00);
    //--Tag Size--
    //Set tag size to cache data.
    numberToData(header.size, numberCache);
    //Add the tag size to header data.
    headerData.append(numberCache, 4);
    //--Item Count--
    //Set the number of items to cache data.
    numberToData(header.itemCount, numberCache);
    //Add the item size to header data.
    headerData.append(numberCache, 4);
    //--Tags Flags--
    //Update the header's flags.
    quint32 flag=header.flags;
    //Set flag to 'Tag contains a header'.
    flag &= 0xFFFFFFFE;
    //Set flag to 'Tag contains a footer'.
    flag |= 0x00000002;
    //Check if it's header.
    if(isHeader)
    {
        //Set the flag to 'This is the header, not the footer'.
        flag |= 0x00000004;
    }
    else
    {
        //Set the flag to 'This is the footer, not the header'.
        flag &= 0xFFFFFFFB;
    }
    //Set the flags to cache data.
    numberToData(flag, numberCache);
    //Add the tag flags to header data.
    headerData.append(numberCache, 4);
    //--Reserved data--
    headerData.append(QByteArray(8, '\0'));
    //Give back the header data.
    return headerData;
}
