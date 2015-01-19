/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicparser.h"
#include "knmusicmodel.h"

#include "knmusicmodelassist.h"

#include <QDebug>

KNMusicModelAssist::KNMusicModelAssist(QObject *parent) :
    QObject(parent)
{
}

KNMusicModelAssist *KNMusicModelAssist::m_instance=nullptr;

KNMusicModelAssist *KNMusicModelAssist::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicModelAssist:m_instance;
}

QList<QStandardItem *> KNMusicModelAssist::generateRow(const KNMusicDetailInfo &detailInfo)
{
    QList<QStandardItem *> musicRow;
    QStandardItem *item;
    for(int i=0; i<MusicDataCount; i++)
    {
        item=new QStandardItem(detailInfo.textLists[i]);
        item->setEditable(false);
        musicRow.append(item);
    }
    item=musicRow.at(Name);
    item->setData(detailInfo.filePath, FilePathRole);
    item->setData(detailInfo.fileName, FileNameRole);
    item->setData(detailInfo.trackFilePath, TrackFileRole);
    item->setData(detailInfo.coverImageHash, ArtworkKeyRole);
    item->setData(detailInfo.startPosition, StartPositionRole);
    item=musicRow.at(Size);
    item->setData(detailInfo.size, Qt::UserRole);
    item->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    item=musicRow.at(DateModified);
    item->setData(detailInfo.dateModified, Qt::UserRole);
    item=musicRow.at(DateAdded);
    item->setData(detailInfo.dateAdded, Qt::UserRole);
    item=musicRow.at(LastPlayed);
    item->setData(detailInfo.lastPlayed, Qt::UserRole);
    item=musicRow.at(Time);
    item->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    item->setData(detailInfo.duration, Qt::UserRole);
    item=musicRow.at(BitRate);
    item->setData(detailInfo.bitRate, Qt::UserRole);
    item=musicRow.at(SampleRate);
    item->setData(detailInfo.samplingRate, Qt::UserRole);
    item=musicRow.at(Rating);
    item->setData(detailInfo.rating, Qt::DisplayRole);
    item->setEditable(true);
    item=musicRow.at(AlbumRating);
    item->setEditable(true);
    return musicRow;
}

bool KNMusicModelAssist::reanalysisRow(KNMusicModel *musicModel,
                                       const QPersistentModelIndex &index,
                                       KNMusicAnalysisItem &analysisItem)
{
    //Get the parser.
    KNMusicParser *parser=KNMusicGlobal::parser();
    //Get the file path.
    QString musicFilePath=musicModel->rowProperty(index.row(),
                                                  FilePathRole).toString();
    //Check the start position role, if it is not -1, means it's a music file.
    if(musicModel->rowProperty(index.row(),StartPositionRole).toLongLong()==-1)
    {
        //Parse the info.
        parser->parseFile(musicFilePath, analysisItem);
        parser->parseAlbumArt(analysisItem);
        return true;
    }
    QList<KNMusicAnalysisItem> currentTrackInfo;
    //Get the track number.
    int currentTrackNumber=musicModel->roleData(index.row(),
                                                TrackNumber,
                                                Qt::DisplayRole).toInt();
    //Parse the list first.
    parser->parseTrackList(musicModel->rowProperty(index.row(),
                                                   TrackFileRole).toString(),
                           currentTrackInfo);
    //No list parsed.
    if(currentTrackInfo.isEmpty())
    {
        return false;
    }
    //Check the beginning of the track list, if it's 0, means the track is indexed at:
    // 0 1 2 3 ...
    //Or else, it is start at 1, we need to reduce the track number.
    if(currentTrackInfo.first().detailInfo.textLists[TrackNumber].toInt()!=0)
    {
        currentTrackNumber--;
        //Still need to check the tracknumber.
        if(currentTrackNumber<0)
        {
            currentTrackNumber=0;
        }
    }
    //Check the size of the new track we get.
    if(currentTrackInfo.size()>currentTrackNumber)
    {
        //Get the track number's current info.
        analysisItem=currentTrackInfo.at(currentTrackNumber);
        //Parse the album art.
        parser->parseAlbumArt(analysisItem);
        return true;
    }
    return false;
}

QString KNMusicModelAssist::dateTimeToString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyy-MM-dd APhh:mm");
}

QString KNMusicModelAssist::dateTimeToDataString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyyMMddHHmmss");
}

QString KNMusicModelAssist::dateTimeToDataString(const QVariant &dateTime)
{
    return dateTime.toDateTime().toString("yyyyMMddHHmmss");
}

QDateTime KNMusicModelAssist::dataStringToDateTime(const QString &text)
{
    return QDateTime::fromString(text, "yyyyMMddHHmmss");
}
