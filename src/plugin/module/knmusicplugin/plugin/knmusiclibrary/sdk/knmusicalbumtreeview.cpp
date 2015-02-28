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
    pal.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    pal.setColor(QPalette::HighlightedText, QColor(0,0,0));
    setPalette(pal);
    //Set the index delegate.
    setItemDelegateForColumn(BlankData,
                             new KNMusicAlbumIndexDelegate(this));
    //Always hide horizontal scroll bar.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Configure the vertiacal scroll bar.
    KNSAOStyle::setScrollBarStyleSheet(verticalScrollBar());
}

void KNMusicAlbumTreeView::resizeHeader(int preferWidth)
{
    //Resize time and rating column.
    resizeColumnToContents(Time);
    //Calculate the row count is larger than the height, to ensure the vertical
    //scroll bar will be visible or not.
    int calculateWidth=preferWidth==-1?width():preferWidth;
    setColumnWidth(Name,
                   calculateWidth-columnWidth(Time)-columnWidth(BlankData)-verticalScrollBar()->width());
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
    //Reorder the columns.
    moveToFirst(Time);
    moveToFirst(Name);
    //No more hack here, move the display data index one by one.
    moveToFirst(BlankData);
    //Set the index column at a enough width.
    setColumnWidth(BlankData, 45);
}
