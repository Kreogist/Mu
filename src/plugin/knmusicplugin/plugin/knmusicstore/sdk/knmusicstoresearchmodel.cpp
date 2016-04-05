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

#include "knmusicstoresearchmodel.h"

KNMusicStoreSearchModel::KNMusicStoreSearchModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_totalCount(-1)
{

}

QVariant KNMusicStoreSearchModel::data(const QModelIndex &index,
                                           int role) const
{
    //Check the index.
    if(!index.isValid())
    {
        //Ignore the invalid index request.
        return QVariant();
    }
    //Check the role.
    switch(role)
    {
    case Qt::DisplayRole:
        //Give back the specific data.
        return m_searchResult.at(index.row()).at(index.column());
    case Qt::SizeHintRole:
        return QSize(0, SearchItemHeight);
    default:
        //All the other data will be null.
        return QVariant();
    }
}

QVariant KNMusicStoreSearchModel::headerData(int section,
                                             Qt::Orientation orientation,
                                             int role) const
{
    //Check out the orientation.
    if(orientation==Qt::Vertical)
    {
        //Ignore the request.
        return QVariant();
    }
    //Check section.
    if(section>m_columnFieldNames.size())
    {
        //The section is a invalid part.
        return QVariant();
    }
    //Check the role.
    switch(role)
    {
    case Qt::DisplayRole:
        //Give back the text.
        return m_columnFieldNames.at(section);
    case Qt::TextAlignmentRole:
        //Vertical center and left align.
        return QVariant(Qt::AlignVCenter | Qt::AlignLeft);
    default:
        return QVariant();
    }
}

int KNMusicStoreSearchModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Search result size is the row count.
    return m_searchResult.size();
}

int KNMusicStoreSearchModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Column field name list provides the column count.
    return m_columnFieldNames.size();
}

void KNMusicStoreSearchModel::clear()
{
    //Check album list is empty first.
    if(m_searchResult.isEmpty())
    {
        //If it's empty, then we don't need to check.
        return;
    }
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), 0, m_searchResult.size()-1);
    //Remove all the data.
    m_searchResult.clear();
    //As the documentation said, called this after remove rows.
    endRemoveRows();
}

QVariant KNMusicStoreSearchModel::searchResultId(int row) const
{
    return m_searchResultId.at(row);
}

void KNMusicStoreSearchModel::setSearchResult(
        const QList<QStringList> &searchResult)
{
    //Clear the original data.
    if(!m_searchResult.isEmpty())
    {
        //Clear the raw data.
        clear();
    }
    //Add this sentence before appending.
    beginInsertRows(QModelIndex(), m_searchResult.size(),m_searchResult.size());
    //Insert new data.
    m_searchResult = searchResult;
    //End insert.
    endInsertRows();
}

void KNMusicStoreSearchModel::setSearchResultId(
        const QList<QVariant> &searchResultId)
{
    m_searchResultId = searchResultId;
}

int KNMusicStoreSearchModel::totalCount() const
{
    return m_totalCount;
}

void KNMusicStoreSearchModel::setTotalCount(int totalCount)
{
    m_totalCount = totalCount;
}

void KNMusicStoreSearchModel::setColumnFieldNames(
        const QStringList &columnFieldNames)
{
    m_columnFieldNames = columnFieldNames;
}
