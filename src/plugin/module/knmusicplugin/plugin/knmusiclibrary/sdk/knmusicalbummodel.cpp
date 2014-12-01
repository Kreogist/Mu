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
#include <QStringList>

#include "knmusicalbummodel.h"

KNMusicAlbumModel::KNMusicAlbumModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
}

void KNMusicAlbumModel::onCategoryAdded(const QList<QStandardItem *> &musicRow)
{
    //Check if it need to be add to blank item.
    QString categoryText=musicRow.at(categoryIndex())->text();
    if(categoryText.isEmpty())
    {
        QModelIndex resultIndex=index(0,0);
        setData(resultIndex,
                data(resultIndex, CategoryItemSizeRole).toInt()+1,
                CategoryItemSizeRole);
        setData(resultIndex,
                1,
                CategoryItemVisibleRole);
        return;
    }
    //Get the album artist, if there's no album artist, use the song artist
    //instead.
    QString albumArtist=musicRow.at(AlbumArtist)->text();
    if(albumArtist.isEmpty())
    {
        albumArtist=musicRow.at(Artist)->text();
    }
    //Search the category text.
    QModelIndexList results=
            match(index(0,0), Qt::DisplayRole, categoryText, 1);
    if(results.isEmpty())
    {
        //We need to generate a new item for it.
        QStandardItem *item=generateItem(categoryText);
        item->setData(1, CategoryItemSizeRole);
        //Set the album artist.
        QHash<QString, QVariant> artistList;
        artistList.insert(albumArtist, 1);
        item->setData(artistList, CategoryArtistList);
        //Add the item to category model.
        appendRow(item);
    }
    else
    {
        //Add the counter of the result.
        QModelIndex resultIndex=results.first();
        setData(resultIndex,
                data(resultIndex, CategoryItemSizeRole).toInt()+1,
                CategoryItemSizeRole);
        //Check whether the artist is in the artist list.
        QHash<QString, QVariant> artistList=data(resultIndex,
                                                 CategoryArtistList).toHash();
        if(artistList.contains(albumArtist))
        {
            //Add the artist to the list, set the data.
            artistList.insert(albumArtist,
                              artistList.value(albumArtist).toInt()+1);
        }
        else
        {
            //Add a new one for it.
            artistList.insert(albumArtist, 1);
        }
        //Set data.
        setData(resultIndex,
                artistList,
                CategoryArtistList);
    }
}

void KNMusicAlbumModel::onCategoryRemoved(const QList<QStandardItem *> &musicRow)
{
    QModelIndex resultIndex;
    QString categoryText=musicRow.at(categoryIndex())->text();
    //Check if it's in a blank item.
    if(categoryText.isEmpty())
    {
        resultIndex=index(0,0);
        int currentCategorySize=data(resultIndex, CategoryItemSizeRole).toInt();
        if(currentCategorySize==1)
        {
            setData(resultIndex,
                    0,
                    CategoryItemSizeRole);
            setData(resultIndex,
                    0,
                    CategoryItemVisibleRole);
        }
        else
        {
            setData(resultIndex,
                    currentCategorySize-1,
                    CategoryItemSizeRole);
        }
        return;
    }
    //Search the category text.
    QModelIndexList results=
            match(index(0,0), Qt::DisplayRole, categoryText, 1);
    if(results.isEmpty())
    {
        //Are you kidding me?
        return;
    }
    resultIndex=results.first();
    int currentCategorySize=resultIndex.data(CategoryItemSizeRole).toInt();
    //If current item is the last item of the category,
    if(currentCategorySize==1)
    {
        //Remove this category.
        removeRow(resultIndex.row());
    }
    else
    {
        //Reduce the count.
        setData(resultIndex, currentCategorySize-1, CategoryItemSizeRole);
        //Check the artist, and reduce the artist count.
        QHash<QString, QVariant> artistList=data(resultIndex,
                                                 CategoryArtistList).toHash();
        QString songArtist=musicRow.at(AlbumArtist)->text();
        if(artistList.contains(songArtist))
        {
            int artistSongCount=artistList.value(songArtist).toInt();
            //If this is the last song of the artist, remove the artist from the
            //list.
            if(artistSongCount==1)
            {
                artistList.remove(songArtist);
            }
            else
            {
                artistList.insert(songArtist, artistSongCount-1);
            }
            setData(resultIndex, artistList, CategoryArtistList);
        }
    }
}

void KNMusicAlbumModel::onCategoryRecover(const QList<QStandardItem *> &musicRow)
{
    //Check if it need to be add to blank item.
    QString categoryText=musicRow.at(categoryIndex())->text();
    if(categoryText.isEmpty())
    {
        QModelIndex resultIndex=index(0,0);
        setData(resultIndex,
                data(resultIndex, CategoryItemSizeRole).toInt()+1,
                CategoryItemSizeRole);
        setData(resultIndex,
                1,
                CategoryItemVisibleRole);
        return;
    }
    //Get the album artist, if there's no album artist, use the song artist
    //instead.
    QString albumArtist=musicRow.at(AlbumArtist)->text();
    if(albumArtist.isEmpty())
    {
        albumArtist=musicRow.at(Artist)->text();
    }
    //Search the category text.
    QModelIndexList results=
            match(index(0,0), Qt::DisplayRole, categoryText, 1);
    if(results.isEmpty())
    {
        //We need to generate a new item for it.
        QStandardItem *item=generateItem(categoryText);
        item->setData(1, CategoryItemSizeRole);
        item->setData(musicRow.at(Name)->data(ArtworkKeyRole),
                      CategoryArtworkKeyRole);
        //Set the album artist.
        QHash<QString, QVariant> artistList;
        artistList.insert(albumArtist, 1);
        item->setData(artistList, CategoryArtistList);
        //Add the item to category model.
        appendRow(item);
    }
    else
    {
        //Add the counter of the result.
        QModelIndex resultIndex=results.first();
        setData(resultIndex,
                data(resultIndex, CategoryItemSizeRole).toInt()+1,
                CategoryItemSizeRole);
        //Check whether the artist is in the artist list.
        QHash<QString, QVariant> artistList=data(resultIndex,
                                                 CategoryArtistList).toHash();
        if(artistList.contains(albumArtist))
        {
            //Add the artist to the list, set the data.
            artistList.insert(albumArtist,
                              artistList.value(albumArtist).toInt()+1);
        }
        else
        {
            //Add a new one for it.
            artistList.insert(albumArtist, 1);
        }
        //Set data.
        setData(resultIndex,
                artistList,
                CategoryArtistList);
    }
}
