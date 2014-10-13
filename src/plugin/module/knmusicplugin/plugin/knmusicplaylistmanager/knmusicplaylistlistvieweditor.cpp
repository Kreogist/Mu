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
#include <QFileDialog>
#include <QBoxLayout>

#include "knanimationmenu.h"
#include "knopacityanimebutton.h"

#include "knmusicplaylistlistvieweditor.h"

KNMusicPlaylistListViewEditor::KNMusicPlaylistListViewEditor(QWidget *parent) :
    KNLinearSenseWidget(parent)
{
    //Initial contents.
    setContentsMargins(0,0,0,0);
    setMinimumHeight(34);

    //Initial layouts.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(8,8,8,8);
    mainLayout->setSpacing(8);
    setLayout(mainLayout);

    //Initial the add button.
    m_add=new KNOpacityAnimeButton(this);
    m_add->setIcon(QPixmap(":/plugin/music/playlist/add.png"));
    //Initial add playlist actions.
    createAddMenu();
    connect(m_add, &KNOpacityAnimeButton::clicked,
            this, &KNMusicPlaylistListViewEditor::showAddMenu);
    mainLayout->addWidget(m_add, 0, Qt::AlignCenter);

    //Initial the remove button.
    m_removeCurrent=new KNOpacityAnimeButton(this);
    m_removeCurrent->setIcon(QPixmap(":/plugin/music/playlist/remove.png"));
    connect(m_removeCurrent, &KNOpacityAnimeButton::clicked,
            this, &KNMusicPlaylistListViewEditor::requireRemoveCurrentPlaylist);
    mainLayout->addWidget(m_removeCurrent, 0, Qt::AlignCenter);

    mainLayout->addStretch();

    //Initial the configure button.
    m_configure=new KNOpacityAnimeButton(this);
    m_configure->setIcon(QPixmap(":/plugin/music/playlist/edit.png"));
    mainLayout->addWidget(m_configure, 0, Qt::AlignCenter);

    retranslate();
}

void KNMusicPlaylistListViewEditor::retranslate()
{
    //Update menu actions.
    m_addActionTitles[AddPlaylist]=tr("New Playlist");
    m_addActionTitles[ImportPlaylist]=tr("Import Playlist");
    //Update filter.
    m_playlistFilter.clear();
    m_playlistFilter<<tr("Mu playlist (*.mplst)");
    //Set add menu actions.
    for(int i=0; i<AddMenuActionsCount; i++)
    {
        m_addActions[i]->setText(m_addActionTitles[i]);
    }
}

void KNMusicPlaylistListViewEditor::showAddMenu()
{
    //Set position.
    m_addMenu->setMouseDownPos(QCursor::pos());
    //Show up add menu.
    m_addMenu->exec(QCursor::pos());
}

void KNMusicPlaylistListViewEditor::showImportDialog()
{
    QFileDialog importFile(this);
    importFile.setFileMode(QFileDialog::ExistingFiles);
    importFile.setNameFilters(m_playlistFilter);
    if(importFile.exec())
    {
        emit requireImportPlaylist(importFile.selectedFiles());
    }
}

void KNMusicPlaylistListViewEditor::createAddMenu()
{
    m_addMenu=new KNAnimationMenu(this);
    //Set seperator style sheet, I can't solve this bug in coding way.
    m_addMenu->setStyleSheet("QMenu::separator {height:1px;"
                             "background: rgba(255, 255, 255, 100);"
                             "margin-left: 5px;"
                             "margin-right: 5px;}");
    //Set palette.
    QPalette pal=m_addMenu->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    pal.setColor(QPalette::Text, QColor(255,255,255,224));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    m_addMenu->setPalette(pal);

    //Initial the actions.
    for(int i=0; i<AddMenuActionsCount; i++)
    {
        m_addActions[i]=new QAction(this);
    }

    connect(m_addActions[AddPlaylist], SIGNAL(triggered()),
            this, SIGNAL(requireAddPlaylist()));
    m_addMenu->addAction(m_addActions[AddPlaylist]);

    m_addMenu->addSeparator();

    connect(m_addActions[ImportPlaylist], SIGNAL(triggered()),
            this, SLOT(showImportDialog()));
    m_addMenu->addAction(m_addActions[ImportPlaylist]);
}
