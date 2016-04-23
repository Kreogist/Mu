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

#include "knmusicstorealbummodel.h"

KNMusicStoreAlbumModel::KNMusicStoreAlbumModel(QObject *parent) :
    KNMusicStoreSongListModel(parent),
    m_albumArt(QPixmap())
{
}

void KNMusicStoreAlbumModel::clear()
{
    //Clear the album detail resource.
    for(int i=0; i<StoreAlbumDetailCount; ++i)
    {
        //Reset the description information.
        m_albumDetail[i]=QString();
    }
    //Reset the pixmap.
    m_albumArt=QPixmap();
    //Clear the model data.
    KNMusicStoreSongListModel::clear();
}

QString KNMusicStoreAlbumModel::albumInfo(StoreAlbumDetail albumInfoField) const
{
    //Give back the detail data.
    return m_albumDetail[albumInfoField];
}

void KNMusicStoreAlbumModel::setAlbumInfo(StoreAlbumDetail albumInfoField,
                                          const QString &albumInfoData)
{
    //Save the information.
    m_albumDetail[albumInfoField]=albumInfoData;
}

QPixmap KNMusicStoreAlbumModel::albumArt() const
{
    return m_albumArt;
}

void KNMusicStoreAlbumModel::setAlbumArt(const QPixmap &albumArt)
{
    m_albumArt = albumArt;
}
