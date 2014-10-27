/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicparser.h"
#include "knmusicanalysiscache.h"
#include "knmusicmodelassist.h"

#include <QFileInfo>

#include <QDebug>

KNMusicAnalysisCache::KNMusicAnalysisCache(QObject *parent) :
    QObject(parent)
{
    m_musicModelAssist=KNMusicModelAssist::instance();
    m_musicGlobal=KNMusicGlobal::instance();
    m_parser=KNMusicGlobal::parser();
    //Connect analysis loop.
    connect(this, &KNMusicAnalysisCache::analysisNext,
            this, &KNMusicAnalysisCache::onActionAnalysisNext);
}

void KNMusicAnalysisCache::appendFilePath(const QString &filePath)
{
    //We will add this file into current music model.
    AnalysisItem currentItem;
    currentItem.filePath=filePath;
    //Add to analysis list.
    m_analysisQueue.append(currentItem);
    //Begin analysis.
    emit analysisNext();
}

void KNMusicAnalysisCache::analysisFile(const QString &filePath)
{
    AnalysisItem currentItem;
    currentItem.filePath=filePath;
    parseItem(currentItem);
}

void KNMusicAnalysisCache::parseItem(KNMusicAnalysisCache::AnalysisItem &currentItem)
{
    //Judge the file is a list or a music file.
    if(m_musicGlobal->isMusicFile(
                currentItem.filePath.mid(currentItem.filePath.lastIndexOf('.')+1)))
    {
        //Parse the file.
        m_parser->parseFile(currentItem.filePath,
                            currentItem.detailInfo);
        //Create music row.
        emit requireAppendMusicRow(KNMusicModelAssist::generateRow(currentItem.detailInfo));
        return;
    }
    //So, it must be a list now.
    QList<KNMusicDetailInfo> trackDetailInfo;
    m_parser->parseTrackList(currentItem.filePath,
                             trackDetailInfo);
    while(!trackDetailInfo.isEmpty())
    {
        emit requireAppendMusicRow(KNMusicModelAssist::generateRow(trackDetailInfo.takeFirst()));
    }
}

void KNMusicAnalysisCache::onActionAnalysisNext()
{
    if(m_analysisQueue.isEmpty())
    {
        return;
    }
    //Take the first item of the queue.
    AnalysisItem currentItem=m_analysisQueue.takeFirst();
    parseItem(currentItem);
    //Require to analysis next.
    emit analysisNext();
}
