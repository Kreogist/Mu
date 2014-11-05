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
#include <cstdlib>

#include "knmusictagm4a.h"

#include <QDebug>

KNMusicTagM4A::KNMusicTagM4A(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Initial metadata atoms.
    char combineText[5]={0};
    combineText[0]=0xA9;
    combineText[1]='a';combineText[2]='l';combineText[3]='b';
    m_metadataAtom[combineText]=Album;
    combineText[1]='A';combineText[2]='R';combineText[3]='T';
    m_metadataAtom[combineText]=Artist;
    m_metadataAtom["aART"]=AlbumArtist;
    combineText[1]='c';combineText[2]='m';combineText[3]='t';
    m_metadataAtom[combineText]=Comments;
    combineText[1]='d';combineText[2]='a';combineText[3]='y';
    m_metadataAtom[combineText]=Year;
    combineText[1]='n';combineText[2]='a';combineText[3]='m';
    m_metadataAtom[combineText]=Name;
    combineText[1]='g';combineText[2]='e';combineText[3]='n';
    m_metadataAtom[combineText]=Genre;
    m_metadataAtom["gnre"]=Genre;
    m_metadataAtom["trkn"]=TrackNumber;
    m_metadataAtom["disk"]=DiscNumber;
    combineText[1]='w';combineText[2]='r';combineText[3]='t';
    m_metadataAtom[combineText]=Composer;
    m_metadataAtom["tmpo"]=BeatsPerMinuate;
    m_metadataAtom["rtng"]=Rating;
    m_metadataAtom["catg"]=Category;
    m_metadataAtom["desc"]=Description;
}

bool KNMusicTagM4A::praseTag(QFile &musicFile,
                             QDataStream &musicDataStream,
                             KNMusicDetailInfo &detailInfo)
{
    Q_UNUSED(musicFile)
    //The m4a file is made of a number of atoms, now they are called 'boxes'.
    /*
     * A box always begins with 4 bytes length and follows 4 bytes name.
     */
    //And first we need to check the header box of the file. Its name is 'ftyp'
    M4ABox moovBox, udtaBox, metaBox, ilstBox;
    if(!getBox(musicDataStream, moovBox, true) ||
            moovBox.name!="ftyp")
    {
        return false;
    }

    //Metadata to be used with iTunes comes in the moov.udta.meta.ilst.
    //So get the moov box first.
    while(moovBox.name!="moov")
    {
        //If we cannot find a box, means there's no "moov" box, return false.
        if(!getBox(musicDataStream, moovBox))
        {
            return false;
        }
    }
    //Expand the "moov" box, and find "udta" box.
    QList<M4ABox> expandList;
    extractBox(moovBox, expandList);
    if(expandList.isEmpty())
    {
        return false;
    }
    bool findUdta=false, findMeta=false;
    for(QList<M4ABox>::iterator i=expandList.begin();
        i!=expandList.end();
        ++i)
    {
        if((*i).name=="udta")
        {
            //Set flag.
            findUdta=true;
            //Change the "udta" box to a independence box.
            udtaBox=(*i);
            toIndependence(udtaBox);
            //Jump out.
            break;
        }
    }
    if(!findUdta)
    {
        return false;
    }
    //Expand the "udta" box, and find "meta" box.
    expandList.clear();
    extractBox(udtaBox, expandList);
    if(expandList.isEmpty())
    {
        return false;
    }
    for(QList<M4ABox>::iterator i=expandList.begin();
        i!=expandList.end();
        ++i)
    {
        if((*i).name=="meta")
        {
            //Set flag.
            findMeta=true;
            //Change the "meta" box to a independence box.
            metaBox=(*i);
            toIndependence(metaBox);
            //Jump out.
            break;
        }
    }
    if(!findMeta)
    {
        return false;
    }
    //Expand the "meta" box, and now, we can fill our data.
    if(!parseMetaBox(metaBox, ilstBox))
    {
        return false;
    }
    //Expand the "ilst" box.
    expandList.clear();
    extractBox(ilstBox, expandList);
    if(expandList.isEmpty())
    {
        return false;
    }
    //Write the expand list to the detail info.
    writeBoxListToDetailInfo(expandList, detailInfo);
    return true;
}

bool KNMusicTagM4A::parseAlbumArt(KNMusicDetailInfo &detailInfo)
{
    //Check flag.
    if(!detailInfo.imageData.contains("M4A"))
    {
        return false;
    }
    //Get the image data. There can be only one image.
    //This box is simple, it only contains a 'data' box.
    QByteArray boxData=detailInfo.imageData["M4A"].at(0);
    M4ABox covrBox;
    covrBox.name="covr";
    covrBox.size=boxData.size();
    covrBox.data=boxData.data();
    QList<M4ABox> expandList;
    extractBox(covrBox, expandList);
    //In the expand list, there's only one box.
    if(expandList.isEmpty())
    {
        return false;
    }
    //Remember, there's 8 bytes version and flags here.
    detailInfo.coverImage.loadFromData(QByteArray(expandList.first().data+8,
                                                  expandList.first().size-8));
    return true;
}

void KNMusicTagM4A::clearBox(M4ABox &box)
{
    //Clear the box information.
    box.name.clear();
    box.size=0;
    box.independence=false;
    //Clear the box data.
    if(box.data!=nullptr)
    {
        //Delete the data.
        delete[] box.data;
        //Set it back to nullptr.
        box.data=nullptr;
    }
}

void KNMusicTagM4A::toIndependence(M4ABox &box)
{
    //Check whether is the box has been independence box.
    if(box.independence)
    {
        return;
    }
    //Set the independence flag.
    box.independence=true;
    //Backup the source data pointer.
    char *sourceData=box.data;
    //Generate a new data source field for the box.
    box.data=new char[box.size];
    //Copy those data.
    memcpy(box.data, sourceData, box.size);
}

void KNMusicTagM4A::writeBoxListToDetailInfo(const QList<M4ABox> &expandList,
                                             KNMusicDetailInfo &detailInfo)
{
    for(QList<M4ABox>::const_iterator i=expandList.begin();
        i!=expandList.end();
        ++i)
    {
        //Check the metadata name first, if is covr, means it's album art.
        if((*i).name=="covr")
        {
            detailInfo.imageData["M4A"].append(QByteArray((*i).data,
                                                          (*i).size));
            continue;
        }
        //Get the index of the current box.
        if(!m_metadataAtom.contains((*i).name))
        {
            continue;
        }
        int atomIndex=m_metadataAtom[(*i).name],
            dataSize=(*i).size-16;
        //Check the size.
        if(dataSize<0)
        {
            continue;
        }
        char *dataPosition=(*i).data+16;
        switch(atomIndex)
        {
        case TrackNumber:
            if((*i).size>6)
            {
                detailInfo.textLists[TrackNumber]=QString::number(dataPosition[3]);
                detailInfo.textLists[TrackCount]=QString::number(dataPosition[5]);
            }
            break;
        case Rating:
            detailInfo.rating=(quint8)dataPosition[0];
            break;
        default:
            setTextData(detailInfo.textLists[atomIndex],
                        QString(QByteArray(dataPosition, dataSize)));
            break;
        }
    }
}

bool KNMusicTagM4A::parseMetaBox(M4ABox &metaBox, M4ABox &ilstBox)
{
    //In the "meta" box, the first 4 bytes is a mystery version. In the
    //document, it says '1 byte atom version (0x00) & 3 bytes atom flags
    //(0x000000)'. So, I have no idea of these flags. I can only ignore it.
    char *data=metaBox.data+4;
    //And now, we need to find the "ilst" box in "meta" box.
    quint32 sourceSize=metaBox.size-4;
    while(sourceSize>0)
    {
        //Get the box size and the name of the box.
        quint32 boxSize=(((quint32)data[0]<<24)&0b11111111000000000000000000000000)+
                        (((quint32)data[1]<<16)&0b00000000111111110000000000000000)+
                        (((quint32)data[2]<<8 )&0b00000000000000001111111100000000)+
                        ( (quint32)data[3]     &0b00000000000000000000000011111111);
        QString boxName=QString(QByteArray(data+4, 4));
        //If current box is "ilst", then mission complete.
        if(boxName=="ilst")
        {
            //Set the data to the ilst box.
            ilstBox.name=boxName;
            ilstBox.size=boxSize-8;
            //Copy the data to that box.
            ilstBox.independence=true;
            ilstBox.data=new char[ilstBox.size];
            memcpy(ilstBox.data, data+8, ilstBox.size);
            return true;
        }
        //Move the pointer and the reduce the size.
        sourceSize-=boxSize;
        data=data+boxSize;
    }
    return false;
}

bool KNMusicTagM4A::getBox(QDataStream &musicDataStream,
                           M4ABox &box,
                           bool ignoreContent)
{
    //Clear box.
    clearBox(box);
    //Get the size of the box, reduce 8 bytes of the size itself and the name.
    musicDataStream>>box.size;
    box.size-=8;
    box.independence=true;
    //Get the box's name.
    char nameField[5]={0};
    if(musicDataStream.readRawData(nameField, 4)==-1)
    {
        return false;
    }
    box.name=nameField;
    //Get the content, or else ignore it.
    if(ignoreContent)
    {
        //Skip the data.
        return musicDataStream.skipRawData(box.size);
    }
    //Expand the byte array.
    box.data=new char[box.size];
    //Read the raw data.
    return musicDataStream.readRawData(box.data, box.size);
}

bool KNMusicTagM4A::extractBox(M4ABox &source,
                               QList<M4ABox> &boxes)
{
    //To extract box in a single box, we need to count the byte we read.
    quint32 sourceSize=source.size;
    char *dataPosition=source.data;
    while(sourceSize>0)
    {
        //Assume the first four bytes are the size.
        quint32 preferSize=(((quint32)dataPosition[0]<<24)&0b11111111000000000000000000000000)+
                           (((quint32)dataPosition[1]<<16)&0b00000000111111110000000000000000)+
                           (((quint32)dataPosition[2]<<8 )&0b00000000000000001111111100000000)+
                           ( (quint32)dataPosition[3]     &0b00000000000000000000000011111111);
        //Check the size to the prefer size.
        if(preferSize==0 || preferSize>sourceSize)
        {
            return false;
        }
        //Set the data to a new box.
        M4ABox currentBox;
        currentBox.size=preferSize-8;
        //Set the name of the size.
        currentBox.name=QString(QByteArray(dataPosition+4, 4));
        //Set the data.
        currentBox.data=dataPosition+8;
        //Add the box to the boxes.
        boxes.append(currentBox);
        //Reduce the byte count and move the position.
        sourceSize-=preferSize;
        dataPosition=dataPosition+preferSize;
    }
    return true;
}
