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

#ifndef KNMUSICPLAYERBASE_H
#define KNMUSICPLAYERBASE_H

#include "kndropproxycontainer.h"

class KNMusicBackend;
class KNMusicNowPlayingBase;
/*!
 * \brief The KNMusicPlayerBase class provides the basic interface and signals
 * of a player widget.
 */
class KNMusicPlayerBase : public KNDropProxyContainer
{
    Q_OBJECT
public:
    KNMusicPlayerBase(QWidget *parent = 0):KNDropProxyContainer(parent){}

    /*!
     * \brief Get the backend of the music plugin. This will control the current
     * music playing, including: play, pause, get information and set position.
     * \param backend The backend pointer.
     */
    virtual void setBackend(KNMusicBackend *backend)=0;

    /*!
     * \brief Get the now playing plugin. This will control the current playing
     * list, including play previous/next, repeat mode, shuffle.
     * \param nowPlaying The now playing plugin.
     */
    virtual void setNowPlaying(KNMusicNowPlayingBase *nowPlaying)=0;

signals:
    /*!
     * \brief Ask to show the current song in 'Songs' tab of the music library
     * tab.
     */
    void requireShowInSongs();

    /*!
     * \brief Ask to show the current song in 'Artists' tab of the music library
     * tab.
     */
    void requireShowInArtists();

    /*!
     * \brief Ask to show the current song in 'Albums' tab of the music library
     * tab.
     */
    void requireShowInAlbums();

    /*!
     * \brief Ask to show the current song in 'Genres' tab of the music library
     * tab.
     */
    void requireShowInGenres();

public slots:
};

#endif // KNMUSICPLAYERBASE_H
