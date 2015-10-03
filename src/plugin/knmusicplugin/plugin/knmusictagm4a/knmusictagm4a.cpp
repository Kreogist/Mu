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
#include <QBuffer>

#include "knmusictagm4a.h"

#include <QDebug>

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
    //Check out the data.
    M4ABox ftypBox;
    if(!getBox(musicDataStream, ftypBox, true) ||
            ftypBox.name!="ftyp")
    {
        //This cannot be a m4a file.
        return false;
    }
    //Metadata to be used with iTunes comes in the ilst box inside the moov box.
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
        //If we cannot find a box, means there's no "moov" box, return false.
        if(!getBox(musicDataStream, moovBox))
        {
            return true;
        }
    }
    //When we comes to here, we should find the "moov" box, expand the box to
    //find out the "udta" box.
    /* moov
     * |-udta       <-
     * | |-meta
     * | | |-ilst
     */
    QList<M4ABox> expandList;
    extractBox(moovBox, expandList);
    //Check the expand list.
    if(expandList.isEmpty())
    {
        return true;
    }
    //Generate a empty box for "udta" box.
    M4ABox udtaBox;
    //Find all the box of the expand list of the "moov" box.
    for(auto i : expandList)
    {
        //Check the name of each box.
        if(i.name=="udta")
        {
            //Save the udta box.
            udtaBox=i;
            //Make the udta box to independent box.
            independent(udtaBox);
            //Clear the moov box.
            clearBox(moovBox);
            break;
        }
    }
    //Check out the data.
    if(udtaBox.name.isEmpty())
    {
        //If the name of the udta box is still empty, means there's no udta box
        //in the moov box, then we are faild to parse.
        return true;
    }
    //Expand the "udta" box, and find "meta" box.
    /* moov
     * |-udta
     * | |-meta     <-
     * | | |-ilst
     */
    expandList.clear();
    extractBox(udtaBox, expandList);
    //Check the expand list.
    if(expandList.isEmpty())
    {
        return true;
    }
    //Generate a empty box for "meta" box.
    M4ABox metaBox;
    //Find all the box of the expand list of the "meta" box.
    for(auto i : expandList)
    {
        //Check the name of each box.
        if(i.name=="meta")
        {
            //Save the udta box.
            metaBox=i;
            //Make the meta box to independent box.
            independent(metaBox);
            //Clear the udta box.
            clearBox(udtaBox);
            break;
        }
    }
    //Check out the box.
    if(metaBox.name.isEmpty())
    {
        //If the name of the meta box is still empty, means we cannot find meta
        //box in the meta box, then we are finished to parse.
        return true;
    }
    //Okay, now we can parse the meta box.
    /* moov
     * |-udta
     * | |-meta
     * | | |-ilst   <-
     */
    //Generate a box for ilst.
    M4ABox ilstBox;
    //Extract the meta box.
    if(!findIlstBox(metaBox, ilstBox))
    {
        //Finished to parse the tag if we cannot file the ilst box.
        return true;
    }
    //Clear the meta box to recover the memory.
    clearBox(metaBox);
    //Okay we are now find out the ilst box. Extract the ilst box and we can now
    //fill our data to the detail info.
    expandList.clear();
    extractBox(ilstBox, expandList);
    //Check out the expand list.
    if(expandList.isEmpty())
    {
        //If there's no data inside the expand list, then our mission is
        //finished.
        return true;
    }
    //Write the expand list to the detail info.
    //Get the detail info.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Check all box inside the list.
    for(auto i : expandList)
    {
        //Check the name of the box.
        //If it's "covr", means we find out the album art box.
        if(i.name=="covr")
        {
            analysisItem.imageData["M4A"].append(QByteArray(i.data, i.size));
            //Continue to next box.
            continue;
        }
        //Check the index of the box inside the map.
        int atomIndex=m_atomIndexMap.value(i.name, -1);
        //If the atom index is -1, then we have to continue to the next box.
        if(atomIndex==-1)
        {
            continue;
        }
        //Check the data size.
        if(i.size<16)
        {
            continue;
        }
        //Get the data position.
        //Actually there's another box inside the box of the i.
        //We can just skip it to read the data.
        char *dataPosition=i.data+16;
        int dataSize=i.size-16;
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
    return true;
}

bool KNMusicTagM4a::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Prepare and get the music file.
    QFile musicFile(detailInfo.filePath);
    //Open the file as read only mode.
    if(!musicFile.open(QIODevice::ReadOnly))
    {
        //Failed to open the source.
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
    //Generate a data stream for music file.
    QDataStream musicDataStream(&musicFile);
    //Read and copy the fytp box. If the first box isn't fytp box, then ignore
    //the file.
    M4ABox ftypBox;
    if(!getBox(musicDataStream, ftypBox) || ftypBox.name!="ftyp")
    {
        //Close both file.
        musicFile.close();
        updatedTagFile.close();
        //Failed to find a m4a file, return false.
        return false;
    }
    //Write ftyp data to the temporary file.
    writeBox(ftypBox, updatedTagFile);
    //We have to keep reading until we find out the moov box.
    //Output all the other data to updated tag file.
    M4ABox moovBox;
    for(;;)
    {
        //If we can get a new box.
        if(getBox(musicDataStream, moovBox))
        {
            //Check out the name.
            if(moovBox.name=="moov")
            {
                break;
            }
            else
            {
                //Copy the data.
                writeBox(moovBox, updatedTagFile);
            }
        }
        else
        {
            //If we cannot find a box, means there's no "moov" box, failed to
            //write data.
            //Close both file.
            musicFile.close();
            updatedTagFile.close();
            //Failed to find a m4a file, return false.
            return false;
        }
    }
    //When we comes to here, we should find the "moov" box, expand the box to
    //find out the "udta" box.
    QList<M4ABox> moovExpandList;
    extractBox(moovBox, moovExpandList);
    //Generate a empty box for "udta" box.
    M4ABox udtaBox;
    //Check the expand list.
    if(moovExpandList.isEmpty() ||
            !findBox("udta", udtaBox, moovExpandList))
    {
        //If the name of the udta box is still empty, means there's no udta box
        //in the moov box, then we are faild to parse.
        //Close both file.
        musicFile.close();
        updatedTagFile.close();
        //Failed to find a m4a file, return false.
        return false;
    }
    //Expand the "udta" box, and find "meta" box.
    QList<M4ABox> udtaExpandList;
    extractBox(udtaBox, udtaExpandList);
    //Generate a empty box for "meta" box.
    M4ABox metaBox;
    //Check the expand list and find box.
    if(udtaExpandList.isEmpty() ||
            !findBox("meta", metaBox, udtaExpandList))
    {
        //If the name of the meta box is still empty, means we cannot find meta
        //box in the meta box, then we are finished to parse.
        //Close both file.
        musicFile.close();
        updatedTagFile.close();
        //Failed to find a m4a file, return false.
        return false;
    }
    //Okay, now we can parse the meta box.
    //Generate a box for ilst.
    M4ABox ilstBox;
    QList<M4ABox> metaExpandList;
    //Extract the meta box.
    extractMetaBox(metaBox, metaExpandList);
    //Find all box of the expand list.
    if(metaExpandList.isEmpty() ||
            !findBox("ilst", ilstBox, metaExpandList))
    {
        //We cannot find ilst box in the meta box.
        //Close both file.
        musicFile.close();
        updatedTagFile.close();
        //Failed to find a m4a file, return false.
        return false;
    }
    //Prepare the ilst expand list.
    QList<M4ABox> ilstExpandList;
    //Expand the ilst box.
    extractBox(ilstBox, ilstExpandList);

    //Now we have to write data to ilst expand list.
    for(int i=0; i<MusicDataCount; ++i)
    {
        //Get the atom name of current data.
        QString atomName=m_indexAtomMap.value(i, QString());
        //Check if the atom name is empty, then go to the next.
        if(atomName.isEmpty())
        {
            continue;
        }
        //Remove the exist data inside the ilst expand list.
        for(int j=ilstExpandList.size()-1; j>-1; --j)
        {
            //Check the name of the item.
            if(ilstExpandList.at(j).name==atomName)
            {
                //Remove it.
                ilstExpandList.removeAt(j);
            }
        }
        //Generate the raw data.
        QByteArray rawData;
        //Write the data to raw data.
        switch(i)
        {
        case TrackNumber:
            //Append three 0x00 first.
            rawData.append((char)0x00);
            rawData.append((char)0x00);
            rawData.append((char)0x00);
            //Append the track index.
            rawData.append((char)detailInfo.textLists[TrackNumber].toString()
                           .toInt());
            //Append splitter 0x00.
            rawData.append((char)0x00);
            //Append the track count.
            rawData.append((char)detailInfo.textLists[TrackCount].toString()
                           .toInt());
            //Append two 0x00 after.
            rawData.append((char)0x00);
            rawData.append((char)0x00);
            break;
        case DiscNumber:
            //Append three 0x00 first.
            rawData.append((char)0x00);
            rawData.append((char)0x00);
            rawData.append((char)0x00);
            //Append the disc index.
            rawData.append((char)detailInfo.textLists[DiscNumber].toString()
                           .toInt());
            //Append splitter 0x00.
            rawData.append((char)0x00);
            //Append the disc count.
            rawData.append((char)detailInfo.textLists[DiscCount].toString()
                           .toInt());
            //Append two 0x00 after.
            rawData.append((char)0x00);
            rawData.append((char)0x00);
            break;
        case Rating:
            //Append the rating to bytes.
            rawData.append((char)detailInfo.textLists[Rating].toString()
                           .toInt());
            break;
        default:
            //Translate the text data to UTF-8, without BOM.
            rawData=detailInfo.textLists[i].toString().toUtf8();
        }
        //Generate the box.
        ilstExpandList.append(generateItemBox(i, atomName, rawData));
    }
    //Remove all the album art atom.
    for(int j=ilstExpandList.size()-1; j>-1; --j)
    {
        //Check the name of the item.
        if(ilstExpandList.at(j).name=="covr")
        {
            //Remove it.
            ilstExpandList.removeAt(j);
        }
    }
    //Check album art.
    if(!analysisItem.coverImage.isNull())
    {
        //Generate the raw data for the image.
        //Add the png raw data to image data.
        QByteArray imageData;
        QBuffer imageBuffer(&imageData);
        //Open the image buffer.
        imageBuffer.open(QIODevice::WriteOnly);
        //Save the data to image data.
        analysisItem.coverImage.save(&imageBuffer, "PNG");
        //Close the image buffer.
        imageBuffer.close();
        //Check the image data, if the data is not empty, then insert data.
        if(imageData.isEmpty())
        {
            //Generate the flag data.
            char covrFlag[5];
            covrFlag[0]=0x00;
            covrFlag[1]=0x00;
            covrFlag[2]=0x00;
            covrFlag[3]=14;
            //Generate item box, insert to list.
            ilstExpandList.append(generateItemBox(covrFlag, "covr", imageData));
        }
    }
    //Combine the ilst data together.
    M4ABox updatedIlstBox=zipBox("ilst", ilstExpandList);
    //Clear the list and original ilst box.
    ilstExpandList.clear();
    clearBox(ilstBox);
    //Replace the original ilst box.
    for(int i=metaExpandList.size()-1; i>-1; --i)
    {
        //Check the name.
        if(metaExpandList.at(i).name=="ilst")
        {
            //Replace the item.
            metaExpandList.replace(i, updatedIlstBox);
            //Stop searching.
            break;
        }
    }
    //Combine the meta expand list data.
    QByteArray metaRawData=combineBoxList(metaExpandList);
    //Clear up the meta expand list.
    metaExpandList.clear();
    //Append the first four bytes raw data to the meta box raw data.
    metaRawData.prepend(metaBox.data, 4);
    //Clear up the no use meta box.
    clearBox(metaBox);
    clearBox(updatedIlstBox);
    //Set the data to new meta box.
    metaBox.name="meta";
    metaBox.independence=true;
    metaBox.size=metaRawData.size();
    metaBox.data=new char[metaBox.size];
    memcpy(metaBox.data, metaRawData.data(), metaBox.size);
    //Replace the original meta box.
    for(int i=udtaExpandList.size()-1; i>-1; --i)
    {
        //Check the name.
        if(udtaExpandList.at(i).name=="meta")
        {
            //Replace the item.
            udtaExpandList.replace(i, metaBox);
            //Stop Searching.
            break;
        }
    }
    //Combine the udta data together.
    M4ABox updatedUdtaBox=zipBox("udta", udtaExpandList);
    //Clear the list and original udta box.
    udtaExpandList.clear();
    clearBox(udtaBox);
    //Replace the original udta box.
    for(int i=moovExpandList.size()-1; i>-1; --i)
    {
        //Check the name.
        if(moovExpandList.at(i).name=="udta")
        {
            //Replace the item
            moovExpandList.replace(i, updatedUdtaBox);
            //Stop searching.
            break;
        }
    }
    //Combine the moov data together.
    M4ABox updatedMoovBox=zipBox("moov", moovExpandList);
    //Clear the list and original moov box.
    moovExpandList.clear();
    clearBox(moovBox);
    //Write the new moov box to the updated tag file.
    writeBox(updatedMoovBox, updatedTagFile);
    //Clear up the updated moov box.
    clearBox(updatedMoovBox);
    //Copy the left data to the updated tag file.
    //Generate the music data cache.
    char *turboCache=new char[DataCacheSize];
    //Copy the music data from the original music file, copy the
    //MusicDataCacheSize bytes once, until there's no bytes to copy.
    int bytesRead=musicFile.read(turboCache, DataCacheSize);
    while(bytesRead>0)
    {
        //Write the cache to temporary file.
        updatedTagFile.write(turboCache, bytesRead);
        //Read new data from the original file to cache.
        bytesRead=musicFile.read(turboCache, DataCacheSize);
    }
    //Close the music file.
    musicFile.close();
    //Reset the temporary file.
    updatedTagFile.reset();
    //Reopen the music file as write only mode, write all the udpated tag file
    //data to the music file.
    if(!musicFile.open(QIODevice::WriteOnly))
    {
        //Close the updated tag file.
        updatedTagFile.close();
        //Failed to write data.
        return false;
    }
    //Copy data from temporary file to music file.
    bytesRead=updatedTagFile.read(turboCache, DataCacheSize);
    while(bytesRead>0)
    {
        //Write the cache to music file.
        musicFile.write(turboCache, bytesRead);
        //Read new data from cache to the original file.
        bytesRead=updatedTagFile.read(turboCache, DataCacheSize);
    }
    //Close the music file and temporary file.
    musicFile.close();
    updatedTagFile.close();
    //Clear up the turbo memory.
    delete[] turboCache;
    //The tag rewrite is finished.
    return true;
}

bool KNMusicTagM4a::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //Check flag.
    if(!analysisItem.imageData.contains("M4A") ||
            analysisItem.imageData.value("M4A").isEmpty())
    {
        return false;
    }
    //Get the image data from the frame.
    QByteArray boxData=analysisItem.imageData.value("M4A").at(0);
    //Generate the covr box for the album art data.
    M4ABox covrBox;
    //Configure the covr box.
    covrBox.size=boxData.size();
    covrBox.data=boxData.data();
    //Generate a expand list for the covr box.
    QList<M4ABox> expandList;
    //Expand the covr box.
    extractBox(covrBox, expandList);
    //Check out the expand list.
    if(expandList.isEmpty())
    {
        //We cannot parse any data from the expand list.
        //Finished the parsing.
        return true;
    }
    //There's 8 bytes version and flags in front of the content box.
    analysisItem.coverImage.loadFromData(QByteArray(expandList.first().data+8,
                                                    expandList.first().size-8));
    //Mission complete.
    return true;
}

bool KNMusicTagM4a::writable() const
{
    return true;
}

bool KNMusicTagM4a::writeCoverImage() const
{
    return true;
}

inline void KNMusicTagM4a::clearBox(KNMusicTagM4a::M4ABox &box)
{
    //Clear the box information.
    box.name.clear();
    box.size=0;
    //Check the data, if there's data in the box.
    if(box.data!=nullptr && box.independence)
    {
        //We have to delete the data to recover the memory.
        delete[] box.data;
    }
    //Reset the data pointer and the independence.
    box.data=nullptr;
    box.independence=false;
}

inline bool KNMusicTagM4a::getBox(QDataStream &musicDataStream,
                                  KNMusicTagM4a::M4ABox &box,
                                  bool ignoreContent)
{
    //Clear the box data.
    clearBox(box);
    //Set the box properties to independet box.
    box.independence=true;
    //Get the size of the box, reduce the 8 bytes size of itself and the name.
    musicDataStream>>box.size;
    box.size-=8;
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
        box.independence=false;
        //Skip the box size data.
        return musicDataStream.skipRawData(box.size);
    }
    //Allocate memory to store the box data.
    box.data=new char[box.size];
    //Read the new data.
    return musicDataStream.readRawData(box.data, box.size);
}

inline bool KNMusicTagM4a::extractBox(const KNMusicTagM4a::M4ABox &source,
                                      QList<KNMusicTagM4a::M4ABox> &boxes)
{
    //Simply give out the size and the data of the source box, and we can parse
    //the normal box.
    return extractData(source.size, source.data, boxes);
}

inline void KNMusicTagM4a::independent(KNMusicTagM4a::M4ABox &box)
{
    //Check the original status of the source box.
    if(box.independence)
    {
        //We will do nothing if a box is independent already.
        return;
    }
    //Set the independent flag.
    box.independence=true;
    //Backup the source data pointer.
    char *sourceData=box.data;
    //Generate a new memory field for the box.
    box.data=new char[box.size];
    //Copy the data from the source data to the new memory field.
    memcpy(box.data, sourceData, box.size);
}

inline bool KNMusicTagM4a::findIlstBox(const KNMusicTagM4a::M4ABox &metaBox,
                                       KNMusicTagM4a::M4ABox &ilstBox)
{
    //In the "meta" box, the first 4 bytes is a mystery version. In the
    //document, it says '1 byte atom version (0x00) & 3 bytes atom flags
    //(0x000000)'. So, I have no idea of these flags. I can only ignore it.
    //The data pointer will be start right after those 4 bytes.
    char *data=metaBox.data+4;
    //And now, we need to find the "ilst" box in "meta" box.
    quint32 sourceSize=metaBox.size-4;
    //Check if we have read all the source data.
    while(sourceSize > 0)
    {
        //Get the box size and the name of the box.
        quint32 boxSize=KNMusicUtil::charToInt32(data);
        //Get the box name.
        QString boxName=QByteArray(data+4, 4);
        //If current box is "ilst", then mission complete.
        if(boxName=="ilst")
        {
            //Set name to the ilst box.
            ilstBox.name=boxName;
            //Set the box size.
            ilstBox.size=boxSize-8;
            //Make the ilst box to become a independent box.
            ilstBox.independence=true;
            //Copy the data to the ilst box.
            ilstBox.data=new char[ilstBox.size];
            //Copy the data to the char array.
            memcpy(ilstBox.data, data+8, ilstBox.size);
            return true;
        }
        //Move the pointer to the next position.
        data+=boxSize;
        //Reduce the surplus data size.
        sourceSize-=boxSize;
    }
    return false;
}

void KNMusicTagM4a::writeBox(const KNMusicTagM4a::M4ABox &source,
                             QFile &targetFile)
{
    //Get the box size.
    int boxSize=source.size+8;
    //Generate the text field.
    char textField[5]={0};
    //Convert data to text field.
    KNMusicUtil::int32ToChar(textField, boxSize);
    //Write four bytes to target file.
    targetFile.write(textField, 4);
    //Write the name of source to the text field.
    targetFile.write(source.name.toLatin1());
    //Then write the data.
    targetFile.write(source.data, source.size);
}

inline bool KNMusicTagM4a::extractMetaBox(const KNMusicTagM4a::M4ABox &metaBox,
                                          QList<KNMusicTagM4a::M4ABox> &boxes)
{
    //Meta box is a little bit bug. We have to skip the 4-bytes atom version &
    //atom flags.
    return extractData(metaBox.size-4, metaBox.data+4, boxes);
}

inline bool KNMusicTagM4a::extractData(quint32 sourceSize,
                                       char *dataPosition,
                                       QList<KNMusicTagM4a::M4ABox> &boxes)
{
    //Read until all the data has been all read.
    while(sourceSize > 0)
    {
        //Read the size of the inner box.
        quint32 preferSize=KNMusicUtil::charToInt32(dataPosition);
        //Check source size and prefer size.
        if(preferSize==0 || preferSize > sourceSize)
        {
            //There should be bad data mix in, we will stop to read the data.
            return false;
        }
        //Generate a new box for the box.
        M4ABox innerBox;
        //Set the data size.
        innerBox.size=preferSize-8;
        //Set the name of the box.
        innerBox.name=QByteArray(dataPosition+4, 4);
        //Set the data pointer.
        innerBox.data=dataPosition+8;
        //All the inner box are dependent on the source box.
        innerBox.independence=false;
        //Add the box to the list.
        boxes.append(innerBox);
        //Reduce the source size and move the position.
        sourceSize-=preferSize;
        dataPosition+=preferSize;
    }
    //If all the data has been parsed to box, then extract complete.
    return true;
}

inline bool KNMusicTagM4a::findBox(const QString &targetName,
                                   KNMusicTagM4a::M4ABox &targetBox,
                                   const QList<KNMusicTagM4a::M4ABox> &boxList)
{
    //Find all the box of the expand list.
    for(auto i : boxList)
    {
        //Check the name of each box.
        if(i.name==targetName)
        {
            //Save the box.
            targetBox=i;
            //Make the target box independent.
            independent(targetBox);
            //We find the box successfully.
            return true;
        }
    }
    //We cannot find the target name in the box list.
    return false;
}

inline KNMusicTagM4a::M4ABox KNMusicTagM4a::generateItemBox(
        const int &column,
        const QString &atomName,
        const QByteArray &rawData)
{
    //Generate the atom flag array.
    char atomFlags[5];
    //For those default atoms, they will use only one byte flag.
    atomFlags[0]=0x00;
    atomFlags[1]=0x00;
    atomFlags[2]=0x00;
    atomFlags[3]=(char)m_indexFlagMap.value(column);
    //Generate the item box.
    return generateItemBox(atomFlags, atomName, rawData);
}

KNMusicTagM4a::M4ABox KNMusicTagM4a::generateItemBox(char *atomFlags,
                                                     const QString &atomName,
                                                     const QByteArray &rawData)
{
    //Generate a M4A box.
    M4ABox itemBox;
    //Set the atom name.
    itemBox.name=atomName;
    //Make the box independent.
    itemBox.independence=true;
    //Generate the temporary data.
    QByteArray temporaryData;
    //Generate the "Data" box.
    //Append the "data" first.
    temporaryData.append('d');
    temporaryData.append('a');
    temporaryData.append('t');
    temporaryData.append('a');
    //According to the data, there should be 1 byte version, then 3 bytes flag.
    //We simplified it to 3 bytes 0x00 and 1 byte flag data.
    temporaryData.append(atomFlags[0]);
    temporaryData.append(atomFlags[1]);
    temporaryData.append(atomFlags[2]);
    temporaryData.append(atomFlags[3]);
    //Append four bytes NULL space(0x00);
    temporaryData.append((char)0x00);
    temporaryData.append((char)0x00);
    temporaryData.append((char)0x00);
    temporaryData.append((char)0x00);
    //Append the real data.
    temporaryData.append(rawData);
    //The data box size is the size of the temporary data plus 4, prepend the
    //size to the temporary data.
    char dataSize[5];
    KNMusicUtil::int32ToChar(dataSize, temporaryData.size()+4);
    temporaryData.prepend(dataSize, 4);
    //Now copy the temporary data to the box.
    itemBox.size=temporaryData.size();
    itemBox.data=new char[itemBox.size];
    //Copy the data.
    memcpy(itemBox.data, temporaryData.data(), itemBox.size);
    //Give back the item box.
    return itemBox;
}

inline KNMusicTagM4a::M4ABox KNMusicTagM4a::zipBox(
        const QString &name,
        const QList<KNMusicTagM4a::M4ABox> &boxes)
{
    //Generate the box.
    M4ABox parentBox;
    //Set the name of the box.
    parentBox.name=name;
    //Set it to independent.
    parentBox.independence=true;
    //Combine the box list.
    QByteArray boxData=combineBoxList(boxes);
    //Set the data and data size.
    parentBox.size=boxData.size();
    parentBox.data=new char[boxData.size()];
    //Copy the data.
    memcpy(parentBox.data, boxData.data(), parentBox.size);
    //Give the box back.
    return parentBox;
}

inline QByteArray KNMusicTagM4a::combineBoxList(const QList<M4ABox> &boxes)
{
    //Generate a byte array.
    QByteArray boxContentData;
    //For all the box in the list, we have to pack it and append to the content
    //data.
    for(auto i : boxes)
    {
        //Append the box data.
        boxContentData.append(packBox(i));
    }
    //Give back the content data.
    return boxContentData;
}

inline QByteArray KNMusicTagM4a::packBox(const KNMusicTagM4a::M4ABox &box)
{
    //Generate the box data field.
    QByteArray boxData;
    //First set the name of the box.
    boxData.append(box.name);
    //Then follow up the box data.
    boxData.append(box.data, box.size);
    //The data box size is the size of the box data plus 4, prepend the size to
    //the box data.
    char dataSize[5];
    KNMusicUtil::int32ToChar(dataSize, boxData.size()+4);
    boxData.prepend(dataSize, 4);
    //Give baack the box data.
    return boxData;
}
