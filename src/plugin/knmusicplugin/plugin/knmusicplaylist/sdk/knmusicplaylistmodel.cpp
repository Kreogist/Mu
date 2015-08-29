/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "knmusicsearcher.h"
#include "knmusicanalysisqueue.h"
#include "knmusicplaylistutil.h"

#include "knmusicplaylistmodel.h"

#include <QDebug>

QString KNMusicPlaylistModel::m_playlistDirPath=QString();

KNMusicPlaylistModel::KNMusicPlaylistModel(QThread *workingThread,
                                           QObject *parent) :
    KNMusicModel(parent),
    m_searcher(new KNMusicSearcher),
    m_analysisQueue(new KNMusicAnalysisQueue),
    m_title(QString()),
    m_filePath(QString()),
    m_contentData(QJsonArray()),
    m_built(false),
    m_changed(false)
{
    //When the row count changed, this model should be marked to be changed.
    connect(this, &KNMusicPlaylistModel::rowCountChanged,
            this, &KNMusicPlaylistModel::onActionModelChanged);
    //When there's a data changed, this model should be marked to be changed
    //as well.
    connect(this, &KNMusicPlaylistModel::dataChanged,
            this, &KNMusicPlaylistModel::onActionModelChanged);

    //Move the searcher to working thread.
    m_searcher->moveToThread(workingThread);
    //Link the require add signal to searcher.
    connect(this, &KNMusicPlaylistModel::requireAnalysisFiles,
            m_searcher, &KNMusicSearcher::analysisPaths,
            Qt::QueuedConnection);

    //Move the analysis queue to wokring thread.
    m_analysisQueue->moveToThread(workingThread);
    //Link the searcher with the analysis queue.
    connect(m_searcher, &KNMusicSearcher::findFile,
            m_analysisQueue, &KNMusicAnalysisQueue::addFile,
            Qt::QueuedConnection);
    connect(m_analysisQueue, &KNMusicAnalysisQueue::analysisComplete,
            this, &KNMusicPlaylistModel::onActionAnalysisComplete,
            Qt::QueuedConnection);
}

KNMusicPlaylistModel::~KNMusicPlaylistModel()
{
    //Remove the searcher and the analysis queue.
    m_searcher->deleteLater();
    m_analysisQueue->deleteLater();
}

QString KNMusicPlaylistModel::title() const
{
    return m_title;
}

void KNMusicPlaylistModel::setTitle(const QString &title)
{
    //Save the title.
    m_title=title;
    //Emit the title changed signal.
    emit titleChanged(m_title);
    //Marked changed flag.
    m_changed=true;
}

QString KNMusicPlaylistModel::filePath() const
{
    return m_filePath;
}

void KNMusicPlaylistModel::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

bool KNMusicPlaylistModel::isBuilt() const
{
    return m_built;
}

void KNMusicPlaylistModel::buildModel()
{
    //Check if we have been build this model before.
    if(m_built)
    {
        return;
    }
    //Generate a detail info cache list.
    QList<KNMusicDetailInfo> detailInfos;
    //Parse all the data in the content array.
    for(auto i=m_contentData.constBegin(); i!=m_contentData.constEnd(); ++i)
    {
        //Parse and add the data to the model.
        detailInfos.append(
                    KNMusicPlaylistUtil::objectToDetailInfo((*i).toObject()));
    }
    //Clear the content data.
    m_contentData=QJsonArray();
    //Add the detail infos to the model.
    appendRows(detailInfos);
    //Set the build flag.
    m_built=true;
    //Reset the changed flag, a model which just finished built cannot be
    //changed before.
    m_changed=false;
}

QJsonArray KNMusicPlaylistModel::contentData() const
{
    return m_contentData;
}

void KNMusicPlaylistModel::setContentData(const QJsonArray &contentData)
{
    m_contentData = contentData;
}

bool KNMusicPlaylistModel::changed() const
{
    return m_changed;
}

void KNMusicPlaylistModel::setChanged(bool changed)
{
    m_changed = changed;
}

QVariant KNMusicPlaylistModel::data(const QModelIndex &index, int role) const
{
    //Check the index at the beginning.
    if(!index.isValid())
    {
        return false;
    }
    //Check the role.
    switch(role)
    {
    case Qt::TextAlignmentRole:
        //For the row state column, make it right and vertical center.
        if(index.column()==MusicRowState)
        {
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        }
        //For the other type of the data, just follow the music model.
        return KNMusicModel::data(index, Qt::TextAlignmentRole);

    default:
        return KNMusicModel::data(index, role);
    }
}

void KNMusicPlaylistModel::onActionModelChanged()
{
    //Set the changed flag to true.
    m_changed=true;
}

void KNMusicPlaylistModel::onActionAnalysisComplete(
        const KNMusicAnalysisItem &analysisItem)
{
    //Add the detail info to the playlist model.
    appendRow(analysisItem.detailInfo);
}

QString KNMusicPlaylistModel::generateFilePath()
{
    //Use the current time as the base name.
    QString baseName=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"),
            filePath=m_playlistDirPath+"/"+baseName;
    //Check the existance of the file path.
    if(QFileInfo::exists(filePath))
    {
        //If the file exist, change the file name by adding counter, until it
        //doesn't exist anymore.
        int count=0;
        while(QFileInfo::exists(filePath))
        {
            //Update the file path.
            filePath=m_playlistDirPath + "/" + baseName + " " +
                     QString::number(++count);
        }
    }
    //Give back the file path.
    return filePath;
}

void KNMusicPlaylistModel::setPlaylistDirPath(const QString &playlistDirPath)
{
    m_playlistDirPath = playlistDirPath;
}
