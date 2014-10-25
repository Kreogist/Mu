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

void KNMusicProxyModel::removeMusicRow(const int &row)
{
    musicModel()->removeMusicRow(row);
}
