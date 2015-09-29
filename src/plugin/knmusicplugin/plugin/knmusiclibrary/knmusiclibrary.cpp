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
#include "knjsondatabase.h"

//Dependences.
#include "knmusicnowplayingbase.h"
#include "sdk/knmusiclibrarymodel.h"
#include "sdk/knmusiccategorymodel.h"

//Library SDK Ports.
#include "sdk/knmusiclibrarytab.h"

//Library SDK Plugins.
#include "sdk/knmusiclibrarysongtab.h"
#include "sdk/knmusiclibraryartisttab.h"

#include "knmusicutil.h"

#include "knmusiclibrary.h"

using namespace MusicUtil;

KNMusicLibrary::KNMusicLibrary(QObject *parent) :
    KNMusicLibraryBase(parent),
    m_libraryPath(knMusicGlobal->musicLibraryPath()+"/Library"),
    m_database(new KNJsonDatabase),
    m_libraryModel(new KNMusicLibraryModel(&m_parseThread, this)),
    m_songTab(new KNMusicLibrarySongTab),
    m_nowPlaying(nullptr)
{
    //Configure the music database.
    // Move to working thread.
    m_database->moveToThread(&m_databaseThread);
    // Set the database file.
    m_database->link(m_libraryPath+"/Music.db");

    //Configure the library tabs.
    m_libraryModel->setDatabase(m_database);

    //Configure the tabs.
    // Song tab.
    m_songTab->setLibraryModel(m_libraryModel);
    linkLoadRequest(m_songTab);
    // Artist tab.
    m_libraryTabs[TabArtists]=new KNMusicLibraryArtistTab;
    m_categoryModel[TabArtists]=new KNMusicCategoryModel(this);
    m_categoryModel[TabArtists]->setCategoryColumn(Artist);


    //Install the category model to library model.
    m_libraryModel->installCategoryModel(m_categoryModel[TabArtists]);
    //Set the category model.
    m_libraryTabs[TabArtists]->setCategoryModel(m_categoryModel[TabArtists]);
    //Set the library model.
    m_libraryTabs[TabArtists]->setLibraryModel(m_libraryModel);
    //Link the load request.
    linkLoadRequest(m_libraryTabs[TabArtists]);

    //Start up threads.
    m_databaseThread.start();
    m_parseThread.start();
    m_imageThread.start();
}

KNMusicLibrary::~KNMusicLibrary()
{
    //Quit and wait for thread quit.
    m_databaseThread.quit();
    m_parseThread.quit();
    m_imageThread.quit();
    //Wait for quitting.
    m_databaseThread.wait();
    m_parseThread.wait();
    m_imageThread.wait();

    //Delete the database object.
    m_database->deleteLater();
}

KNMusicTab *KNMusicLibrary::songTab()
{
    return m_songTab;
}

KNMusicTab *KNMusicLibrary::artistTab()
{
    return m_libraryTabs[TabArtists];
}

KNMusicTab *KNMusicLibrary::albumTab()
{
    return nullptr;
}

KNMusicTab *KNMusicLibrary::genreTab()
{
    return nullptr;
}

void KNMusicLibrary::showInSongTab()
{
    //Check out now playing pointer.
    if(m_nowPlaying)
    {
        //Show the playing song in the song tab.
        m_songTab->showInTab(m_nowPlaying->playingItem().detailInfo);
    }
}

void KNMusicLibrary::showInArtistTab()
{
    //Check out now playing pointer.
    if(m_nowPlaying)
    {
        //Show the playing song in the artist tab.
        m_libraryTabs[TabArtists]->showInTab(
                    m_nowPlaying->playingItem().detailInfo);
    }
}

void KNMusicLibrary::showInAlbumTab()
{
    ;
}

void KNMusicLibrary::showInGenreTab()
{
    ;
}

void KNMusicLibrary::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //Save the now playing pointer.
    m_nowPlaying=nowPlaying;
}

void KNMusicLibrary::onActionLoadLibrary()
{
    //Disconnect all links.
    m_loadHandler.disconnectAll();
    //Recover the library model.
    m_libraryModel->recoverModel();
    //Emit the load signal.
    ;
}

void KNMusicLibrary::linkLoadRequest(KNMusicLibraryTab *libraryTab)
{
    //Link the library tab, add to load request handler.
    m_loadHandler.append(
                connect(libraryTab, &KNMusicLibraryTab::requireLoadLibrary,
                        this, &KNMusicLibrary::onActionLoadLibrary));
}
