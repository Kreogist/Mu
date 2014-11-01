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
#include "knlocalemanager.h"

#include "knmusicmultimenu.h"

KNMusicMultiMenu::KNMusicMultiMenu(QWidget *parent) :
    KNMusicMultiMenuBase(parent)
{
    //Set seperator style sheet, I can't solve this bug in coding way.
    setStyleSheet("QMenu::separator {height:1px;"
                  "background: rgba(255, 255, 255, 100);"
                  "margin-left: 5px;"
                  "margin-right: 5px;}");
    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    pal.setColor(QPalette::Text, QColor(255,255,255,224));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);
    //Generate actions.
    createActions();

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicMultiMenu::retranslate);
    //Get the latest translation.
    retranslate();
}

void KNMusicMultiMenu::retranslate()
{
    //Update captions.
    m_actionTitles[DeleteSelections]=tr("Delete");

    //Update actions.
    for(int i=0; i<MultiMenuActionsCount; i++)
    {
        m_actions[i]->setText(m_actionTitles[i]);
    }
}

void KNMusicMultiMenu::createActions()
{
    //Initial actions.
    for(int i=0; i<MultiMenuActionsCount; i++)
    {
        m_actions[i]=new QAction(this);
    }
    //Delete selections.
    connect(m_actions[DeleteSelections], SIGNAL(triggered()),
            this, SIGNAL(requireDeleteSelection()));
    addAction(m_actions[DeleteSelections]);
}
