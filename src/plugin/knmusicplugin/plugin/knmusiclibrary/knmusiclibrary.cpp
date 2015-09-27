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
#include "sdk/knmusiclibrarymodel.h"

//Library SDK Ports.
#include "sdk/knmusiclibrarytab.h"

//Library SDK Plugins.
#include "sdk/knmusiclibrarysongtab.h"

#include "knmusicglobal.h"

#include "knmusiclibrary.h"

KNMusicLibrary::KNMusicLibrary(QObject *parent) :
    KNMusicLibraryBase(parent),
    m_libraryPath(knMusicGlobal->musicLibraryPath()+"/Library"),
    m_database(new KNJsonDatabase),
    m_libraryModel(new KNMusicLibraryModel(this)),
    m_songTab(new KNMusicLibrarySongTab)
{
    //Configure the music database.
    // Move to working thread.
    m_database->moveToThread(&m_databaseThread);
    m_database->link(m_libraryPath+"/Music.db");
    // Initial the music image manager.

    //Configure the library tabs.
    m_libraryModel->setDatabase(m_database);

    //Start up threads.
    m_databaseThread.start();
    m_imageThread.start();
}

KNMusicLibrary::~KNMusicLibrary()
{
    //Quit and wait for thread quit.
    m_databaseThread.quit();
    m_imageThread.quit();
    //Wait for quitting.
    m_databaseThread.wait();
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
    return nullptr;
}

KNMusicTab *KNMusicLibrary::albumTab()
{
    return nullptr;
}

KNMusicTab *KNMusicLibrary::genreTab()
{
    return nullptr;
}


