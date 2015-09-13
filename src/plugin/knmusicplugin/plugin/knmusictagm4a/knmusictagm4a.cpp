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

#include <QDebug>

QHash<QString, int> KNMusicTagM4a::m_atomIndexMap=QHash<QString, int>();

KNMusicTagM4a::KNMusicTagM4a(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Check the atom map has been initialized.
    if(m_atomIndexMap.isEmpty())
    {
        //Initial metadata atoms.
        char combineText[5]={0};
        combineText[0]=0xA9;
        combineText[1]='a';combineText[2]='l';combineText[3]='b';
        m_atomIndexMap.insert(combineText, Album);
        combineText[1]='A';combineText[2]='R';combineText[3]='T';
        m_atomIndexMap.insert(combineText, Artist);
        combineText[1]='c';combineText[2]='m';combineText[3]='t';
        m_atomIndexMap.insert(combineText, Comments);
        combineText[1]='d';combineText[2]='a';combineText[3]='y';
        m_atomIndexMap.insert(combineText, Year);
        combineText[1]='n';combineText[2]='a';combineText[3]='m';
        m_atomIndexMap.insert(combineText, Name);
        combineText[1]='g';combineText[2]='e';combineText[3]='n';
        m_atomIndexMap.insert(combineText, Genre);
        combineText[1]='w';combineText[2]='r';combineText[3]='t';
        m_atomIndexMap.insert(combineText, Composer);
        m_atomIndexMap.insert("aART", AlbumArtist);
        m_atomIndexMap.insert("gnre", Genre);
        m_atomIndexMap.insert("trkn", TrackNumber);
        m_atomIndexMap.insert("disk", DiscNumber);
        m_atomIndexMap.insert("tmpo", BeatsPerMinuate);
        m_atomIndexMap.insert("rtng", Rating);
        m_atomIndexMap.insert("catg", Category);
        m_atomIndexMap.insert("desc", Description);
    }
}

bool KNMusicTagM4a::parseTag(QFile &musicFile,
                             QDataStream &musicDataStream,
                             KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(musicFile)
    //The m4a file is made of a number of atoms, now they are called 'boxes'.
    /*
     * A box always begins with 4 bytes length and follows 4 bytes name.
     */
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
    if(!extractMetaBox(metaBox, ilstBox))
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

inline bool KNMusicTagM4a::extractBox(KNMusicTagM4a::M4ABox &source,
                                      QList<KNMusicTagM4a::M4ABox> &boxes)
{
    //To extract box in a single box, we need to count the byte we read.
    quint32 sourceSize=source.size;
    char *dataPosition=source.data;
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

void KNMusicTagM4a::independent(KNMusicTagM4a::M4ABox &box)
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

inline bool KNMusicTagM4a::extractMetaBox(const KNMusicTagM4a::M4ABox &metaBox,
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

