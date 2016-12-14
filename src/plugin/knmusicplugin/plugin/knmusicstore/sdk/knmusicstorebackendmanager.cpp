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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "knmusicstorecontainer.h"
#include "knmusicstorebackend.h"
#include "knmusicstoreutil.h"
#include "knmusicstorepage.h"

#include "knmusicstorebackendmanager.h"

using namespace MusicStoreUtil;

KNMusicStoreBackendManager *KNMusicStoreBackendManager::m_instance=nullptr;

KNMusicStoreBackendManager::KNMusicStoreBackendManager(QObject *parent) :
    QObject(parent),
    m_pageContainer(nullptr)
{
}

void KNMusicStoreBackendManager::setPageContainer(
        KNMusicStoreContainer *pageContainer)
{
    //Save the page container.
    m_pageContainer = pageContainer;
}

void KNMusicStoreBackendManager::showAlbum(const QString &backendId,
                                           const QString &albumInfo)
{
    Q_ASSERT(m_pageContainer);
    //Get the backend.
    KNMusicStoreBackend *backend=m_backendMap.value(backendId, nullptr);
    //Check backend pointer.
    if(!backend)
    {
        //Failed to operate the backend.
        return;
    }
    //Set the backend to the page.
    m_pageContainer->page(PageAlbum)->setBackend(backend);
    //Ask the backend to show the information.
    backend->showAlbum(albumInfo);
}

void KNMusicStoreBackendManager::showSingleSong(const QString &backendId,
                                                const QString &songInfo)
{
    Q_ASSERT(m_pageContainer);
    //Get the backend.
    KNMusicStoreBackend *backend=m_backendMap.value(backendId, nullptr);
    //Check backend pointer.
    if(!backend)
    {
        //Failed to operate the backend.
        return;
    }
    //Set the backend to the page.
    m_pageContainer->page(PageSingleSong)->setBackend(backend);
    //Ask the backend to show the information.
    backend->showSingleSong(songInfo);
}

KNMusicStoreBackendManager *KNMusicStoreBackendManager::instance()
{
    return m_instance;
}

void KNMusicStoreBackendManager::initial(QObject *parent)
{
    //Check the instance pointer.
    if(m_instance)
    {
        //Ignore the useless function call.
        return;
    }
    //Create the instance.
    m_instance=new KNMusicStoreBackendManager(parent);
}

void KNMusicStoreBackendManager::addBackend(KNMusicStoreBackend *backend)
{
    //Add backend to the map.
    m_backendMap.insert(backend->objectName(), backend);
    //Link the backend request to the header.
    connect(backend, &KNMusicStoreBackend::requireSetNavigatorItem,
            this, &KNMusicStoreBackendManager::requireSetNavigatorItem);
}
