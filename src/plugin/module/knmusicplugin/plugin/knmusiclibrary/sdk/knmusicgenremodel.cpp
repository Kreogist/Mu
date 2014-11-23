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
#include "knglobal.h"

#include "knmusicgenremodel.h"

#include <QDebug>

KNMusicGenreModel::KNMusicGenreModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    //Initial all the icon in the folder path.
    loadGenreIcons();
}

QIcon KNMusicGenreModel::genreIcon(const QString &genreName)
{
    return m_genreIconMap.value(genreName.toLower(), noAlbumIcon());
}

void KNMusicGenreModel::onCoverImageUpdate(const QString &categoryText,
                                           const QString &imageKey,
                                           const QPixmap &image)
{
    Q_UNUSED(categoryText)
    Q_UNUSED(imageKey)
    Q_UNUSED(image)
    //Do nothing.
    return;
}

QStandardItem *KNMusicGenreModel::generateItem(const QString &itemText,
                                               const QPixmap &itemIcon)
{
    Q_UNUSED(itemIcon)
    QStandardItem *currentItem=new QStandardItem(itemText);
    currentItem->setData(0, CategoryItemSizeRole);
    if(itemText.isEmpty())
    {
        currentItem->setData(noAlbumIcon(), Qt::DecorationRole);
    }
    else
    {
        QString artworkKey=itemText.toLower();
        currentItem->setData(m_genreIconMap.contains(artworkKey)?
                                 m_genreIconMap.value(artworkKey):
                                 noAlbumIcon(),
                             Qt::DecorationRole);
    }
    currentItem->setEditable(false);
    return currentItem;
}

void KNMusicGenreModel::loadGenreIcons()
{
    //Clear the hash list.
    m_genreIconMap.clear();
    //Initial the genre dir.
    QDir genreIconDir(
                KNGlobal::ensurePathAvaliable(KNMusicGlobal::musicLibraryPath()+"/Genres/"));
    //Initial the icon list.
    QFileInfoList iconInfoList=genreIconDir.entryInfoList();
    for(QFileInfoList::iterator i=iconInfoList.begin();
        i!=iconInfoList.end();
        ++i)
    {
        //If we can find a png image file, load it to hash list.
        if((*i).isFile() && (*i).suffix().toLower()=="png")
        {
            //Load the image, if it's usable, insert to genre icon map.
            QPixmap genreImage=QPixmap((*i).absoluteFilePath());
            if(!genreImage.isNull())
            {
                m_genreIconMap.insert((*i).baseName().toLower(), genreImage);
            }
        }
    }
}
