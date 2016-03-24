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
#include "knmusicstoresongdetailinfo.h"

KNMusicStoreSongDetailInfo::KNMusicStoreSongDetailInfo(QObject *parent) :
    QObject(parent)
{
    ;
}

QString KNMusicStoreSongDetailInfo::songData(int propertyId)
{
    return m_songProperites[propertyId];
}

void KNMusicStoreSongDetailInfo::setSongData(int propertyId,
                                             const QString &data)
{
    //Save the song property data.
    m_songProperites[propertyId]=data;
}

QPixmap KNMusicStoreSongDetailInfo::albumArt() const
{
    return m_albumArt;
}

void KNMusicStoreSongDetailInfo::setAlbumArt(const QPixmap &albumArt)
{
    m_albumArt = albumArt;
}

void KNMusicStoreSongDetailInfo::reset()
{
    //Clear the text data.
    for(int i=0; i<SongPropertiesCount; ++i)
    {
        //Clear the song properties.
        m_songProperites[i]=QString();
    }
    //Clear the album art data.
    m_albumArt=QPixmap();
}

QStringList KNMusicStoreSongDetailInfo::artistsId() const
{
    return m_artistsId;
}

void KNMusicStoreSongDetailInfo::setArtistsId(const QStringList &artistsId)
{
    m_artistsId = artistsId;
}

QStringList KNMusicStoreSongDetailInfo::artists() const
{
    return m_artists;
}

void KNMusicStoreSongDetailInfo::setArtists(const QStringList &artists)
{
    m_artists = artists;
}
