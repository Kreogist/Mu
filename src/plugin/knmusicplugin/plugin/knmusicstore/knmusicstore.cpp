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
#include <QBoxLayout>
#include <QModelIndex>
#include <QTimeLine>

#include "kncategorytab.h"
#include "knlocalemanager.h"
#include "knsideshadowwidget.h"
#include "knthememanager.h"

#include "sdk/knmusicstoredownloadlist.h"
#include "sdk/knmusicstoreglobal.h"
#include "sdk/knmusicstorecontainer.h"
#include "sdk/knmusicstoreheader.h"
#include "sdk/knmusicstoreerrordimmer.h"
#include "sdk/knmusicstoreloadingdimmer.h"
#include "sdk/knmusicstorebackendmanager.h"

//Music store plugins.
#include "plugin/knmusicstoreneteasebackend/knmusicstoreneteasebackend.h"

#include "knmusicstore.h"

#define ShadowHeight    15

#include <QDebug>

KNMusicStore::KNMusicStore(QWidget *parent) :
    KNMusicStoreBase(parent),
    m_showContainer(new QTimeLine(400, this)),
    m_tab(new KNCategoryTab(this)),
    m_container(nullptr),
    m_headerContainer(new QWidget(this)),
    m_header(new KNMusicStoreHeader(this)),
    m_downloadList(new KNMusicStoreDownloadList(this)),
    m_errorDimmer(new KNMusicStoreErrorDimmer(this)),
    m_loadingDimmer(new KNMusicStoreLoadingDimmer(this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadowWidget::TopShadow, this)),
    m_initialLoad(true)
{
    setObjectName("MusicStore");
    //Initial the global object.
    KNMusicStoreGlobal::initial(this);
    //Configure the tab.
    m_tab->setIcon(QIcon(":/plugin/music/category/store.png"));
    //Configure container.
    m_container=new KNMusicStoreContainer(this);
    m_container->hide();
    m_container->raise(); //Raise the container to top.
    connect(m_container, &KNMusicStoreContainer::requireSetNavigatorItem,
            m_header, &KNMusicStoreHeader::setNavigatorText);
    connect(m_container, &KNMusicStoreContainer::requireDownload,
            m_downloadList, &KNMusicStoreDownloadList::downloadSong);
    //Configure the header container.
    m_headerContainer->setAutoFillBackground(true);
    m_headerContainer->setObjectName("MusicStoreHeaderContainer");
    m_headerContainer->hide();
    m_headerContainer->raise();
    knTheme->registerWidget(m_headerContainer);
    //Configure the side shadow.
    m_topShadow->hide();
    m_topShadow->raise();
    //Initial the header container layout.
    QBoxLayout *headerLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(0);
    m_headerContainer->setLayout(headerLayout);
    //Add widget to layout.
    headerLayout->addStretch();
    headerLayout->addWidget(m_header);
    headerLayout->addStretch();
    //Configure the header.
    m_header->setChangeOpacity(true);
    m_header->setSenseRange(0x00, 0x15);
    m_header->updateObjectName("MusicStoreHeader");
    connect(m_header, &KNMusicStoreHeader::requireShowPage,
            m_container, &KNMusicStoreContainer::showPageIndex);
    //Add widget to header.
    m_header->addStateWidget(m_downloadList->stateButton());
    m_header->addStateWidget(knMusicStoreGlobal->connectStateWheel());
    //For the first time page changed, we need to show the page container.
    connect(m_container, &KNMusicStoreContainer::currentPageChanged,
            this, &KNMusicStore::showPageContainer);
    //Give the page container to the backend manager.
    knMusicStoreBackendManager->setPageContainer(m_container);
    //Link the container fetch data signals.
    connect(this, &KNMusicStore::requireShowHome,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::showHomePage,
            Qt::QueuedConnection);
    connect(this, &KNMusicStore::requireLoadPlugin,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::loadPlugins,
            Qt::QueuedConnection);
    connect(m_container, &KNMusicStoreContainer::requireShowHome,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::showHomePage,
            Qt::QueuedConnection);
    connect(m_container, &KNMusicStoreContainer::requireShowAlbum,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::showAlbum,
            Qt::QueuedConnection);
    connect(m_container, &KNMusicStoreContainer::requireShowSingleSong,
            knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::showSingleSong,
            Qt::QueuedConnection);
    //Configrue the download list.
    m_downloadList->raise();
    m_downloadList->hide();
    //Configure the error dimmer.
    m_errorDimmer->setObjectName("MusicStoreErrorDimmer");
    m_errorDimmer->raise();
    m_errorDimmer->hide();
    connect(knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::requireShowError,
            m_errorDimmer, &KNMusicStoreErrorDimmer::showDimmer,
            Qt::QueuedConnection);
    connect(m_errorDimmer, &KNMusicStoreErrorDimmer::requireRefresh,
            m_container, &KNMusicStoreContainer::refresh);
    //Configure the loading dimmer.
    ;
    //Configure the time line.
    m_showContainer->setEasingCurve(QEasingCurve::OutCubic);
    m_showContainer->setUpdateInterval(16);
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
    //Calculate the content size.
    //Get current width.
    int contentWidth=width();
    contentWidth=(contentWidth>KNMusicStoreUtil::maximumContentWidth())?
                        KNMusicStoreUtil::maximumContentWidth():
                        contentWidth;
    //Set the content width to widgets.
    m_header->setFixedWidth(contentWidth);
    //Update the header container width.
    m_headerContainer->resize(width(), KNMusicStoreUtil::headerHeight());
    //Update the shadow size.
    m_topShadow->resize(width(), ShadowHeight);
    //Resize the content widgets.
    // Widgets container.
    m_container->resize(size());
    // Error dimmer.
    m_errorDimmer->resize(size());
    // Loading dimmer.
    m_loadingDimmer->resize(size());
    // Download list.
    m_downloadList->resize(size());
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
    m_headerContainer->move(0, -KNMusicStoreUtil::headerHeight());
    m_topShadow->move(0, -ShadowHeight);
    //Make the container visible.
    m_container->show();
    m_headerContainer->show();
    m_topShadow->show();
    //Start the anime.
    m_showContainer->start();
    //Enabled the okay button.
    m_errorDimmer->setOkayEnabled(true);
}

void KNMusicStore::onAnimeShowContainer(int frame)
{
    //Move the container.
    m_container->move(0, frame-height());
    //Move the header container.
    int headerContainerTop=m_showContainer->currentValue()*
                            (qreal)(m_headerContainer->height())-
                            m_headerContainer->height();
    m_headerContainer->move(0, headerContainerTop);
    //Move the top shadow.
    m_topShadow->move(0, m_showContainer->currentValue()*
                            (qreal)ShadowHeight-
                            ShadowHeight
                         +headerContainerTop+KNMusicStoreUtil::headerHeight());
    //Set the value to loading dimmer.
    m_loadingDimmer->setDarkness(m_showContainer->currentValue());
}
