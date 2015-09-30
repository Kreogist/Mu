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
#include "knmusiccategorymodelbase.h"

#include "knmusiccategoryproxymodel.h"

KNMusicCategoryProxyModel::KNMusicCategoryProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

QModelIndex KNMusicCategoryProxyModel::categoryIndex(
        const QVariant &categoryText)
{
    //Check the source model first.
    if(sourceModel()==nullptr)
    {
        //Give back a null index.
        return QModelIndex();
    }
    //If it's empty, return the default empty category.
    if(categoryText.toString().isEmpty())
    {
        //The first item is the right one.
        return index(0, 0);
    }
    //Or else, we have to find out the data.
    KNMusicCategoryModelBase *categoryModel=
            static_cast<KNMusicCategoryModelBase *>(sourceModel());
    //Give out the category model.
    QModelIndex sourceCategoryIndex=categoryModel->categoryIndex(categoryText);
    //Check the validation of the source category index.
    if(sourceCategoryIndex.isValid())
    {
        //Give back the proxy index.
        return mapFromSource(sourceCategoryIndex);
    }
    //Or else, give back the null index.
    return QModelIndex();
}

bool KNMusicCategoryProxyModel::lessThan(const QModelIndex &source_left,
                                         const QModelIndex &source_right) const
{
    //Here's a hack to make the no category item to be at the top most.
    //No matter who meets the 0 row, it will always be the smaller one.
    if(source_left.row()==0)
    {
        return true;
    }
    if(source_right.row()==0)
    {
        return false;
    }
    //For others, we can simply compare those data.
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

bool KNMusicCategoryProxyModel::filterAcceptsRow(
        int source_row,
        const QModelIndex &source_parent) const
{
    //Check out the source row.
    //If the source row is 0 row and it has no data, we won't accept the row.
    if(source_row==0)
    {
        return sourceModel()->data(
                    sourceModel()->index(0, 0),
                    KNMusicCategoryModelBase::CategorySizeRole).toInt()!=0;
    }
    //Use the default accept.
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

