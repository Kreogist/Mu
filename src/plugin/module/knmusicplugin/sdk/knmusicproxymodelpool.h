/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICPROXYMODELPOOL_H
#define KNMUSICPROXYMODELPOOL_H

#include "knmusicproxymodel.h"

#include <QObject>

class KNMusicProxyModelPool : public QObject
{
    Q_OBJECT
public:
    ~KNMusicProxyModelPool();
    static KNMusicProxyModelPool *instance();
    KNMusicProxyModel *alloct();
    bool occupy(KNMusicProxyModel *model);
    bool release(KNMusicProxyModel *model);
    bool isOccupied(KNMusicProxyModel *model);

signals:

public slots:

private:
    static KNMusicProxyModelPool *m_instance;
    explicit KNMusicProxyModelPool(QObject *parent = 0);
    QList<KNMusicProxyModel *> m_available,
                               m_occupation;
};

#endif // KNMUSICPROXYMODELPOOL_H
