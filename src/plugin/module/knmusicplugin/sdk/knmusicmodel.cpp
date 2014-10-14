/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicsearcher.h"
#include "knmusicanalysiscache.h"

#include "knmusicmodel.h"

KNMusicModel::KNMusicModel(QObject *parent) :
    QStandardItemModel(parent)
{
    //Initial music global.
    m_musicGlobal=KNMusicGlobal::instance();

    //Initial file searcher.
    m_searcher=new KNMusicSearcher;
    m_searcher->moveToThread(m_musicGlobal->searchThread());
    connect(this, &KNMusicModel::requireAnalysisFiles,
            m_searcher, &KNMusicSearcher::analysisUrls);

    //Initial analysis cache.
    m_analysisCache=new KNMusicAnalysisCache;
    m_analysisCache->moveToThread(m_musicGlobal->analysisThread());
    connect(m_searcher, &KNMusicSearcher::fileFound,
            m_analysisCache, &KNMusicAnalysisCache::appendFilePath);
    connect(m_analysisCache, &KNMusicAnalysisCache::requireAppendMusicRow,
            this, &KNMusicModel::appendMusicRow);
}

KNMusicModel::~KNMusicGlobal()
{
    //Delete objects.
    delete m_searcher;
    delete m_analysisCache;
}

Qt::DropActions KNMusicModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

Qt::ItemFlags KNMusicModel::flags(const QModelIndex &index) const
{
    //Enabled drag and drop flags when the index is valid.
    if(index.isValid())
    {
        return Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled | QStandardItemModel::flags(index);
    }
    //Or else, just enabled drop.
    return Qt::ItemIsDropEnabled;
}

void KNMusicModel::appendMusicRow(const QList<QStandardItem *> &musicRow)
{
    //Clear all the icons.
    musicRow.at(Name)->setData(QPixmap(), Qt::DecorationRole);
    //Calculate new total duration.
    m_totalDuration+=musicRow.at(Time)->data(Qt::UserRole).toInt();
    //Append this row.
    appendRow(musicRow);
    emit rowCountChanged();
}
