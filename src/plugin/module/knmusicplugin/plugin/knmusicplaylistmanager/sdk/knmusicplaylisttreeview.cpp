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
#include "knmusicplaylistindex.h"
#include "knmusicproxymodel.h"

#include "knmusicplaylisttreeview.h"

KNMusicPlaylistTreeView::KNMusicPlaylistTreeView(QWidget *parent) :
    KNMusicTreeViewBase(parent)
{
    //Set properties.
    setDragDropMode(QAbstractItemView::DragDrop);

    //Set default color.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0x10, 0x10, 0x10));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);

    //Set index delegate.
    setItemDelegateForColumn(BlankData,
                             new KNMusicPlaylistIndex(this));
}

void KNMusicPlaylistTreeView::resetHeaderState()
{
    KNMusicTreeViewBase::resetHeaderState();
    //We need to make the play times visible, because it's playlist.
    setColumnHidden(Plays, false);
    //No more hack here, move the display data index one by one.
    moveToFirst(BlankData);
    //Set the index column at a enough width.
    setColumnWidth(BlankData, fontMetrics().width('6')*4+30);
}

void KNMusicPlaylistTreeView::playCurrentrPlaylist()
{
    //Check the proxy model is available or not.
    if(proxyModel()->rowCount()>0)
    {
        KNMusicGlobal::instance()->requirePlayMusicRow(proxyModel(),
                                                       0,
                                                       musicTab());
    }
}
