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
#include "sdk/knmusiclibrarysongtab.h"
#include "sdk/knmusiclibraryartisttab.h"
#include "sdk/knmusiclibraryalbumtab.h"
#include "sdk/knmusiclibrarygenretab.h"

#include "knmusiclibrary.h"

KNMusicLibrary::KNMusicLibrary(QObject *parent) :
    KNMusicLibraryBase(parent)
{
    //Initial the tabs.
    m_songTab=new KNMusicLibrarySongTab(this);
    m_artistTab=new KNMusicLibraryArtistTab(this);
    m_albumTab=new KNMusicLibraryAlbumTab(this);
    m_genreTab=new KNMusicLibraryGenreTab(this);
}

KNMusicTab *KNMusicLibrary::songTab()
{
    return m_songTab;
}

KNMusicTab *KNMusicLibrary::artistTab()
{
    return m_artistTab;
}

KNMusicTab *KNMusicLibrary::albumTab()
{
    return m_albumTab;
}

KNMusicTab *KNMusicLibrary::genreTab()
{
    return m_genreTab;
}
