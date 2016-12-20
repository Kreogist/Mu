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
#include <QThread>

#include "knmusicstorecontainer.h"
#include "knmusicstorebackend.h"
#include "knmusicstoreutil.h"
#include "knmusicstorepage.h"

//Music store backends
#include "../plugin/knmusicstoreneteasebackend/knmusicstoreneteasebackend.h"

#include "knmusicstorebackendmanager.h"

#include <QDebug>

using namespace MusicStoreUtil;

KNMusicStoreBackendManager *KNMusicStoreBackendManager::m_instance=nullptr;

KNMusicStoreBackendManager::KNMusicStoreBackendManager(QThread *workingThread) :
    QObject(nullptr),
    m_pageContainer(nullptr)
{
    //Move to the thread.
    moveToThread(workingThread);
}

void KNMusicStoreBackendManager::setPageContainer(
        KNMusicStoreContainer *pageContainer)
{
    //Save the page container.
    m_pageContainer = pageContainer;
}

void KNMusicStoreBackendManager::loadPlugins()
{
    //Add plugins.
    addBackend(new KNMusicStoreNeteaseBackend(this));
}

void KNMusicStoreBackendManager::showHomePage()
{
    Q_ASSERT(m_pageContainer);
    //Get the backend. The home page data is provided by Netease. This could be
    //changed later in the settings.
    //! FIXME: Home backend could be changed in the settings.
    KNMusicStoreBackend *backend=m_backendMap.value("MusicStoreNeteaseBackend",
                                                    nullptr);
    //Check backend pointer.
    if(!backend)
    {
        //Failed to operate the backend.
        return;
    }
    //Set the backend to the page.
    m_pageContainer->page(PageHome)->setBackend(backend);
    //Ask the backend to fetch those information.
    backend->showHome();
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

void KNMusicStoreBackendManager::initial(QThread *workingThread)
{
    //Check the instance pointer.
    if(m_instance)
    {
        //Ignore the useless function call.
        return;
    }
    //Create the instance.
    m_instance=new KNMusicStoreBackendManager(workingThread);
}

void KNMusicStoreBackendManager::addBackend(KNMusicStoreBackend *backend)
{
    //Add backend to the map.
    m_backendMap.insert(backend->objectName(), backend);
    //Link the backend request to the manager.
    connect(backend, &KNMusicStoreBackend::requireSetNavigatorItem,
            this, &KNMusicStoreBackendManager::requireSetNavigatorItem);
    connect(backend, &KNMusicStoreBackend::requireAddConnectionCount,
            this, &KNMusicStoreBackendManager::requireAddConnectionCount);
    connect(backend, &KNMusicStoreBackend::requireReduceConnectionCount,
            this, &KNMusicStoreBackendManager::requireReduceConnectionCount);
}
