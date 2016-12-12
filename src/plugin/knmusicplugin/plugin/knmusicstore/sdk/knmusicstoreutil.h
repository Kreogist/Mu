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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREUTIL_H
#define KNMUSICSTOREUTIL_H

#include <QString>

namespace MusicStoreUtil
{
    enum MusicStoreError
    {
        MusicStoreErrorCount
    };

    enum MusicStorePages
    {
        PageHome,
        PageSearchResult,
        PageList,
        PageAlbum,
        PageSingleSong,
        PagesCount
    };

    enum MusicStoreAlbumLabel
    {
        AlbumMetadata,
        AlbumArt
    };

    enum MusicStoreSingleLabel
    {
        SingleAlbumArt,
        SingleMetadata,
        SingleArtist,
        SingleLyrics
    };

    struct KNMusicStoreSongInfo
    {
        QString index;
        QString name;
        QString artist;
        QString artistId;
        QString duration;
        QString customData;
        KNMusicStoreSongInfo() :
            index(QString()),
            name(QString()),
            artist(QString()),
            artistId(QString()),
            duration(QString()),
            customData(QString())
        {
        }
    };
}

/*!
 * \brief The KNMusicStoreUtil class provides several data and static functions.
 * All the enumeration and structures will be defines here.
 */
class KNMusicStoreUtil
{
public:
    /*!
     * \brief Get the store header widget height.
     * \return The height of the header. It is a fixed number.
     */
    static int headerHeight()
    {
        return 48;
    }

private:
    KNMusicStoreUtil();
    KNMusicStoreUtil(const KNMusicStoreUtil &);
    KNMusicStoreUtil(KNMusicStoreUtil &&);
};

#endif // KNMUSICSTOREUTIL_H
