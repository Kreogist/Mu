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
#include <QCryptographicHash>
#include <QDir>
#include <QIcon>

#include "knmusicglobal.h"
#include "knmusicparser.h"

#include "knmusiclibraryimagemanager.h"

#include <QDebug>

KNMusicLibraryImageManager::KNMusicLibraryImageManager(QObject *parent) :
    QObject(parent),
    m_analysisQueue(QLinkedList<AnalysisQueueItem>()),
    m_hashAlbumArt(nullptr)
{
    //Link the analysis request signal and response slot in queue connection.
    connect(this, &KNMusicLibraryImageManager::requireAnalysisNext,
            this, &KNMusicLibraryImageManager::analysisNext,
            Qt::QueuedConnection);
}

void KNMusicLibraryImageManager::analysisAlbumArt(
        QPersistentModelIndex itemIndex,
        KNMusicAnalysisItem item)
{
    //Check out the validation of item index.
    if(!itemIndex.isValid())
    {
        //Ignore the item index.
        return;
    }
    //Generate the queue item.
    AnalysisQueueItem queueItem;
    //Save the item information.
    queueItem.itemIndex=itemIndex;
    queueItem.item=item;
    //Add the item to queue.
    m_analysisQueue.append(queueItem);
    //And of course, ask to analysis the next item, start looping.
    emit requireAnalysisNext();
}

void KNMusicLibraryImageManager::recoverAlbumArt()
{
    //Check out the image hash list has been set or not.
    if(m_hashAlbumArt==nullptr)
    {
        return;
    }
    //Get the image folder path information.
    QFileInfo pathChecker(m_imageFolderPath);
    //Check is the path exist or it's a file
    if(pathChecker.exists() && pathChecker.isFile())
    {
        //Delete the file.
        QFile::remove(pathChecker.absoluteFilePath());
    }
    //Generate the image directory.
    QDir imageDir(m_imageFolderPath);
    //Check if it's not exist, simply generate the folder.
    if(!pathChecker.exists())
    {
        //Generate the folder.
        imageDir.mkpath(imageDir.absolutePath());
        //Do nothing, return.
        return;
    }
    //Check the existance again, if the path is still not exist, then failed.
    if(!pathChecker.isDir() || !pathChecker.exists())
    {
        return;
    }
    //Get the entry info list.
    QFileInfoList contentInfos=imageDir.entryInfoList();
    //Check each file.
    for(auto i : contentInfos)
    {
        //Check the information of all the file and the suffix.
        if(i.isFile() && i.suffix().toLower()=="png")
        {
            //Load the image.
            QPixmap currentPixmap=QPixmap(i.absoluteFilePath(), "png");
            //If there's image data is not null.
            if(currentPixmap.isNull())
            {
                //Remove the file away.
                QFile::remove(i.absoluteFilePath());
            }
            else
            {
                //Insert the image to the hash list.
                m_hashAlbumArt->insert(i.completeBaseName(),
                                       QVariant(currentPixmap));
            }
        }
    }
    //After loading the images, emit recover signal.
    emit recoverImageComplete();
}

void KNMusicLibraryImageManager::analysisNext()
{
    //Check is there no item in the queue.
    if(m_analysisQueue.isEmpty())
    {
        return;
    }
    //Get the first item from the queue.
    AnalysisQueueItem currentItem=m_analysisQueue.takeFirst();
    //Get the analysis item.
    KNMusicAnalysisItem &analysisItem=currentItem.item;
    //Use the parser to parse the analysis item.
    knMusicGlobal->parser()->parseAlbumArt(analysisItem);
    //Check the result of the cover image.
    if(!analysisItem.coverImage.isNull() && m_hashAlbumArt!=nullptr)
    {
        //The cover image is not null, get the hash key.
        analysisItem.detailInfo.coverImageHash=
                insertHashImage(analysisItem.coverImage);
        //Ask to update the row, check the index first.
        if(currentItem.itemIndex.isValid())
        {
            //Emit the update signal.
            emit requireUpdateRow(currentItem.itemIndex.row(),
                                  analysisItem.detailInfo);
        }
    }
    //Ask to update the next item.
    emit requireAnalysisNext();
}

inline QString KNMusicLibraryImageManager::insertHashImage(const QImage &image)
{
    //Calculate the meta data of the image(MD4 of image content).
    QByteArray hashResult=
            QCryptographicHash::hash(QByteArray((char *)image.bits(),
                                                image.byteCount()),
                                     QCryptographicHash::Md4);
    //Get the image key of the image.
    QString imageHashKey;
    //Add hash result data to image hash key.
    for(int i=0; i<hashResult.size(); ++i)
    {
        //Get the text string of the current number.
        QString currentByteText=QString::number((quint8)hashResult.at(i), 16);
        //Check out the data of image hash key.
        imageHashKey.append(hashResult.at(i)<16?"0"+currentByteText:
                                                currentByteText);
    }
    //Check whether the hash is already exists in image list.
    if(!m_hashAlbumArt->contains(imageHashKey))
    {
        //If this image is first time exist in the hash list, save the image
        //first.
        m_hashAlbumArt->insert(imageHashKey,
                               QVariant(QPixmap::fromImage(image)));
        //Ask to save the image.
        emit requireSaveImage(imageHashKey);
    }
    return imageHashKey;
}

QString KNMusicLibraryImageManager::imageFolderPath() const
{
    return m_imageFolderPath;
}

void KNMusicLibraryImageManager::setImageFolderPath(
        const QString &imageFolderPath)
{
    m_imageFolderPath = imageFolderPath;
}

QHash<QString, QVariant> *KNMusicLibraryImageManager::hashAlbumArt() const
{
    return m_hashAlbumArt;
}

void KNMusicLibraryImageManager::setHashAlbumArt(
        QHash<QString, QVariant> *hashAlbumArt)
{
    m_hashAlbumArt = hashAlbumArt;
}
