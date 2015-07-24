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
#include "knmusicplaylistmodel.h"

#include "knmusicplaylistlistmodel.h"

KNMusicPlaylistListModel::KNMusicPlaylistListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_playlistList(QList<KNMusicPlaylistModel *>()),
    m_icon(QIcon(":/plugin/music/playlist/playlist_icon.png"))
{
}

void KNMusicPlaylistListModel::append(KNMusicPlaylistModel *model)
{
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(),
                    m_playlistList.size(),
                    m_playlistList.size() + 1);
    //Append this data at the end of the list.
    m_playlistList.append(model);
    //End the insertation.
    endInsertRows();
}

int KNMusicPlaylistListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The row count is the size of the playlist model list size.
    return m_playlistList.size();
}

QVariant KNMusicPlaylistListModel::data(const QModelIndex &index,
                                        int role) const
{
    //The invalid index data cannot have any valid data.
    if(!index.isValid())
    {
        return QVariant();
    }
    //Get the specfic model.
    KNMusicPlaylistModel *model=m_playlistList.at(index.row());
    //Get the data of the role.
    switch(role)
    {
    case Qt::DisplayRole:
        return model->title();
    case Qt::DecorationRole:
        return m_icon;
    default:
        return QVariant();
    }
}

bool KNMusicPlaylistListModel::setData(const QModelIndex &index,
                                       const QVariant &value,
                                       int role)
{
    //Check validate of the index.
    if(!index.isValid())
    {
        return false;
    }
    //Get the specfic model.
    KNMusicPlaylistModel *model=m_playlistList.at(index.row());
    //For different role, we have different ways to set data.
    //Actually, you can only change the data of the display role.
    switch(role)
    {
    case Qt::DisplayRole:
        model->setTitle(value.toString());
        return true;
    default:
        return true;
    }
}

QVariant KNMusicPlaylistListModel::headerData(int section,
                                              Qt::Orientation orientation,
                                              int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    //No matter what, just simply return a null variant.
    return QVariant();
}

bool KNMusicPlaylistListModel::findTitle(const QString &title)
{
    //Find all the playlist item in the playlist.
    for(auto i=m_playlistList.constBegin(); i!=m_playlistList.constEnd(); ++i)
    {
        //Chec if the title is the same as the title we searched, then return
        //true, means we find it.
        if((*i)->title()==title)
        {
            return true;
        }
    }
    //Or else, we cannot find this title, it will be false.
    return false;
}
