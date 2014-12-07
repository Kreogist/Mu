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

class KNConnectionHandler;
class KNMusicAnalysisExtend;
class KNMusicParser;
class KNMusicAnalysisCache : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicAnalysisCache(QObject *parent = 0);
    KNMusicAnalysisExtend *extend() const;
    void setExtend(KNMusicAnalysisExtend *extend);

signals:
    void analysisNext();
    void requireAppendRow(QList<QStandardItem *> musicRow);
    void analysisComplete(KNMusicDetailInfo detailInfo);

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
    void parseItem(AnalysisItem &currentItem, bool blocked=false);
    QList<AnalysisItem> m_analysisQueue;
    KNMusicAnalysisExtend *m_extend=nullptr;
    KNConnectionHandler *m_extendConnections;
    KNMusicParser *m_parser;
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICANALYSISCACHE_H
