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
#include <QAction>

#include "sdk/knmusiclibrarymodel.h"
#include "sdk/knmusiccategorymodel.h"
#include "sdk/knmusicgenremodel.h"
#include "sdk/knmusiclibrarytab.h"
#include "sdk/knmusiclibrarysongtab.h"
#include "sdk/knmusiclibraryartisttab.h"
#include "sdk/knmusiclibraryalbumtab.h"
#include "sdk/knmusiclibrarygenretab.h"
#include "knmusicsolomenubase.h"

#include "knmusiclibrary.h"

KNMusicLibrary::KNMusicLibrary(QObject *parent) :
    KNMusicLibraryBase(parent)
{
    //Initial the music model.
    m_libraryModel=new KNMusicLibraryModel(this);

    QList<QAction *> showInActionList;
    //Initial the song tab.
    initialSongTab();
    //Set the library model for song tab.
    m_librarySongTab->setLibraryModel(m_libraryModel);
    //Get the go to action.
    showInActionList.append(m_librarySongTab->showInAction());

    //Initial the category models.
    for(int i=0; i<CategoryTabsCount; i++)
    {
        //Initial the model and proxy model.
        m_categoryModel[i]=new KNMusicCategoryModel(this);
        //Install the category model to library model.
        m_libraryModel->installCategoryModel(m_categoryModel[i]);
    }

    //Initial the category tabs.
    initialArtistTab();
    initialAlbumTab();
    initialGenreTab();

    //Set library model, get the go to action.
    for(int i=0; i<CategoryTabsCount; i++)
    {
        //Set the category model
        m_libraryTabs[i]->setCategoryModel(m_categoryModel[i]);
        //Set the library model.
        m_libraryTabs[i]->setLibraryModel(m_libraryModel);
        //Get the action list.
        showInActionList.append(m_libraryTabs[i]->showInAction());
    }
    //Add the show in actions to solo menu.
    KNMusicGlobal::soloMenu()->addMusicActions(showInActionList);
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
    //Initial the artist tab.
    m_libraryTabs[TabArtists]=new KNMusicLibraryArtistTab(this);
    //Initial the model and proxy model.
    m_categoryModel[TabArtists]->setCategoryIndex(Artist);
}

void KNMusicLibrary::initialAlbumTab()
{
    //Initial the album tab.
    m_libraryTabs[TabAlbums]=new KNMusicLibraryAlbumTab(this);
    //Initial the model and proxy model.
    m_categoryModel[TabAlbums]->setCategoryIndex(Album);
}

void KNMusicLibrary::initialGenreTab()
{
    //Initial the genre tab.
    m_libraryTabs[TabGenres]=new KNMusicLibraryGenreTab(this);
    //Initial the model and proxy model.
    m_categoryModel[TabGenres]->setCategoryIndex(Genre);
}
