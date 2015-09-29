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
#include <QIcon>

#include "knutil.h"

#include "knmusiclibraryimagesaver.h"

KNMusicLibraryImageSaver::KNMusicLibraryImageSaver(QObject *parent) :
    QObject(parent),
    m_savingQueue(QStringList()),
    m_imageFolderPath(QString()),
    m_hashAlbumArt(nullptr)
{
    //Link the save next signal to save next slot.
    connect(this, &KNMusicLibraryImageSaver::requireSaveNext,
            this, &KNMusicLibraryImageSaver::onActionSaveNext,
            Qt::QueuedConnection);
}

QHash<QString, QVariant> *KNMusicLibraryImageSaver::hashAlbumArt() const
{
    return m_hashAlbumArt;
}

void KNMusicLibraryImageSaver::setHashAlbumArt(
        QHash<QString, QVariant> *hashAlbumArt)
{
    m_hashAlbumArt = hashAlbumArt;
}

void KNMusicLibraryImageSaver::saveImage(const QString &imageHashKey)
{
    //Append the image hash to queue.
    m_savingQueue.append(imageHashKey);
    //Start save.
    emit requireSaveNext();
}

void KNMusicLibraryImageSaver::onActionSaveNext()
{
    //Check out whether the saving queue is empty, and the hash list pointer.
    if(m_hashAlbumArt==nullptr || m_savingQueue.isEmpty())
    {
        //We won't start the queue.
        return;
    }
    //Pick up a hash key from the queue.
    QString imageKey=m_savingQueue.takeFirst();
    QPixmap imagePixmap=(m_hashAlbumArt->value(imageKey,
                                               QVariant()).value<QPixmap>());
    //Write the image pixmap to image folder.
    if(!imagePixmap.isNull())
    {
        //Transfrom the pixmap to image.
        imagePixmap.toImage().save(KNUtil::ensurePathValid(m_imageFolderPath) +
                                   "/" + imageKey + ".png",
                                   "PNG");
    }
    //Ask to save next.
    emit requireSaveNext();
}

QString KNMusicLibraryImageSaver::imageFolderPath() const
{
    return m_imageFolderPath;
}

void KNMusicLibraryImageSaver::setImageFolderPath(
        const QString &imageFolderPath)
{
    m_imageFolderPath = KNUtil::ensurePathValid(imageFolderPath);
}
