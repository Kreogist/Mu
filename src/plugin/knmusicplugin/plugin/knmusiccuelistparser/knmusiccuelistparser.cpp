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
#include <QFileInfo>
#include <QTextStream>
#include <QTemporaryFile>
#include <QFile>

#include "knmusiccuelistparser.h"

#include <QDebug>

//1MB data cache copy size.
#define DataCacheSize 1048576

QHash<QString, int> KNMusicCueListParser::m_trackCommandList=
        QHash<QString, int>();
QHash<QString, int> KNMusicCueListParser::m_albumCommandList=
        QHash<QString, int>();
QHash<QString, int> KNMusicCueListParser::m_publicCommandList=
        QHash<QString, int>();

KNMusicCueListParser::KNMusicCueListParser(QObject *parent) :
    KNMusicListParser(parent)
{
    //Check whether the track list is empty.
    if(m_trackCommandList.isEmpty())
    {
        //Initial the public command.
        m_publicCommandList.insert("GENRE", Genre);
        m_publicCommandList.insert("DATE", Year);
        m_publicCommandList.insert("COMMENT", Comments);

        //Initial the track command list hash.
        m_trackCommandList.insert("TITLE", Name);
        m_trackCommandList.insert("PERFORMER", Artist);
        m_trackCommandList.insert("ISRC", Comments);

        //Initial the album global command list hash.
        m_albumCommandList.insert("TITLE", Album);
        m_albumCommandList.insert("PERFORMER", AlbumArtist);
        m_albumCommandList.insert("ISRC", Comments);
    }
}

bool KNMusicCueListParser::parseList(QFile &listFile,
                                     KNMusicListDetailInfo &listDetailInfo)
{
    //Get the file info of the list file.
    QFileInfo listFileInfo(listFile);
    //Initial the file as a text file.
    QTextStream trackStream(&listFile);
    //Read the first text line.
    QString rawLine=trackStream.readLine();
    //Generate the parse cache data.
    QString currentFilePath;            //Current track music file.
    bool inTrack=false;                  //Inside a track or global settings.
    KNMusicListTrackDetailInfo track;   //Track detail info.
    //Check the raw line
    while(!rawLine.isNull())
    {
        //Simplified the line.
        rawLine=rawLine.simplified();
        //Generate command part and data part cache.
        QString rawCommand, rawData;
        //Parse the command.
        parseCommand(rawLine, rawCommand, rawData);
        //Check if command is FILE.
        //In the FILE command, the structure will be like this:
        /*
         * FILE "C:\MYAUDIO.WAV" WAVE
         *  TRACK 01 AUDIO
         *   INDEX 01 00:00:00
         * CATALOG 3898347789120
         * FILE "C:\TRACK2.WAV" WAVE
         *  TRACK 02 AUDIO
         *   INDEX 00 05:49:65  ; 1 second pregap
         *   INDEX 01 05:50:65
         *  TRACK 03 AUDIO
         *   INDEX 00 09:45:50  ; 2 second pregap
         *   INDEX 01 09:47:50
         *  TRACK 04 AUDIO
         *   ISRC ABCDE1234567
         *   INDEX 01 15:12:53
         *  TRACK 05 AUDIO
         *   INDEX 01 25:02:40  ; the track pregap
         *  TRACK 06 AUDIO
         *   TITLE "Stay away"
         *   INDEX 01 27:34:05
         * FILE "TRACK3.WAV" WAVE
         *  TRACK 07 AUDIO
         *   INDEX 01 31:58:53
         *  TRACK 08 AUDIO
         *   INDEX 01 35:08:65
         */
        if(rawCommand=="FILE")
        {
            //Get the file name from the raw data.
            QString filePath=rawLine.left(rawLine.lastIndexOf('\"'));
            filePath.remove(0, filePath.indexOf('\"')+1);
            //Check whether this file is using an absolute path.
            if(QFileInfo::exists(filePath))
            {
                currentFilePath=filePath;
            }
            else
            {
                //It should use a relatively path.
                //Because the cue file is in the same directory of the music
                //file, so we need to combine the music file path.
                filePath=listFileInfo.absoluteDir().filePath(filePath);
                //Check existence.
                if(QFileInfo::exists(filePath))
                {
                    currentFilePath=filePath;
                }
                else
                {
                    //If there's no music file, try to find the music file using
                    //the same name as the cue sheet.
                    filePath=listFileInfo.absoluteDir().filePath(
                                listFileInfo.completeBaseName()+"."+
                                QFileInfo(filePath).suffix());
                    //Check existence.
                    if(!QFileInfo::exists(filePath))
                    {
                        //If we still cannot find the file, then failed to parse
                        //this file.
                        return false;
                    }
                    //Save the file path.
                    currentFilePath=filePath;
                }
            }
            //Update the in file state.
            inTrack=true;
        }
        else if(rawCommand=="TRACK")
        {
            //Check if the previous index is valid.
            if(track.index>-1)
            {
                //Add the previous track to the list.
                listDetailInfo.trackList.append(track);
            }
            //When we get here, means we will have new track.
            //Reset the track detail info.
            track=KNMusicListTrackDetailInfo();
            //Configure the track detail info.
            track.musicFilePath=currentFilePath;
            //The rawData should be like
            //  01 AUDIO
            //The 01 is the track index, get the track index.
            //Use the trackIndex variable find the space temporarily.
            int trackIndex=rawData.indexOf(' ');
            //Get the track index.
            trackIndex=
                    (trackIndex==-1?rawData:rawData.left(trackIndex)).toInt();
            //Check the track index, track index must be start at least 01.
            if(trackIndex>0)
            {
                //Save the track index.
                track.index=trackIndex;
            }
        }
        else if(rawCommand=="INDEX")
        {
            //Check whether we are inside a track, an INDEX command will only
            //available when there's a valid track.
            if(inTrack)
            {
                //Here's the important part, INDEX command record the start
                //time(INDEX 01) and stop time for last track (INDEX 00).
                //Get the current index.
                int indexID=rawData.indexOf(' ');
                QString indexRecord;
                if(indexID!=-1)
                {
                    //Get the index time text.
                    indexRecord=rawData.mid(indexID+1);
                    //Update the index ID.
                    indexID=(indexID==-1?rawData:rawData.left(indexID)).toInt();
                }
                //Check the index ID.
                switch(indexID)
                {
                case -1:    //Invalid.
                    break;
                case 0:     //Stop time for last track.
                {
                    //Check whether there's tracks in the list.
                    if(!listDetailInfo.trackList.isEmpty())
                    {
                        //Get the last track from the list.
                        KNMusicListTrackDetailInfo &lastTrack=
                                listDetailInfo.trackList.last();
                        //Set the duration of the track.
                        lastTrack.trackDuration=
                                textToTime(indexRecord)-lastTrack.startPosition;
                    }
                    break;
                }
                case 1:     //Start time.
                {
                    //Set the start position.
                    track.startPosition=textToTime(indexRecord);
                    //Check the start position.
                    if(track.startPosition==-1)
                    {
                        //Failed to parse the track list.
                        return false;
                    }
                    //Check the previous end duration.
                    //Because the previous one cannot play until to the current
                    //track. If the duration of the previous track is -1, we
                    //need to update the data.
                    //Check whether there's tracks in the list.
                    if(!listDetailInfo.trackList.isEmpty())
                    {
                        //Get the last track from the list.
                        KNMusicListTrackDetailInfo &lastTrack=
                                listDetailInfo.trackList.last();
                        //Set the duration of the track.
                        lastTrack.trackDuration=
                                track.startPosition-lastTrack.startPosition;
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }
        else
        {
            //If the command is not FILE, then it should be metadata.
            /*
             * PERFORMER "BiBi ~ Ayase Eri(Nanjo Yoshino), Nishikino Maki(Pile),
             *                                Yazawa Nico(Tokui Sora) from μ's~"
             * TITLE "Cutie Panther"
             * REM DATE 2013
             * REM DISCID 3507C005
             * REM COMMENT ExactAudioCopy v1.0b2
             */
            //Generate the metadata variable.
            QString metaData;
            //Get the command index and parse the command.
            int commandIndex=parseMetaCommand(rawCommand,
                                              rawData,
                                              metaData,
                                              inTrack);
            //Check the command index, if parse successfully,
            if(commandIndex!=-1)
            {
                if(inTrack && track.index!=-1)
                {
                    //Write the data to the current track.
                    track.metaData.insert(commandIndex, metaData);
                }
                else
                {
                    //Add the current data to list detail info's data.
                    //The later on global variable will over write the previous
                    //one.
                    listDetailInfo.metaData.insert(commandIndex, metaData);
                }
            }
        }
        //Read the next line.
        rawLine=trackStream.readLine();
    }
    //Now the last track is still not be put in the track list.
    //Check if the last track index valid.
    if(track.index>-1)
    {
        //Add the last track to the list.
        listDetailInfo.trackList.append(track);
    }
    //Parse success.
    return true;
}

bool KNMusicCueListParser::writeDetail(const KNMusicAnalysisItem &analysisItem)
{
    //Get the track index.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Prepare the orignial file data and the temporary file data.
    QTemporaryFile updatedListFile;
    QFile listFile(detailInfo.trackFilePath);
    //Open the list file.
    if(!listFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Open the list file.
    if(!updatedListFile.open())
    {
        //Close the opened music file.
        listFile.close();
        return false;
    }
    //Read the list file.
    //Initial the list file and temporary file stream as a text file.
    QTextStream trackStream(&listFile), temporaryStream(&updatedListFile);
    //Read until the track index comes to the track index.
    QString rawLine=trackStream.readLine();
    //Check the raw line.
    while(!rawLine.isNull())
    {
        //Simplified the raw line.
        QString commandRawLine=rawLine.simplified();
        //Generate command part and data part cache.
        QString rawCommand, rawData;
        //Parse the command.
        parseCommand(commandRawLine, rawCommand, rawData);
        //Now we are only taken care about the TRACK.
        if(rawCommand=="TRACK")
        {
            //Use the trackIndex variable find the space temporarily.
            int trackIndex=rawData.indexOf(' ');
            //Get the track index.
            trackIndex=
                    (trackIndex==-1?rawData:rawData.left(trackIndex)).toInt();
            //Check the track index.
            if(trackIndex==detailInfo.trackIndex)
            {
                //Hit the track.
                //First we have to output the raw line data.
                temporaryStream << rawLine << '\n';
                //Then check the detail info data.
                //We have to write out these informations:
                // PERFORMER Artist
                // TITLE Name
                QString trackData;
                trackData.append(generateLine(
                                     "    TITLE ",
                                     detailInfo.textLists[Name].toString()));
                trackData.append(generateLine(
                                     "    PERFORMER ",
                                     detailInfo.textLists[Artist].toString()));
                //Write the track data to temporary file.
                temporaryStream << trackData;
                //Read the original file again until to INDEX, skip all the
                //other data.
                rawLine=trackStream.readLine();
                //Simplified the raw line.
                commandRawLine=rawLine.simplified();
                //Parse the raw line.
                parseCommand(commandRawLine, rawCommand, rawData);
                //Read until we get the INDEX.
                while(!rawLine.isNull() && rawCommand!="INDEX")
                {
                    //Continue skip the file.
                    rawLine=trackStream.readLine();
                    //Simplified the raw line.
                    commandRawLine=rawLine.simplified();
                    //Parse the raw line.
                    parseCommand(commandRawLine, rawCommand, rawData);
                }
                //So now the data should be the first INDEX.
                //Output the data and continue copy the data.
                temporaryStream << rawLine << '\n';
            }
            else
            {
                //Simply output the data.
                temporaryStream << rawLine << '\n';
            }
        }
        else
        {
            //Simply output the data.
            temporaryStream << rawLine << '\n';
        }
        //Read the next line.
        rawLine=trackStream.readLine();
    }
    //Flush the data.
    temporaryStream << flush;
    //Now the data should be all done.
    //Close the list file.
    listFile.close();
    //Reset the temporary file.
    updatedListFile.reset();
    //Reopen the list file, open as write only mode.
    if(!listFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    //Write all the data to list file.
    //Generate the music data cache.
    char *turboCache=new char[DataCacheSize];
    //Now copy all the content from the original file to temporary file.
    int bytesRead=updatedListFile.read(turboCache, DataCacheSize);
    while(bytesRead>0)
    {
        //Write the cache to the list file.
        listFile.write(turboCache, bytesRead);
        //Read new data from the original file to cache.
        bytesRead=updatedListFile.read(turboCache, DataCacheSize);
    }
    //Close the list file and temporary file.
    listFile.close();
    updatedListFile.close();
    //Clear up the turbo cache.
    delete[] turboCache;
    //Written finished.
    return true;
}

inline int KNMusicCueListParser::parseMetaCommand(const QString &command,
                                                  const QString &data,
                                                  QString &metaData,
                                                  const bool &inTrack)
{
    //Check is the command "REM", because there's some metadata like: GENRE and
    //DATE are using in REM by public. e.g.:
    //  REM GENRE J-POP
    //  REM DATE 2010-05-26
    //So we still need to parse these data:
    if(command=="REM")
    {
        //Prepare the comment parse variable.
        QString commentCommand, commentData;
        //Split the comment data.
        parseCommand(data, commentCommand, commentData);
        //Find the command in the public command list.
        int commandIndex=m_publicCommandList.value(commentCommand, -1);
        //Check the validation of the command index.
        if(commandIndex==-1)
        {
            //Clear the meta data.
            metaData.clear();
            //Give back a invalid index.
            return -1;
        }
        //Some data is like this:
        //  TITLE "My Soul,Your Beats!"
        //So now the comment command will be like:
        //  "My Soul,Your Beats!"
        //We need to remove the quote.
        metaData=(commentData.at(0)=='\"' && commentData.length()>2)?
                    commentData.mid(1, commentData.length()-2):
                    commentData;
        //Give back the command index.
        return commandIndex;
    }
    //Now it should be a command in commmand track list.
    //Check the inTrack and we can determine to use which mapper.
    int commandIndex=inTrack?m_trackCommandList.value(command, -1):
                             m_albumCommandList.value(command, -1);
    //Check the validation of the command index.
    if(commandIndex==-1)
    {
        //Clear the meta data.
        metaData.clear();
        //Give back a invalid index.
        return -1;
    }
    //Remove the quote from the data. If there's no quote, just write the data
    //to metadata.
    metaData=(data.at(0)=='\"' && data.length()>2)?
                data.mid(1, data.length()-2):
                data;
    //Give back the command index.
    return commandIndex;
}

inline void KNMusicCueListParser::parseCommand(const QString &rawLine,
                                               QString &command,
                                               QString &data)
{
    //Find the first space.
    int firstSpace=rawLine.indexOf(' ');
    //Check the first space, if we cannot find any space.
    if(!firstSpace)
    {
        //Give the whole line as the command.
        command=rawLine;
        //Mission complete.
        return;
    }
    //Split the command and the data part of the raw data.
    command=rawLine.left(firstSpace).toUpper();
    //Get the data.
    data=rawLine.mid(firstSpace+1);
    //Find the ';' for comment.
    int semicolonIndex=data.indexOf(';');
    //Check whether there's semicolon.
    if(semicolonIndex!=-1)
    {
        //Remove the comment data.
        data=data.left(semicolonIndex).simplified();
    }
    else
    {
        //Or else just simplified the data.
        data=data.simplified();
    }
}

inline QString KNMusicCueListParser::generateLine(const QString &command,
                                                  const QString &data)
{
    //According to the data generate the total line.
    return data.isEmpty()?QString():command+data+"\n";
}

inline qint64 KNMusicCueListParser::textToTime(const QString &cueTimeText)
{
    //CUE's time is a very strange time:
    //  mm:ss:ff
    //mm=minute, ss=second, ff=frame. and 1second = 75frames.
    //WTF?! Why not 1000ms?!
    //Check is the cue time text format correct.
    if(cueTimeText.at(2)==':' && cueTimeText.at(5)==':')
    {
        //Calculate the time.
        return cueTimeText.left(2).toInt()*60000+
                cueTimeText.mid(3,2).toInt()*1000+
                cueTimeText.right(2).toDouble()*13.3;
    }
    return -1;
}

