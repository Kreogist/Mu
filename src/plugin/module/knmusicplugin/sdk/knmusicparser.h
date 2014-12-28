/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICPARSER_H
#define KNMUSICPARSER_H

#include <QDateTime>
#include <QFileInfo>
#include <QStringList>
#include <QList>

#include "knmusicanalysiser.h"
#include "knmusictagpraser.h"
#include "knmusiclistparser.h"

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNGlobal;
class KNMusicParser : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicParser(QObject *parent = 0);
    ~KNMusicParser();
    void parseFile(QString filePath,
                   KNMusicAnalysisItem &analysisItem);
    void parseTrackList(const QString &filePath,
                        QList<KNMusicAnalysisItem> &trackDetailList);
    void installAnalysiser(KNMusicAnalysiser *analysiser);
    void installTagParser(KNMusicTagParser *tagParser);
    void installListParser(KNMusicListParser *listParser);
    static QString bitRateText(const qint64 &bitRateNumber);
    static QString sampleRateText(const qint64 &sampleRateNumber);

signals:

public slots:
    void parseAlbumArt(KNMusicAnalysisItem &analysisItem);

private:
    inline void parseTag(const QString &filePath,
                         KNMusicAnalysisItem &analysisItem);
    inline void analysis(const QString &filePath,
                         KNMusicDetailInfo &detailInfo);
    inline bool findImageFile(const QString &imageBaseFileName,
                              KNMusicAnalysisItem &analysisItem);
    inline bool checkImageFile(const QString &imageFileInfo,
                               KNMusicAnalysisItem &analysisItem);
    KNGlobal *m_global;
    KNMusicGlobal *m_musicGlobal;
    QList<KNMusicAnalysiser *> m_analysisers;
    QList<KNMusicTagParser *> m_tagParsers;
    QList<KNMusicListParser *> m_listParsers;
};

#endif // KNMUSICPARSER_H
