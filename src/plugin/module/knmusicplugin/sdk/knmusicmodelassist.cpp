/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QJsonDocument>

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
    item->setData(detailInfo.trackIndex, TrackIndexRole);
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

QList<QStandardItem *> KNMusicModelAssist::generateRow(const QJsonArray &itemDataArray)
{
    //Get the information and property array.
    QJsonArray textInformationArray=itemDataArray.at(0).toArray(),
               propertyArray=itemDataArray.at(1).toArray();
    //Generate the row items.
    QList<QStandardItem *> musicRow;
    QStandardItem *item;
    for(int i=0; i<MusicDataCount; i++)
    {
        item=new QStandardItem(textInformationArray.at(i).toString());
        item->setEditable(false);
        musicRow.append(item);
    }
    item=musicRow.at(Name);
    item=musicRow.at(Name);
    item->setData(propertyArray.at(PropertyFilePath).toString(), FilePathRole);
    item->setData(propertyArray.at(PropertyFileName).toString(), FileNameRole);
    item->setData(propertyArray.at(PropertyTrackFilePath).toString(), TrackFileRole);
    item->setData(propertyArray.at(PropertyTrackIndex).toInt(), TrackIndexRole);
    item->setData(propertyArray.at(PropertyCoverImageHash).toString(), ArtworkKeyRole);
    item->setData(propertyArray.at(PropertyStartPosition).toString().toLongLong(), StartPositionRole);
    item=musicRow.at(Size);
    item->setData(propertyArray.at(PropertySize).toString().toLongLong(), Qt::UserRole);
    item->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    item=musicRow.at(DateModified);
    item->setData(KNMusicModelAssist::dataStringToDateTime(propertyArray.at(PropertyDateModified).toString()),
                  Qt::UserRole);
    item=musicRow.at(DateAdded);
    item->setData(KNMusicModelAssist::dataStringToDateTime(propertyArray.at(PropertyDateAdded).toString()),
                  Qt::UserRole);
    item=musicRow.at(LastPlayed);
    item->setData(KNMusicModelAssist::dataStringToDateTime(propertyArray.at(PropertyLastPlayed).toString()),
                  Qt::UserRole);
    item=musicRow.at(Time);
    item->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    item->setData(propertyArray.at(PropertyDuration).toString().toLongLong(), Qt::UserRole);
    item=musicRow.at(BitRate);
    item->setData(propertyArray.at(PropertyBitRate).toInt(), Qt::UserRole);
    item=musicRow.at(SampleRate);
    item->setData(propertyArray.at(PropertySampleRating).toInt(), Qt::UserRole);
    item=musicRow.at(Rating);
    item->setData(propertyArray.at(PropertyRating).toInt(), Qt::DisplayRole);
    item->setEditable(true);
    item=musicRow.at(AlbumRating);
    item->setEditable(true);
    return musicRow;
}

QJsonArray KNMusicModelAssist::rowToJsonArray(KNMusicModel *musicModel,
                                              const int &row)
{
    QJsonArray textInformationArray, propertyArray, itemDataArray;
    for(int i=0; i<MusicDataCount; i++)
    {
        textInformationArray.append(musicModel->itemText(row, i));
    }
    propertyArray.append(musicModel->rowProperty(row, FilePathRole).toString()); //PropertyFilePath
    propertyArray.append(musicModel->rowProperty(row, FileNameRole).toString()); //PropertyFileName
    propertyArray.append(musicModel->rowProperty(row, ArtworkKeyRole).toString()); //PropertyCoverImageHash
    propertyArray.append(musicModel->roleData(row, BitRate, Qt::UserRole).toInt()); //PropertyBitRate
    propertyArray.append(musicModel->roleData(row, Rating, Qt::DisplayRole).toInt()); //PropertyRating
    propertyArray.append(musicModel->roleData(row, SampleRate, Qt::UserRole).toInt()); //PropertySampleRating
    propertyArray.append(QString::number(musicModel->roleData(row, Size, Qt::UserRole).toLongLong())); //PropertySize
    propertyArray.append(QString::number(musicModel->roleData(row, Time, Qt::UserRole).toLongLong())); //PropertyDuration
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(musicModel->roleData(row, DateAdded, Qt::UserRole))); //PropertyDateAdded
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(musicModel->roleData(row, DateModified, Qt::UserRole))); //PropertyDateModified
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(musicModel->roleData(row, LastPlayed, Qt::UserRole))); //PropertyLastPlayed
    propertyArray.append(musicModel->rowProperty(row, TrackFileRole).toString()); //PropertyTrackFilePath
    propertyArray.append(musicModel->rowProperty(row, TrackIndexRole).toInt()); //PropertyTrackIndex
    propertyArray.append(QString::number(musicModel->rowProperty(row, StartPositionRole).toLongLong())); //PropertyStartPosition
    itemDataArray.append(textInformationArray);
    itemDataArray.append(propertyArray);
    return itemDataArray;
}

QJsonArray KNMusicModelAssist::byteDataToJsonArray(const QByteArray &rowData)
{
    return QJsonDocument::fromBinaryData(rowData).array();
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
    int currentTrackNumber=musicModel->rowProperty(index.row(),
                                                   TrackIndexRole).toInt();
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
    if(currentTrackInfo.first().detailInfo.trackIndex!=0)
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
