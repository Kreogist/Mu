/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICANALYSISCACHE_H
#define KNMUSICANALYSISCACHE_H

#include <QList>
#include <QStandardItem>

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNMusicParser;
class KNMusicModelAssist;
class KNMusicAnalysisCache : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicAnalysisCache(QObject *parent = 0);

signals:
    void analysisNext();
    void requireAppendMusicRow(QList<QStandardItem *> musicRow);

public slots:
    void appendFilePath(const QString &filePath);
    void analysisFile(const QString &filePath);
    void onActionAnalysisNext();

private:
    struct AnalysisItem
    {
        QString filePath;
        KNMusicDetailInfo detailInfo;
    };
    void parseItem(AnalysisItem &currentItem);
    QList<AnalysisItem> m_analysisQueue;
    KNMusicParser *m_parser;
    KNMusicGlobal *m_musicGlobal;
    KNMusicModelAssist *m_musicModelAssist;
};

#endif // KNMUSICANALYSISCACHE_H
