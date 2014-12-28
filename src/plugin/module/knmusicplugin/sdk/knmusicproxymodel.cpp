/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicmodel.h"

#include "knmusicproxymodel.h"

KNMusicProxyModel::KNMusicProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    //Set properties.
    setFilterKeyColumn(-1); //Read from all columns.
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

KNMusicModel *KNMusicProxyModel::musicModel()
{
    return (KNMusicModel *)sourceModel();
}

int KNMusicProxyModel::playingItemColumn()
{
    return musicModel()->playingItemColumn();
}

KNMusicDetailInfo KNMusicProxyModel::detailInfoFromRow(const int &row)
{
    return musicModel()->detailInfoFromRow(sourceRow(row));
}

inline int KNMusicProxyModel::sourceRow(const int &proxyRow) const
{
    Q_ASSERT(proxyRow>-1 && proxyRow<rowCount());
    return mapToSource(index(proxyRow, 0)).row();
}

bool KNMusicProxyModel::lessThan(const QModelIndex &left,
                                 const QModelIndex &right) const
{
    QVariant sortFlag=sourceModel()->headerData(left.column(),
                                                Qt::Horizontal,
                                                Qt::UserRole);
    if(!sortFlag.isValid())
    {
        return QSortFilterProxyModel::lessThan(left, right);
    }
    QVariant leftUserData=sourceModel()->data(left, Qt::UserRole),
             rightUserData=sourceModel()->data(right, Qt::UserRole);
    switch(sortFlag.toInt())
    {
    case SortByInt:
        return sourceModel()->data(left, Qt::DisplayRole).toInt()<
                sourceModel()->data(right, Qt::DisplayRole).toInt();
    case SortUserByInt:
        return leftUserData.toInt()<rightUserData.toInt();
    case SortUserByFloat:
        return leftUserData.toFloat()<rightUserData.toFloat();
    case SortUserByDate:
        return leftUserData.toDateTime()<rightUserData.toDateTime();
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

void KNMusicProxyModel::updateMusicRow(const int &row,
                                       const KNMusicDetailInfo &detailInfo)
{
    musicModel()->updateMusicRow(mapToSource(index(row, Name)).row(),
                                 detailInfo);
}

void KNMusicProxyModel::removeMusicRow(const int &row)
{
    musicModel()->removeMusicRow(mapToSource(index(row, Name)).row());
}

void KNMusicProxyModel::removeSourceMusicRow(const int &row)
{
    musicModel()->removeMusicRow(row);
}
