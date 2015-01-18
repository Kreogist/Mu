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
#include "knmusicproxymodel.h"
#include "knmusicnowplayingbase.h"
#include "knmusicdetailtooltipbase.h"

#include "knmusiclibrarytreeview.h"

#include <QDebug>

KNMusicLibraryTreeView::KNMusicLibraryTreeView(QWidget *parent) :
    KNMusicTreeViewBase(parent)
{
    //Enable sort for treeview.
    setSortingEnabled(true);
    //Set default color.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0x10, 0x10, 0x10));
    pal.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);
}

void KNMusicLibraryTreeView::resetHeaderState()
{
    KNMusicTreeViewBase::resetHeaderState();
    //I'd like to let plays to see at default.
    setColumnHidden(Plays, false);
    //Hide the category column.
    //Because we set that column, so we don't need to display it.
    //They are all the same!
    setColumnHidden(proxyModel()->filterKeyColumn(), true);
    //No more hack here, move the display data index one by one.
    moveToFirst(BlankData);
    //Set the index column at a enough width.
    setColumnWidth(BlankData, fontMetrics().width('6')*4+30);
}

void KNMusicLibraryTreeView::setCategoryColumn(const int &column)
{
    //Hide the detail tooltip first.
    KNMusicGlobal::detailTooltip()->hide();
    //Set the column to the proxy model.
    proxyModel()->setFilterKeyColumn(column);
}

void KNMusicLibraryTreeView::setCategoryText(const QString &fixedText)
{
    //Hide the detail tool tip first.
    KNMusicGlobal::detailTooltip()->hide();
    //We need to check the now playing model before we change the category.
    if(KNMusicGlobal::instance()->nowPlaying()->playingMusicModel()==proxyModel()->musicModel())
    {
        KNMusicGlobal::instance()->nowPlaying()->shadowPlayingModel();
    }
    QRegExp nameFilter("^"+
                       QRegExp::escape(fixedText)+
                       "$");
    nameFilter.setPatternSyntax(QRegExp::RegExp);
    //Change the filter string.
    proxyModel()->setFilterRegExp(nameFilter);
}
