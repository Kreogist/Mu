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
#include "knmusicglobal.h"

#include "knmusiccategorymodel.h"

#include <QDebug>

KNMusicCategoryModel::KNMusicCategoryModel(QObject *parent) :
    QAbstractListModel(parent),
    m_noAlbumArt(QVariant())
{
    //Set the default no album art data.
    setNoAlbumArt(knMusicGlobal->noAlbumArt());
}

void KNMusicCategoryModel::reset()
{
    //Check out the list size.
    if(m_categoryList.isEmpty())
    {
        //We have to insert the no category data to the first.
        generateNoCategoryItem();
        //Mission complete.
        return;
    }
    //Check out the list if the list only contains one item.
    if(m_categoryList.size()==1)
    {
        //Then it should be the no category item.
        //Reset the no category item's data.
        m_categoryList.replace(0, generateNoCategoryItem());
        //Emit data changed signal.
        emit dataChanged(index(0, 0), index(0, 0));
        //Mission complete.
        return;
    }
    //Or else the data needs to be cleared and then updated.
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), 0, m_categoryList.size()-1);
    //Remove all the data.
    m_categoryList.clear();
    //Insert the new no category item data to the item.
    m_categoryList.append(generateNoCategoryItem());
    //As the documentation said, called this after remove rows.
    endRemoveRows();
}

QModelIndex KNMusicCategoryModel::categoryIndex(const QVariant &categoryText)
{
    //Find through the list.
    for(int i=0; i<m_categoryList.size(); ++i)
    {
        //If we find the text.
        if(m_categoryList.at(i).displayText==categoryText)
        {
            //Give back the index.
            return index(i, 0);
        }
    }
    //Or else return a null index.
    return QModelIndex();
}

int KNMusicCategoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The row count is the size of the category list size.
    return m_categoryList.size();
}

QVariant KNMusicCategoryModel::data(const QModelIndex &index, int role) const
{
    //The invalid index data connot have any valid data.
    if(!index.isValid())
    {
        return QVariant();
    }
    //Get the category item
    const CategoryItem &item=m_categoryList.at(index.row());
    //Get the data.
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return item.displayText;
    case Qt::DecorationRole:
        return item.albumArtHash.isEmpty()?
                    m_noAlbumArt:
                    item.albumArtHash.first();
    case CategorySize:
        return item.count;
    default:
        return QVariant();
    }
}

Qt::ItemFlags KNMusicCategoryModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

QVariant KNMusicCategoryModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    //No matter what, just simply return a null variant.
    return QVariant();
}

void KNMusicCategoryModel::setNoAlbumArt(const QPixmap &noAlbumArt)
{
    //Save the no album art icon data.
    m_noAlbumArt = QVariant(noAlbumArt.scaled(40, 40));
}

QString KNMusicCategoryModel::noCategoryText() const
{
    return m_noCategoryText;
}

bool KNMusicCategoryModel::setData(const QModelIndex &index,
                                   const QVariant &value,
                                   int role)
{
    //Check the valid of the index first.
    if(!index.isValid())
    {
        return false;
    }
    //Get the category item.
    CategoryItem categoryItem=m_categoryList.at(index.row());
    //Change the data according to the index.
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        //Change the display text.
        categoryItem.displayText=value;
        //Replace the item.
        m_categoryList.replace(index.row(), categoryItem);
        //Emit data changed signal.
        emit dataChanged(index, index, QVector<int>(1, role));
        return true;
    case CategorySize:
        //Change the size.
        categoryItem.count=value.toInt();
        //Replace the item.
        m_categoryList.replace(index.row(), categoryItem);
        //Emit the data changed signal.
        emit dataChanged(index, index, QVector<int>(1, role));
        return true;
    default:
        return false;
    }
}

void KNMusicCategoryModel::setNoCategoryText(const QString &noCategoryText)
{
    //Save the text first.
    m_noCategoryText = noCategoryText;
    //Check the list.
    if(m_categoryList.isEmpty())
    {
        //The category list is not initialized.
        //We have to insert the no category data to the first.
        appendItem(generateNoCategoryItem());
        //Mission complete.
        return;
    }
    //Get the first item in the list.
    CategoryItem noCategory=m_categoryList.at(0);
    //Change the text.
    noCategory.displayText=m_noCategoryText;
    //Replace the item.
    m_categoryList.replace(0, noCategory);
    //Emit the data changed signal.
    emit dataChanged(index(0, 0),
                     index(0, 0),
                     QVector<int>(1, Qt::DisplayRole));
}

void KNMusicCategoryModel::onCategoryAdd(const KNMusicDetailInfo &detailInfo)
{
    //Get the category text.
    QVariant categoryText=detailInfo.textLists[m_categoryColumn];
    //Check if the category is blank.
    if(categoryText.toString().isEmpty())
    {
        //Get the category list data.
        CategoryItem item=m_categoryList.at(0);
        //Increase the count.
        ++(item.count);
        //Add count of no category item.
        replaceItem(0, item);
        //Mission complete.
        return;
    }
    //Find the category text.
    for(int i=0; i<m_categoryList.size(); ++i)
    {
        //If we could find the item.
        if(m_categoryList.at(i).displayText==categoryText)
        {
            //Get the category list data.
            CategoryItem item=m_categoryList.at(i);
            //Increase the count.
            ++(item.count);
            //Check out the cover image hash.
            if(!detailInfo.coverImageHash.isEmpty())
            {
                //Add the hash string to the category.
                item.albumArtHash.append(detailInfo.coverImageHash);
            }
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
    item.count=1;
    //Check out the hash key.
    if(!detailInfo.coverImageHash.isEmpty())
    {
        //Add the cover image hash to the category.
        item.albumArtHash.append(detailInfo.coverImageHash);
    }
    //Append the item.
    appendItem(item);
}

void KNMusicCategoryModel::onCategoryRemove(const KNMusicDetailInfo &detailInfo)
{
    //Get the category text.
    QVariant categoryText=detailInfo.textLists[m_categoryColumn];
    //Check if the category is blank.
    if(categoryText.toString().isEmpty())
    {
        //Reduce count of the no category item.
        reduceCount(0);
        //Mission complete.
        return;
    }
    //Find the category text.
    for(int i=0; i<m_categoryList.size(); ++i)
    {
        //If we could find the item.
        if(m_categoryList.at(i).displayText==categoryText)
        {
            //Check out the counter.
            if(m_categoryList.at(i).count==1)
            {
                //This is the last item, we will remove this category.
                removeItem(i);
                return;
            }
            //Reduce count of row i.
            reduceCount(i);
            //Mission complete.
            return;
        }
    }
    //If we can get here...I mean WTF...
}

inline KNMusicCategoryModel::CategoryItem
        KNMusicCategoryModel::generateNoCategoryItem()
{
    //Generate the no category item.
    CategoryItem noCategory;
    noCategory.displayText=m_noCategoryText;
    //Give back the no category item.
    return noCategory;
}

inline void KNMusicCategoryModel::appendItem(const CategoryItem &item)
{
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(),
                    m_categoryList.size(),
                    m_categoryList.size());
    //Append this data at the end of the list.
    m_categoryList.append(item);
    //End the insertation.
    endInsertRows();
}

void KNMusicCategoryModel::removeItem(const int &row)
{
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), row, row);
    //Remove the data in the category list.
    m_categoryList.removeAt(row);
    //As the documentation said, called this after remove rows.
    endRemoveRows();
}

void KNMusicCategoryModel::replaceItem(const int &row,
                                       const CategoryItem &item)
{
    //Replace the original item.
    m_categoryList.replace(row, item);
    //Emit the data changed signal.
    emit dataChanged(index(row), index(row));
}

void KNMusicCategoryModel::reduceCount(const int &row)
{
    //Get the category list data.
    CategoryItem item=m_categoryList.at(row);
    //Increase the item counter.
    --(item.count);
    //Replace the original item.
    m_categoryList.replace(row, item);
    //Emit the data changed signal.
    emit dataChanged(index(row), index(row), QVector<int>(1, CategorySize));
}

int KNMusicCategoryModel::categoryColumn() const
{
    return m_categoryColumn;
}

void KNMusicCategoryModel::setCategoryColumn(int categoryColumn)
{
    //Save the category item first.
    m_categoryColumn = categoryColumn;
    //Check the validation.
    if(m_categoryColumn>MusicDataCount || m_categoryColumn<0)
    {
        //Make it valid.
        m_categoryColumn=Name;
    }
}
