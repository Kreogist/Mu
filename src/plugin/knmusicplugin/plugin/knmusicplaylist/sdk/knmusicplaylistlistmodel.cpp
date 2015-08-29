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

KNMusicPlaylistListModel::~KNMusicPlaylistListModel()
{
    //Delete all the surplus model to recover the memory.
    for(auto i=m_playlistList.begin(); i!=m_playlistList.end(); ++i)
    {
        //Change the parent of the current model to nullptr, and the parent
        //won't delete it twice.
        (*i)->setParent(nullptr);
        //Delete the model.
        delete (*i);
    }
}

KNMusicPlaylistModel *KNMusicPlaylistListModel::playlist(
        const QModelIndex &index)
{
    //Ensure the valid of the index.
    if(!index.isValid())
    {
        return nullptr;
    }
    //Return the model in the list.
    return m_playlistList.at(index.row());
}

QModelIndex KNMusicPlaylistListModel::append(KNMusicPlaylistModel *model)
{
    //Check the model
    if(!model)
    {
        //Give back a null index.
        return QModelIndex();
    }
    //We should check the model in the list first.
    if(m_playlistList.contains(model))
    {
        //Give back the existing index.
        return index(m_playlistList.indexOf(model), 0);
    }
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(),
                    m_playlistList.size(),
                    m_playlistList.size());
    //Change the parent relationship.
    model->setParent(this);
    //Append this data at the end of the list.
    m_playlistList.append(model);
    //End the insertation.
    endInsertRows();
    //Check the size of the playlist, if it's 1, the ask to show the content.
    if(m_playlistList.size()==1)
    {
        emit requireShowContent();
    }
    //Give back the index of the model.
    return index(m_playlistList.size()-1, 0);
}

bool KNMusicPlaylistListModel::removeModel(KNMusicPlaylistModel *model)
{
    //Check the model is null or not.
    if(!model)
    {
        return false;
    }
    //Check the row of the model.
    int rowIndex=m_playlistList.indexOf(model);
    //If we cannot find the row, this mission is failed.
    if(rowIndex==-1)
    {
        //Failed to remove the model.
        return false;
    }
    //Remove the row.
    return removeRow(rowIndex);
}

QModelIndex KNMusicPlaylistListModel::insert(int row,
                                             KNMusicPlaylistModel *model)
{
    //Ensure the row is valid.
    Q_ASSERT(row>-1 && row<m_playlistList.size());
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(), row, row + 1);
    //Insert the data to the list.
    m_playlistList.insert(row, model);
    //End the insertation.
    endInsertRows();
    //Check the size of the playlist, if it's 1, the ask to show the content.
    if(m_playlistList.size()==1)
    {
        emit requireShowContent();
    }
    //Give back the model index.
    return index(row, 0);
}

bool KNMusicPlaylistListModel::removeRows(int row,
                                          int count,
                                          const QModelIndex &parent)
{
    Q_UNUSED(parent)
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), row, row+count-1);
    //Remove those from the list.
    while(count--)
    {
        //Get the model from the playlist list.
        KNMusicPlaylistModel *model=m_playlistList.takeAt(row);
        //Remove the parent, and the parent won't clear this again.
        model->setParent(nullptr);
        //Delete the model to recover the memory.
        delete model;
    }
    //As the documentation said, called this after remove rows.
    endRemoveRows();
    //Check the size of the playlist, if it's 0, the ask to hide the content.
    if(m_playlistList.size()==0)
    {
        emit requireHideContent();
    }
    //Remove complete.
    return true;
}

int KNMusicPlaylistListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The row count is the size of the playlist model list size.
    return m_playlistList.size();
}

Qt::ItemFlags KNMusicPlaylistListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractListModel::flags(index);
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
    case Qt::EditRole:
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
    case Qt::EditRole:
        //The display and edit role of the model is the title of the model.
        model->setTitle(value.toString());
        //Emit the data changed signal.
        emit dataChanged(index, index, QVector<int>(1, role));
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
