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

#include "knmusictagm4a.h"

//1MB music data cache copy size.
#define DataCacheSize 1048576

QHash<QString, int> KNMusicTagM4a::m_atomIndexMap=QHash<QString, int>();
QHash<int, QString> KNMusicTagM4a::m_indexAtomMap=QHash<int, QString>();
QHash<int, quint8> KNMusicTagM4a::m_indexFlagMap=QHash<int, quint8>();

KNMusicTagM4a::KNMusicTagM4a(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Check the atom map has been initialized.
    if(m_atomIndexMap.isEmpty())
    {
        //Initial metadata atoms.
        char combineText[5]={0};
        combineText[0]=(char)0xA9;
        combineText[1]='a';combineText[2]='l';combineText[3]='b';
        m_atomIndexMap.insert(combineText, Album);
        m_indexAtomMap.insert(Album, combineText);
        m_indexFlagMap.insert(Album, 1);
        combineText[1]='A';combineText[2]='R';combineText[3]='T';
        m_atomIndexMap.insert(combineText, Artist);
        m_indexAtomMap.insert(Artist, combineText);
        m_indexFlagMap.insert(Artist, 1);
        combineText[1]='c';combineText[2]='m';combineText[3]='t';
        m_atomIndexMap.insert(combineText, Comments);
        m_indexAtomMap.insert(Comments, combineText);
        m_indexFlagMap.insert(Comments, 1);
        combineText[1]='d';combineText[2]='a';combineText[3]='y';
        m_atomIndexMap.insert(combineText, Year);
        m_indexAtomMap.insert(Year, combineText);
        m_indexFlagMap.insert(Year, 1);
        combineText[1]='n';combineText[2]='a';combineText[3]='m';
        m_atomIndexMap.insert(combineText, Name);
        m_indexAtomMap.insert(Name, combineText);
        m_indexFlagMap.insert(Name, 1);
        combineText[1]='g';combineText[2]='e';combineText[3]='n';
        m_atomIndexMap.insert(combineText, Genre);
        m_indexAtomMap.insert(Genre, combineText);
        m_indexFlagMap.insert(Genre, 1);
        combineText[1]='w';combineText[2]='r';combineText[3]='t';
        m_atomIndexMap.insert(combineText, Composer);
        m_indexAtomMap.insert(Composer, combineText);
        m_indexFlagMap.insert(Composer, 1);

        m_atomIndexMap.insert("aART", AlbumArtist);
        m_atomIndexMap.insert("gnre", Genre);
        m_atomIndexMap.insert("trkn", TrackNumber);
        m_atomIndexMap.insert("disk", DiscNumber);
        m_atomIndexMap.insert("tmpo", BeatsPerMinuate);
        m_atomIndexMap.insert("rtng", Rating);
        m_atomIndexMap.insert("catg", Category);
        m_atomIndexMap.insert("desc", Description);

        m_indexAtomMap.insert(AlbumArtist    , "aART");
        m_indexAtomMap.insert(TrackNumber    , "trkn");
        m_indexAtomMap.insert(DiscNumber     , "disk");
        m_indexAtomMap.insert(BeatsPerMinuate, "tmpo");
        m_indexAtomMap.insert(Rating         , "rtng");
        m_indexAtomMap.insert(Category       , "catg");
        m_indexAtomMap.insert(Description    , "desc");

        m_indexFlagMap.insert(AlbumArtist    , 1);
        m_indexFlagMap.insert(TrackNumber    , 0);
        m_indexFlagMap.insert(DiscNumber     , 0);
        m_indexFlagMap.insert(BeatsPerMinuate, 21);
        m_indexFlagMap.insert(Rating         , 21);
        m_indexFlagMap.insert(Category       , 1);
        m_indexFlagMap.insert(Description    , 1);
    }
}

QString KNMusicTagM4a::tagParserName()
{
    return "m4a";
}

bool KNMusicTagM4a::parseTag(QFile &musicFile,
                              QDataStream &musicDataStream,
                              KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(musicFile)
    //Some comments is from:
    //      http://atomicparsley.sourceforge.net/mpeg-4files.html
    //The m4a file is made of a number of atoms, now they are called 'boxes'.
    //   A box always begins with 4 bytes length and follows 4 bytes name.
    //And first we need to check the header box of the file. Its name is 'ftyp'
    //Check out the first box is ftyp box.
    {
        //Generate an empty box for ftyp box.
        M4ABox ftypBox;
        //Read the data.
        if(!getBox(musicDataStream, ftypBox, true) ||
                ftypBox.name!="ftyp")
        {
            //This cannot be a m4a file.
            return false;
        }
    }
    //Now it should be a m4a file.
    //So, although the following might failed to find a tag, but it will still
    //return true, cuz the it's m4a format.
    QHash<QString, QByteArray> boxList;
    {
        //Metadata to be used with iTunes comes in the ilst box inside the moov
        //box.
        //The structure of the ilst is:
        /* moov         <-
         * |-udta
         * | |-meta
         * | | |-ilst
         */
        //We have to keep reading until we find out the moov box.
        M4ABox moovBox;
        while(moovBox.name!="moov")
        {
            //If we cannot find a box, means there's no "moov" box, complete.
            if(!getBox(musicDataStream, moovBox))
            {
                return true;
            }
        }
        //When we comes to here, we should find the "moov" box, expand the box
        //to find out the "udta" box.
        /* moov
         * |-udta       <-
         * | |-meta
         * | | |-ilst
         */
        parseBox(moovBox, boxList);
        //Check the expand list, find the udta box inside the box list.
        if(!boxList.contains("udta"))
        {
            //There's no udta box in the moov box, then we are faild to parse.
            return true;
        }
        //Generate a empty box for "udta" box.
        M4ABox udtaBox;
        //Set the udta box data.
        udtaBox.data=boxList.value("udta");
        //Expand the "udta" box, and find "meta" box.
        /* moov
         * |-udta
         * | |-meta     <-
         * | | |-ilst
         */
        boxList.clear();
        parseBox(udtaBox, boxList);
        //Check the expand list.
        if(!boxList.contains("meta"))
        {
            //There's no meta box in the meta box, then we are failed to parse.
            return true;
        }
        //Generate a empty box for "meta" box.
        M4ABox metaBox;
        //Set the meta box data.
        metaBox.data=boxList.value("meta");
        //Okay, now we can parse the meta box.
        /* moov
         * |-udta
         * | |-meta
         * | | |-ilst   <-
         */
        //Generate a box for ilst.
        M4ABox ilstBox;
        //Extract the meta box to find the ilst box.
        if(!findIlstBox(metaBox.data, ilstBox))
        {
            //Finished to parse the tag if we cannot file the ilst box.
            return true;
        }
        //Okay we are now find out the ilst box. Extract the ilst box and we can
        //now fill our data to the detail info.
        boxList.clear();
        extractBox(ilstBox, boxList);
        //Check out the box list.
        if(boxList.isEmpty())
        {
            //If there's no data inside the expand list, then our mission is
            //finished.
            return true;
        }
    }
    //Write the expand list to the detail info.
    //Get the detail info.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Check all box inside the list.
    for(auto i=boxList.begin(); i!=boxList.end(); ++i)
    {
        //Check the name of the box.
        //If it's "covr", means we find out the album art box.
        if(i.key()=="covr")
        {
            analysisItem.imageData["M4A"].append(i.value());
            //Continue to next box.
            continue;
        }
        //Check the index of the box inside the map.
        int atomIndex=m_atomIndexMap.value(i.key(), -1);
        //If the atom index is -1, then we have to continue to the next box.
        //Check the data size.
        if(atomIndex==-1 || i.value().size()<16)
        {
            continue;
        }
        //Get the data position.
        //Actually there's another box inside the box of the i.
        //We can just skip it to read the data.
        char *dataPosition=i.value().data()+16;
        int dataSize=i.value().size()-16;
        //Output box data to detail info.
        switch(atomIndex)
        {
        case TrackNumber:
            //Ensure the data is enough to set the track number and track count.
            if(dataSize>6)
            {
                //Pick up the third byte data of the position as the track
                //number.
                detailInfo.textLists[TrackNumber]=
                        QString::number(dataPosition[3]);
                //Pick up the fifth byte data of the position as the track
                //count.
                detailInfo.textLists[TrackCount]=
                        QString::number(dataPosition[5]);
            }
            break;
        case DiscNumber:
            //Ensure the data is enough to set the disc number and disc count.
            if(dataSize>6)
            {
                //Pick up the third byte data of the position as the disc
                //number.
                detailInfo.textLists[DiscNumber]=
                        QString::number(dataPosition[3]);
                //Pick up the fifth byte data of the position as the disc
                //count.
                detailInfo.textLists[DiscCount]=
                        QString::number(dataPosition[5]);
            }
            break;
        case Rating:
            if(dataSize>0)
            {
                //Turn the first byte into the rating data.
                detailInfo.textLists[Rating]=
                        QString::number((quint8)dataPosition[0]);
            }
            break;
        default:
            //Check out the data size first.
            if(dataSize>0)
            {
                //Set the whole data as the text data.
                setTextData(detailInfo.textLists[atomIndex],
                            QByteArray(dataPosition, dataSize));
            }
            break;
        }
    }
    //Finished.
    return true;
}

bool KNMusicTagM4a::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(analysisItem)
    //Do not support write M4A yet.
    return false;
}

bool KNMusicTagM4a::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //Check flag.
    if(!analysisItem.imageData.contains("M4A") ||
            analysisItem.imageData.value("M4A").isEmpty())
    {
        return false;
    }
    //Generate the covr box for the album art data.
    M4ABox covrBox;
    //Get the image data from the frame.
    covrBox.data=analysisItem.imageData.value("M4A").at(0);
    //Generate a hash list for the covr box.
    QHash<QString, QByteArray> expandList;
    //Expand the covr box.
    parseBox(covrBox, expandList);
    //Check out the expand list.
    if(expandList.isEmpty())
    {
        //We cannot parse any data from the expand list.
        //Finished the parsing.
        return true;
    }
    //There's 8 bytes version and flags in front of the content box.
    analysisItem.coverImage.loadFromData(expandList.begin().value().mid(8));
    //Mission complete.
    return true;
}

bool KNMusicTagM4a::writable() const
{
    return false;
}

bool KNMusicTagM4a::writeCoverImage() const
{
    return false;
}

inline bool KNMusicTagM4a::getBox(QDataStream &musicDataStream,
                                   M4ABox &box,
                                   bool ignoreContent)
{
    //Clear the box data.
    box.data.clear();
    box.name.clear();
    //Get the size of the box.
    quint32 boxSize=0;
    //Get the size of the box, reduce the 8 bytes size of itself and the name.
    musicDataStream>>boxSize;
    boxSize-=8;
    //Generate the name field.
    char nameField[5]={0};
    //Get the name of the box.
    if(musicDataStream.readRawData(nameField, 4)==-1)
    {
        //If you cannot read the data, then it's failed to read the box.
        return false;
    }
    //Save the box name.
    box.name=nameField;
    //Get the content, or else we will simply get back.
    if(ignoreContent)
    {
        //Skip the box size data.
        return musicDataStream.skipRawData(boxSize);
    }
    //Allocate memory to store the box data.
    char *boxData=new char[boxSize];
    //Read the new data.
    int readSize=musicDataStream.readRawData(boxData, boxSize);
    //Check out the size.
    if(readSize<0)
    {
        //Clear the box data.
        delete[] boxData;
        //Failed to read the data.
        return false;
    }
    //Copy the data to the byte array.
    box.data=QByteArray(boxData, readSize);
    //Clear the box data.
    delete[] boxData;
    //Mission complete.
    return true;
}

inline bool KNMusicTagM4a::parseBox(const M4ABox &source,
                                     QHash<QString, QByteArray> &boxes)
{
    //Simply give out the size and the data of the source box, and we can parse
    //the normal box.
    return parseData(source.data.size(),
                     (char *)source.data.data(),
                     boxes);
}

inline bool KNMusicTagM4a::parseData(quint32 sourceSize,
                                      char *dataPosition,
                                      QHash<QString, QByteArray> &boxes)
{
    //Read until all the data has been all read.
    while(sourceSize > 0)
    {
        //Read the size of the inner box.
        quint32 preferSize=KNMusicUtil::charToInt32(dataPosition);
        //Check source size and prefer size.
        if(preferSize<8 || preferSize > sourceSize)
        {
            //There should be bad data mix in, we will stop to read the data.
            return false;
        }
        //Insert the box to the hash boxes.
        //Use fromRawData() to avoid the deep copy.
        boxes.insert(QByteArray(dataPosition+4, 4),
                     QByteArray::fromRawData(dataPosition+8, preferSize-8));
        //Reduce the source size and move the position.
        sourceSize-=preferSize;
        dataPosition+=preferSize;
    }
    //If all the data has been parsed to box, then extract complete.
    return true;
}

inline bool KNMusicTagM4a::extractBox(const M4ABox &source,
                                       QHash<QString, QByteArray> &boxes)
{
    //Simply give out the size and the data of the source box, and we can
    //extract the normal box.
    return extractData(source.data.size(),
                       (char *)source.data.data(),
                       boxes);
}

inline bool KNMusicTagM4a::extractData(quint32 sourceSize,
                                        char *dataPosition,
                                        QHash<QString, QByteArray> &boxes)
{
    //Read until all the data has been all read.
    while(sourceSize > 0)
    {
        //Read the size of the inner box.
        quint32 preferSize=KNMusicUtil::charToInt32(dataPosition);
        //Check source size and prefer size.
        if(preferSize<8 || preferSize > sourceSize)
        {
            //There should be bad data mix in, we will stop to read the data.
            return false;
        }
        //Insert the box to the hash boxes.
        boxes.insert(QByteArray(dataPosition+4, 4),
                     QByteArray(dataPosition+8, preferSize-8));
        //Reduce the source size and move the position.
        sourceSize-=preferSize;
        dataPosition+=preferSize;
    }
    //If all the data has been parsed to box, then extract complete.
    return true;
}

inline bool KNMusicTagM4a::findIlstBox(const QByteArray &metaBox,
                                        M4ABox &ilstBox)
{
    //In the "meta" box, the first 4 bytes is a mystery version. In the
    //document, it says '1 byte atom version (0x00) & 3 bytes atom flags
    //(0x000000)'. So, I have no idea of these flags. I can only ignore it.
    //The data pointer will be start right after those 4 bytes.
    char *data=(char *)metaBox.data()+4;
    //And now, we need to find the "ilst" box in "meta" box.
    quint32 sourceSize=metaBox.size()-4;
    //Check if we have read all the source data.
    while(sourceSize > 0)
    {
        //Get the box size and the name of the box.
        quint32 boxSize=KNMusicUtil::charToInt32(data);
        //Get the box name.
        //If current box is "ilst", then mission complete.
        if(QByteArray(data+4, 4)=="ilst")
        {
            //Copy the data to the ilst box byte array.
            ilstBox.data=QByteArray::fromRawData(data+8, boxSize-8);
            //Mission complete.
            return true;
        }
        //Move the pointer to the next position.
        data+=boxSize;
        //Reduce the surplus data size.
        sourceSize-=boxSize;
    }
    //Failed to find ilst.
    return false;
}
