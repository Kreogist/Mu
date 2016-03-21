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
    m_singleSong(new KNMusicStoreSingleSongWidget(this))
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
    //Show the home widget.
//    m_home->show();
//    m_storeSwitcher->setWidget(m_home);
    m_list->show();
    m_storeSwitcher->setWidget(m_list);

    KNMusicStoreNetease *test=new KNMusicStoreNetease(this);
//    m_home->setBackend(test);
    m_list->setBackend(test);
//    test->fetchHomeWidgetInfo();
//    test->fetchAlbumDetail("34515142");
//    test->fetchAlbumDetail("497011");

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

void KNMusicStore::retranslate()
{
    //Update the tab title.
    m_tab->setText(tr("Store"));
}
