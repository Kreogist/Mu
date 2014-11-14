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
#include "sdk/knmusiccategorymodel.h"
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

    initialSongTab();
    initialArtistTab();
    m_libraryTabs[TabAlbums]=new KNMusicLibraryAlbumTab(this);
    m_libraryTabs[TabGenres]=new KNMusicLibraryGenreTab(this);

    //Set library model.
    m_librarySongTab->setLibraryModel(m_libraryModel);
    for(int i=0; i<CategoryTabsCount; i++)
    {
        m_libraryTabs[i]->setLibraryModel(m_libraryModel);
    }
}

KNMusicTab *KNMusicLibrary::songTab()
{
    return m_librarySongTab;
}

KNMusicTab *KNMusicLibrary::artistTab()
{
    return m_libraryTabs[TabArtists];
}

KNMusicTab *KNMusicLibrary::albumTab()
{
    return m_libraryTabs[TabAlbums];
}

KNMusicTab *KNMusicLibrary::genreTab()
{
    return m_libraryTabs[TabGenres];
}

void KNMusicLibrary::initialSongTab()
{
    //Genreate these tabs.
    m_librarySongTab=new KNMusicLibrarySongTab(this);
}

void KNMusicLibrary::initialArtistTab()
{
    //Initial the model and proxy model.
    m_categoryModel[TabArtists]=new KNMusicCategoryModel(this);
    m_categoryModel[TabArtists]->setCategoryIndex(Artist);
    //Install the category model to library model.
    m_libraryModel->installCategoryModel(m_categoryModel[TabArtists]);
    //Initial the artist tab.
    m_libraryTabs[TabArtists]=new KNMusicLibraryArtistTab(this);
    m_libraryTabs[TabArtists]->setCategoryModel(m_categoryModel[TabArtists]);

}
