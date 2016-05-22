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
#include <QScrollBar>

#include "knthememanager.h"

#include "knmusicutil.h"
#include "knmusicglobal.h"
#include "knmusicnowplayingbase.h"
#include "knmusicproxymodel.h"
#include "knmusicalbumlistdelegate.h"

#include "knmusicalbumlistview.h"

using namespace MusicUtil;

KNMusicAlbumListView::KNMusicAlbumListView(QWidget *parent, KNMusicTab *tab) :
    KNMusicListViewBase(parent, tab)
{
    setObjectName("AlbumListView");
    //Set properties.
    setItemDelegate(new KNMusicAlbumListDelegate(this));
    knTheme->registerWidget(this);
    //Set the proxy model category column.
    proxyModel()->setCategoryColumn(Album);
}

void KNMusicAlbumListView::setCategoryText(const QVariant &text)
{
    //Check the proxy model is being played or not.
    if(knMusicGlobal->nowPlaying())
    {
        //Get the now playing object.
        KNMusicNowPlayingBase *nowPlaying=knMusicGlobal->nowPlaying();
        //Check the current playing model is the current proxy and the proxy
        //model is copied from the treeview.
        if(nowPlaying->playingModel()==proxyModel())
        {
            //If so, shadow the proxy model.
            nowPlaying->shadowPlayingModel();
        }
    }
    //Set the proxy model category text.
    proxyModel()->setCategoryContent(text.toString());
}
