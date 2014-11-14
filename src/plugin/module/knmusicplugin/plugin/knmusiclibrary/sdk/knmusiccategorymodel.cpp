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
    resetModel();
}

void KNMusicCategoryModel::resetModel()
{
    //Clear the model.
    clear();
    //Add initial item: the blank item.
    QStandardItem *currentItem=new QStandardItem();
    currentItem->setData(KNMusicGlobal::instance()->noAlbumArt(),
                         Qt::DecorationRole);
    currentItem->setEditable(false);
    currentItem->setText(m_noCategoryText);
    currentItem->setData(0, CategoryItemSizeRole);
    appendRow(currentItem);
}
int KNMusicCategoryModel::categoryIndex() const
{
    return m_categoryIndex;
}

void KNMusicCategoryModel::setCategoryIndex(int categoryIndex)
{
    m_categoryIndex = categoryIndex;
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
        setData(index(0,0),
                data(index(0,0), CategoryItemSizeRole).toInt()+1,
                CategoryItemSizeRole);
        return;
    }
    //Search the category text.
    QModelIndexList results=
            match(index(0,0), Qt::DisplayRole, categoryText, 1);
    if(results.isEmpty())
    {
        //We need to generate a new item for it.
        QStandardItem *item=new QStandardItem(categoryText);
        item->setEditable(false);
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
    //Check if it's in a blank item.
    if(categoryText.isEmpty())
    {
        setData(index(0,0),
                data(index(0,0), CategoryItemSizeRole).toInt()-1,
                CategoryItemSizeRole);
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
    QModelIndex resultIndex=results.first();
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
