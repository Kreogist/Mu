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
#include "knmusicalbumlistproxymodel.h"

#include <QDebug>

KNMusicAlbumListProxyModel::KNMusicAlbumListProxyModel(QObject *parent) :
    KNMusicProxyModel(parent)
{
}

bool KNMusicAlbumListProxyModel::lessThan(const QModelIndex &source_left,
                                          const QModelIndex &source_right) const
{
    int leftDisc=source_left.data(AlbumDiscRole).toInt(),
            rightDisc=source_right.data(AlbumDiscRole).toInt();
    //First compare the disc index.
    if(leftDisc!=rightDisc)
    {
        //Compare the disc number.
        return leftDisc<rightDisc;
    }
    //Then compare the track index.
    int leftTrack=source_left.data(AlbumTrackRole).toInt(),
            rightTrack=source_right.data(AlbumTrackRole).toInt();
    if(leftTrack!=rightTrack)
    {
        return leftTrack<rightTrack;
    }
    //For most of them, it should be okay here. But for all of those who doesn't
    //have disc and track number, compare as what it is.
    return KNMusicProxyModel::lessThan(source_left, source_right);
}
