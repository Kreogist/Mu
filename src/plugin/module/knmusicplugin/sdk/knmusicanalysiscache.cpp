/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicparser.h"
#include "knmusicanalysiscache.h"
#include "knmusicanalysisextend.h"
#include "knconnectionhandler.h"

#include <QFileInfo>

#include <QDebug>

KNMusicAnalysisCache::KNMusicAnalysisCache(QObject *parent) :
    QObject(parent)
{
    m_musicGlobal=KNMusicGlobal::instance();
    m_parser=KNMusicGlobal::parser();
    //Initial connection handler.
    m_extendConnections=new KNConnectionHandler(this);
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
        m_parser->parseFile(currentItem.filePath, currentItem.detailInfo);
        //Emit the analysis finished signal, give out the detail info.
        emit analysisComplete(currentItem.detailInfo);
        return;
    }
    //So, it must be a list now.
    QList<KNMusicDetailInfo> trackDetailInfo;
    m_parser->parseTrackList(currentItem.filePath,
                             trackDetailInfo);
    while(!trackDetailInfo.isEmpty())
    {
        //Give out the analysis complete info by track index.
        emit analysisComplete(trackDetailInfo.takeFirst());
    }
}

KNMusicAnalysisExtend *KNMusicAnalysisCache::extend() const
{
    return m_extend;
}

void KNMusicAnalysisCache::setExtend(KNMusicAnalysisExtend *extend)
{
    //If we have has a old extend, clear it.
    if(m_extend!=nullptr)
    {
        //Remove all connections.
        m_extendConnections->disConnectAll();
    }
    //Save the extend pointer.
    m_extend = extend;
    //Establish the connections if it's not null.
    if(m_extend!=nullptr)
    {
        m_extendConnections->addConnectionHandle(
                    connect(this, &KNMusicAnalysisCache::analysisComplete,
                            m_extend, &KNMusicAnalysisExtend::onActionAnalysisComplete));
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
