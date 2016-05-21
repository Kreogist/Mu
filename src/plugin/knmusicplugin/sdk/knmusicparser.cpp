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
#include <QDir>

#include "knglobal.h"

#include "knmusicglobal.h"
#include "knmusicanalysiser.h"
#include "knmusictagparser.h"
#include "knmusiclistparser.h"

#include "knmusicparser.h"

#include <QDebug>

KNMusicParser::KNMusicParser(QObject *parent) :
    QObject(parent)
{
    //Initial the image types.
    m_imageTypes.append("jpg");
    m_imageTypes.append("png");
    m_imageTypes.append("jpeg");
    m_imageTypes.append("bmp");
}

void KNMusicParser::installAnalysiser(KNMusicAnalysiser *analysiser)
{
    //Configure the analysiser.
    //Move the analysiser to the parser's thread.
    analysiser->moveToThread(thread());
    //Change the relationship.
    analysiser->setParent(this);
    //Add analysiser to list.
    m_analysisers.append(analysiser);
}

void KNMusicParser::installTagParser(KNMusicTagParser *tagParser)
{
    //Configure the tag parser.
    //Move the tag parser to the parser's thread.
    tagParser->moveToThread(thread());
    //Change the relationship.
    tagParser->setParent(this);
    //Add tag parser to list.
    m_tagParsers.append(tagParser);
}

void KNMusicParser::installListParser(KNMusicListParser *listParser)
{
    //Configure the tag parser.
    //Move the list parser to parser's thread.
    listParser->moveToThread(thread());
    //Change the relationship.
    listParser->setParent(this);
    //Add the list parser to list.
    m_listParsers.append(listParser);
}

void KNMusicParser::parseFile(const QFileInfo &fileInfo,
                              KNMusicAnalysisItem &analysisItem)
{
    //Get the file info class of the current analysis item.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Set the database added time to current time.
    detailInfo.dateAdded=QDateTime::currentDateTime();
    //Set the file info of the detail info.
    detailInfo.filePath=fileInfo.absoluteFilePath();
    detailInfo.fileName=fileInfo.fileName();
    detailInfo.size=fileInfo.size();
    detailInfo.dateLastPlayed=fileInfo.lastRead();
    detailInfo.dateModified=fileInfo.lastModified();
    //Set the track to be -1. For music file it won't have track.
    detailInfo.trackIndex=-1;
    //Set the default value of some data.
    detailInfo.textLists[Name]=detailInfo.fileName;
    detailInfo.textLists[Size]=knGlobal->byteToString(detailInfo.size);
    detailInfo.textLists[DateAdded]=
            KNMusicUtil::dateTimeToText(detailInfo.dateAdded);
    detailInfo.textLists[DateModified]=
            KNMusicUtil::dateTimeToText(detailInfo.dateModified);
    detailInfo.textLists[LastPlayed]=
            KNMusicUtil::dateTimeToText(detailInfo.dateLastPlayed);
    //Get the description of the suffix.
    detailInfo.textLists[Kind]=
            knMusicGlobal->typeDescription(fileInfo.suffix());
    //Analysis music file.
    //Step 1, parse the tag of the music file.
    QFile file(detailInfo.filePath);
    //Open the music file at read only mode.
    if(file.open(QIODevice::ReadOnly))
    {
        //Initial a binary data stream for music file reading.
        QDataStream dataStream(&file);
        //Using all the tag parser parse the data.
        for(auto i : m_tagParsers)
        {
            //Seeks to the start of input.
            file.reset();
            //Parse the file using the tag parser.
            i->parseTag(file, dataStream, analysisItem);
        }
        //Close the music file after parsing.
        file.close();
    }
    //Step 2, parse the detail information of the music file. e.g. duration.
    //Using all the analysiser to analysis file.
    //If there's one analysiser can analysis this, exit.
    for(auto i: m_analysisers)
    {
        if(i->analysis(detailInfo))
        {
            break;
        }
    }
    //Check the duration. If the duration of the file is lesser than 0, make it
    //to 0.
    if(detailInfo.duration < 0)
    {
        detailInfo.duration = 0;
    }
    //Set the text data according to the analysis result.
    detailInfo.textLists[Time]=
            KNMusicUtil::msecondToString(detailInfo.duration);
    detailInfo.textLists[BitRate]=
            QString::number(detailInfo.bitRate)+" Kbps";
    detailInfo.textLists[SampleRate]=
            QString::number((qreal)(detailInfo.samplingRate)/1000)+" kHz";
}

void KNMusicParser::parseTrackList(const QString &filePath,
                                   QList<KNMusicAnalysisItem> &trackItemList)
{
    //Get the track list file.
    QFile listFile(filePath);
    //Open the track list file at read only mode.
    if(listFile.open(QIODevice::ReadOnly))
    {
        //Using all the tag parser parse the list.
        for(auto i=m_listParsers.constBegin();
            i!=m_listParsers.constEnd();
            ++i)
        {
            //Seeks to the start of input.
            listFile.reset();
            //Generate the temporary list detail info.
            KNMusicListDetailInfo listDetailInfo;
            //If there's one parser can parse this file, that means we find the
            //the right parser to do this.
            if((*i)->parseList(listFile, listDetailInfo))
            {
                //Generate the current file path.
                QString currentMusicFile;
                //Parse the music file which the list point to.
                //Prepare the analysis info.
                KNMusicAnalysisItem currentItem;
                //Prepare the detail info.
                KNMusicDetailInfo &musicDetailInfo=currentItem.detailInfo;
                //Set track count.
                musicDetailInfo.textLists[TrackCount]=
                        QString::number(listDetailInfo.trackList.size());
                //Set the track file path.
                musicDetailInfo.trackFilePath=filePath;
                //Check all the track data.
                for(auto track : listDetailInfo.trackList)
                {
                    //Check whether the file path change.
                    if(currentMusicFile!=track.musicFilePath)
                    {
                        //Update the current music file.
                        currentMusicFile=track.musicFilePath;
                        //Update the current item.
                        //Parse the music file.
                        parseFile(currentMusicFile, currentItem);
                        //Set the meta data to the template detail info.
                        for(auto j=listDetailInfo.metaData.begin();
                            j!=listDetailInfo.metaData.end();
                            ++j)
                        {
                            //Set the global metadata to detail info.
                            musicDetailInfo.textLists[j.key()]=j.value();
                        }
                    }
                    //Generate current track info from template.
                    KNMusicAnalysisItem trackItem=currentItem;
                    KNMusicDetailInfo &trackInfo=trackItem.detailInfo;
                    //Set the text data.
                    for(auto j=track.metaData.begin();
                        j!=track.metaData.end();
                        ++j)
                    {
                        //Set the track metadata to detail info.
                        trackInfo.textLists[j.key()]=j.value();
                    }
                    //Set the track number.
                    trackInfo.trackIndex=track.index;
                    trackInfo.textLists[TrackNumber]=
                            QString::number(track.index);
                    //Set the track position.
                    trackInfo.startPosition=track.startPosition;
                    //Calculate the duration, check whether is the duration -1,
                    //If so, means this track is to the last of the music file.
                    trackInfo.duration=track.trackDuration==-1?
                                (musicDetailInfo.duration-track.startPosition):
                                track.trackDuration;
                    //Make sure that the duration is greater than 0.
                    if(trackInfo.duration<0)
                    {
                        trackInfo.duration=0;
                    }
                    //Update the time text.
                    trackInfo.textLists[Time]=
                            KNMusicUtil::msecondToString(trackInfo.duration);
                    //Add track item to detail list.
                    trackItemList.append(trackItem);
                }
                //Stop to try other parser, break the looping.
                break;
            }
        }
        //Close the file.
        listFile.close();
    }
}

void KNMusicParser::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //The analysis item should contains the data when the tag parser parse the
    //tag. If the image data is not empty, then try all the parser.
    if(!analysisItem.imageData.isEmpty())
    {
        //Using all the tag parser to parse the album art.
        for(auto i=m_tagParsers.begin();
            i!=m_tagParsers.end();
            ++i)
        {
            (*i)->parseAlbumArt(analysisItem);
        }
    }

    //Now the cover image shouldn't be null if tag includes the image.
    //But sometimes the tag contains a null image or it doesn't contains image,
    //we have to find the external album art image.
    if(analysisItem.coverImage.isNull())
    {
        //Get the file info of the music file.
        QFileInfo musicFileInfo(analysisItem.detailInfo.filePath);
        //Try to find external images, here is the policy:
        // 1. Find the same file name in the same folder.
        if(findImageFile(musicFileInfo.absolutePath() + "/" +
                         musicFileInfo.completeBaseName(), analysisItem) ||
           findImageFile(musicFileInfo.absolutePath() + "/" +
                         musicFileInfo.baseName(), analysisItem))
        {
            return;
        }
        // 2. Find the 'cover' named image in the save folder.
        if(findImageFile(musicFileInfo.absoluteFilePath() + "/cover",
                         analysisItem))
        {
            return;
        }
        //3. Find the file name contains 'cover' in the same folder.
        QDir musicDir(musicFileInfo.absoluteDir());
        QFileInfoList dirFileList=musicDir.entryInfoList();
        for(auto i=dirFileList.begin();
            i!=dirFileList.end();
            ++i)
        {
            //Ignore dot and dot dot.
            if((*i).fileName()=="." || (*i).fileName()=="..")
            {
                continue;
            }
            //Check if the current item is image type. Then find text 'cover' in
            //the file name.
            //If we can finished those steps, then load this file as cover
            //image.
            if(m_imageTypes.contains((*i).suffix().toLower()) &&
                    (*i).fileName().toLower().contains("cover") &&
                    checkImageFile((*i).absoluteFilePath(), analysisItem))
            {
                break;
            }
        }
    }
}

bool KNMusicParser::reanalysisItem(KNMusicAnalysisItem &analysisItem)
{
    //Get the detail info of the item.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Check whether this file is an URL detail info, if so, then ignore the
    //detail info checking.
    if(!detailInfo.url.isEmpty())
    {
        //Reanalysis complete.
        return true;
    }
    //Check the type of the anlaysis item, if it's a single file, then we will
    //use the parseFile() and parseAlbumArt() to parse the file.
    if(-1==detailInfo.trackIndex)
    {
        //Use parse function to reanalysis the analysis item.
        parseFile(detailInfo.filePath, analysisItem);
        //Use parseAlbumArt function to update the album art.
        parseAlbumArt(analysisItem);
        //Reanlaysis complete.
        return true;
    }
    //So now we should parse the track number in the track list file.
    QList<KNMusicAnalysisItem> trackItemList;
    //Parse the track list.
    parseTrackList(detailInfo.trackFilePath, trackItemList);
    //Check the size of the list, if it's empty then means parse the list
    //failed.
    if(trackItemList.isEmpty())
    {
        return false;
    }
    //Get the original track index.
    int targetIndex=detailInfo.trackIndex;
    bool hitTarget=false;
    //Find the target index.
    for(auto i : trackItemList)
    {
        //If the index is the i.
        if(i.detailInfo.trackIndex==targetIndex)
        {
            //Hit the target index.
            analysisItem=i;
            //Update the hit target flag.
            hitTarget=true;
            //Exit.
            break;
        }
    }
    //Check whether we hit the target.
    if(!hitTarget)
    {
        //Failed to hit the target.
        return false;
    }
    //Parse the album art.
    parseAlbumArt(analysisItem);
    //Reanlaysis complete.
    return true;
}

bool KNMusicParser::writeAnalysisItem(const KNMusicAnalysisItem &analysisItem)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Generate the music file.
    QFile musicFile(detailInfo.filePath);
    //Open the music file as read only mode.
    if(!musicFile.open(QIODevice::ReadOnly))
    {
        //Faild to write the analysis item.
        return false;
    }
    //Generate a data stream for the music file.
    QDataStream musicDataStream(&musicFile);
    //Generate a empty analysis item.
    KNMusicAnalysisItem item;
    //Generate the written parser pointer.
    QList<KNMusicTagParser *> tagParserList;
    //Tried to parse the file.
    for(auto i : m_tagParsers)
    {
        //Seeks to the start of input.
        musicFile.reset();
        //Check whether we can write the item.
        if(i->parseTag(musicFile, musicDataStream, item) &&
                i->writable())
        {
            //We can parse the file by this parser, check whether this parser is
            //writable and it can save the image data.
            tagParserList.append(i);
        }
    }
    //Check the tag parser.
    if(tagParserList.isEmpty())
    {
        //There's no tagging in the file.
        //We have to detect the file suffix to write new tag into the file.
        //Get the suffix.
        QString &&suffix=QFileInfo(musicFile).suffix().toLower();
        //Check out the suffix.
        if(suffix=="mp3")
        {
            //MP3 file format can write ID3v1, ID3v2 and APEv2 format tag.
            //But we only want ID3v2, because it's the most powerful.
            tagParser("id3v2", tagParserList);
        }
        else if(suffix=="ape")
        {
            //Add APEv2 parser.
            tagParser("apev2", tagParserList);
        }
        //Check whether the tag parser list is still empty.
        if(tagParserList.isEmpty())
        {
            //We cannot write any data.
            return false;
        }
    }
    //First we will set the result to be true.
    bool writeResult=true;
    //Write the tag with all the information.
    for(auto i : tagParserList)
    {
        //Write the tag.
        writeResult=writeResult && (i->writeTag(analysisItem));
    }
    //Use the write tag function to write the tag.
    return writeResult;
}

bool KNMusicParser::writeAlbumArt(const KNMusicAnalysisItem &analysisItem)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Generate the music file.
    QFile musicFile(detailInfo.filePath);
    //Open the music file as read only mode.
    if(!musicFile.open(QIODevice::ReadOnly))
    {
        //Faild to write the analysis item.
        return false;
    }
    //Generate a data stream for the music file.
    QDataStream musicDataStream(&musicFile);
    //Generate a empty analysis item.
    KNMusicAnalysisItem item;
    //Generate the written parser pointer.
    QList<KNMusicTagParser *> tagParserList;
    //Tried to parse the file.
    for(auto i : m_tagParsers)
    {
        //Seeks to the start of input.
        musicFile.reset();
        //Check whether we can write the item.
        if(i->parseTag(musicFile, musicDataStream, item) &&
                i->writeCoverImage())
        {
            //We can parse the file by this parser, check whether this parser is
            //writable and it can save the image data.
            tagParserList.append(i);
        }
    }
    //Check the tag parser.
    if(tagParserList.isEmpty())
    {
        //There's no tagging in the file.
        //We have to detect the file suffix to write new tag into the file.
        //Get the suffix.
        QString &&suffix=QFileInfo(musicFile).suffix().toLower();
        //Check out the suffix.
        if(suffix=="mp3")
        {
            //MP3 file format can write ID3v1, ID3v2 and APEv2 format tag.
            //But we only want ID3v2, because it's the most powerful.
            tagParser("id3v2", tagParserList);
        }
        //Check whether the tag parser list is still empty.
        if(tagParserList.isEmpty())
        {
            //We cannot write any data.
            return false;
        }
    }
    //First we will set the result to be true.
    bool writeResult=true;
    //Write the tag with all the information.
    for(auto i : tagParserList)
    {
        //Write the tag.
        writeResult=writeResult && (i->writeTag(analysisItem));
    }
    //Use the write tag function to write the tag.
    return writeResult;
}

bool KNMusicParser::findImageFile(const QString &baseName,
                                  KNMusicAnalysisItem &item)
{
    //Check all the types in the image suffix list.
    for(auto i=m_imageTypes.begin();
        i!=m_imageTypes.end();
        ++i)
    {
        //If any one of the suffix can be loaded, then return true.
        if(checkImageFile(baseName + "." + (*i), item))
        {
            return true;
        }
    }
    //All failed, return false.
    return false;
}

bool KNMusicParser::checkImageFile(const QString &filePath,
                                   KNMusicAnalysisItem &item)
{
    //Use bool QFileInfo::exists(const QString & file) [static].
    //In the document:
    //   Using this function is faster than using QFileInfo(file).exists() for
    //file system access.
    if(QFileInfo::exists(filePath))
    {
        //Load the image.
        item.coverImage=QImage(filePath);
        //Check the loading result.
        return item.coverImage.isNull();
    }
    //If the file doesn't exist, of course not load success.
    return false;
}

inline void KNMusicParser::tagParser(const QString &parserName,
                                     QList<KNMusicTagParser *> &tagParserList)
{
    //Find all the parser.
    for(auto i : m_tagParsers)
    {
        //Check out the parser name.
        if(i->tagParserName()==parserName)
        {
            //We have find the tag parser, add it to tag parser list.
            tagParserList.append(i);
            //Mission complete.
            return;
        }
    }
}
