/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicmodelassist.h"

KNMusicModelAssist::KNMusicModelAssist(QObject *parent) :
    QObject(parent)
{
}

KNMusicModelAssist *KNMusicModelAssist::m_instance=nullptr;

KNMusicModelAssist *KNMusicModelAssist::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicModelAssist:m_instance;
}

QList<QStandardItem *> KNMusicModelAssist::generateRow(
        const KNMusicDetailInfo &detailInfo)
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
    item->setEditable(true);
    item=musicRow.at(AlbumRating);
    item->setEditable(true);
    return musicRow;
}

QString KNMusicModelAssist::dateTimeToString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyy-MM-dd APhh:mm");
}

QString KNMusicModelAssist::dateTimeToDataString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyyMMddHHmmss");
}

QDateTime KNMusicModelAssist::dataStringToDateTime(const QString &text)
{
    return QDateTime::fromString(text, "yyyyMMddHHmmss");
}
