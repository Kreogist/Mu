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
#include <QSize>

#include "knmusiccategoryproxymodel.h"

KNMusicCategoryProxyModel::KNMusicCategoryProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    //Set properties.
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterKeyColumn(0);
}

QVariant KNMusicCategoryProxyModel::data(const QModelIndex &index,
                                         int role) const
{
    //Hack the size.
    if(role==Qt::SizeHintRole)
    {
        return QSize(35, 40);
    }
    return QSortFilterProxyModel::data(index, role);
}

QModelIndex KNMusicCategoryProxyModel::categoryIndex(const QString &categoryText)
{
    //Check the source model first.
    if(sourceModel()==nullptr)
    {
        return QModelIndex();
    }
    //If it's empty, return the default empty category.
    if(categoryText.isEmpty())
    {
        return index(0,0);
    }
    //Find the category in display role.
    QModelIndexList categoryMatch=match(index(0,0),
                                        Qt::DisplayRole,
                                        categoryText,
                                        1);
    //If it's empty, means we can't find it.
    if(categoryMatch.isEmpty())
    {
        return QModelIndex();
    }
    //Or else return the result.
    return categoryMatch.first();
}

bool KNMusicCategoryProxyModel::lessThan(const QModelIndex &left,
                                         const QModelIndex &right) const
{
    //The 0 row is always the minimal row.
    if(left.row()==0)
    {
        return true;
    }
    if(right.row()==0)
    {
        return false;
    }
    //Return the default less than.
    return QSortFilterProxyModel::lessThan(left, right);
}

bool KNMusicCategoryProxyModel::filterAcceptsRow(int source_row,
                                                 const QModelIndex &source_parent) const
{
    //If the source row is 0 row and it's visible.
    if(source_row==0 && sourceModel()->data(sourceModel()->index(0,0),
                                            Qt::UserRole+1).toInt()==0)
    {
        return false;
    }
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}
