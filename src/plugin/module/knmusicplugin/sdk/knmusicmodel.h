/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QStandardItemModel>

class KNMusicModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);

signals:

public slots:

};

#endif // KNMUSICMODEL_H
