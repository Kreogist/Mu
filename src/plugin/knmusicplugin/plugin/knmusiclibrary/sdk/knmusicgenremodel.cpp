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
#include <QDir>

#include "knmusicglobal.h"
#include "knutil.h"

#include "knmusicgenremodel.h"

#include <QDebug>

KNMusicGenreModel::KNMusicGenreModel(QObject *parent) :
    KNMusicCategoryModel(parent)
{
    //Load all the genre icons.
    loadGenreIcons();
}

QVariant KNMusicGenreModel::data(const QModelIndex &index, int role) const
{
    //The invalid index data connot have any valid data.
    if(!index.isValid())
    {
        return QVariant();
    }
    //Get the category item.
    const CategoryItem &item=itemAt(index.row());
    //Check out the role, we will hack the decoration role data.
    if(role==Qt::DecorationRole)
    {
        //Check out the validation of the album art hash.
        return item.albumArtHash.size()!=0?
                    m_genreIconMap.value(item.albumArtHash.first(), noAlbumArt()):
                    noAlbumArt();
    }
    //Otherwise, do the original one.
    return KNMusicCategoryModel::data(index, role);
}

void KNMusicGenreModel::onCategoryAdd(const KNMusicDetailInfo &detailInfo)
{
    //Get the category text.
    QVariant categoryText=detailInfo.textLists[categoryColumn()];
    //Check if the category is blank.
    if(categoryText.toString().isEmpty())
    {
        //Get the category list data.
        CategoryItem item=itemAt(0);
        //Increase the count.
        ++(item.count);
        //Add count of no category item.
        replaceItem(0, item);
        //Mission complete.
        return;
    }
    //Find the category text.
    for(int i=0, bound=rowCount(); i<bound; ++i)
    {
        //If we cound find the item.
        if(itemAt(i).displayText==categoryText)
        {
            //Get the category list data.
            CategoryItem item=itemAt(i);
            //Increase the count.
            ++(item.count);
            //Replace the item.
            replaceItem(i, item);
            //Mission complete.
            return;
        }
    }
    //We have to add a new category.
    CategoryItem item;
    //Save the category data.
    item.displayText=categoryText;
    item.albumArtHash.append(categoryText.toString().toLower());
    item.count=1;
    //Append the item.
    appendItem(item);
}

void KNMusicGenreModel::onCategoryUpdate(const KNMusicDetailInfo &before,
                                         const KNMusicDetailInfo &after)
{
    //Check out the category item have ever changed.
    if(before.textLists[categoryColumn()]==after.textLists[categoryColumn()])
    {
        //Nothing changed, we will do nothing.
        //Mission complete.
        return;
    }
    //Or else, we have to remove the previous data.
    onCategoryRemove(before);
    //And then add the after category.
    onCategoryAdd(after);
}

void KNMusicGenreModel::onCategoryAlbumArtUpdate(
        const KNMusicDetailInfo &detailInfo)
{
    //Now we will do nothing in the genre model.
    Q_UNUSED(detailInfo)
}

void KNMusicGenreModel::onActionImageRecoverComplete()
{
    //In genre model, we won't need to do anything.
}

inline void KNMusicGenreModel::loadGenreIcons()
{
    //Clear the hash list.
    m_genreIconMap.clear();
    //Initial the genre directory.
    QDir genreIconDir(
                KNUtil::ensurePathValid(
                    knMusicGlobal->musicLibraryPath()+"/Genres/"));
    //Initial the icon file info list.
    QFileInfoList iconInfoList=genreIconDir.entryInfoList();
    //Read throught the info list.
    for(auto i : iconInfoList)
    {
        //Check if the file is a png file, and it's readable.
        if(i.isFile() && i.suffix().toLower()=="png")
        {
            //Load the genre, if it's usable, load it to genre icon map.
            QPixmap genreImage=QPixmap(i.absoluteFilePath(), "png");
            //Check the image validation.
            if(!genreImage.isNull())
            {
                //Insert the image to the icon map.
                m_genreIconMap.insert(i.baseName().toLower(),
                                      QVariant(genreImage));
                //Continue to next item.
                continue;
            }
        }
        //Remove the no use file.
        QFile::remove(i.absoluteFilePath());
    }
}

