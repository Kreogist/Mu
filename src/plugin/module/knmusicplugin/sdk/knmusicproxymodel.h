/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICPROXYMODEL_H
#define KNMUSICPROXYMODEL_H

#include <QSortFilterProxyModel>

class KNMusicModel;
class KNMusicProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicProxyModel(QObject *parent = 0);
    KNMusicModel *musicModel();

signals:

public slots:

};

#endif // KNMUSICPROXYMODEL_H
