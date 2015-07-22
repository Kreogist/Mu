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

#include "knmusicmodel.h"

#include <QDebug>

KNMusicModel::KNMusicModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_detailInfos(QList<KNMusicDetailInfo>())
{

}

void KNMusicModel::appendRow(const KNMusicDetailInfo &detailInfo)
{
    //Append the data at the end of the list.
    m_detailInfos.append(detailInfo);
    //Emit data changed signal.
    int rowIndex=m_detailInfos.size()-1;
    emit dataChanged(index(rowIndex, 0),
                     index(rowIndex, MusicDataCount));
}

void KNMusicModel::appendRows(const QList<KNMusicDetailInfo> &detailInfos)
{
    //Append the data at the end of the rows.
    m_detailInfos.append(detailInfos);
    //Emit data changed signal.
    int topRow=m_detailInfos.size()-detailInfos.size()-1;
    emit dataChanged(index(topRow, 0),
                     index(m_detailInfos.size()-1, MusicDataCount));
}

bool KNMusicModel::insertRow(int row, const KNMusicDetailInfo &detailInfo)
{
    //Check the row first.
    Q_ASSERT(row>-1 && row<m_detailInfos.size());
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(), row, row + 1);
    //Insert the detail info into the list.
    m_detailInfos.insert(row, detailInfo);
    //As the documentation said, called this after insert rows.
    endInsertRows();
    return true;
}

bool KNMusicModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    //Remove those datas from the list.
    while(rows--)
    {
        m_detailInfos.removeAt(position);
    }
    //As the documentation said, called this after remove rows.
    endRemoveRows();
    return true;
}

void KNMusicModel::clear()
{
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), 0, m_detailInfos.size()-1);
    //Clear the detail info list.
    m_detailInfos.clear();
    //As the documentation said, called this after remove rows.
    endRemoveRows();
}

int KNMusicModel::rowCount(const QModelIndex &parent) const
{
    //No sub rows for every avaliable parent.
    if(parent.isValid())
    {
        return 0;
    }
    //The row count is actually the songs size.
    return m_detailInfos.size();
}

int KNMusicModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //This is a fixed column model.
    return MusicDataCount+1;
}

QVariant KNMusicModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    //Ignore the vertical request.
    if(Qt::Vertical==orientation)
    {
        return QAbstractTableModel::headerData(section, Qt::Vertical, role);
    }
    //For horizontal headers, the caption should be the
    switch(role)
    {
    case Qt::DisplayRole:
        return section==MusicDataCount?
                    QString():
                    knMusicGlobal->treeViewHeaderText(section);
    default:
        return QAbstractTableModel::headerData(section, Qt::Horizontal, role);
    }

}

QVariant KNMusicModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        return index.column()==MusicDataCount?
                    QString():
                    m_detailInfos.at(index.row()).textLists[index.column()];
    default:
        return QVariant();
    }
}

