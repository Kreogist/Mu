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
#include "knmusicutil.h"
#include "knmusicmodel.h"
#include "knmusicproxymodel.h"
#include "knmusicnowplayingbase.h"
#include "knmusicplaylistindexdelegate.h"
#include "knmusicbackend.h"

#include "knmusicplaylisttreeview.h"

#include <QDebug>

using namespace MusicUtil;

KNMusicPlaylistTreeView::KNMusicPlaylistTreeView(QWidget *parent,
                                                 KNMusicTab *tab) :
    KNMusicTreeViewBase(parent, tab)
{
    setObjectName("PlaylistTreeView");
    //Update the palettle.
    onActionThemeUpdate();
    //Enabled the drag and drop.
    setDragDropMode(QAbstractItemView::DragDrop);
    setDragEnabled(true);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    //Set the delegate.
    setItemDelegateForColumn(MusicRowState,
                             new KNMusicPlaylistIndexDelegate(this));
}

bool KNMusicPlaylistTreeView::playCurrentPlaylist()
{
    //Check several things.
    //1. Is now playing loaded?
    if((!knMusicGlobal->nowPlaying()) ||
    //2. Is music model nullptr?.
            musicModel()==nullptr ||
    //3. Check the playlist is empty or not.
            musicModel()->rowCount()==0)
    {
        //Ignore the playing request.
        return false;
    }
    //Get the now playing model.
    KNMusicNowPlayingBase *nowPlaying=knMusicGlobal->nowPlaying();
    //Check whether the current playing model is the current one.
    if(nowPlaying->playingMusicModel()==musicModel())
    {
        //Check the current playing index is valid or not.
        if(nowPlaying->playingIndex().isValid())
        {
            //Give back false.
            return false;
        }
    }
    //So, now we need to play the first song in the playlist.
    playIndex(proxyModel()->index(0, 0));
    //Give back true.
    return true;
}

void KNMusicPlaylistTreeView::resetHeaderState()
{
    //Do the previous reset first.
    KNMusicTreeViewBase::resetHeaderState();
    //To playlist, we will make the play times visible.
    setColumnHidden(Plays, false);
    //No more back here, move the last column to the front, because it's the
    //playlist index.
    moveToFirst(MusicRowState);
    //Resize the index column at a enough width.
    setColumnWidth(MusicRowState, fontMetrics().width('6')*4+30);
}
