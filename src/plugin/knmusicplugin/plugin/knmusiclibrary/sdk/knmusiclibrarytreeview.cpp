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
#include "knthememanager.h"

#include "knmusicutil.h"
#include "knmusicproxymodel.h"

#include "knmusiclibrarytreeview.h"

using namespace MusicUtil;

KNMusicLibraryTreeView::KNMusicLibraryTreeView(QWidget *parent,
                                               KNMusicTab *tab) :
    KNMusicTreeViewBase(parent, tab)
{
    setObjectName("LibraryTreeView");
    //Set properties.
    setSortingEnabled(true);
    //Link to theme manager.
    knTheme->registerWidget(this);
}

void KNMusicLibraryTreeView::setCategoryColumn(const int &column)
{
    //Set the proxy model category column.
    proxyModel()->setCategoryColumn(column);
}

void KNMusicLibraryTreeView::setCategoryText(const QVariant &text)
{
    //Set the proxy model category text.
    proxyModel()->setCategoryContent(text);
}

void KNMusicLibraryTreeView::resetHeaderState()
{
    //Do the original header reset.
    KNMusicTreeViewBase::resetHeaderState();
    //I'd like to let plays to see at default.
    setColumnHidden(Plays, false);
    //No more hack here, move the display data index one by one.
    moveToFirst(MusicRowState);
    //Set the index
    setColumnWidth(MusicRowState, fontMetrics().width('6')*4+30);
}

