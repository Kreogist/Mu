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

#include "knmusiclyricsdetaillistmodel.h"

KNMusicLyricsDetailListModel::KNMusicLyricsDetailListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_nullData(QVariant())
{
}

QList<KNMusicLyricsDownloader::KNMusicLyricsDetails>
KNMusicLyricsDetailListModel::lyricsList() const
{
    return m_lyricsList;
}

void KNMusicLyricsDetailListModel::setLyricsList(
        const QList<KNMusicLyricsDownloader::KNMusicLyricsDetails> &lyricsList)
{
    //Emit the reset model.
    emit beginResetModel();
    //Save the lyrics list.
    m_lyricsList = lyricsList;
    //Emit the end reset model.
    emit endResetModel();
}

QVariant KNMusicLyricsDetailListModel::data(const QModelIndex &index,
                                            int role) const
{
    //Check the validation of the data.
    if(!index.isValid())
    {
        //Give back a null data.
        return m_nullData;
    }
    //Get the lyrics list item.
    KNMusicLyricsDownloader::KNMusicLyricsDetails detail=
            m_lyricsList.at(index.row());
    //Check the role.
    switch(role)
    {
    case Qt::DisplayRole:
        return QVariant(detail.title + "\n" + detail.artist);
    case Qt::SizeHintRole:
        return QSize(12, 34);
    case Qt::UserRole:
        return detail.lyricsData;
    default:
        return m_nullData;
    }
}

int KNMusicLyricsDetailListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_lyricsList.size();
}
