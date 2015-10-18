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

#include "sao/knsaostyle.h"
#include "knthememanager.h"

#include "knmusicmodel.h"
#include "knmusicglobal.h"
#include "knmusicproxymodel.h"
#include "knmusicnowplayingbase.h"
#include "knmusicnowplayingdelegate.h"

#include "knmusicnowplayinglistview.h"

using namespace MusicUtil;

KNMusicNowPlayingListView::KNMusicNowPlayingListView(QWidget *parent) :
    QListView(parent)
{
    setObjectName("NowPlayingListView");
    //Set item delegate.
    setItemDelegate(new KNMusicNowPlayingDelegate(this));
    KNSaoStyle::styleVerticalScrollBar(verticalScrollBar());
    //Add the widget the theme manager.
    knTheme->registerWidget(this);

    //Link the tree view signal and slot.
    connect(this, &KNMusicNowPlayingListView::activated,
            this, &KNMusicNowPlayingListView::onActionActivate);
}

KNMusicProxyModel *KNMusicNowPlayingListView::proxyModel()
{
    //Give back the model of this list view.
    return static_cast<KNMusicProxyModel *>(model());
}

KNMusicModel *KNMusicNowPlayingListView::musicModel()
{
    //Check out the proxy model.
    return proxyModel()==nullptr?nullptr:proxyModel()->musicModel();
}

void KNMusicNowPlayingListView::showEvent(QShowEvent *event)
{
    //Show the widget.
    QListView::showEvent(event);
    //Check out whether the proxy model is null or not.
    //Check whether the music model is null or not.
    //Check the playing index.
    if(proxyModel()==nullptr || musicModel()==nullptr ||
            (!musicModel()->playingIndex().isValid()))
    {
        //Give up.
        return;
    }
    //Get the playing proxy index..
    QModelIndex playingProxyIndex=
            proxyModel()->mapFromSource(musicModel()->playingIndex());
    //Scroll to the index.
    scrollTo(playingProxyIndex, PositionAtCenter);
    //Set current index to the playing proxy index.
    setCurrentIndex(playingProxyIndex);
}

void KNMusicNowPlayingListView::onActionActivate(const QModelIndex &index)
{
    //Play the activate index.
    playIndex(index);
}

inline void KNMusicNowPlayingListView::playIndex(const QModelIndex &index)
{
    //Check the music row and the index is valid.
    if(musicModel()==nullptr || !index.isValid())
    {
        //Ignore those invalid request.
        return;
    }
    //Get the now playing from the music global.
    KNMusicNowPlayingBase *nowPlaying=knMusicGlobal->nowPlaying();
    //Check null.
    if(nowPlaying==nullptr)
    {
        return;
    }
    //Ask the now playing to play the index row.
    nowPlaying->playMusicRow(proxyModel(),
                             index.row(),
                             nowPlaying->playingTab());
}
