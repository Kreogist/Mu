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
#include <QTextCodec>
#include <QTemporaryFile>

#include "knmusicglobal.h"
#include "knlocalemanager.h"

#include "knmusictagid3v2.h"

#include <QDebug>

//1MB music data cache copy size.
#define DataCacheSize 1048576

//Initail codeces as null.
QTextCodec *KNMusicTagId3v2::m_localeCodec=nullptr;
QTextCodec *KNMusicTagId3v2::m_isoCodec=nullptr;
QTextCodec *KNMusicTagId3v2::m_utf16BECodec=nullptr;
QTextCodec *KNMusicTagId3v2::m_utf16LECodec=nullptr;
QTextCodec *KNMusicTagId3v2::m_utf16Codec=nullptr;
QTextCodec *KNMusicTagId3v2::m_utf8Codec=nullptr;

//Initial the map list.
QHash<QString, int> KNMusicTagId3v2::m_frameIDIndex=QHash<QString, int>();
QHash<int, QString> KNMusicTagId3v2::m_IndexFrameID3=QHash<int, QString>();
QHash<int, QString> KNMusicTagId3v2::m_IndexFrameID4=QHash<int, QString>();

//Initial the unsynchronisation data.
QByteArray KNMusicTagId3v2::m_unsynchronisationRaw=QByteArray();
QByteArray KNMusicTagId3v2::m_unsynchronisationTo=QByteArray();

KNMusicTagId3v2::KNMusicTagId3v2(QObject *parent) :
    KNMusicTagParser(parent),
    m_useDefaultCodec(true)
{
    //Check whether the static variables has been initial or not.
    //Use a codec pointer as a detecter. If any codec is nullptr, then initial
    //all the static variables.
    if(m_localeCodec==nullptr)
    {
        //Initial codecs.
        m_localeCodec=knI18n->localeCodec();
        m_isoCodec=QTextCodec::codecForName("ISO 8859-1");
        m_utf16BECodec=QTextCodec::codecForName("UTF-16BE");
        m_utf16LECodec=QTextCodec::codecForName("UTF-16LE");
        m_utf16Codec=QTextCodec::codecForName("UTF-16");
        m_utf8Codec=QTextCodec::codecForName("UTF-8");

        //Initial frame ID Index.
        //Four chars frame ID for ID3v2.3 and later.
        m_frameIDIndex.insert("TIT2", Name);
        m_frameIDIndex.insert("TPE1", Artist);
        m_frameIDIndex.insert("TALB", Album);
        m_frameIDIndex.insert("TPE2", AlbumArtist);
        m_frameIDIndex.insert("TBPM", BeatsPerMinuate);
        m_frameIDIndex.insert("TIT1", Category);
        m_frameIDIndex.insert("COMM", Comments);
        m_frameIDIndex.insert("TCOM", Composer);
        m_frameIDIndex.insert("TIT3", Description);
        m_frameIDIndex.insert("TCON", Genre);
        m_frameIDIndex.insert("TRCK", TrackNumber);
        m_frameIDIndex.insert("TPOS", DiscNumber);
        m_frameIDIndex.insert("POPM", Rating);
        m_frameIDIndex.insert("TYER", Year);

        m_IndexFrameID4.insert(Name           , "TIT2");
        m_IndexFrameID4.insert(Artist         , "TPE1");
        m_IndexFrameID4.insert(Album          , "TALB");
        m_IndexFrameID4.insert(AlbumArtist    , "TPE2");
        m_IndexFrameID4.insert(BeatsPerMinuate, "TBPM");
        m_IndexFrameID4.insert(Category       , "TIT1");
        m_IndexFrameID4.insert(Comments       , "COMM");
        m_IndexFrameID4.insert(Composer       , "TCOM");
        m_IndexFrameID4.insert(Description    , "TIT3");
        m_IndexFrameID4.insert(Genre          , "TCON");
        m_IndexFrameID4.insert(TrackNumber    , "TRCK");
        m_IndexFrameID4.insert(DiscNumber     , "TPOS");
        m_IndexFrameID4.insert(Rating         , "POPM");
        m_IndexFrameID4.insert(Year           , "TYER");

        m_frameIDIndex.insert("TT2", Name);
        m_frameIDIndex.insert("TP1", Artist);
        m_frameIDIndex.insert("TAL", Album);
        m_frameIDIndex.insert("TP2", AlbumArtist);
        m_frameIDIndex.insert("TBP", BeatsPerMinuate);
        m_frameIDIndex.insert("TT1", Category);
        m_frameIDIndex.insert("COM", Comments);
        m_frameIDIndex.insert("TCM", Composer);
        m_frameIDIndex.insert("TT3", Description);
        m_frameIDIndex.insert("TCO", Genre);
        m_frameIDIndex.insert("TRK", TrackNumber);
        m_frameIDIndex.insert("TPA", DiscNumber);
        m_frameIDIndex.insert("POP", Rating);
        m_frameIDIndex.insert("TYE", Year);

        m_IndexFrameID3.insert(Name           , "TT2");
        m_IndexFrameID3.insert(Artist         , "TP1");
        m_IndexFrameID3.insert(Album          , "TAL");
        m_IndexFrameID3.insert(AlbumArtist    , "TP2");
        m_IndexFrameID3.insert(BeatsPerMinuate, "TBP");
        m_IndexFrameID3.insert(Category       , "TT1");
        m_IndexFrameID3.insert(Comments       , "COM");
        m_IndexFrameID3.insert(Composer       , "TCM");
        m_IndexFrameID3.insert(Description    , "TT3");
        m_IndexFrameID3.insert(Genre          , "TCO");
        m_IndexFrameID3.insert(TrackNumber    , "TRK");
        m_IndexFrameID3.insert(DiscNumber     , "TPA");
        m_IndexFrameID3.insert(Rating         , "POP");
        m_IndexFrameID3.insert(Year           , "TYE");

        //Initial unsynchronisation data.
        //Using forced conversation to ignore the ambiguous calling.
        m_unsynchronisationRaw.append((char)0xff);
        m_unsynchronisationRaw.append((char)0x00);
        m_unsynchronisationTo.append((char)0xff);
    }
}

bool KNMusicTagId3v2::parseTag(QFile &musicFile,
                               QDataStream &musicDataStream,
                               KNMusicAnalysisItem &analysisItem)
{
    //If file is less than ID3v2 header, it can't contains ID3v2 tag.
    if(musicFile.size()<10)
    {
        return false;
    }
    //Initial datas.
    char rawHeader[10];
    ID3v2Header header;
    //Read the ID3v2 header raw data.
    musicDataStream.readRawData(rawHeader, 10);
    //Try to parse the header. If parse it failed or the version of ID3v2 is
    //greater than 4(ID3v2.4), then return false.
    //We cannot parse a higher version of ID3v2.
    if(!parseID3v2Header(rawHeader, header) ||
            header.major>4)
    {
        return false;
    }
    //Check is file's size smaller than tag size.
    if(musicFile.size()<(header.size+10))
    {
        //File is smaller than the tag says, failed to get the tag.
        return false;
    }
    //Read the raw tag data.
    char *rawTagData=new char[header.size];
    musicDataStream.readRawData(rawTagData, header.size);
    //Get the function set according to the minor version of the header.
    ID3v2FunctionSet functionSet;
    getId3v2FunctionSet(header.major, functionSet);
    //Generate the raw frame linked list.
    QLinkedList<ID3v2Frame> frames;
    //Parse the raw data.
    if(parseID3v2RawData(rawTagData, header, functionSet, frames) &&
            !frames.isEmpty())
    {
        //Write the tag to analysis info.
        writeFrameToDetails(frames, functionSet, analysisItem);
    }
    //Recover the memeory.
    delete[] rawTagData;
    //Finish the tag reading.
    return true;
}

bool KNMusicTagId3v2::writeTag(KNMusicAnalysisItem &analysisItem)
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

    //Initial datas.
    char rawHeader[10];
    //Generate a header structure.
    ID3v2Header header;
    //Generate the raw frame linked list to store the original frame.
    QHash<QString, ID3v2DataFrame> frameMap;
    //Generate the default encoding.
    int encoding=EncodeUTF16BELE;
    //Parse the original data.
    //If file is less than ID3v2 header, it can't contains ID3v2 tag. We have to
    //use a default values.
    if(musicFile.size()<10)
    {
        //Use ID3v2.3 with no flag as default.
        header.major=3;
    }
    else
    {
        if(musicFile.open(QIODevice::ReadOnly))
        {
            //Generate a data stream.
            QDataStream musicDataStream(&musicFile);
            //Read the ID3v2 header raw data.
            musicDataStream.readRawData(rawHeader, 10);
            //Try to parse the header. If parse it parsed successful, then parse
            //the detail data.
            if(parseID3v2Header(rawHeader, header))
            {
                //If the tag version is ID3v2.4, use UTF-8 as default encoding.
                if(header.major==4)
                {
                    encoding=EncodeUTF8;
                }
                //Check is file's size smaller than tag size, only parse it if
                //there's enough bytes.
                if(musicFile.size()>(header.size+10))
                {
                    //Generate a frame list.
                    QLinkedList<ID3v2Frame> frames;
                    //Read the raw tag data.
                    char *rawTagData=new char[header.size];
                    musicDataStream.readRawData(rawTagData, header.size);
                    //Get the function set according to the minor version of the
                    //header.
                    ID3v2FunctionSet functionSet;
                    getId3v2FunctionSet(header.major, functionSet);
                    //Parse the raw data.
                    parseID3v2RawData(rawTagData, header, functionSet, frames);
                    //Transfer frame list to frame hash.
                    for(auto i=frames.begin(); i!=frames.end(); i++)
                    {
                        //Generate a data frame.
                        ID3v2DataFrame dataFrame;
                        //Copy the flag data.
                        dataFrame.flags[0]=(*i).flags[0];
                        dataFrame.flags[1]=(*i).flags[1];
                        //Copy the frame data.
                        dataFrame.data.append((*i).start, (*i).size);
                        //Insert the data frame to map.
                        frameMap.insert((*i).frameID, dataFrame);
                    }
                    //Recover the raw tag data.
                    delete[] rawTagData;
                }
            }
            else
            {
                //Use ID3v2.3 as default.
                header.major=3;
            }
            musicFile.close();
        }
    }

    //Generate the item mapper.
    //Get the frame matcher.
    const QHash<int, QString> &frameMatcher=
            (header.major>2)?m_IndexFrameID4:m_IndexFrameID3;
    //Add all the text labels to detail frames if it's not empty.
    for(int i=0; i<MusicDataCount; i++)
    {
        //Check if the text is empty.
        if(detailInfo.textLists[i].isEmpty())
        {
            continue;
        }
        //Get the frame id if the frame index is valid in the hash list.
        QString frameID=frameMatcher.value(i, "");
        //If the frame id is empty, means you cannot write this data to ID3v2.
        if(frameID.isEmpty())
        {
            continue;
        }
        //Generate a data frame.
        ID3v2DataFrame dataFrame;
        //According to the frame, generate the frame.
        switch(i)
        {
        case DiscNumber:
        {
            //If disc count isn't empty, then add disc count to disc number
            //data.
            dataFrame.data=
                    stringToContent(
                        (detailInfo.textLists[DiscCount].isEmpty())?
                            detailInfo.textLists[DiscNumber]:
                            detailInfo.textLists[DiscNumber] + "/" +
                                detailInfo.textLists[DiscCount],
                         encoding);
            break;
        }
        case TrackNumber:
        {
            //If track count isn't empty, then add track count to track number
            //data.
            dataFrame.data=
                    stringToContent(
                        (detailInfo.textLists[TrackCount].isEmpty())?
                            detailInfo.textLists[TrackNumber]:
                            detailInfo.textLists[TrackNumber] + "/" +
                                detailInfo.textLists[TrackCount],
                         encoding);
            break;
        }
        case Rating:
        {
            //Simply add rating number to data frame.
            dataFrame.data.append(detailInfo.rating);
            break;
        }
        default:
            //Treat it as a text frame.
            dataFrame.data=stringToContent(detailInfo.textLists[i], encoding);
        }
        //Insert the data frame to frame map, because the insert function will
        //replace the old one, so it will keep the data to the latest one.
        frameMap.insert(frameID, dataFrame);
    }

    //Now translate the frame structure data to the raw data.
    QByteArray tagRawData;
    //Get all the keys of the hash map.
    QStringList frameIDs=frameMap.keys();
    //Get the toolset of the header.
    ID3v2FunctionSet toolset;
    getId3v2FunctionSet(header.major, toolset);
    //Append the frame data to the raw data array.
    while(!frameIDs.isEmpty())
    {
        //Get the current frame ID.
        QString frameID=frameIDs.takeFirst();
        //Add the frame raw data to tag raw data array.
        tagRawData.append(frameToRawData(frameID,
                                         toolset,
                                         frameMap.take(frameID)));
    }
    //Backup the original header size.
    quint32 originalHeaderSize=header.size;
    //Update the tag size data stored in the header structure.
    header.size=tagRawData.size();
    //Generate the header raw data array.
    char tagRawHeaderData[10];
    //Write the header data information to the array.
    generateID3v2Header(tagRawHeaderData, header);

    //Open the music file again.
    if(!musicFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Generate a temporary file, write the new data to the temporary file.
    QTemporaryFile updatedTagFile;
    //Open the temporary file in write only mode, if we cannot open the
    //temporary file it will be failed to write the tag.
    if(!updatedTagFile.open())
    {
        //Close the opened music file.
        musicFile.close();
        return false;
    }
    //Skip the original header size, ignore the raw file data.
    musicFile.seek(10+originalHeaderSize);
    //Wrire the tag header data to the tempoary file.
    updatedTagFile.write(tagRawHeaderData, 10);
    //Write the tag data to the temporary file.
    updatedTagFile.write(tagRawData);
    //Generate the music data cache.
    char fileCache[DataCacheSize];
    //Copy the music data from the original music file, copy the
    //MusicDataCacheSize bytes once, until there's no bytes to copy.
    int bytesRead=musicFile.read(fileCache, DataCacheSize);
    while(bytesRead>0)
    {
        //Write the cache to temporary file.
        updatedTagFile.write(fileCache, bytesRead);
        //Read new data from the original file to cache.
        bytesRead=musicFile.read(fileCache, DataCacheSize);
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
        //Read new data from cache to the original file.
        bytesRead=updatedTagFile.read(fileCache, DataCacheSize);
    }
    //Close the music file and temporary file.
    musicFile.close();
    updatedTagFile.close();
    //The tag rewrite is finished.
    return true;
}

bool KNMusicTagId3v2::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //Check the image data contains ID3v2 tag, and the data of the tag is empty
    //or not.
    if(!analysisItem.imageData.contains("ID3v2") ||
            analysisItem.imageData["ID3v2"].isEmpty())
    {
        //If it doesn't contain the ID3v2 tag or it's empty, it's failed to
        //parse the album art.
        return false;
    }
    //Get the total size of images, because the .
    QByteArray imageTypes=analysisItem.imageData["ID3v2"].takeLast();
//    int imageCount=imageTypes.size();
    QHash<int, ID3v2PictureFrame> imageMap;
    //Parse all the images. Because of the standard has been changed, in the
    //earlier version of ID3v2(minor<=2.0), the frame name is PIC. Now it is
    //APIC.
    for(auto i=imageTypes.constBegin(); i!=imageTypes.constEnd(); i++)
    {
        //Check the flag, 1 is for APIC, 0 is for PIC.
        if((*i)==1) //APIC
        {
            parseAPICImageData(
                        analysisItem.imageData["ID3v2_Images"].takeFirst(),
                        imageMap);
        }
        else //PIC
        {
            parsePICImageData(
                        analysisItem.imageData["ID3v2_Images"].takeFirst(),
                        imageMap);
        }
    }
    //If there's a album art image after parse all the album art, set.
    if(imageMap.contains(3))
    {
        analysisItem.coverImage=imageMap[3].image;
    }
    else
    {
        //Or else use the first image.
        if(!imageMap.isEmpty())
        {
            analysisItem.coverImage=imageMap.begin().value().image;
        }
    }
    //No matter what, return true here. Because the images in ID3v2 has been
    //parsed.
    return true;
}

QString KNMusicTagId3v2::contentToString(QByteArray content)
{
    //Check is content empty.
    if(content.isEmpty())
    {
        return QString();
    }
    //Get the codec according to the first char.
    //The first char of the ID3v2 text is the encoding of the current text.
    //Using a state to catch the convert state.
    quint8 encoding=(quint8)(content.at(0));
    content.remove(0, 1);
    //Get the content according to the encoding index.
    switch(encoding)
    {
    case EncodeISO: //0 = ISO-8859-1
        //Due to many windows software like TTPlayer, Windows Media Player and
        //some other player. Many players use the system default codec to write
        //the data.
        //Use QChar('\0') instead of '\0', using MSVC compile it will have error
        //that it's not clear to call the overload function.
        return m_useDefaultCodec?
                    m_localeCodec->toUnicode(
                        content).simplified().remove(QChar('\0')):
                    m_isoCodec->toUnicode(
                        content).simplified().remove(QChar('\0'));
    case EncodeUTF16BELE: //1 = UTF-16 LE/BE (Treat other as no BOM UTF-16)
        //Decode via first two bytes.
        if(content.size()>1)
        {
            //UTF-16 BE BOM captured.
            if((quint8)content.at(0)==0xFE && (quint8)content.at(1)==0xFF)
            {
                return m_utf16BECodec->toUnicode(
                            content).simplified().remove(QChar('\0'));
            }
            //UTF-16 LE BOM captured.
            if((quint8)content.at(0)==0xFF && (quint8)content.at(1)==0xFE)
            {
                return m_utf16LECodec->toUnicode(
                            content).simplified().remove(QChar('\0'));
            }
        }
        //Treat the reset situation as normal UTF-16.
        return m_utf16Codec->toUnicode(content).simplified().remove(QChar('\0'));
    case EncodeUTF16: //2 = UTF-16 BE without BOM
        //Decode with UTF-16
        return m_utf16Codec->toUnicode(content).simplified().remove(QChar('\0'));
    case EncodeUTF8: //3 = UTF-8
        //Use UTF-8 to decode it.
        return m_utf8Codec->toUnicode(content).simplified().remove(QChar('\0'));
    default://Use locale codec.
        return m_localeCodec->toUnicode(
                    content).simplified().remove(QChar('\0'));
    }
}

QByteArray KNMusicTagId3v2::stringToContent(const QString &string,
                                            const quint8 &encoding)
{
    //Generate a byte array start with the encoding.
    QByteArray content=QByteArray(1, encoding);
    //Append content data after the encoding.
    switch(encoding)
    {
    case EncodeISO: //0 = ISO-8859-1
        //No matter how other software writes, we will obey the standard.
        content.append(m_isoCodec->fromUnicode(string));
        //Add a '\0' at the end of the content.
        content.append('\0');
        break;
    case EncodeUTF16BELE: //1 = UTF-16 LE/BE (Treat other as no BOM UTF-16 LE)
        //Using UTF-16 LE as the default BE/LE codec.
        //It will automatically add BOM in front of the content. We won't need
        //to add that mannually.
        content.append(m_utf16BECodec->fromUnicode(string));
        //Add two '\0' for text ending.
        content.append('\0');
        content.append('\0');
        break;
    case EncodeUTF16: //2 = UTF-16 BE without BOM
        //Encode with UTF-16
        content.append(m_utf16Codec->fromUnicode(string));
        //Add two '\0' for text ending.
        content.append('\0');
        content.append('\0');
        break;
    case EncodeUTF8: //3 = UTF-8
        //Encode with UTF-8. Good thing, if it's ID3v2.4, use UTF-8 as default.
        content.append(m_utf8Codec->fromUnicode(string));
        //Add a '\0' at the end of the content.
        content.append('\0');
        break;
    }
    //Give back the content data.
    return content;
}

bool KNMusicTagId3v2::parseID3v2RawData(char *rawTagData,
                                        const ID3v2Header &header,
                                        const ID3v2FunctionSet &property,
                                        QLinkedList<ID3v2Frame> &frameList)
{
    //Backup the raw tag data pointer, use it as the position pointer.
    char *rawPosition=rawTagData;
    //Backup the tag size, use it as a counter, if there's no more data we can
    //get, stop parsing process.
    quint32 rawTagDataSurplus=header.size;
    //Parse untill there's no content to read.
    while(rawTagDataSurplus>0)
    {
        //If the first byte is 0, means behind of these datas are all '\0'.
        //Some tag may have this 'fill' content. e.g. TTPlayer.
        if(rawPosition[0]==0)
        {
            break;
        }
        //Calculate the size first.
        quint32 frameSize=
                ((*(property.toSize))(rawPosition+property.frameIDSize));
        //Check the frame size, if the frame size is invalid or it's larger than
        //the surplus counter, stop parsing.
        if(frameSize<=0 || frameSize>rawTagDataSurplus)
        {
            break;
        }
        //Generate a raw frame.
        ID3v2Frame currentFrame;
        //Clear the frameID.
        memset(currentFrame.frameID, 0, sizeof(currentFrame.frameID));
        memset(currentFrame.flags, 0, sizeof(currentFrame.flags));
        //Copy the frame ID from raw data char array.
        memcpy(currentFrame.frameID, rawPosition, property.frameIDSize);
        //Save the start position and size.
        currentFrame.start=rawPosition+property.frameHeaderSize;
        currentFrame.size=frameSize;
        //Save the flag from the raw data if the function set have a save flag
        //function.
        if(property.saveFlag!=nullptr)
        {
            (*(property.saveFlag))(rawPosition, currentFrame);
        }
        //Append the frame to the frame list.
        frameList.append(currentFrame);

        //Reduce the surplus, move to the next frame position.
        int frameContentSize=property.frameHeaderSize+currentFrame.size;
        rawTagDataSurplus-=frameContentSize;
        rawPosition=rawPosition+frameContentSize;
    }
    //Finish parsing.
    return true;
}

void KNMusicTagId3v2::writeFrameToDetails(const QLinkedList<ID3v2Frame> &frames,
                                          const ID3v2FunctionSet &property,
                                          KNMusicAnalysisItem &analysisItem)
{
    //Get the detail info of the analysis item.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Prepare the image type list.
    QByteArray imageTypeList;
    //Try to parse all the raw frame data in the analysis list.
    for(QLinkedList<ID3v2Frame>::const_iterator i=frames.constBegin();
        i!=frames.constEnd();
        ++i)
    {
        //Process the data according to the flag before we use it.
        //Check if it contains a data length indicator, if so, use the size
        //calculator to calculate the size of data.
        QByteArray frameData=
                ((*i).flags[1] & FrameDataLengthIndicator)?
                    QByteArray((*i).start+4,
                               ((*(property.toSize))((*i).start))):
                    QByteArray((*i).start, (*i).size);
        //Check the frame is unsynchronisation. If so, replace the
        //unsynchronisation data.
        if((*i).flags[1] & FrameUnsynchronisation)
        {
            frameData.replace(m_unsynchronisationRaw,
                              m_unsynchronisationTo);
        }
        //Get the frame Index.
        QString frameID=QString((*i).frameID).toUpper();
        //For APIC and PIC frame(The frame include the album art), we have a
        //different process.
        if(frameID=="APIC" || frameID=="PIC")
        {
            //Here is a hack:
            //Using "ID3v2" as a counter, the size of the byte array is the
            //number of how many images contains in ID3v2.
            //If the frameID is "APIC", add a 1 to the "ID3v2" array, or else
            //add a 0.
            imageTypeList.append((int)(frameID=="APIC"));
            analysisItem.imageData["ID3v2_Images"].append(frameData);
            continue;
        }
        //Get the frame index.
        int frameIndex=m_frameIDIndex.value((*i).frameID, -1);
        //If the frame index is invalid, continue to check the next frame.
        if(frameIndex==-1)
        {
            continue;
        }
        //Most of those frames can just simply translate it into a string. For
        //some frames parse it in specific way.
        switch(frameIndex)
        {
        case DiscNumber:
        {
            //Get the text of the frame.
            QString frameText=contentToString(frameData);
            //Search the character '/', check the format is like 1/9.
            int diagonalPosition=frameText.indexOf('/');
            //If there's no diagonal, means it only contains disc number.
            if(diagonalPosition==-1)
            {
                setTextData(detailInfo.textLists[DiscNumber], frameText);
            }
            else
            {
                //Set the disc index.
                setTextData(detailInfo.textLists[DiscNumber],
                            frameText.left(diagonalPosition));
                //Set the disc count.
                setTextData(detailInfo.textLists[DiscCount],
                            frameText.mid(diagonalPosition+1));
            }
            break;
        }
        case TrackNumber:
        {
            //Get the text of the frame.
            QString frameText=contentToString(frameData);
            //Search the character '/', check whether the format is like 1/9
            int diagonalPosition=frameText.indexOf('/');
            //If there's no diagonal, means it only contains track number.
            if(diagonalPosition==-1)
            {
                setTextData(detailInfo.textLists[TrackNumber], frameText);
            }
            else
            {
                //Set the track index.
                setTextData(detailInfo.textLists[TrackNumber],
                            frameText.left(diagonalPosition));
                //Set the track count.
                setTextData(detailInfo.textLists[TrackCount],
                            frameText.mid(diagonalPosition+1));
            }
            break;
        }
        case Genre:
        {
            //Get the text of the frame.
            QString frameText=contentToString(frameData);
            //Check is the frame text is format like: (9) or (9) XXX
            if(frameText[0]=='(')
            {
                //Check is there any ')'.
                int rightBracketsPosition=frameText.indexOf(')');
                if(rightBracketsPosition==-1)
                {
                    //This text is like '(XXXXX', WTF, remove first '('.
                    frameText.remove(0, 1);
                    bool indexTranslateOk=false;
                    //Try to translate this into a number.
                    int attemptIndex=frameText.toInt(&indexTranslateOk);
                    //If it's a number, set the indexed genre, or set the raw
                    //text.
                    setTextData(detailInfo.textLists[Genre],
                                indexTranslateOk?
                                    knMusicGlobal->indexedGenre(attemptIndex):
                                    frameText);
                }
                else
                {
                    //Try to translate the content in the brackets.
                    bool indexTranslateOk=false;
                    int attemptIndex=
                            frameText.mid(1,
                                          rightBracketsPosition-1).toInt(
                                                            &indexTranslateOk);
                    //If it's a number, set the indexed genre, or set the raw
                    //text.
                    setTextData(detailInfo.textLists[Genre],
                                indexTranslateOk?
                                    knMusicGlobal->indexedGenre(attemptIndex):
                                    frameText);
                }
            }
            else
            {
                //Treat the whole text as genre.
                setTextData(detailInfo.textLists[Genre],
                            frameText);
            }
            break;
        }
        case Rating:
        {
            //I don't know why all the file using this.
            if(QString(frameData.left(29))=="Windows Media Player 9 Series")
            {
                //Translate the last bytes as rating.
                detailInfo.rating=ratingStars((quint8)(frameData.at(30)));
            }
            else
            {
                //Treat the first bytes as rating.
                if(!frameData.isEmpty())
                {
                    detailInfo.rating=frameData.at(0);
                }
            }
            break;
        }
        default:
            //Simply set the data at frame index as the string translated from
            //the frame data.
            setTextData(detailInfo.textLists[frameIndex],
                        contentToString(frameData));
        }
    }
    //If there's any data in type list, add it to image data.
    if(!imageTypeList.isEmpty())
    {
        analysisItem.imageData["ID3v2"].append(imageTypeList);
    }
}

inline void KNMusicTagId3v2::parseAPICImageData(
        QByteArray imageData,
        QHash<int, ID3v2PictureFrame> &imageMap)
{
    //APIC frame structure:
    /*
           Text encoding      $xx
           MIME type          <text string> $00
           Picture type       $xx
           Description        <text string according to encoding> $00 (00)
           Picture data       <binary data>
    */
    //In the official document above, (00) in the description means there's a
    //00 byte after the $00, I finally understand what this mean.
    //Get mime string end position and initial description end position.
    int mimeTypeEnd=imageData.indexOf('\0', 1),
        descriptionEnd=mimeTypeEnd+2;
    //Generate a picture frame.
    ID3v2PictureFrame imageFrame;
    //Save the mimetype.
    imageFrame.mimeType=contentToString(imageData.left(mimeTypeEnd));
    //Get the text encording and get the image type.
    quint8 textEncoding=imageData.at(0),
           type=imageData.at(mimeTypeEnd+1);
    //Find the description end position.
    switch (textEncoding)
    {
    //For ISO and UTF-8 codec, simply find the '\0' character.
    case EncodeISO:
    case EncodeUTF8:
        descriptionEnd=imageData.indexOf('\0', mimeTypeEnd+2);
        break;
    //For UTF-16 series, we have to use a loop structure to find the end.
    case EncodeUTF16:
    case EncodeUTF16BELE:
        while(imageData.at(descriptionEnd)!='\0' ||
                      imageData.at(descriptionEnd+1)!='\0')
        {
            descriptionEnd+=2;
        }
        break;
    default:
        break;
    }
    //Get the description part data.
    QByteArray description=imageData.mid(mimeTypeEnd+2,
                                         descriptionEnd-(mimeTypeEnd+2));
    //Add text encoding before the text content, use contentToString() to parse
    //the text.
    description.insert(0, textEncoding);
    //Save the description.
    imageFrame.description=contentToString(description);
    //Get the image data, remove the header data from the image data array.
    switch (textEncoding)
    {
    case EncodeISO:
    case EncodeUTF8:
        imageData.remove(0, descriptionEnd+1);
        break;
    case EncodeUTF16BELE:
    case EncodeUTF16:
        imageData.remove(0, descriptionEnd+2);
        break;
    default:
        break;
    }
    //Load image from the data.
    imageFrame.image.loadFromData(imageData);
    //If parse the image success, add it to map.
    if(!imageFrame.image.isNull())
    {
        imageMap[type]=imageFrame;
    }
}

void KNMusicTagId3v2::parsePICImageData(
        QByteArray imageData,
        QHash<int, ID3v2PictureFrame> &imageMap)
{
    //PIC frame structure:
    /*
          Text encoding      $xx
          Image format       $xx xx xx
          Picture type       $xx
          Description        <textstring> $00 (00)
          Picture data       <binary data>
    */
    //In the official document above, the (00) in description is the same as
    //APIC.
    //Generate a image frame.
    ID3v2PictureFrame imageFrame;
    //Get the mime type text, it's much simple than minor>=3.0.
    imageFrame.mimeType=imageData.mid(1, 3);
    //Backup the text encording and get the picture type.
    quint8 textEncoding=imageData.at(0),
           type=imageData.at(4);
    //Initial the description end.
    int descriptionEnd=5;
    //Find the description end.
    switch (textEncoding)
    {
    //For ISO and UTF-8, simply find the '\0' character after start.
    case EncodeISO:
    case EncodeUTF8:
        descriptionEnd=imageData.indexOf('\0', 5);
        break;
    //For UTF-16 series, use a loop to find the end of description.
    case EncodeUTF16BELE:
    case EncodeUTF16:
        while(imageData.at(descriptionEnd)!='\0' ||
              imageData.at(descriptionEnd+1)!='\0')
        {
            descriptionEnd+=2;
        }
        break;
    default:
        break;
    }
    //Get the description part data.
    QByteArray description=imageData.mid(5, descriptionEnd-5);
    //Add text encoding before the text content, use contentToString() to parse
    //the text.
    description.insert(0, textEncoding);
    //Save the description.
    imageFrame.description=contentToString(description);
    //Get the image data, remove the header data from the image data array.
    switch (textEncoding)
    {
    case EncodeISO:
    case EncodeUTF8:
        imageData.remove(0, descriptionEnd+1);
        break;
    case EncodeUTF16BELE:
    case EncodeUTF16:
        imageData.remove(0, descriptionEnd+2);
        break;
    default:
        break;
    }
    //Load image from the data.
    imageFrame.image.loadFromData(imageData);
    //If parse the image success, add it to map.
    if(!imageFrame.image.isNull())
    {
        imageMap[type]=imageFrame;
    }
}

QByteArray KNMusicTagId3v2::frameToRawData(const QString frameID,
                                           const ID3v2FunctionSet &toolset,
                                           const ID3v2DataFrame &frame)
{
    //Generate the raw data.
    QByteArray rawData;
    //Append the frame ID to the raw data.
    rawData.append(frameID.toLatin1());
    //Check the size of frame id.
    //If it's 3, then simply add the length of the raw data after the frame.
    if(frameID.length()==3)
    {
        //Generate frame length cache.
        char frameLength[3];
        //Calculate the frame length.
        toolset.writeSize(frameLength, frame.data.size());
        //Append the raw data.
        rawData.append(frameLength, 3);
    }
    else //If it's 4, add frame size, then add frame flag before the raw data.
    {
        //Generate frame length cache.
        char frameLength[4];
        //Calculate the frame length.
        toolset.writeSize(frameLength, frame.data.size());
        //Append the raw data.
        rawData.append(frameLength, 4);
        //Append the flag of the frame.
        rawData.append(frame.flags, 2);
    }
    //Append the raw data.
    rawData.append(frame.data);
    //Return the raw data byte array.
    return rawData;
}
