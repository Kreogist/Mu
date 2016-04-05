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
#include <QScrollArea>
#include <QScrollBar>

//Include infrastructure.
#include "knlocalemanager.h"
#include "kncategorytab.h"
#include "knhwidgetswitcher.h"
#include "sao/knsaostyle.h"

//Include store sdk.
#include "sdk/knmusicstoreglobal.h"
#include "sdk/knmusicstorehomewidget.h"
#include "sdk/knmusicstorelistwidget.h"
#include "sdk/knmusicstoresearchresultwidget.h"
#include "sdk/knmusicstoresinglesongwidget.h"
#include "sdk/knmusicstoreemptywidget.h"
#include "sdk/knmusicstorebackend.h"

//Include plugins.
#include "plugin/knmusicstorenetease/knmusicstorenetease.h"

#include "knmusicstore.h"

#include <QDebug>

KNMusicStore::KNMusicStore(QWidget *parent) :
    KNMusicStoreBase(parent),
    m_tab(new KNCategoryTab(this)),
    m_storeGlobal(KNMusicStoreGlobal::initial(this)),
    m_storeSwitcher(new QScrollArea(this)),
    m_home(new KNMusicStoreHomeWidget(this)),
    m_searchResult(new KNMusicStoreSearchResultWidget(this)),
    m_list(new KNMusicStoreListWidget(this)),
    m_singleSong(new KNMusicStoreSingleSongWidget(this)),
    m_emptyWidget(new KNMusicStoreEmptyWidget(this)),
    m_backend(nullptr)
{
    //Configure the tab.
    m_tab->setIcon(QIcon(":/plugin/music/category/store.png"));
    //Configure the scroll area.
    m_storeSwitcher->setAlignment(Qt::AlignHCenter);
    m_storeSwitcher->setSizeAdjustPolicy(QScrollArea::AdjustToContents);
    KNSaoStyle::styleHorizontalScrollBar(
                m_storeSwitcher->horizontalScrollBar());
    KNSaoStyle::styleVerticalScrollBar(m_storeSwitcher->verticalScrollBar());
    //Hide the content widgets.
    m_home->hide();
    m_searchResult->hide();
    m_list->hide();
    m_singleSong->hide();
    m_storeSwitcher->setWidget(m_emptyWidget);
    //Show the home widget.
//    m_home->show();
//    m_storeSwitcher->setWidget(m_home);
//    m_list->show();
//    m_storeSwitcher->setWidget(m_list);
//    m_singleSong->show();
//    m_storeSwitcher->setWidget(m_singleSong);
//    m_searchResult->show();
//    m_storeSwitcher->setWidget(m_searchResult);

    //Initial the backend.
    loadBackend(new KNMusicStoreNetease);
//    test->fetchAlbumDetail("34515142");
//    test->fetchSongDetail("404543841");
//    test->fetchSearchResult("Top");

    //Link retranslator.
    knI18n->link(this, &KNMusicStore::retranslate);
    retranslate();
}

QAbstractButton *KNMusicStore::tab()
{
    //Give back the tab widget.
    return m_tab;
}

void KNMusicStore::showIndex(KNMusicModel *musicModel,
                             const QModelIndex &index)
{
    //Well, it should be no use here.
    Q_UNUSED(musicModel)
    Q_UNUSED(index)
}

void KNMusicStore::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KNMusicStoreBase::resizeEvent(event);
    //Resize the switcher.
    m_storeSwitcher->resize(size());
    //Resize the content widget.
    if(m_storeSwitcher->widget())
    {
        //Resize the switcher.
        m_storeSwitcher->widget()->resize(
                    m_storeSwitcher->widget()->width(),
                    qMax(m_storeSwitcher->sizeHint().height(),
                         m_storeSwitcher->height()-2-
                         (m_storeSwitcher->horizontalScrollBar()->isVisible()?
                              m_storeSwitcher->horizontalScrollBar()->height():
                              0)));
    }
}

void KNMusicStore::showEvent(QShowEvent *event)
{
    //Show the widget.
    KNMusicStoreBase::showEvent(event);
    //Check the switcher.
    if(m_emptyWidget->isVisible() && m_backend)
    {
        //Start to fetch home content.
        m_backend->fetchHomeInfo();
    }
}

void KNMusicStore::retranslate()
{
    //Update the tab title.
    m_tab->setText(tr("Store"));
}

void KNMusicStore::onActionShowHome()
{
    //Check backend pointer.
    if(!m_backend)
    {
        //Ignore the null backend.
        return;
    }
    //Fetch home widget information.
    m_backend->fetchHomeInfo();
}

void KNMusicStore::loadBackend(KNMusicStoreBackend *backend)
{
    //Save the backend.
    m_backend=backend;
    //Check the backend pointer.
    if(m_backend==nullptr)
    {
        //Ignore the backend set.
        return;
    }
    //Change relationship.
    m_backend->setParent(this);
    //Set backends to all the widget.
    m_home->setBackend(m_backend);
    m_list->setBackend(m_backend);
    m_singleSong->setBackend(m_backend);
    m_searchResult->setBackend(m_backend);
}
