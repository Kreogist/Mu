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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "knhashpixmaplist.h"

#include "knmusiclibraryimagemanager.h"

KNMusicLibraryImageManager::KNMusicLibraryImageManager(QObject *parent) :
    QObject(parent)
{
    //Connect mission request loop.
    connect(this, &KNMusicLibraryImageManager::requireSaveNext,
            this, &KNMusicLibraryImageManager::onActionSaveNext);
}

KNHashPixmapList *KNMusicLibraryImageManager::pixmapList() const
{
    return m_pixmapList;
}

void KNMusicLibraryImageManager::setPixmapList(KNHashPixmapList *pixmapList)
{
    //Save pixmap list.
    m_pixmapList=pixmapList;
    //Link request.
    connect(m_pixmapList, &KNHashPixmapList::requireSaveImage,
            this, &KNMusicLibraryImageManager::saveImage);
}

void KNMusicLibraryImageManager::recoverFromFolder()
{
    //Check is the path exist or it's a file.
    QFileInfo pathChecker(m_imageFolderPath);
    QDir imageDir(m_imageFolderPath);
    //Check it's existance and it's file or not.
    if(pathChecker.exists() && pathChecker.isFile())
    {
        //Delete the file.
        QFile fileRemover(pathChecker.absoluteFilePath());
        fileRemover.remove();
    }
    //Check if it's not exist, simply generate the folder.
    if(!pathChecker.exists())
    {
        //Generate the folder.
        imageDir.mkpath(imageDir.absolutePath());
        //Do nothing, return.
        return;
    }
    //Load the image in the dir.
    QFileInfoList contentInfos=imageDir.entryInfoList();
    //Check each file.
    for(QFileInfoList::iterator i=contentInfos.begin();
        i!=contentInfos.end();
        ++i)
    {
        //Load all the png image to hash list.
        if((*i).isFile() && (*i).suffix().toLower()=="png")
        {
            //Load the image.
            QImage currentImage=QImage((*i).absoluteFilePath(), "png");
            //If there's image data, insert to pixmap list.
            if(!currentImage.isNull())
            {
                m_pixmapList->setImage((*i).completeBaseName(), currentImage);
            }
        }
    }
    //Emit recover complete signal.
    emit recoverComplete();
}

void KNMusicLibraryImageManager::removeImage(const QString &imageHash)
{
    //Check is the file exist.
    QFileInfo imageFileInfo(m_imageFolderPath + "/" + imageHash + ".png");
    //If it's exist.
    if(imageFileInfo.exists())
    {
        //If it's a file, remove it.
        if(imageFileInfo.isFile())
        {
            QFile imageFile(imageFileInfo.absoluteFilePath());
            imageFile.remove();
            return;
        }
    }
}

void KNMusicLibraryImageManager::saveImage(const QString &imageHash)
{
    //Append the image hash to mission list.
    m_imageHashList.append(imageHash);
    //Ask to save.
    emit requireSaveNext();
}

void KNMusicLibraryImageManager::onActionSaveNext()
{
    //Check the mission list is empty or not.
    if(m_imageHashList.isEmpty())
    {
        return;
    }
    //Get the first image.
    QString hashData=m_imageHashList.takeFirst();
    QImage currentImage=m_pixmapList->image(hashData);
    //Check is the image null, if not, save it.
    if(!currentImage.isNull())
    {
        //Using hash data as file name.
        currentImage.save(m_imageFolderPath + "/" + hashData + ".png",
                          "PNG");
    }
    //Ask to save next image.
    emit requireSaveNext();
}

QString KNMusicLibraryImageManager::imageFolderPath() const
{
    return m_imageFolderPath;
}

void KNMusicLibraryImageManager::setImageFolderPath(const QString &imageFolderPath)
{
    m_imageFolderPath=imageFolderPath;
}
