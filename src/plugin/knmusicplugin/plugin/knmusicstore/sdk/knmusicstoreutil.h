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

#include <QPixmap>
#include <QString>

namespace MusicStoreUtil
{
    enum MusicStoreErrorType
    {
        ErrorTypeConnection,
        ErrorTypeContent
    };

    enum MusicStoreContentError
    {
        EmptyContent,
        ContentFormatError,
        CannotFindContent,
        MusicStoreContentErrorCount
    };

    enum MusicStoreDownloadRole
    {
        DownBytesRole = Qt::UserRole + 1,
        TotalBytesRole,
        DownMegabytesRole,
        TotalMegabytesRole,
        StateRole
    };

    enum MusicStoreDownloadMissionState
    {
        MissionWaiting,
        MissionRunning,
        MissionPaused
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

    enum HomeSongList
    {
        ListNewAlbum,
        ListNewSongs,
        ListBillboard,
        ListOricon,
        ListItunes,
        ListTopSongs,
        HomeSongListCount
    };

    enum MusicStoreHomeLabel
    {
        HomeNewAlbumData,
        HomeNewAlbumArt,
        HomeNewSongData,
        HomeNewSongArt,
        HomeBillboardList,
        HomeOriconList,
        HomeItunesList,
        HomeTopSongsList
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

    struct KNMusicStoreHomeItem
    {
        QPixmap artwork;
        QString title;
        QString subheading;
        QString customData;
        KNMusicStoreHomeItem() :
            artwork(QPixmap()),
            title(QString()),
            subheading(QString()),
            customData(QString())
        {
        }
    };

    struct KNMusicStoreHomeUpdateArtwork
    {
        QPixmap artwork;
        int index;
        KNMusicStoreHomeUpdateArtwork() :
            artwork(QPixmap()),
            index(-1)
        {
        }
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
Q_DECLARE_METATYPE(MusicStoreUtil::KNMusicStoreHomeUpdateArtwork)

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

    /*!
     * \brief Get the maximum content width size, unit is pixel.
     * \return The content width.
     */
    static int maximumContentWidth()
    {
        return 1108;
    }

private:
    KNMusicStoreUtil();
    KNMusicStoreUtil(const KNMusicStoreUtil &);
    KNMusicStoreUtil(KNMusicStoreUtil &&);
};

#endif // KNMUSICSTOREUTIL_H
