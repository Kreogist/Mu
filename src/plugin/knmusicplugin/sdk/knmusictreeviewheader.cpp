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
#include "knmusictreeviewheadermenu.h"
#include "knthememanager.h"

#include "knmusictreeviewheader.h"

KNMusicTreeViewHeader::KNMusicTreeViewHeader(QWidget *parent) :
    KNMouseSenseHeader(parent),
    m_headerMenu(new KNMusicTreeViewHeaderMenu(this))
{
    //Configure the header menu.
    m_headerMenu->setObjectName("MusicTreeViewHeaderMenu");
    knTheme->registerWidget(m_headerMenu);
    //Connect the custom context menu show request.
    connect(this, &KNMusicTreeViewHeader::customContextMenuRequested,
            this, &KNMusicTreeViewHeader::showCustomContextMenu);

    //Link the request of the context menu to the header.
    connect(m_headerMenu, &KNMusicTreeViewHeaderMenu::requireSetColumnVisible,
            this, &KNMusicTreeViewHeader::setSectionHidden);
    connect(m_headerMenu, &KNMusicTreeViewHeaderMenu::requireResizeAll,
            this, &KNMusicTreeViewHeader::resizeAllColumns);
    connect(m_headerMenu, &KNMusicTreeViewHeaderMenu::requireResize,
            this, &KNMusicTreeViewHeader::requireResizeColumnToContents);
}

void KNMusicTreeViewHeader::hideColumnVisibleAction(const int &column)
{
    //Hide the specific column action.
    m_headerMenu->setActionVisible(column, false);
}

void KNMusicTreeViewHeader::resizeAllColumns()
{
    //Resize all the visible section.
    for(int i=Name; i<MusicColumnCount; i++)
    {
        //Check the section visible.
        if(!isSectionHidden(i))
        {
            emit requireResizeColumnToContents(i);
        }
    }
}

void KNMusicTreeViewHeader::showCustomContextMenu(const QPoint &mousePoint)
{
    //The set the logical index where the mouse point is to the header menu.
    m_headerMenu->setMouseDownLogicalIndex(logicalIndexAt(mousePoint));
    //Synchnorse column visible state.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        //Here should use logical index.
        m_headerMenu->setVisibleState(i, !isSectionHidden(i));
    }
    //Get the global position of the mouse pressed point.
    QPoint menuStartPosition=mapToGlobal(mousePoint);
    //Set it as the start position of the header menu.
    m_headerMenu->setMouseDownPos(menuStartPosition);
    //Execute the menu.
    m_headerMenu->exec(menuStartPosition);
}

