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

QString KNMusicProxyModel::itemText(const int &row, const int &column) const
{
    Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
    //Only for text easy access.
    return roleData(row, column, Qt::DisplayRole).toString();
}

QVariant KNMusicProxyModel::roleData(int row, int column, int role) const
{
    Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
    //Only for easy access.
    return data(index(row, column), role);
}

QString KNMusicProxyModel::filePathFromRow(const int &row)
{
    Q_ASSERT(row>-1 && row<rowCount());
    //Return the file path role data.
    return data(index(row, Name), FilePathRole).toString();
}

QString KNMusicProxyModel::fileNameFromRow(const int &row)
{
    Q_ASSERT(row>-1 && row<rowCount());
    //Return the file path role data.
    return data(index(row, Name), FileNameRole).toString();
}

void KNMusicProxyModel::addPlayTimes(const QModelIndex &sourceIndex)
{
    //Get the destination index.
    QModelIndex playTimesIndex=index(mapFromSource(sourceIndex).row(), Plays);
    //Add the data.
    setData(playTimesIndex,
            data(playTimesIndex, Qt::DisplayRole).toInt()+1,
            Qt::DisplayRole);
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
