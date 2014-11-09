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
    QString itemText(const int &row, const int &column) const;
    QVariant roleData(int row, int column, int role) const;
    QString filePathFromRow(const int &row);
    QString fileNameFromRow(const int &row);
    void addPlayTimes(const QModelIndex &sourceIndex);

signals:

public slots:
    void updateMusicRow(const int &row,
                        const KNMusicDetailInfo &detailInfo);
    void removeMusicRow(const int &row);
    void removeSourceMusicRow(const int &row);
};

#endif // KNMUSICPROXYMODEL_H
