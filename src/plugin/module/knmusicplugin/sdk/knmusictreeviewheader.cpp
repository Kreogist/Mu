/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusictreeviewheadermenu.h"

#include "knmusictreeviewheader.h"

#include <QDebug>

KNMusicTreeViewHeader::KNMusicTreeViewHeader(QWidget *parent) :
    KNMouseSenseHeader(parent)
{
    //Initial menu.
    m_headerMenu=new KNMusicTreeViewHeaderMenu(this);

    //Connect request.
    connect(this, &KNMusicTreeViewHeader::customContextMenuRequested,
            this, &KNMusicTreeViewHeader::showCustomContextMenu);
    connect(m_headerMenu, &KNMusicTreeViewHeaderMenu::requireSetColumnVisible,
            this, &KNMusicTreeViewHeader::setSectionHidden);
    connect(m_headerMenu, &KNMusicTreeViewHeaderMenu::requireResizeAll,
            this, &KNMusicTreeViewHeader::resizeAllColumns);
    connect(m_headerMenu, &KNMusicTreeViewHeaderMenu::requireResize,
            this, &KNMusicTreeViewHeader::resizeColumn);
}

KNMusicTreeViewHeader::~KNMusicTreeViewHeader()
{
//    delete m_headerMenu;
}

void KNMusicTreeViewHeader::resizeAllColumns()
{
    for(int i=Name; i<MusicDisplayDataCount; i++)
    {
        if(!isSectionHidden(i))
        {
            emit requireResizeColumnToContents(i);
        }
    }
}

void KNMusicTreeViewHeader::resizeColumn(const int &logicalIndex)
{
    emit requireResizeColumnToContents(logicalIndex);
}

void KNMusicTreeViewHeader::showCustomContextMenu(const QPoint &mousePoint)
{
    m_headerMenu->setMouseDownLogicalIndex(logicalIndexAt(mousePoint));
    //Sync column visible state.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        //Here should use logical index.
        m_headerMenu->syncVisibleState(i,
                                       !isSectionHidden(i));
    }
    QPoint mouseDownPos=mapToGlobal(mousePoint);
    m_headerMenu->setMouseDownPos(mouseDownPos);
    m_headerMenu->exec(mouseDownPos);
}
