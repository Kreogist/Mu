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

#ifndef KNMUSICLIBRARYBASE_H
#define KNMUSICLIBRARYBASE_H

#include <QObject>

class KNConfigure;
class KNMusicTab;
class KNMusicNowPlayingBase;
/*!
 * \brief The KNMusicLibraryBase class provides all the basic functions which a
 * music library plugin should realized.\n
 * First, it should provides four tabs for the music library. Song, artist,
 * album and genre tabs. These four tabs will be added to the music plugin
 * container.
 */
class KNMusicLibraryBase : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryBase object.
     * \param parent The parent object.
     */
    KNMusicLibraryBase(QObject *parent = 0) : QObject(parent){}

    /*!
     * \brief Get the song tab widget pointer.
     * \return Song tab pointer.
     */
    virtual KNMusicTab *songTab()=0;

    /*!
     * \brief Get the artist tab widget pointer.
     * \return Artist tab pointer.
     */
    virtual KNMusicTab *artistTab()=0;

    /*!
     * \brief Get the album tab widget pointer.
     * \return Album tab pointer.
     */
    virtual KNMusicTab *albumTab()=0;

    /*!
     * \brief Get the genre tab widget pointer.
     * \return Genre tab pointer.
     */
    virtual KNMusicTab *genreTab()=0;

    /*!
     * \brief Get whether the music library is working to adding files to music
     * library data.
     * \return If the music plugin is working, return true.
     */
    virtual bool isWorking()=0;

signals:
    /*!
     * \brief Ask music plugin to show the playlist float list widget.
     */
    void requireShowPlaylistList();

    /*!
     * \brief Ask music plugin to hide the playlist float list widget.
     */
    void requireHidePlaylistList();

public slots:
    /*!
     * \brief Show the now playing song in the song tab.
     */
    virtual void showInSongTab()=0;

    /*!
     * \brief Show the now playing song in the artist tab.
     */
    virtual void showInArtistTab()=0;

    /*!
     * \brief Show the now playing song in the album tab.
     */
    virtual void showInAlbumTab()=0;

    /*!
     * \brief Show the now playing song in the genre tab.
     */
    virtual void showInGenreTab()=0;

    /*!
     * \brief Set the now playing class to library plugin.
     * \param nowPlaying Now playing class pointer.
     */
    virtual void setNowPlaying(KNMusicNowPlayingBase *nowPlaying)=0;

    /*!
     * \brief Load the library data from the disk.
     */
    virtual void loadLibrary()=0;

private:
};

#endif // KNMUSICLIBRARYBASE_H
