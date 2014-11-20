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
#include "knmusiccategorymodel.h"

KNMusicCategoryModel::KNMusicCategoryModel(QObject *parent) :
    QStandardItemModel(parent)
{
    //Set the default no album icon.
    setNoAlbumIcon(KNMusicGlobal::instance()->noAlbumArt());
    //Initial the model.
    resetModel();
}

void KNMusicCategoryModel::resetModel()
{
    //Clear the model.
    clear();
    //Add initial item: the blank item.
    QStandardItem *currentItem=generateItem(m_noCategoryText);
    currentItem->setData(0, CategoryItemSizeRole);
    appendRow(currentItem);
}

QPixmap KNMusicCategoryModel::noAlbumIcon() const
{
    return m_noAlbumIcon;
}

void KNMusicCategoryModel::setNoAlbumIcon(const QPixmap &noAlbumIcon)
{
    m_noAlbumIcon = noAlbumIcon.scaled(m_iconSize,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
}

int KNMusicCategoryModel::categoryIndex() const
{
    return m_categoryIndex;
}

void KNMusicCategoryModel::setCategoryIndex(int categoryIndex)
{
    m_categoryIndex=categoryIndex;
}


QString KNMusicCategoryModel::noCategoryText() const
{
    return m_noCategoryText;
}

void KNMusicCategoryModel::setNoCategoryText(const QString &noCategoryText)
{
    m_noCategoryText=noCategoryText;
    //Update the data.
    setData(index(0,0), m_noCategoryText, Qt::DisplayRole);
}

void KNMusicCategoryModel::onCategoryAdded(const QString &categoryText)
{
    //Check if it need to be add to blank item.
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
    //Search the category text.
    QModelIndexList results=
            match(index(0,0), Qt::DisplayRole, categoryText, 1);
    if(results.isEmpty())
    {
        //We need to generate a new item for it.
        QStandardItem *item=generateItem(categoryText);
        item->setData(1, CategoryItemSizeRole);
        appendRow(item);
    }
    else
    {
        //Add the counter of the result.
        QModelIndex resultIndex=results.first();
        setData(resultIndex,
                data(resultIndex, CategoryItemSizeRole).toInt()+1,
                CategoryItemSizeRole);
    }
}

void KNMusicCategoryModel::onCategoryRemoved(const QString &categoryText)
{
    QModelIndex resultIndex;
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
    }
}

void KNMusicCategoryModel::onCoverImageUpdate(const QString &categoryText,
                                              const QString &imageKey,
                                              const QPixmap &image)
{
    //Check if it need to be add to blank item.
    if(categoryText.isEmpty())
    {
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
    else
    {
        //Check is the cover image has a key.
        QModelIndex resultIndex=results.first();
        if(data(resultIndex, CategoryArtworkKeyRole).isNull())
        {
            //Set the image key.
            setData(resultIndex,
                    imageKey,
                    CategoryArtworkKeyRole);
            //Set the cover image.
            setData(resultIndex,
                    image.scaled(m_iconSize,
                                 Qt::KeepAspectRatio,
                                 Qt::SmoothTransformation),
                    Qt::DecorationRole);
        }
    }
}

QStandardItem *KNMusicCategoryModel::generateItem(const QString &itemText,
                                                  const QPixmap &itemIcon)
{
    QStandardItem *currentItem=new QStandardItem(itemText);
    currentItem->setData(itemIcon.isNull()?
                             m_noAlbumIcon:
                             itemIcon.scaled(m_iconSize,
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation),
                         Qt::DecorationRole);
    currentItem->setEditable(false);
    return currentItem;
}

QSize KNMusicCategoryModel::iconSize() const
{
    return m_iconSize;
}
