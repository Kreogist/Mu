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
#include "knmusicplaylistindexdelegate.h"

#include "knmusicplaylisttreeview.h"

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
