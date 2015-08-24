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
class KNMusicPlayerBase : public KNDropProxyContainer
{
    Q_OBJECT
public:
    KNMusicPlayerBase(QWidget *parent = 0):KNDropProxyContainer(parent){}

    virtual void setBackend(KNMusicBackend *backend)=0;

    virtual void setNowPlaying(KNMusicNowPlayingBase *nowPlaying)=0;

signals:
    void requirePlayPrevious();
    void requirePlayNext();

    void requireChangeLoopState();

    void requireShowInSongs();
    void requireShowInArtists();
    void requireShowInAlbums();
    void requireShowInGenres();

public slots:
};

#endif // KNMUSICPLAYERBASE_H
