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
#include <QMimeData>
#include <QJsonDocument>

#include "knmusicsearcher.h"
#include "knmusicanalysisqueue.h"

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
    connect(m_searcher, &KNMusicSearcher::findFiles,
            m_analysisQueue, &KNMusicAnalysisQueue::addFiles,
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
        detailInfos.append(KNMusicUtil::objectToDetailInfo((*i).toObject()));
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

QMimeData *KNMusicPlaylistModel::mimeData(const QModelIndexList &indexes) const
{
    //First get the mime data from the music model.
    QMimeData *mimeData=KNMusicModel::mimeData(indexes);
    //Add the playlist type to the mime data.
    mimeData->setData(ModelType, QString("Playlist").toUtf8());
    //Give back the mime data.
    return mimeData;
}

bool KNMusicPlaylistModel::dropMimeData(const QMimeData *data,
                                        Qt::DropAction action,
                                        int row,
                                        int column,
                                        const QModelIndex &parent)
{
    //Check move or copy action enabled.
    if(action==Qt::MoveAction || action==Qt::CopyAction)
    {
        //Check if the drop operation is from a music model.
        if(data->hasFormat(ModelMimeType))
        {
            //Check whether the data is from it self.
            if(data->data(ModelMimeType).toLongLong()==(qint64)this)
            {
                //Generate the remove index list.
                QList<QPersistentModelIndex> sourceRowIndexes;
                //Translate the row list to source row indexes list.
                //We have to move the source indexes to the target positions.
                QJsonArray rawRowData=
                        QJsonDocument::fromBinaryData(
                            data->data(ModelRowList)).array();
                //Translate all the row into persistant list.
                for(auto i=rawRowData.begin(); i!=rawRowData.end(); ++i)
                {
                    //Add the target index to source row indexes.
                    sourceRowIndexes.append(index((*i).toInt(), Name));
                }
                //Sort the indexes as the row.
                //We want to sort the list from a greater row to lesser row.
                std::sort(sourceRowIndexes.begin(), sourceRowIndexes.end(),
                          [](const QModelIndex &left, const QModelIndex &right)
                          {return left.row()>right.row();});
                //Translate the target position to persistant index.
                int targetRow=(row==-1)?parent.row():row;
                QPersistentModelIndex targetIndex=
                        index((targetRow==-1)?rowCount()-1:targetRow, Name);
                //Move all the rows to target position.
                while(!sourceRowIndexes.isEmpty())
                {
                    //Get the source row.
                    int sourceRow=sourceRowIndexes.takeLast().row();
                    //Move the source row to target row.
                    moveRow(QModelIndex(), sourceRow,
                            QModelIndex(), targetIndex.row());
                }
                return true;
            }
            //We can simply parse the row information.
            QJsonArray rawRowData=
                    QJsonDocument::fromBinaryData(
                        data->data(ModelRowData)).array();
            //Generate a detail info list.
            QList<KNMusicDetailInfo> detailInfoList;
            //Parse all the data inside the array list.
            for(auto i=rawRowData.begin(); i!=rawRowData.end(); ++i)
            {
                //Add the detail info to the detail info list.
                detailInfoList.append(
                            KNMusicUtil::objectToDetailInfo((*i).toObject()));
            }
            //Check the row.
            if(row==-1)
            {
                //We have to move all of these data to the end of the model.
                appendRows(detailInfoList);
            }
            else
            {
                //We have to insert those rows to a specific positions.
                insertMusicRows(row, detailInfoList);
            }
            //Accept.
            return true;
        }
    }
    //Or others, throw it to KNMusicModel.
    return KNMusicModel::dropMimeData(data, action, row, column, parent);
}

QList<KNMusicDetailInfo> KNMusicPlaylistModel::playlistDetailInfos() const
{
    //Give back the detail info list.
    return detailInfos();
}
