/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICMODELASSIST_H
#define KNMUSICMODELASSIST_H

#include <QList>
#include <QStandardItem>
#include <QDateTime>

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNMusicModelAssist : public QObject
{
    Q_OBJECT
public:
    static KNMusicModelAssist *instance();
    static QString dateTimeToString(const QDateTime &dateTime);
    static QString dateTimeToDataString(const QDateTime &dateTime);
    static QDateTime dataStringToDateTime(const QString &text);
    static QList<QStandardItem *> generateRow(const KNMusicDetailInfo &detailInfo);

signals:

public slots:

private:
    static KNMusicModelAssist *m_instance;
    KNMusicModelAssist(QObject *parent = 0);
};

#endif // KNMUSICMODELASSIST_H
