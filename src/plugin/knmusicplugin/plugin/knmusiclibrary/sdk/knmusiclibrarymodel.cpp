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

#include "knutil.h"
#include "knnotification.h"

#include "knmusiccategorymodelbase.h"
#include "knmusicsearcher.h"
#include "knmusicanalysisqueue.h"
#include "knmusiclibraryimagemanager.h"

#include "knmusiclibrarymodel.h"

#include <QDebug>

#define MajorVersion 4
#define MinorVersion 0
#define MaxOperateCount 900

KNMusicLibraryModel::KNMusicLibraryModel(QObject *parent) :
    KNMusicModel(parent),
    m_scaledHashAlbumArt(QHash<QString, QVariant>()),
    m_databasePath(QString()),
    m_operateCounter(0),
    m_searcher(new KNMusicSearcher),
    m_analysisQueue(new KNMusicAnalysisQueue),
    m_imageManager(new KNMusicLibraryImageManager)
{
    //Move the searcher to working thread.
    m_searcher->moveToThread(&m_searchThread);
    //Link the require add signal to searcher.
    connect(this, &KNMusicLibraryModel::requireAnalysisFiles,
            m_searcher, &KNMusicSearcher::analysisPaths,
            Qt::QueuedConnection);
    connect(m_searcher, &KNMusicSearcher::searchFinish,
            [=](const qint64 &count)
            {
                knNotification->pushOnly(
                            tr("Search complete"),
                            tr("%1 files have been to music library.").arg(
                                QString::number(count)));
            });

    //Move the analysis queue to working thread.
    m_analysisQueue->moveToThread(&m_analysisThread);
    //Link the searcher with the analysis queue.
    connect(m_searcher, &KNMusicSearcher::findFile,
            m_analysisQueue, &KNMusicAnalysisQueue::addFile,
            Qt::QueuedConnection);
    connect(m_analysisQueue, &KNMusicAnalysisQueue::analysisComplete,
            this, &KNMusicLibraryModel::onActionAnalysisComplete,
            Qt::QueuedConnection);

    //Move the image manager to working thread.
    m_imageManager->setHashAlbumArt(&m_scaledHashAlbumArt);
    m_imageManager->moveToThread(&m_imageThread);
    //Link the signal from the library model.
    connect(this, &KNMusicLibraryModel::requireRecoverImage,
            m_imageManager, &KNMusicLibraryImageManager::recoverAlbumArt,
            Qt::QueuedConnection);
    connect(m_imageManager, &KNMusicLibraryImageManager::recoverImageComplete,
            this, &KNMusicLibraryModel::onActionImageRecoverComplete,
            Qt::QueuedConnection);
    connect(m_imageManager, &KNMusicLibraryImageManager::requireUpdateRow,
            this, &KNMusicLibraryModel::onActionImageUpdateRow,
            Qt::QueuedConnection);

    //Start working threads.
    m_searchThread.start();
    m_analysisThread.start();
    m_imageThread.start();
}

KNMusicLibraryModel::~KNMusicLibraryModel()
{
    //Quit and wait for the thread quit.
    m_searchThread.quit();
    m_analysisThread.quit();
    m_imageThread.quit();
    //Wait for thread quit.
    m_searchThread.wait();
    m_analysisThread.wait();
    m_imageThread.wait();

    //Write all the database data to the hard disk.
    writeDatabase();
    //Recover the memory.
    m_searcher->deleteLater();
    m_analysisQueue->deleteLater();
    m_imageManager->deleteLater();
}

void KNMusicLibraryModel::appendRow(const KNMusicDetailInfo &detailInfo)
{
    //Check the image cover image hash.
    if(!detailInfo.coverImageHash.isEmpty())
    {
        //Increase the new cover image hash.
        m_hashAlbumArtCounter.insert(
                    detailInfo.coverImageHash,
                    m_hashAlbumArtCounter.value(detailInfo.coverImageHash)+1);
    }
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
    //Increase the operation counter.
    count();
}

void KNMusicLibraryModel::appendRows(
        const QList<KNMusicDetailInfo> &detailInfos)
{
    //Append all the data to the databases.
    for(auto i : detailInfos)
    {
        //Check the image cover image hash.
        if(!i.coverImageHash.isEmpty())
        {
            //Increase the new cover image hash.
            m_hashAlbumArtCounter.insert(
                        i.coverImageHash,
                        m_hashAlbumArtCounter.value(i.coverImageHash)+1);
        }
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
    //Increase the operation counter.
    count(detailInfos.size());
}

bool KNMusicLibraryModel::insertRow(int row,
                                    const KNMusicDetailInfo &detailInfo)
{
    //Check the image cover image hash.
    if(!detailInfo.coverImageHash.isEmpty())
    {
        //Increase the new cover image hash.
        m_hashAlbumArtCounter.insert(
                    detailInfo.coverImageHash,
                    m_hashAlbumArtCounter.value(detailInfo.coverImageHash)+1);
    }
    //Add the detail info to category models.
    addCategoryDetailInfo(detailInfo);
    //Check out the database size.
    if(rowCount()==0)
    {
        //This is the first record, emit the signal.
        emit libraryNotEmpty();
    }
    //Do the original append operation.
    bool &&result=KNMusicModel::insertRow(row, detailInfo);
    //Increase the count.
    count();
    //Give back the result.
    return result;
}

bool KNMusicLibraryModel::insertMusicRows(
        int row,
        const QList<KNMusicDetailInfo> &detailInfos)
{
    //Insert the array to the database.
    for(int i=detailInfos.size()-1; i>-1; --i)
    {
        //Get the detail info.
        const KNMusicDetailInfo &detailInfo=detailInfos.at(i);
        //Check the image cover image hash.
        if(!detailInfo.coverImageHash.isEmpty())
        {
            //Increase the new cover image hash.
            m_hashAlbumArtCounter.insert(
                        detailInfo.coverImageHash,
                        m_hashAlbumArtCounter.value(
                            detailInfo.coverImageHash)+1);
        }
        //Add the detail info to category models.
        addCategoryDetailInfo(detailInfo);
    }
    //Check out the database size.
    if(rowCount()==0)
    {
        //This is the first record, emit the signal.
        emit libraryNotEmpty();
    }
    //Do the original insert operation.
    bool &&result=KNMusicModel::insertMusicRows(row, detailInfos);
    //Increase the count.
    count(detailInfos.size());
    //Give back the result.
    return result;
}

bool KNMusicLibraryModel::updateRow(int row, KNMusicAnalysisItem analysisItem)
{
    //This function will be called only from the now playing, and the analysis
    //item should contains the image.
    //So, the only thing we have to do with the image hash key is to check the
    //size.
    //Get the detail info.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo,
    //Get the original detail info.
                      &&originalDetailInfo=rowDetailInfo(row);
    //Check the current image.
    if(!analysisItem.coverImage.isNull())
    {
        //Get the latest image hash.
        detailInfo.coverImageHash=
                m_imageManager->insertArtwork(analysisItem.coverImage);
    }
    //Check the cover image hash is changed or not.
    if(detailInfo.coverImageHash!=originalDetailInfo.coverImageHash)
    {
        //Reduce the original cover image.
        reduceHashImage(originalDetailInfo.coverImageHash);
        //Increase the new cover image hash.
        m_hashAlbumArtCounter.insert(
                    detailInfo.coverImageHash,
                    m_hashAlbumArtCounter.value(detailInfo.coverImageHash)+1);
    }
    //Update the model row.
    bool &&result=updateModelRow(row, analysisItem);
    //Increase the operation counter.
    count();
    //Give back the result.
    return result;
}

bool KNMusicLibraryModel::replaceRow(int row,
                                     const KNMusicDetailInfo &detailInfo)
{
    //Update the category data.
    updateCategoryDetailInfo(rowDetailInfo(row), detailInfo);
    //Do the original operation.
    bool &&result=KNMusicModel::replaceRow(row, detailInfo);
    //Increase the operation counter.
    count();
    //Give back the result.
    return result;
}

bool KNMusicLibraryModel::removeRows(int position,
                                     int rows,
                                     const QModelIndex &index)
{
    //Remove the detail info from the category models.
    for(int i=0; i<rows; ++i)
    {
        //Get the row detail info.
        const KNMusicDetailInfo &detailInfo=rowDetailInfo(position+i);
        //Reduce the original cover image.
        reduceHashImage(detailInfo.coverImageHash);
        //Remove the data from the category model.
        removeCategoryDetailInfo(detailInfo);
    }
    //Check out the database size.
    if(rowCount()==rows)
    {
        //The model will be empty again, emit this signal.
        emit libraryEmpty();
    }
    //Do the original remove rows operations.
    bool &&result=KNMusicModel::removeRows(position, rows, index);
    //Increase the operation counter.
    count(rows);
    //Give back the result.
    return result;
}

void KNMusicLibraryModel::clear()
{
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
    //Actually, we can simply write the database here.
    writeDatabase();
}

bool KNMusicLibraryModel::setData(const QModelIndex &index,
                                  const QVariant &value,
                                  int role)
{
    //Do the original data set operation.
    bool &&result=KNMusicModel::setData(index, value, role);
    //Increase the count.
    count();
    //Give back the result.
    return result;
}

QPixmap KNMusicLibraryModel::artwork(const int &row)
{
    //Get the artwork from the hash list.
    QPixmap &&artworkPixmap=
            m_imageManager->artwork(data(index(row,
                                               Name),
                                         ArtworkKeyRole).toString());
    //Check out the variant is null or not.
    return artworkPixmap.isNull()?knMusicGlobal->noAlbumArt():artworkPixmap;
}

QPixmap KNMusicLibraryModel::artwork(const QString &hashKey)
{
    //Give back the hash key image from the pixmap.
    return m_scaledHashAlbumArt.contains(hashKey)?
                m_imageManager->artwork(hashKey):
                knMusicGlobal->noAlbumArt();
}

void KNMusicLibraryModel::recoverModel()
{
    //Check out the row count first, if there's any data then we have to ignore
    //this calling.
    if(rowCount()>0 || m_databasePath.isEmpty())
    {
        return;
    }
    //Get the database file.
    QFile databaseFile(m_databasePath);
    //Check out the file existance.
    if(!databaseFile.exists())
    {
        //Database file doesn't exist, ignore it.
        return;
    }
    //Open the file as read only mode.
    if(!databaseFile.open(QIODevice::ReadOnly))
    {
        //Open failed.
        return;
    }
    //Initial a data stream.
    QDataStream databaseStream(&databaseFile);
    //The file format should be:
    /*
     * |Major Version (unsigned 32-bit int)|
     * |Minor Version (unsigned 32-bit int)|
     * |rowCount() (unsigned 64-bit int)   |
     * |Detail Info Data Block 0           |
     * | ...                               |
     */
    //Initial the version data.
    quint32 major, minor;
    //Read the version data.
    databaseStream >> major >> minor;
    //Check the major and minor version.
    if(major!=MajorVersion && minor!=MinorVersion)
    {
        //Close the file, the database file version is not correct.
        databaseFile.close();
        //Mission complete.
        return;
    }
    //Read the row count.
    quint64 listSize;
    databaseStream >> listSize;
    //Check out whether the database is empty.
    if(listSize==0)
    {
        //Close the file.
        databaseFile.close();
        //Ask to recover image, clear out the no used art counter.
        emit requireRecoverImage(QStringList());
        //Mission complete.
        return;
    }
    //Initial the total duration.
    quint64 totalDuration=0;
    //Start to insert data to the model.
    beginInsertRows(QModelIndex(),
                    0,
                    listSize - 1);
    //Generate a detail info.
    KNMusicDetailInfo turboDetailInfo;
    //Read the data through the database.
    while(listSize--)
    {
        //Read the detail info.
        databaseStream >> turboDetailInfo;
        //Append it to the model.
        appendDetailInfo(turboDetailInfo);
        //Add to category models.
        addCategoryDetailInfo(turboDetailInfo);
        //Calcualte the total duration.
        totalDuration+=turboDetailInfo.duration;
        //Add hash list to image hash list counter.
        m_hashAlbumArtCounter.insert(
                    turboDetailInfo.coverImageHash,
                    m_hashAlbumArtCounter.value(turboDetailInfo.coverImageHash,
                                                0)+1);
    }
    //Close the database file.
    databaseFile.close();
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
    //Ask to recover image.
    emit requireRecoverImage(m_hashAlbumArtCounter.keys());
}

void KNMusicLibraryModel::installCategoryModel(KNMusicCategoryModelBase *model)
{
    //Set hash list to category model.
    model->setHashAlbumArt(&m_scaledHashAlbumArt);
    //Append the model to the category models.
    m_categoryModels.append(model);
}

void KNMusicLibraryModel::setDatabase(const QString &databasePath)
{
    m_databasePath = databasePath;
}

void KNMusicLibraryModel::setLibraryPath(const QString &libraryPath)
{
    //Get the artwork folder.
    QString artworkFolder=libraryPath + "/Artworks";
    //Set the artwork folder to image manager.
    m_imageManager->setImageFolderPath(artworkFolder);
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
        updateModelRow(detailInfoIndex, analysisItem);
        //Mission complete.
        return;
    }
    //Add the detail info to the model.
    appendRow(analysisItem.detailInfo);
    //Emit the analysis signal.
    m_imageManager->analysisAlbumArt(index(rowCount()-1, 0),
                                     analysisItem);
}

void KNMusicLibraryModel::onActionImageUpdateRow(
        const int &row,
        const KNMusicDetailInfo &detailInfo)
{
    //Generate a useless analysis item.
    KNMusicAnalysisItem item;
    //Set the detail info.
    item.detailInfo=detailInfo;
    //Update the data in the model.
    updateModelRow(row, item);
    //Called all the category model to add the image key hash.
    for(auto i=m_categoryModels.begin(); i!=m_categoryModels.end(); ++i)
    {
        //Called onActionImageRecoverComplete() slot.
        (*i)->onCategoryAlbumArtUpdate(detailInfo);
    }
}

void KNMusicLibraryModel::onActionImageRecoverComplete()
{
    //Called all the category model to udpate their data.
    for(auto i=m_categoryModels.begin(); i!=m_categoryModels.end(); ++i)
    {
        //Called onActionImageRecoverComplete() slot.
        (*i)->onActionImageRecoverComplete();
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

bool KNMusicLibraryModel::updateModelRow(
        int row,
        const KNMusicAnalysisItem &analysisItem)
{
    //Get the previous detail info.
    const KNMusicDetailInfo &previousDetailInfo=rowDetailInfo(row);
    //Update the category data.
    updateCategoryDetailInfo(previousDetailInfo, analysisItem.detailInfo);
    //Do the original update operation.
    bool result=KNMusicModel::updateRow(row, analysisItem);
    //Count down.
    count();
    //Give the result back.
    return result;
}

inline void KNMusicLibraryModel::updateCategoryDetailInfo(
        const KNMusicDetailInfo &before,
        const KNMusicDetailInfo &after)
{
    //For all the category models,
    for(auto i=m_categoryModels.begin(); i!=m_categoryModels.end(); ++i)
    {
        //Called the on action add slot.
        (*i)->onCategoryUpdate(before, after);
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

inline void KNMusicLibraryModel::reduceHashImage(const QString &imageKey)
{
    //Check the size of the original cover image, if it's 1, then remove the
    //image.
    int originalCount=m_hashAlbumArtCounter.value(imageKey, 0);
    //If the original count is 0, means the image is not exist, ignore it.
    switch(originalCount)
    {
    case 0:
        break;
    case 1:
        //Remove the count.
        m_hashAlbumArtCounter.remove(imageKey);
        //Remove the image.
        m_scaledHashAlbumArt.remove(imageKey);
        //Delete the file.
        m_imageManager->removeHashImage(imageKey);
        break;
    default:
        //Reduce the hash album art counter down.
        m_hashAlbumArtCounter.insert(imageKey, originalCount-1);
    }
}

inline void KNMusicLibraryModel::count(int counts)
{
    //Increase the counter.
    m_operateCounter+=counts;
    //Check out the operate counter is greater than limit.
    if(m_operateCounter >= MaxOperateCount)
    {
        //Clear the operate counter.
        m_operateCounter = 0;
        //Write the database to hard disk.
        writeDatabase();
    }
}

inline void KNMusicLibraryModel::writeDatabase()
{
    //Check out the database path.
    if(m_databasePath.isEmpty())
    {
        return;
    }
    //Initial the file info.
    QFileInfo fileInfo(m_databasePath);
    //Check the database file environment.
    KNUtil::ensurePathValid(fileInfo.absolutePath());
    //Open the database file.
    QFile databaseFile(m_databasePath);
    //Open the database file.
    if(!databaseFile.open(QIODevice::WriteOnly))
    {
        //Failed to open the database file.
        return;
    }
    //Geneate the data stream for data writing.
    QDataStream databaseStream(&databaseFile);
    //Write the version of database.
    databaseStream << (quint32)MajorVersion << (quint32)MinorVersion
                   //Write the row count.
                   << (quint64)rowCount();
    //Write all the detail infos.
    for(int i=0, rows=rowCount(); i<rows; ++i)
    {
        //Write the detail info data.
        databaseStream << rowDetailInfo(i);
    }
    //Close the file.
    databaseFile.close();
}

KNMusicLibraryImageManager *KNMusicLibraryModel::imageManager() const
{
    return m_imageManager;
}
