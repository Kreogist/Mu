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

#include "knmusicalbumindexdelegate.h"

#include "knmusicalbumtreeview.h"

KNMusicAlbumTreeView::KNMusicAlbumTreeView(QWidget *parent) :
    KNMusicLibraryTreeView(parent)
{
    //Set properties.
    setAnimateState(false);
    //Reset the headers.
    setHeaderHidden(true);
    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(0,0,0));
    pal.setColor(QPalette::Text, QColor(0,0,0));
    pal.setColor(QPalette::Button, QColor(255,255,255));
    setPalette(pal);
    //Set the index delegate.
    setItemDelegateForColumn(BlankData,
                             new KNMusicAlbumIndexDelegate(this));
}

void KNMusicAlbumTreeView::resizeHeader()
{
    //Resize time and rating column.
    resizeColumnToContents(Time);
    int preferWidth=width()-columnWidth(Time)-columnWidth(BlankData);
    //If the width is too small, hide the rating.
    if(width()<350)
    {
        //Hide the rating column.
        setColumnHidden(Rating, true);
    }
    else
    {
        //Show and resize rating size.
        setColumnHidden(Rating, false);
        resizeColumnToContents(Rating);
        //Set the prefer width.
        preferWidth-=columnWidth(Rating);
    }
    //Check vertical scroll bar is visible.
    setColumnWidth(Name,verticalScrollBar()->isVisible()?
                            preferWidth-verticalScrollBar()->width():
                            preferWidth);
}

void KNMusicAlbumTreeView::resetHeaderState()
{
    //Hide all column first.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        setColumnHidden(i, true);
    }
    //Show the default columns.
    setColumnHidden(Time, false);
    setColumnHidden(Rating, false);
    //Reorder the columns.
    moveToFirst(Time);
    moveToFirst(Rating);
    moveToFirst(Name);
    //No more hack here, move the display data index one by one.
    moveToFirst(BlankData);
    //Set the index column at a enough width.
    setColumnWidth(BlankData, 45);
}
