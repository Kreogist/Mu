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
#include "knlocalemanager.h"
#include "kncategorytab.h"
#include "knthememanager.h"

#include "sdk/knmusicstoreglobal.h"
#include "sdk/knmusicstorecontainer.h"
#include "sdk/knmusicstoreloadingdimmer.h"

#include "knmusicstore.h"

KNMusicStore::KNMusicStore(QWidget *parent) :
    KNMusicStoreBase(parent),
    m_tab(new KNCategoryTab(this)),
    m_container(nullptr),
    m_errorDimmer(new QWidget(this)),
    m_loadingDimmer(new KNMusicStoreLoadingDimmer(this))
{
    setObjectName("MusicStore");
    //Initial the global object.
    KNMusicStoreGlobal::initial(this);
    //Configure the tab.
    m_tab->setIcon(QIcon(":/plugin/music/category/store.png"));
    //Initial and configure the container.
    //The container must be initialized after initial the global object.
    m_container=new KNMusicStoreContainer(this);
    //Configure the error dimmer.
    ;
    //Configure the loading dimmer.
    ;

    //Add to theme manager.
    knTheme->registerWidget(this);

    //Link retranslate.
    knI18n->link(this, &KNMusicStore::retranslate);
    retranslate();

    //Debug
    m_container->raise();
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

void KNMusicStore::resizeEvent(QResizeEvent *event)
{
    //Resize the base widget.
    KNMusicStoreBase::resizeEvent(event);
    //Resize the content widgets.
    // Widgets container.
    m_container->resize(size());
    // Error dimmer.
    m_errorDimmer->resize(size());
    // Loading dimmer.
    m_loadingDimmer->resize(size());
}

void KNMusicStore::retranslate()
{
    //Update the tab title.
    m_tab->setText(tr("Store"));
}
