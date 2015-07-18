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
#include <QFileInfo>
#include <QDir>

#include "knglobal.h"

#include "knmusicglobal.h"
#include "knmusicanalysiser.h"
#include "knmusictagparser.h"
#include "knmusiclistparser.h"

#include "knmusicparser.h"

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

void KNMusicParser::parseFile(QString filePath,
                              KNMusicAnalysisItem &analysisItem)
{
    //Get the file info class of the current analysis item.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Set the database added time to current time.
    detailInfo.dateAdded=QDateTime::currentDateTime();
    //Get the file info of the music file.
    QFileInfo fileInfo(filePath);
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
            KNMusicUtil::dateTimeToString(detailInfo.dateAdded);
    detailInfo.textLists[DateModified]=
            KNMusicUtil::dateTimeToString(detailInfo.dateModified);
    detailInfo.textLists[LastPlayed]=
            KNMusicUtil::dateTimeToString(detailInfo.dateLastPlayed);
    //Get the description of the suffix.
    detailInfo.textLists[Kind]=
            knMusicGlobal->typeDescription(fileInfo.suffix());
    //Analysis music file.
    //Step 1, parse the tag of the music file.
    QFile file(filePath);
    //Open the music file at read only mode.
    if(file.open(QIODevice::ReadOnly))
    {
        //Initial a binary data stream for music file reading.
        QDataStream dataStream(&file);
        //Using all the tag parser parse the data.
        for(auto i=m_tagParsers.constBegin();
            i!=m_tagParsers.constEnd();
            ++i)
        {
            //Seeks to the start of input.
            file.reset();
            //Parse the file using the tag parser.
            (*i)->parseTag(file, dataStream, analysisItem);
        }
        //Close the music file after parsing.
        file.close();
    }
    //Step 2, parse the detail information of the music file. e.g. duration.
    //Using all the analysiser to analysis file.
    //If there's one analysiser can analysis this, exit.
    for(auto i=m_analysisers.constBegin();
            i!=m_analysisers.constEnd();
            ++i)
    {
        if((*i)->analysis(detailInfo))
        {
            break;
        }
    }
    //Check the duration. If the duration of the file is lesser than 0, make it
    //to 0.
    if(detailInfo.duration<0)
    {
        detailInfo.duration=0;
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
                                   QList<KNMusicAnalysisItem> &trackDetailList)
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
                //Parse the music file which the list point to.
                //Step 1, prepare the analysis info.
                KNMusicAnalysisItem currentItem;
                //Parse the music file.
                parseFile(listDetailInfo.musicFilePath, currentItem);
                //Generate the template detail info.
                KNMusicDetailInfo &musicDetailInfo=currentItem.detailInfo;
                musicDetailInfo.trackFilePath=filePath;
                //Set the meta data to the template detail info.
                while(!listDetailInfo.metaData.isEmpty())
                {
                    int firstKey=listDetailInfo.metaData.firstKey();
                    musicDetailInfo.textLists[firstKey]=
                            listDetailInfo.metaData.take(firstKey);
                }
                //Set track count.
                musicDetailInfo.textLists[TrackCount]=
                        QString::number(listDetailInfo.trackList.size());
                //Generate track data.
                while(!listDetailInfo.trackList.isEmpty())
                {
                    //Take the track info.
                    KNMusicListTrackDetailInfo track=
                            listDetailInfo.trackList.takeFirst();
                    //Generate current track info from template.
                    KNMusicAnalysisItem trackItem=currentItem;
                    KNMusicDetailInfo &trackInfo=trackItem.detailInfo;
                    //Set the text data.
                    while(!track.metaData.isEmpty())
                    {
                        int firstKey=track.metaData.firstKey();
                        trackInfo.textLists[firstKey]=
                                track.metaData.take(firstKey);
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
                    trackInfo.textLists[Time]=
                            KNMusicUtil::msecondToString(trackInfo.duration);
                    //Add track item to detail list.
                    trackDetailList.append(trackItem);
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
