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
#include "sdk/knmusiclibrarymodel.h"
#include "sdk/knmusiclibrarytab.h"
#include "sdk/knmusiclibrarysongtab.h"
#include "sdk/knmusiclibraryartisttab.h"
#include "sdk/knmusiclibraryalbumtab.h"
#include "sdk/knmusiclibrarygenretab.h"

#include "knmusiclibrary.h"

KNMusicLibrary::KNMusicLibrary(QObject *parent) :
    KNMusicLibraryBase(parent)
{
    //Initial the music model.
    m_libraryModel=new KNMusicLibraryModel(this);
    initialTabs();
}

KNMusicTab *KNMusicLibrary::songTab()
{
    return m_libraryTabs[Songs];
}

KNMusicTab *KNMusicLibrary::artistTab()
{
    return m_libraryTabs[Artists];
}

KNMusicTab *KNMusicLibrary::albumTab()
{
    return m_libraryTabs[Albums];
}

KNMusicTab *KNMusicLibrary::genreTab()
{
    return m_libraryTabs[Genres];
}

void KNMusicLibrary::initialTabs()
{
    //Genreate these tabs.
    m_libraryTabs[Songs]=new KNMusicLibrarySongTab(this);
    m_libraryTabs[Artists]=new KNMusicLibraryArtistTab(this);
    m_libraryTabs[Albums]=new KNMusicLibraryAlbumTab(this);
    m_libraryTabs[Genres]=new KNMusicLibraryGenreTab(this);
    //Set model.
    for(int i=0; i<LibraryTabs; i++)
    {
        m_libraryTabs[i]->setLibraryModel(m_libraryModel);
    }
}
