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
