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

class KNMusicTab;
class KNMusicNowPlayingBase;
class KNMusicLibraryBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLibraryBase(QObject *parent = 0);

    virtual KNMusicTab *songTab()=0;
    virtual KNMusicTab *artistTab()=0;
    virtual KNMusicTab *albumTab()=0;
    virtual KNMusicTab *genreTab()=0;

signals:

public slots:
    virtual void showInSongTab()=0;
    virtual void showInArtistTab()=0;
    virtual void showInAlbumTab()=0;
    virtual void showInGenreTab()=0;
    virtual void setNowPlaying(KNMusicNowPlayingBase *nowPlaying)=0;

private:
};

#endif // KNMUSICLIBRARYBASE_H
