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

#include "knjsondatabase.h"

#include "knmusiccategorymodel.h"
#include "knmusicsearcher.h"
#include "knmusicanalysisqueue.h"

#include "knmusiclibrarymodel.h"

#include <QDebug>

KNMusicLibraryModel::KNMusicLibraryModel(QThread *workingThread,
                                         QObject *parent) :
    KNMusicModel(parent),
    m_database(nullptr),
    m_searcher(new KNMusicSearcher),
    m_analysisQueue(new KNMusicAnalysisQueue)
{
    //Move the searcher to working thread.
    m_searcher->moveToThread(workingThread);
    //Link the require add signal to searcher.
    connect(this, &KNMusicLibraryModel::requireAnalysisFiles,
            m_searcher, &KNMusicSearcher::analysisPaths,
            Qt::QueuedConnection);

    //Move the analysis queue to wokring thread.
    m_analysisQueue->moveToThread(workingThread);
    //Link the searcher with the analysis queue.
    connect(m_searcher, &KNMusicSearcher::findFile,
            m_analysisQueue, &KNMusicAnalysisQueue::addFile,
            Qt::QueuedConnection);
    connect(m_analysisQueue, &KNMusicAnalysisQueue::analysisComplete,
            this, &KNMusicLibraryModel::onActionAnalysisComplete,
            Qt::QueuedConnection);
}

KNMusicLibraryModel::~KNMusicLibraryModel()
{
    //Write all the database data to the harddisk.
    m_database->write();
}

void KNMusicLibraryModel::appendRow(const KNMusicDetailInfo &detailInfo)
{
    //Append data to the database.
    m_database->append(generateDataArray(detailInfo));
    //Add the detail info to category models.
    addCategoryDetailInfo(detailInfo);
    //Do the original append operations.
    KNMusicModel::appendRow(detailInfo);
    //Check out the row count.
    if(rowCount()==1)
    {
        //This is the first record, emit the signal.
        emit libraryNotEmpty();
    }
}

void KNMusicLibraryModel::appendRows(
        const QList<KNMusicDetailInfo> &detailInfos)
{
    //Append all the data to the databases.
    for(auto i : detailInfos)
    {
        //Append all the detail info to the database.
        m_database->append(generateDataArray(i));
        //Add the detail info to category models.
        addCategoryDetailInfo(i);
    }
    //Do the original append operations.
    KNMusicModel::appendRows(detailInfos);
    //Check out the row count.
    if(rowCount()==1)
    {
        //This is the first record, emit the signal.
        emit libraryNotEmpty();
    }
}

bool KNMusicLibraryModel::insertRow(int row,
                                    const KNMusicDetailInfo &detailInfo)
{
    //Insert the array to the database.
    m_database->insert(row, generateDataArray(detailInfo));
    //Add the detail info to category models.
    addCategoryDetailInfo(detailInfo);
    //Check out the database size.
    if(m_database->size()==1)
    {
        //This is the first record, emit the signal.
        emit libraryNotEmpty();
    }
    //Do the original append operation.
    return KNMusicModel::insertRow(row, detailInfo);
}

bool KNMusicLibraryModel::insertMusicRows(
        int row,
        const QList<KNMusicDetailInfo> &detailInfos)
{
    //Insert the array to the database.
    for(int i=detailInfos.size()-1; i>-1; --i)
    {
        //Insert the data to the specific position.
        m_database->insert(row, generateDataArray(detailInfos.at(i)));
        //Add the detail info to category models.
        addCategoryDetailInfo(detailInfos.at(i));
    }
    //Check out the database size.
    if(m_database->size()==1)
    {
        //This is the first record, emit the signal.
        emit libraryNotEmpty();
    }
    //Do the original insert operation.
    return KNMusicModel::insertMusicRows(row, detailInfos);
}

bool KNMusicLibraryModel::updateRow(int row, KNMusicDetailInfo detailInfo)
{
    //Do the original update operation.
    bool result=KNMusicModel::updateRow(row, detailInfo);
    //Update the data in the database.
    m_database->replace(row, generateDataArray(rowDetailInfo(row)));
    //Give the result back.
    return result;
}

bool KNMusicLibraryModel::replaceRow(int row,
                                     const KNMusicDetailInfo &detailInfo)
{
    //Replace the data in the database.
    m_database->replace(row, generateDataArray(detailInfo));
    //Do the original operation.
    return KNMusicModel::replaceRow(row, detailInfo);
}

bool KNMusicLibraryModel::removeRows(int position,
                                     int rows,
                                     const QModelIndex &index)
{
    //Remove the data from the database.
    int rowsWaitToRemove=rows;
    //Remove 'rows' times from position in the database.
    while(rowsWaitToRemove--)
    {
        //Remove the position row.
        m_database->removeAt(position);
    }
    //Remove the detail info from the category models.
    for(int i=0; i<rows; ++i)
    {
        //Remove the data from the category model.
        removeCategoryDetailInfo(rowDetailInfo(position+i));
    }
    //Check out the database size.
    if(m_database->size()==0)
    {
        //The model will be empty again, emit this signal.
        emit libraryEmpty();
    }
    //Do the original remove rows operations.
    return KNMusicModel::removeRows(position, rows, index);
}

void KNMusicLibraryModel::clear()
{
    //Clear up the database.
    m_database->clear();
    //Reset the category models.
    //For all the category models,
    for(auto i=m_categoryModels.begin(); i!=m_categoryModels.end(); ++i)
    {
        //Called the on action remove slot.
        (*i)->reset();
    }
    //Do the model clear.
    KNMusicModel::clear();
    //The library is empty, emit the signal.
    emit libraryEmpty();
}

bool KNMusicLibraryModel::setData(const QModelIndex &index,
                                  const QVariant &value,
                                  int role)
{
    //Do the original data set operation.
    bool result=KNMusicModel::setData(index, value, role);
    //Update the data in the database.
    m_database->replace(index.row(),
                        generateDataArray(rowDetailInfo(index.row())));
    //Give back the result.
    return result;
}

void KNMusicLibraryModel::recoverModel()
{
    //Check out the row count first, if there's any data then we have to ignore
    //this calling.
    if(rowCount()>0)
    {
        return;
    }
    //Read the database information.
    m_database->read();
    //Check out whether the database is empty.
    if(m_database->size()==0)
    {
        //Mission complete.
        return;
    }
    //Initial the total duration.
    quint64 totalDuration=0;
    //Start to insert data to the model.
    beginInsertRows(QModelIndex(),
                    0,
                    m_database->size() - 1);
    //Read the data through the database.
    for(auto i=m_database->begin(); i!=m_database->end(); ++i)
    {
        //Generate the detail info.
        KNMusicDetailInfo turboDetailInfo=generateDetailInfo((*i).toArray());
        //Append it to the model.
        appendDetailInfo(turboDetailInfo);
        //Add to category models.
        addCategoryDetailInfo(turboDetailInfo);
        //Calcualte the total duration.
        totalDuration+=turboDetailInfo.duration;
    }
    //Set the total duration.
    initialTotalDuration(totalDuration);
    //End to insert data.
    endInsertRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
    //Check out the data.
    if(rowCount()>0)
    {
        //Before this the row count cannot be 0.
        emit libraryNotEmpty();
    }
}

inline KNMusicDetailInfo KNMusicLibraryModel::generateDetailInfo(
        const QJsonArray &dataArray)
{
    //Generate a new detail info.
    KNMusicDetailInfo detailInfo;
    //Get the information and property array.
    QJsonArray textArray=dataArray.at(0).toArray(),
               propertyArray=dataArray.at(1).toArray();
    //Set the detail info data.
    for(int i=0; i<MusicDataCount; ++i)
    {
        //Save the text list.
        detailInfo.textLists[i]=textArray.at(i).toString();
    }
    //Save the property data.
    detailInfo.filePath=propertyArray.at(PropertyFilePath).toString();
    //Check out the existance of the file path.
    if(!QFileInfo::exists(detailInfo.filePath))
    {
        //Set the cannot play flag to true if the file isn't exist.
        detailInfo.cannotPlay=true;
    }
    detailInfo.fileName=propertyArray.at(PropertyFileName).toString();
    detailInfo.trackFilePath=propertyArray.at(PropertyTrackFilePath).toString();
    detailInfo.trackIndex=propertyArray.at(PropertyTrackIndex).toInt();
    detailInfo.coverImageHash=
            propertyArray.at(PropertyCoverImageHash).toString();
    detailInfo.startPosition=
            propertyArray.at(PropertyStartPosition).toString().toLongLong();
    detailInfo.size=
            propertyArray.at(PropertySize).toString().toLongLong();
    detailInfo.dateModified=
            KNMusicUtil::dataToDateTime(
                propertyArray.at(PropertyDateModified).toString());
    detailInfo.dateAdded=
            KNMusicUtil::dataToDateTime(
                propertyArray.at(PropertyDateAdded).toString());
    detailInfo.dateLastPlayed=
            KNMusicUtil::dataToDateTime(
                propertyArray.at(PropertyLastPlayed).toString());
    detailInfo.duration=
            propertyArray.at(PropertyDuration).toString().toLongLong();
    detailInfo.bitRate=propertyArray.at(PropertyBitRate).toInt();
    detailInfo.samplingRate=propertyArray.at(PropertySamplingRate).toInt();
    return detailInfo;
}

inline QJsonArray KNMusicLibraryModel::generateDataArray(
        const KNMusicDetailInfo &detailInfo)
{
    //Generate text data JSON array.
    QJsonArray textArray;
    //Insert data to array.
    for(int i=0; i<MusicDataCount; ++i)
    {
        //Add the text data of detail info to text array.
        textArray.append(detailInfo.textLists[i].toString());
    }
    //Generate property data JSON array.
    QJsonArray propertyArray;
    //Append property data in order to the array.
    propertyArray.append(detailInfo.filePath); //PropertyFilePath
    propertyArray.append(detailInfo.fileName); //PropertyFileName,
    propertyArray.append(detailInfo.coverImageHash); //PropertyCoverImageHash,
    propertyArray.append(detailInfo.bitRate); //PropertyBitRate,
    propertyArray.append(detailInfo.samplingRate); //PropertySamplingRate,
    //PropertySize,
    propertyArray.append(QString::number(detailInfo.size));
    //PropertyDuration,
    propertyArray.append(QString::number(detailInfo.duration));
    //PropertyDateAdded,
    propertyArray.append(KNMusicUtil::dateTimeToData(detailInfo.dateAdded));
    //PropertyDateModified,
    propertyArray.append(KNMusicUtil::dateTimeToData(detailInfo.dateModified));
    //PropertyLastPlayed,
    propertyArray.append(
                KNMusicUtil::dateTimeToData(detailInfo.dateLastPlayed));
    //PropertyTrackFilePath,
    propertyArray.append(detailInfo.trackFilePath);
    //PropertyTrackIndex,
    propertyArray.append(detailInfo.trackIndex);
    //PropertyStartPosition,
    propertyArray.append(QString::number(detailInfo.startPosition));
    //Generate the data array.
    QJsonArray dataArray;
    dataArray.append(textArray);
    dataArray.append(propertyArray);
    //Give back the data array.
    return dataArray;
}

KNJsonDatabase *KNMusicLibraryModel::database() const
{
    return m_database;
}

void KNMusicLibraryModel::installCategoryModel(KNMusicCategoryModel *model)
{
    //Append the model to the category models.
    m_categoryModels.append(model);
}

void KNMusicLibraryModel::setDatabase(KNJsonDatabase *database)
{
    m_database = database;
}

void KNMusicLibraryModel::onActionAnalysisComplete(
        const KNMusicAnalysisItem &analysisItem)
{
    //Check out whether the detail info is existed in the model, find the detail
    //info in the model.
    int detailInfoIndex=detailInfoRow(analysisItem.detailInfo);
    //Check out the index.
    if(detailInfoIndex!=-1)
    {
        //Update the detail info.
        updateRow(detailInfoIndex, analysisItem.detailInfo);
        //Mission complete.
        return;
    }
    //Add the detail info to the model.
    appendRow(analysisItem.detailInfo);
    //Check out the row after append the row.
    if(rowCount()==1)
    {
        //Emit the library not empty signal.
        emit libraryNotEmpty();
    }
}

inline void KNMusicLibraryModel::addCategoryDetailInfo(
        const KNMusicDetailInfo &detailInfo)
{
    //For all the category models,
    for(auto i=m_categoryModels.begin(); i!=m_categoryModels.end(); ++i)
    {
        //Called the on action add slot.
        (*i)->onCategoryAdd(detailInfo);
    }
}

inline void KNMusicLibraryModel::removeCategoryDetailInfo(
        const KNMusicDetailInfo &detailInfo)
{
    //For all the category models,
    for(auto i=m_categoryModels.begin(); i!=m_categoryModels.end(); ++i)
    {
        //Called the on action remove slot.
        (*i)->onCategoryRemove(detailInfo);
    }
}
