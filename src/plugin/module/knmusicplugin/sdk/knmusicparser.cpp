/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QFile>
#include <QDataStream>

#include "knglobal.h"

#include "knmusicparser.h"

#include <QDebug>

KNMusicParser::KNMusicParser(QObject *parent) :
    QObject(parent)
{
    m_global=KNGlobal::instance();
    m_musicGlobal=KNMusicGlobal::instance();
}

KNMusicParser::~KNMusicParser()
{
    //Delete all analysisers.
    qDeleteAll(m_analysisers);
    m_analysisers.clear();
    //Delete all tag parsers.
    qDeleteAll(m_tagParsers);
    m_tagParsers.clear();
    //Delete all list parsers.
    qDeleteAll(m_listParsers);
    m_listParsers.clear();
}

void KNMusicParser::parseFile(QString filePath,
                              KNMusicAnalysisItem &analysisItem)
{
    //Get the detail info.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Set the added date.
    detailInfo.dateAdded=QDateTime::currentDateTime();
    QFileInfo fileInfo(filePath);
    //Set detail info.
    detailInfo.filePath=fileInfo.absoluteFilePath();
    detailInfo.fileName=fileInfo.fileName();
    detailInfo.size=fileInfo.size();
    detailInfo.lastPlayed=fileInfo.lastRead();
    detailInfo.dateModified=fileInfo.lastModified();
    //Track index of single file is always -1.
    detailInfo.trackIndex=-1;
    //Generate basic info.
    detailInfo.textLists[Name]=detailInfo.fileName;
    detailInfo.textLists[Size]=m_global->byteToHigherUnit(detailInfo.size);
    detailInfo.textLists[DateAdded]=
            KNMusicGlobal::dateTimeToString(detailInfo.dateAdded);
    detailInfo.textLists[DateModified]=
            KNMusicGlobal::dateTimeToString(detailInfo.dateModified);
    detailInfo.textLists[LastPlayed]=
            KNMusicGlobal::dateTimeToString(detailInfo.lastPlayed);
    detailInfo.textLists[Kind]=
            m_musicGlobal->typeDescription(fileInfo.suffix());
    //Analysis Music.
    parseTag(filePath, analysisItem);
    analysis(filePath, detailInfo);
    //Check the duration.
    if(detailInfo.duration<0)
    {
        detailInfo.duration=0;
    }
    //Generate analysis info.
    detailInfo.textLists[Time]=KNMusicGlobal::msecondToString(detailInfo.duration);
    detailInfo.textLists[BitRate]=bitRateText(detailInfo.bitRate);
    detailInfo.textLists[SampleRate]=sampleRateText(detailInfo.samplingRate);
}

void KNMusicParser::installAnalysiser(KNMusicAnalysiser *analysiser)
{
    m_analysisers.append(analysiser);
}

void KNMusicParser::installTagParser(KNMusicTagParser *tagParser)
{
    m_tagParsers.append(tagParser);
}

void KNMusicParser::installListParser(KNMusicListParser *listParser)
{
    m_listParsers.append(listParser);
}

QString KNMusicParser::bitRateText(const qint64 &bitRateNumber)
{
    return QString::number(bitRateNumber)+" Kbps";
}

QString KNMusicParser::sampleRateText(const qint64 &sampleRateNumber)
{
    return QString::number((qreal)sampleRateNumber/1000)+" kHz";
}

void KNMusicParser::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    //Using all the tag parser try to parse the album art.
    for(auto i=m_tagParsers.begin();
        i!=m_tagParsers.end();
        ++i)
    {
        (*i)->parseAlbumArt(analysisItem);
    }
    if(analysisItem.coverImage.isNull())
    {
        //Try to find external images, here is the policy.
        //  1. Find the same file name in the same folder.
        QFileInfo musicFileInfo(analysisItem.detailInfo.filePath);
        if(findImageFile(musicFileInfo.absolutePath()+"/"+
                         musicFileInfo.completeBaseName(),
                         analysisItem))
        {
            return;
        }
        //  2. Find the "cover" named image in the same folder.
        if(findImageFile(musicFileInfo.absolutePath()+"/cover",
                         analysisItem))
        {
            return;
        }
        //  3. Find the name contains "cover" in the same folder.
        //!FIXME: add code here.
    }
}

void KNMusicParser::parseTag(const QString &filePath,
                             KNMusicAnalysisItem &analysisItem)
{
    QFile musicFile(filePath);
    //Open the music file at read only mode.
    if(musicFile.open(QIODevice::ReadOnly))
    {
        //Initial a binary data stream for music file reading.
        QDataStream musicDataStream(&musicFile);
        //Using all the tag parser parse the data.
        for(auto i=m_tagParsers.begin();
            i!=m_tagParsers.end();
            ++i)
        {
            musicFile.reset();
            (*i)->praseTag(musicFile, musicDataStream, analysisItem);
        }
        //Close the file.
        musicFile.close();
    }
}

void KNMusicParser::parseTrackList(const QString &filePath,
                                   QList<KNMusicAnalysisItem> &trackDetailList)
{
    QFile trackListFile(filePath);
    //Open the track list file at read only mode.
    if(trackListFile.open(QIODevice::ReadOnly))
    {
        //Using all the tag parser parse the list.
        for(QList<KNMusicListParser *>::iterator i=m_listParsers.begin();
            i!=m_listParsers.end();
            ++i)
        {
            trackListFile.reset();
            KNMusicListDetailInfo listDetailInfo;
            //If there's one parser can parse this file, that means we find the
            //the right parser to do this.
            if((*i)->parseList(trackListFile, listDetailInfo))
            {
                //Parse the music file which the list point to.
                KNMusicAnalysisItem currentItem;
                KNMusicDetailInfo &musicFileDetailInfo=currentItem.detailInfo;
                musicFileDetailInfo.trackFilePath=filePath;
                //Parse the tag.
                parseFile(listDetailInfo.musicFilePath,
                          currentItem);
                //Generate the template detail info.
                while(!listDetailInfo.metaData.isEmpty())
                {
                    int firstKey=listDetailInfo.metaData.firstKey();
                    musicFileDetailInfo.textLists[firstKey]=
                            listDetailInfo.metaData.take(firstKey);
                }
                //Set track count.
                musicFileDetailInfo.textLists[TrackCount]=
                        QString::number(listDetailInfo.trackList.size());
                //Generate track data.
                while(!listDetailInfo.trackList.isEmpty())
                {
                    //Take the track info.
                    KNMusicListTrackDetailInfo currentTrack=
                            listDetailInfo.trackList.takeFirst();
                    //Generate current track info from template.
                    KNMusicAnalysisItem currentTrackItem=currentItem;
                    KNMusicDetailInfo &currentInfo=currentTrackItem.detailInfo;
                    //Set the text data.
                    while(!currentTrack.metaData.isEmpty())
                    {
                        int firstKey=currentTrack.metaData.firstKey();
                        currentInfo.textLists[firstKey]=
                                currentTrack.metaData.take(firstKey);
                    }
                    //Set the track number.
                    currentInfo.trackIndex=currentTrack.index;
                    currentInfo.textLists[TrackNumber]=
                            QString::number(currentTrack.index);
                    //Set the track position.
                    currentInfo.startPosition=currentTrack.startPosition;
                    //Calculate the duration, check whether is the duration -1,
                    //If so, means this track is to the last of the music file.
                    currentInfo.duration=currentTrack.trackDuration==-1?
                                (musicFileDetailInfo.duration-currentTrack.startPosition):
                                currentTrack.trackDuration;
                    //To ensure that the duration is greater than 0.
                    if(currentInfo.duration<0)
                    {
                        currentInfo.duration=0;
                    }
                    currentInfo.textLists[Time]=
                            KNMusicGlobal::msecondToString(currentInfo.duration);
                    trackDetailList.append(currentTrackItem);
                }
                break;
            }
        }
        //Close the file.
        trackListFile.close();
    }
}

void KNMusicParser::analysis(const QString &filePath,
                             KNMusicDetailInfo &detailInfo)
{
    //Using all the analysiser to analysis file.
    //If there's one analysiser can analysis this, exit.
    for(auto i=m_analysisers.begin();
        i!=m_analysisers.end();
        ++i)
    {
        if((*i)->analysis(filePath, detailInfo))
        {
            return;
        }
    }
}

bool KNMusicParser::findImageFile(const QString &imageBaseFileName,
                                  KNMusicAnalysisItem &analysisItem)
{
    return  checkImageFile(imageBaseFileName+".jpg", analysisItem) ||
            checkImageFile(imageBaseFileName+".png", analysisItem) ||
            checkImageFile(imageBaseFileName+".jpeg", analysisItem) ||
            checkImageFile(imageBaseFileName+".bmp", analysisItem);
}

bool KNMusicParser::checkImageFile(const QString &imageFilePath,
                                   KNMusicAnalysisItem &analysisItem)
{
    QFileInfo imageFileInfo(imageFilePath);
    if(imageFileInfo.exists())
    {
        analysisItem.coverImage=QImage(imageFileInfo.absoluteFilePath());
        return true;
    }
    return false;
}
