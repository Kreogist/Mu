/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICANALYSISEXTEND_H
#define KNMUSICANALYSISEXTEND_H

#include <QObject>

#include "knmusicglobal.h"

using namespace KNMusic;

class KNMusicAnalysisExtend : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicAnalysisExtend(QObject *parent = 0);

signals:
    void requireAppendRow(QList<QStandardItem *> musicRow);

public slots:
    virtual void onActionAnalysisComplete(const KNMusicDetailInfo &detailInfo);
};

#endif // KNMUSICANALYSISEXTEND_H
