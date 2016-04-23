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
#ifndef KNMUSICSTOREUTIL_H
#define KNMUSICSTOREUTIL_H

#include <QPixmap>
#include <QString>
#include <QVariant>

#define StoreAlbumSize 140

namespace MusicStoreUtil
{
    enum StoreAlbumRole
    {
        AlbumArtistRole = Qt::UserRole + 1,
        AlbumFetchDataRole
    };

    enum StoreSearchCategories
    {
        CategorySong,
        CategoryArtist,
        CategoryAlbum,
        StoreSearchCategoryCount
    };

    enum StoreAlbumDetail
    {
        StoreAlbumTitle,
        StoreAlbumArtist,
        StoreAlbumReleaseTime,
        StoreAlbumReleaseCompany,
        StoreAlbumId,
        StoreAlbumDetailCount
    };

    enum StorePanels
    {
        PanelHome,
        PanelSearch,
        PanelList,
        PanelSong,
        StorePanelCount
    };

    enum StoreStates
    {
        StateNetwork,
        StoreStateCount
    };

    enum SongProperties
    {
        StoreSongName,
        StoreSongAlbumName,
        StoreSongAlbumId,
        StoreSongId,
        StoreSongLyrics,
        StoreSongOnlineUrl,
        StoreSongHighUrl,
        StoreSongLossLessUrl,
        SongPropertiesCount
    };

    struct StoreAlbumListItem
    {
        QPixmap albumArt;
        QString name;
        QString artist;
        QVariant albumData;
    };
}

/*!
 * \brief The KNMusicStoreUtil class provides several public value for music
 * store.
 */
class KNMusicStoreUtil
{
public:
    struct StoreSongItem
    {
        QVariant songData;
        QString name;
        QString duration;
        QString artist;
        QString urlLossless;
        QString urlHigh;
        QString urlOnline;
        int index;
    };

private:
    KNMusicStoreUtil();
    KNMusicStoreUtil(const KNMusicStoreUtil &);
    KNMusicStoreUtil(KNMusicStoreUtil &&);
};

#endif // KNMUSICSTOREUTIL_H
