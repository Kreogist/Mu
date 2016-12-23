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
#include <QModelIndex>
#include <QTimeLine>

#include "knlocalemanager.h"
#include "kncategorytab.h"
#include "knthememanager.h"

#include "sdk/knmusicstoreglobal.h"
#include "sdk/knmusicstorecontainer.h"
#include "sdk/knmusicstoreerrordimmer.h"
#include "sdk/knmusicstoreloadingdimmer.h"
#include "sdk/knmusicstorebackendmanager.h"

//Music store plugins.
#include "plugin/knmusicstoreneteasebackend/knmusicstoreneteasebackend.h"

#include "knmusicstore.h"

#include <QDebug>

KNMusicStore::KNMusicStore(QWidget *parent) :
    KNMusicStoreBase(parent),
    m_showContainer(new QTimeLine(400, this)),
    m_tab(new KNCategoryTab(this)),
    m_container(nullptr),
    m_errorDimmer(new KNMusicStoreErrorDimmer(this)),
    m_loadingDimmer(new KNMusicStoreLoadingDimmer(this)),
    m_initialLoad(true)
{
    setObjectName("MusicStore");
    //Initial the global object.
    KNMusicStoreGlobal::initial(this);
    //Configure the tab.
    m_tab->setIcon(QIcon(":/plugin/music/category/store.png"));
    //Initial and configure the container.
    //The container must be initialized after initial the global object.
    m_container=new KNMusicStoreContainer(this);
    //Configure container.
    m_container->hide();
    //Raise the container to top.
    m_container->raise();
    //For the first time page changed, we need to show the page container.
    connect(m_container, &KNMusicStoreContainer::currentPageChanged,
            this, &KNMusicStore::showPageContainer);
    //Give the page container to the backend manager.
    knMusicStoreBackendManager->setPageContainer(m_container);
    //Link the backend set value signals.
    connect(knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::requireSetNavigatorItem,
            m_container, &KNMusicStoreContainer::setNavigatorText,
            Qt::QueuedConnection);
    //Link the container fetch data signals.
    connect(this, &KNMusicStore::requireShowHome,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::showHomePage,
            Qt::QueuedConnection);
    connect(this, &KNMusicStore::requireLoadPlugin,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::loadPlugins,
            Qt::QueuedConnection);
    connect(m_container, &KNMusicStoreContainer::requireShowAlbum,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::showAlbum,
            Qt::QueuedConnection);
    connect(m_container, &KNMusicStoreContainer::requireShowSingleSong,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::showSingleSong,
            Qt::QueuedConnection);
    //Configure the error dimmer.
    m_errorDimmer->setObjectName("MusicStoreErrorDimmer");
    m_errorDimmer->raise();
    m_errorDimmer->hide();
    //Configure the loading dimmer.
    ;
    //Configure the time line.
    m_showContainer->setEasingCurve(QEasingCurve::OutCubic);
    m_showContainer->setUpdateInterval(33);
    m_showContainer->setStartFrame(0);
    connect(m_showContainer, &QTimeLine::frameChanged,
            this, &KNMusicStore::onAnimeShowContainer);

    //Add to theme manager.
    knTheme->registerWidget(this);

    //Link retranslate.
    knI18n->link(this, &KNMusicStore::retranslate);
    retranslate();
}

QAbstractButton *KNMusicStore::tab()
{
    return m_tab;
}

void KNMusicStore::showIndex(KNMusicModel *musicModel, const QModelIndex &index)
{
    Q_UNUSED(musicModel)
    Q_UNUSED(index)
}

void KNMusicStore::loadPlugins()
{
    //Add music store backend to the backend manager.
    emit requireLoadPlugin();
}

void KNMusicStore::resizeEvent(QResizeEvent *event)
{
    //Resize the base widget.
    KNMusicStoreBase::resizeEvent(event);
    //Resize the content widgets.
    //Check the container is shown or not.
    // Widgets container.
    m_container->resize(size());
    // Error dimmer.
    m_errorDimmer->resize(size());
    // Loading dimmer.
    m_loadingDimmer->resize(size());
    //Update the time line parameter.
    m_showContainer->setEndFrame(height());
}

void KNMusicStore::showEvent(QShowEvent *event)
{
    //Show the page.
    KNMusicStoreBase::showEvent(event);
    //Check initial load flag.
    if(m_initialLoad)
    {
        //Reset the flag to false.
        m_initialLoad=false;
        //Emit the feteching signal.
        emit requireShowHome();
    }
}

void KNMusicStore::retranslate()
{
    //Update the tab title.
    m_tab->setText(tr("Store"));
}

void KNMusicStore::showPageContainer()
{
    //Disconnet the signal.
    disconnect(m_container, &KNMusicStoreContainer::currentPageChanged,
               this, &KNMusicStore::showPageContainer);
    //Reset container position.
    m_container->move(0, -height());
    //Make the container visible.
    m_container->show();
    //Start the anime.
    m_showContainer->start();
}

void KNMusicStore::onAnimeShowContainer(int frame)
{
    //Move the container.
    m_container->move(0, frame-height());
    //Set the value to loading dimmer.
    m_loadingDimmer->setDarkness(m_showContainer->currentValue());
}
