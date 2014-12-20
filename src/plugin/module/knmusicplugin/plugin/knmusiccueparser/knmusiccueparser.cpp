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
#include <QTextStream>

#include "knmusiccueparser.h"

#include <QDebug>

KNMusicCueParser::KNMusicCueParser(QObject *parent) :
    KNMusicListParser(parent)
{
    m_publicCommandList["GENRE"]=Genre;
    m_publicCommandList["DATE"]=Year;
    m_publicCommandList["COMMENT"]=Comments;

    m_trackCommandList["TITLE"]=Name;
    m_trackCommandList["PERFORMER"]=Artist;
    m_trackCommandList["ISRC"]=Comments;

    m_albumCommandList["TITLE"]=Album;
    m_albumCommandList["PERFORMER"]=AlbumArtist;
    m_albumCommandList["ISRC"]=Comments;
}

bool KNMusicCueParser::parseList(QFile &listFile,
                                 KNMusicListDetailInfo &listDetailInfo)
{
    //Initial the file as a text file.
    QTextStream trackStream(&listFile);
    //Read the first text line.
    QString rawLine=trackStream.readLine().simplified();
    //Read every line until no text line.
    while(!rawLine.isNull())
    {
        QString rawCommand, rawData;
        parseCommand(rawLine, rawCommand, rawData);
        if(rawCommand=="FILE")
        {
            //Get the file name.
            QString musicFilePath=
                    rawLine.left(rawLine.lastIndexOf('\"'));
            musicFilePath.remove(0, musicFilePath.indexOf('\"')+1);
            //Because the cue file must be the same directory of the music file,
            //so we need to combine the music file path.
            QFileInfo listFileInfo(listFile);
            musicFilePath=listFileInfo.absolutePath()+"/"+musicFilePath;
            //Check is the music file exist.
            QFileInfo musicFileInfo(musicFilePath);
            if(musicFileInfo.exists())
            {
                musicFilePath=musicFileInfo.absoluteFilePath();
            }
            else
            {
                //If there's no music file, try to find the music file using the
                //same name.
                musicFilePath=listFileInfo.absolutePath()+"/"+
                        listFileInfo.completeBaseName()+"."+musicFileInfo.suffix();
                QFileInfo preferMusicFileInfo(musicFilePath);
                if(!preferMusicFileInfo.exists())
                {
                    //If we still can't find this, then is real failed.
                    return false;
                }
                musicFilePath=preferMusicFileInfo.absoluteFilePath();
            }
            //Save the path to listDetailInfo.
            listDetailInfo.musicFilePath=musicFilePath;

            //Now there must all be track data.
            QString trackLine=trackStream.readLine().simplified();
            parseCommand(trackLine, rawCommand, rawData);
            //Do parse until the rawCommand is not "TRACK".
            while(rawCommand=="TRACK" && !trackLine.isNull())
            {
                //Get the current Track Index.
                int trackIndex=rawData.left(rawData.indexOf(' ')).toInt();
                if(trackIndex==0)
                {
                    trackLine=trackStream.readLine();
                    parseCommand(trackLine, rawCommand, rawData);
                    //Read line until the next track.
                    while(rawCommand!="TRACK")
                    {
                        trackLine=trackStream.readLine();
                        parseCommand(trackLine, rawCommand, rawData);
                    }
                    continue;
                }
                //Here must be at least track 01.
                KNMusicListTrackDetailInfo currentTrack;
                //Parse all the data in the track.
                currentTrack.index=trackIndex;
                trackLine=trackStream.readLine().simplified();
                parseCommand(trackLine, rawCommand, rawData);
                while(rawCommand!="TRACK" && !trackLine.isNull())
                {
                    if(rawCommand=="INDEX")
                    {
                        //Here's the important part, INDEX command record the
                        //start time(INDEX 01) and stop time for last track
                        //(INDEX 00).
                        int indexSplitter=rawData.indexOf(' '),
                                indexID=rawData.left(indexSplitter).toInt();
                        QString indexRecordTime=rawData.mid(indexSplitter+1);
                        //Set the track index.
                        switch(indexID)
                        {
                        case 0:
                            //Check is there a track in the list.
                            if(!listDetailInfo.trackList.isEmpty())
                            {
                                listDetailInfo.trackList.last().trackDuration
                                        =timeTextToPosition(indexRecordTime)-
                                         listDetailInfo.trackList.last().startPosition;
                            }
                            break;
                        case 1:
                            //Set the start position.
                            currentTrack.startPosition=
                                    timeTextToPosition(indexRecordTime);
                            //Check is there a track in the list.
                            if(!listDetailInfo.trackList.isEmpty())
                            {
                                if(listDetailInfo.trackList.last().trackDuration==-1)
                                {
                                    //Check the is the last track's track duration
                                    //-1. If so, using the current start as it's
                                    //stop.
                                    listDetailInfo.trackList.last().trackDuration
                                            =currentTrack.startPosition-
                                            listDetailInfo.trackList.last().startPosition;
                                }
                            }
                            //Check is the start position available.
                            if(currentTrack.startPosition==-1)
                            {
                                return false;
                            }
                            break;
                        }
                    }
                    else
                    {
                        int commandIndex;
                        QString commandData;
                        //It must be a metadata.
                        parseMetaCommand(rawCommand,
                                         rawData,
                                         commandIndex,
                                         commandData,
                                         true);
                        //If there's a command we can't parse, that means parse failed.
                        if(commandIndex!=-1)
                        {
                            currentTrack.metaData[commandIndex]=
                                    commandIndex==Comments?
                                        currentTrack.metaData[commandIndex]+commandData:
                                        commandData;
                        }
                    }
                    //Parse next line.
                    trackLine=trackStream.readLine().simplified();
                    parseCommand(trackLine, rawCommand, rawData);
                }
                //Add current track to track list.
                listDetailInfo.trackList.append(currentTrack);
            }
            //If there's no track in the list, means parse failed.
            return listDetailInfo.trackList.size()!=0;
        }
        //This command must be a meta data.
        int commandIndex;
        QString commandData;
        //Parse the current command.
        parseMetaCommand(rawCommand,
                         rawData,
                         commandIndex,
                         commandData,
                         false);
        //If there's a command we can't parse, that means parse failed.
        if(commandIndex!=-1)
        {
            //Add current data to list detail info's data.
            listDetailInfo.metaData[commandIndex]=
                    commandIndex==Comments?
                        listDetailInfo.metaData[commandIndex]+commandData:
                        commandData;
        }
        //Read the next line.
        rawLine=trackStream.readLine().simplified();
    }
    return true;
}

inline void KNMusicCueParser::parseCommand(const QString &rawLine,
                                           QString &command,
                                           QString &commandData)
{
    int commandSpliter=rawLine.indexOf(' ');
    command=rawLine.left(commandSpliter);
    commandData=rawLine.mid(commandSpliter+1).simplified();
}

inline void KNMusicCueParser::parseMetaCommand(const QString &command,
                                               const QString &data,
                                               int &commandIndex,
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
        //Get the command in the comment.
        int spliter=data.indexOf(' ');
        QString commentCommand=data.left(spliter);
        //Find the comment command, these command must be in public command list
        commandIndex=m_publicCommandList.value(commentCommand, -1);
        //Check is the index vaild.
        if(commandIndex==-1)
        {
            metaData="";
        }
        else
        {
            QString commentCommandData=data.mid(spliter+1);
            //Some data is like this:
            //  TITLE "My Soul,Your Beats!"
            //We need to remove the quote.
            metaData=commentCommandData.at(0)=='\"'?
                        commentCommandData.mid(1, metaData.length()-2):
                        commentCommandData;
        }
    }
    else
    {
        //Get the index.
        commandIndex=commandToIndex(command, inTrack);
        //Check is the index vaild.
        if(commandIndex==-1)
        {
            metaData="";
        }
        else
        {
            //Do the quote check.
            metaData=data.at(0)=='\"'?
                        data.mid(1, data.length()-2):
                        data;
        }
    }
}

inline qint64 KNMusicCueParser::timeTextToPosition(const QString &cueTimeText)
{
    //Check is the text vaild.
    if(cueTimeText.at(2)==':' && cueTimeText.at(5)==':')
    {
        //CUE's time is a very strange time:
        //  mm:ss:ff
        //mm=minuate, ss=second, ff=frame. and 1second = 75frames.
        //WTF?! Why not 1000ms?!
        return cueTimeText.left(2).toInt()*60000+
                cueTimeText.mid(3,2).toInt()*1000+
                cueTimeText.right(2).toDouble()*13.3;
    }
    return -1;
}

inline int KNMusicCueParser::commandToIndex(const QString &command,
                                            const bool &inTrack)
{
    //Check is it in the public command list.
    return inTrack?m_trackCommandList.value(command, -1):
                   m_albumCommandList.value(command, -1);
}
