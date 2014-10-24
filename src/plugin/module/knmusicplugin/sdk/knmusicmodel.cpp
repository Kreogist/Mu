/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QMimeData>

#include "knglobal.h"
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

KNMusicModel::~KNMusicModel()
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
        return Qt::ItemIsDragEnabled | QStandardItemModel::flags(index);
    }
    //Or else, just enabled drop.
    return Qt::ItemIsDropEnabled;
}

QStringList KNMusicModel::mimeTypes() const
{
    //Add url list to mimetypes, but I don't know why should add uri.
    //14.08.21: Add .
    QStringList types=QStandardItemModel::mimeTypes();
    types<<"text/uri-list"
         <<"org.kreogist.mu/MusicModelRow";
    return types;
}

bool KNMusicModel::dropMimeData(const QMimeData *data,
                                Qt::DropAction action,
                                int row,
                                int column,
                                const QModelIndex &parent)
{
    //When mimedata contains url data, and ensure that move&copy action enabled.
    if((action==Qt::MoveAction || action==Qt::CopyAction))
    {
        if(data->hasUrls())
        {
            addFiles(KNGlobal::urlToPathList(data->urls()));
            return true;
        }
        if(data->hasFormat("org.kreogist.mu/MusicModelRow"))
        {
            //!FIXME: Add row process code here.
            return true;
        }
    }
    return QStandardItemModel::dropMimeData(data, action, row, column, parent);
}

qint64 KNMusicModel::totalDuration() const
{
    return m_totalDuration;
}

QString KNMusicModel::filePathFromRow(const int &row)
{
    Q_ASSERT(row>-1 && row<rowCount());
    //Return the file path role data.
    return data(index(row, Name), FilePathRole).toString();
}

QString KNMusicModel::filePathFromIndex(const QModelIndex &index)
{
    Q_ASSERT(index.isValid());
    //Return the file path role data.
    return filePathFromRow(index.row());
}

QModelIndexList KNMusicModel::indexFromFilePath(const QString &filePath)
{
    //If the file path is empty, return a null index.
    if(filePath.isEmpty())
    {
        return QModelIndexList();
    }
    //Using match to find all pathes.
    return match(index(0, Name),
                 FilePathRole,
                 filePath);
}

QString KNMusicModel::itemText(const int &row, const int &column) const
{
    Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
    //Only for text easy access.
    return roleData(row, column, Qt::DisplayRole).toString();
}

QVariant KNMusicModel::roleData(int row, int column, int role) const
{
    Q_ASSERT(row>-1 && row<rowCount() && column>-1 && column<columnCount());
    //Only for easy access.
    return data(index(row, column), role);
}

QList<QStandardItem *> KNMusicModel::songRow(const int &row) const
{
    Q_ASSERT(row>-1 && row<rowCount());
    //Get all the items, add to a list.
    QList<QStandardItem *> songRowData;
    for(int i=0; i<MusicDataCount; i++)
    {
        //Using clone to do data copy.
        songRowData.append(item(row, i)->clone());
    }
    return songRowData;
}

QVariant KNMusicModel::rowProperty(const int &row, const int &propertyRole)
{
    Q_ASSERT(row>-1 && row<rowCount());
    //All the property of a song is stored in the first item.
    return roleData(row, 0, propertyRole);
}

QPixmap KNMusicModel::songAlbumArt(const int &row)
{
    Q_UNUSED(row)
    //We don't store the album art data for default, models should find their
    //ways to store this thing. This function is only a port.
    return QPixmap();
}

qint64 KNMusicModel::songDuration(const int &row)
{
    Q_ASSERT(row>-1 && row<rowCount());
    //For easy access.
    return roleData(row, Time, Qt::UserRole).toLongLong();
}

int KNMusicModel::playingItemColumn()
{
    return Name;
}

void KNMusicModel::addFiles(const QStringList &fileList)
{
    emit requireAnalysisFiles(fileList);
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

void KNMusicModel::removeMusicRow(const int &row)
{
    //We need to do sth before remove a row.
    m_totalDuration-=data(index(row, Time), Qt::UserRole).toInt();
    //Remove that row.
    removeRow(row);
    //Tell other's to update.
    emit rowCountChanged();
}

void KNMusicModel::clearMusicRow()
{
    //Clear the duration.
    m_totalDuration=0;
    //Remove all the rows.
    removeRows(0, rowCount());
    //Tell other's to update.
    emit rowCountChanged();
}

void KNMusicModel::blockAddFile(const QString &filePath)
{
    //WARNING: This function is working in a block way to adding file, may cause
    //performance reduce.
    if(m_searcher->isFilePathAccept(filePath))
    {
        m_analysisCache->analysisFile(filePath);
    }
}