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
#include <QThread>

#include "knjsondatabase.h"

#include "sdk/knmusiclibrarymodel.h"
#include "sdk/knmusiccategorymodel.h"
#include "sdk/knmusicalbummodel.h"
#include "sdk/knmusicgenremodel.h"
#include "sdk/knmusiclibrarytab.h"
#include "sdk/knmusiclibrarysongtab.h"
#include "sdk/knmusiclibraryartisttab.h"
#include "sdk/knmusiclibraryalbumtab.h"
#include "sdk/knmusiclibrarygenretab.h"
#include "sdk/knmusiclibraryimagemanager.h"

#include "knmusicheaderplayerbase.h"
#include "knmusicmainplayerbase.h"
#include "knmusicsolomenubase.h"

#include "knmusiclibrary.h"

KNMusicLibrary::KNMusicLibrary(QObject *parent) :
    KNMusicLibraryBase(parent)
{
    //Initial the music threads.
    m_libraryDatabaseThread=new QThread(this);
    m_libraryImageThread=new QThread(this);
    //Initial the music library folder path.
    m_libraryPath=KNMusicGlobal::musicLibraryPath()+"/Library";
    //Initial the music database.
    m_libraryDatabase=new KNJSONDatabase;
    m_libraryDatabase->moveToThread(m_libraryDatabaseThread);
    m_libraryDatabase->setDatabaseFile(m_libraryPath+"/Music.db");
    //Initial the music image manager.
    m_libraryImageManager=new KNMusicLibraryImageManager;
    m_libraryImageManager->moveToThread(m_libraryImageThread);
    m_libraryImageManager->setImageFolderPath(m_libraryPath+"/Artworks");
    connect(this, &KNMusicLibrary::requireLoadImageLibrary,
            m_libraryImageManager, &KNMusicLibraryImageManager::recoverFromFolder);

    //Initial the music model.
    m_libraryModel=new KNMusicLibraryModel(this);
    m_libraryModel->setDatabase(m_libraryDatabase);
    m_libraryModel->setImageManager(m_libraryImageManager);

    QList<QAction *> showInActionList;
    //Initial the song tab.
    initialSongTab();
    //Set the library model for song tab.
    m_librarySongTab->setLibraryModel(m_libraryModel);
    //Link the load request.
    connect(m_librarySongTab, &KNMusicLibraryTab::requireLoadLibrary,
            this, &KNMusicLibrary::onActionLoadLibrary);
    //Get the go to action.
    showInActionList.append(m_librarySongTab->showInAction());

    //Initial the category tabs.
    initialArtistTab();
    initialAlbumTab();
    initialGenreTab();

    //Set library model, get the go to action.
    for(int i=0; i<CategoryTabsCount; i++)
    {
        //Install the category model to library model.
        m_libraryModel->installCategoryModel(m_categoryModel[i]);
        //Set the category model
        m_libraryTabs[i]->setCategoryModel(m_categoryModel[i]);
        //Set the library model.
        m_libraryTabs[i]->setLibraryModel(m_libraryModel);
        //Link the load request.
        connect(m_libraryTabs[i], &KNMusicLibraryCategoryTab::requireLoadLibrary,
                this, &KNMusicLibrary::onActionLoadLibrary);
        //Get the action list.
        showInActionList.append(m_libraryTabs[i]->showInAction());
    }
    //Add the show in actions to solo menu.
    KNMusicGlobal::soloMenu()->addMusicActions(showInActionList);

    //Start threads.
    m_libraryDatabaseThread->start();
    m_libraryImageThread->start();
}

KNMusicLibrary::~KNMusicLibrary()
{
    //Quit the threads.
    m_libraryDatabaseThread->quit();
    m_libraryImageThread->quit();
    m_libraryDatabaseThread->wait();
    m_libraryImageThread->wait();

    //Save the database.
    m_libraryDatabase->write();

    delete m_libraryDatabase;
    delete m_libraryImageManager;
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

void KNMusicLibrary::setHeaderPlayer(KNMusicHeaderPlayerBase *player)
{
    m_headerPlayer=player;
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireShowInSongs,
            this, &KNMusicLibrary::onActionShowInSongs);
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireShowInArtists,
            this, &KNMusicLibrary::onActionShowInArtists);
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireShowInAlbums,
            this, &KNMusicLibrary::onActionShowInAlbums);
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::requireShowInGenres,
            this, &KNMusicLibrary::onActionShowInGenres);
}

void KNMusicLibrary::setMainPlayer(KNMusicMainPlayerBase *player)
{
    connect(player, &KNMusicMainPlayerBase::requireShowInSongs,
            this, &KNMusicLibrary::onActionShowInSongs);
    connect(player, &KNMusicMainPlayerBase::requireShowInArtists,
            this, &KNMusicLibrary::onActionShowInArtists);
    connect(player, &KNMusicMainPlayerBase::requireShowInAlbums,
            this, &KNMusicLibrary::onActionShowInAlbums);
    connect(player, &KNMusicMainPlayerBase::requireShowInGenres,
            this, &KNMusicLibrary::onActionShowInGenres);
}

void KNMusicLibrary::onActionLoadLibrary()
{
    //Disconnect all the links of the music tab.
    disconnect(m_librarySongTab, &KNMusicLibraryTab::requireLoadLibrary,
               this, &KNMusicLibrary::onActionLoadLibrary);
    for(int i=0; i<CategoryTabsCount; i++)
    {
        //Link the load request.
        disconnect(m_libraryTabs[i], &KNMusicLibraryCategoryTab::requireLoadLibrary,
                   this, &KNMusicLibrary::onActionLoadLibrary);
    }
    //Read the database.
    //**!WARNING!**
    //Don't change the order of the following code.
    m_libraryModel->recoverModel();
    emit requireLoadImageLibrary();
}

void KNMusicLibrary::onActionShowInSongs()
{
    m_librarySongTab->showInTab(m_headerPlayer->currentAnalysisItem().detailInfo);
}

void KNMusicLibrary::onActionShowInArtists()
{
    m_libraryTabs[TabArtists]->showInTab(m_headerPlayer->currentAnalysisItem().detailInfo);
}

void KNMusicLibrary::onActionShowInAlbums()
{
    m_libraryTabs[TabAlbums]->showInTab(m_headerPlayer->currentAnalysisItem().detailInfo);
}

void KNMusicLibrary::onActionShowInGenres()
{
    m_libraryTabs[TabGenres]->showInTab(m_headerPlayer->currentAnalysisItem().detailInfo);
}

inline void KNMusicLibrary::initialSongTab()
{
    //Genreate these tabs.
    m_librarySongTab=new KNMusicLibrarySongTab(this);
}

inline void KNMusicLibrary::initialArtistTab()
{
    //Initial the artist tab.
    m_libraryTabs[TabArtists]=new KNMusicLibraryArtistTab(this);
    //Initial the model and proxy model.
    m_categoryModel[TabArtists]=new KNMusicCategoryModel(this);
    m_categoryModel[TabArtists]->setCategoryIndex(Artist);
}

inline void KNMusicLibrary::initialAlbumTab()
{
    //Initial the album tab.
    m_libraryTabs[TabAlbums]=new KNMusicLibraryAlbumTab(this);
    //Initial the model and proxy model.
    m_categoryModel[TabAlbums]=new KNMusicAlbumModel(this);
    m_categoryModel[TabAlbums]->setCategoryIndex(Album);
}

inline void KNMusicLibrary::initialGenreTab()
{
    //Initial the genre tab.
    m_libraryTabs[TabGenres]=new KNMusicLibraryGenreTab(this);
    //Initial the model and proxy model.
    m_categoryModel[TabGenres]=new KNMusicGenreModel(this);
    m_categoryModel[TabGenres]->setCategoryIndex(Genre);
}
