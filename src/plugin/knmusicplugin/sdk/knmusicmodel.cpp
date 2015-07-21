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

void KNMusicModel::appendMusic(const KNMusicDetailInfo &detailInfo)
{
    m_detailInfos.append(detailInfo);
    //Emit data changed signal.
    int appendRow=m_detailInfos.size();
    emit dataChanged(index(appendRow, 0),
                     index(appendRow, MusicDataCount));
}

void KNMusicModel::removeMusic(int row)
{
    //Check the row before.
    Q_ASSERT(row>-1 && row<m_detailInfos.size());
    //Check the data size.
    m_detailInfos.removeAt(row);
    //Update the data changed information.

}

void KNMusicModel::clear()
{
    //Clear the detail info list.
    m_detailInfos.clear();

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

