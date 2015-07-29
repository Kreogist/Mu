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
#include <QAction>
#include <QSignalMapper>

#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusictreeviewheadermenu.h"

KNMusicTreeViewHeaderMenu::KNMusicTreeViewHeaderMenu(QWidget *parent) :
    KNAnimationMenu(parent),
    m_tweakAllWidth(new QAction(this)),
    m_tweakWidth(new QAction(this)),
    m_visibleMapper(new QSignalMapper(this)),
    m_mouseDownLogicalIndex(-1)
{
    setObjectName("MusicTreeViewHeaderMenu");
    //Set properties.
    setAutoFillBackground(true);
    //Set seperator style sheet, I can't solve this bug in coding way.
    setSeparatorColor(QColor(255, 255, 255, 100));

    //Initial the actions of controlling the visible of the columns.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        //Initial the visible action.
        m_itemVisible[i]=new QAction(this);
        //Set the default checkable to true.
        m_itemVisible[i]->setCheckable(true);
        //Link the action with the visble mapper.
        connect(m_itemVisible[i], SIGNAL(triggered()),
                m_visibleMapper, SLOT(map()));
        m_visibleMapper->setMapping(m_itemVisible[i], i);
    }

    //Link the visible mapper with the action slot.
    connect(m_visibleMapper,
            static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
            this,
            &KNMusicTreeViewHeaderMenu::onActionVisibleChange);
    //Link the tweak width action with the action slot.
    connect(m_tweakWidth, &QAction::triggered,
            this,
            &KNMusicTreeViewHeaderMenu::onActionResizeCurrent);
    connect(m_tweakAllWidth, &QAction::triggered,
            this,
            &KNMusicTreeViewHeaderMenu::requireResizeAll);

    //Add actions to menu.
    addAction(m_tweakWidth);
    addAction(m_tweakAllWidth);
    addSeparator();
    //Add visible actions to menu.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        addAction(m_itemVisible[i]);
    }

    //Register the menu to theme manager.
    knTheme->registerWidget(this);
    //Link the retranslate.
    knI18n->link(this, &KNMusicTreeViewHeaderMenu::retranslate);
    retranslate();
}

void KNMusicTreeViewHeaderMenu::setVisibleState(const int &logicalIndex,
                                                const bool &visible)
{
    m_itemVisible[logicalIndex]->setChecked(visible);
}

void KNMusicTreeViewHeaderMenu::setMouseDownLogicalIndex(const int &index)
{
    //Check whether the tweak width action should be visible.
    //Only show the tweak width action when the column is valid.
    m_tweakWidth->setVisible(index!=-1 && index<MusicDataCount);
    //Save the current index.
    m_mouseDownLogicalIndex=index;
}

void KNMusicTreeViewHeaderMenu::retranslate()
{
    //Set tweak width action text.
    m_tweakWidth->setText(tr("Auto Size Column"));
    m_tweakAllWidth->setText(tr("Auto Size All Columns"));
    //Set visible action text.
    for(int i=Name+1; i<MusicDataCount; i++)
    {
        m_itemVisible[i]->setText(m_musicGlobal->treeViewHeaderText(i));
    }
}

void KNMusicTreeViewHeaderMenu::onActionVisibleChange(const int &index)
{
    //Ask to change the column visible to the opposite.
    emit requireSetColumnVisible(index,
                                 !m_itemVisible[index]->isChecked());
}

void KNMusicTreeViewHeaderMenu::onActionResizeCurrent()
{
    emit requireResize(m_mouseDownLogicalIndex);
}
