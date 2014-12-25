/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICPROXYMODEL_H
#define KNMUSICPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "knmusicglobal.h"

using namespace KNMusic;

class KNMusicModel;
class KNMusicProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicProxyModel(QObject *parent = 0);
    KNMusicModel *musicModel();
    int playingItemColumn();
    inline QString itemText(const int &row, const int &column) const
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for text easy access.
        return roleData(row, column, Qt::DisplayRole).toString();
    }
    inline QVariant rowProperty(const int &row, const int &propertyRole)
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //All the property of a song is stored in the first item.
        return roleData(row, 0, propertyRole);
    }
    inline QVariant roleData(int row, int column, int role) const
    {
        Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
        //Only for easy access.
        return data(index(row, column), role);
    }
    inline QString filePathFromRow(const int &row)
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //Return the file path role data.
        return data(index(row, Name), FilePathRole).toString();
    }
    inline QString fileNameFromRow(const int &row)
    {
        Q_ASSERT(row>-1 && row<rowCount());
        //Return the file path role data.
        return data(index(row, Name), FileNameRole).toString();
    }
    inline void addPlayTimes(const QModelIndex &sourceIndex)
    {
        //Get the destination index.
        QModelIndex playTimesIndex=index(mapFromSource(sourceIndex).row(), Plays);
        //Add the data.
        setData(playTimesIndex,
                data(playTimesIndex, Qt::DisplayRole).toInt()+1,
                Qt::DisplayRole);
    }

signals:

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public slots:
    void updateMusicRow(const int &row,
                        const KNMusicDetailInfo &detailInfo);
    void removeMusicRow(const int &row);
    void removeSourceMusicRow(const int &row);
};

#endif // KNMUSICPROXYMODEL_H
