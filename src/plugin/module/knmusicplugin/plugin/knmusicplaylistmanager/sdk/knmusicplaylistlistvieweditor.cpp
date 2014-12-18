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
#include "knmusicplaylistloader.h"
#include "knmusicplaylistlistview.h"

#include "knlocalemanager.h"

#include "knmusicplaylistlistvieweditor.h"

#include <QDebug>

KNMusicPlaylistListViewEditor::KNMusicPlaylistListViewEditor(QWidget *parent) :
    KNLinearSenseWidget(parent)
{
    //Initial contents.
    setContentsMargins(0,0,0,0);
    setMinimumHeight(34);

    //Initial menu.
    initialMenu();

    //Initial layouts.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(8,8,8,8);
    mainLayout->setSpacing(8);
    setLayout(mainLayout);

    //Initial the add button.
    m_add=new KNOpacityAnimeButton(this);
    m_add->setIcon(QPixmap(":/plugin/music/playlist/add.png"));
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
    connect(m_configure, &KNOpacityAnimeButton::clicked,
            this, &KNMusicPlaylistListViewEditor::showConfigureMenu);
    mainLayout->addWidget(m_configure, 0, Qt::AlignCenter);

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicPlaylistListViewEditor::retranslate);
}

void KNMusicPlaylistListViewEditor::retranslate()
{
    //Update menu actions.
    m_addActions[AddPlaylist]->setText(tr("New Playlist"));
    m_addActions[ImportPlaylist]->setText(tr("Import Playlist"));

    m_configureActions[ExportPlaylist]->setText(tr("Export"));
    m_configureActions[CopyPlaylist]->setText(tr("Copy"));
    //Update filter.
    //Get the types and suffixs.
    QStringList playlistTypes, playlistSuffixs;
    m_playlistLoader->getPlaylistTypeAndSuffix(playlistTypes, playlistSuffixs);

    //Initial the all support filter.
    QString allSupportFilter=tr("All Support Playlist") + "(*.mplst";
    //Initial the filters.
    m_playlistFilter.clear();
    m_playlistFilter<<tr("Mu playlist") + " (*.mplst)";
    for(int i=0; i<playlistTypes.size(); i++)
    {
        //Add the all support filter.
        allSupportFilter.append(" "+playlistSuffixs.at(i));
        //Add to filter list.
        m_playlistFilter<<playlistTypes.at(i)+" ("+playlistSuffixs.at(i)+")";
    }
    allSupportFilter.append(")");
    //Insert the all support filter to filter list.
    m_playlistFilter.prepend(allSupportFilter);
}

void KNMusicPlaylistListViewEditor::showAddMenu()
{
    //Set position.
    m_addMenu->setMouseDownPos(QCursor::pos());
    //Show add menu.
    m_addMenu->exec(QCursor::pos());
}

void KNMusicPlaylistListViewEditor::showConfigureMenu()
{
    //Set position.
    m_configureMenu->setMouseDownPos(QCursor::pos());
    //Show configure menu.
    m_configureMenu->exec(QCursor::pos());
}

void KNMusicPlaylistListViewEditor::importPlaylists()
{
    QFileDialog importFile(this);
    importFile.setFileMode(QFileDialog::ExistingFiles);
    importFile.setNameFilters(m_playlistFilter);
    if(importFile.exec() &&
            !importFile.selectedFiles().isEmpty())
    {
        emit requireImportPlaylist(importFile.selectedFiles());
    }
}

void KNMusicPlaylistListViewEditor::exportPlaylist()
{
    //Ensure the current index is vaild.
    if(!m_playlistListView->currentIndex().isValid())
    {
        return;
    }
    //Set the export file dialog properties.
    QFileDialog exportFile(this);
    exportFile.setFileMode(QFileDialog::AnyFile);
    exportFile.setAcceptMode(QFileDialog::AcceptSave);
    //Set the default playlist file name.
    exportFile.selectFile(m_playlistListView->currentPlaylistTitle());
    //Set the export filter.
    QStringList exportFilter=m_playlistFilter;
    exportFilter.removeFirst();
    exportFile.setNameFilters(exportFilter);
    if(exportFile.exec() &&
            !exportFile.selectedFiles().isEmpty())
    {
        emit requireExportPlaylist(exportFile.selectedFiles().first(),
                                   m_playlistListView->currentIndex());
    }
}

void KNMusicPlaylistListViewEditor::initialMenu()
{
    m_addMenu=new KNAnimationMenu(this);
    m_configureMenu=new KNAnimationMenu(this);
    //Set seperator style sheet, I can't solve this bug in coding way.
    QString seperatorStyleSheet="QMenu::separator {height:1px;"
                                "background: rgba(255, 255, 255, 100);"
                                "margin-left: 5px;"
                                "margin-right: 5px;}";
    m_addMenu->setStyleSheet(seperatorStyleSheet);
    m_configureMenu->setStyleSheet(seperatorStyleSheet);
    //Set palette.
    QPalette pal=m_addMenu->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    pal.setColor(QPalette::Text, QColor(255,255,255,224));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    m_addMenu->setPalette(pal);
    m_configureMenu->setPalette(pal);

    //Initial add actions.
    for(int i=0; i<AddMenuActionsCount; i++)
    {
        m_addActions[i]=new QAction(this);
    }

    //Set connections.
    connect(m_addActions[AddPlaylist], SIGNAL(triggered()),
            this, SIGNAL(requireAddPlaylist()));
    connect(m_addActions[ImportPlaylist], SIGNAL(triggered()),
            this, SLOT(importPlaylists()));

    //Generate menu.
    m_addMenu->addAction(m_addActions[AddPlaylist]);
    m_addMenu->addSeparator();
    m_addMenu->addAction(m_addActions[ImportPlaylist]);

    //Initial configure actions.
    for(int i=0; i<ConfigureMenuActionCount; i++)
    {
        m_configureActions[i]=new QAction(this);
    }

    connect(m_configureActions[ExportPlaylist], SIGNAL(triggered()),
            this, SLOT(exportPlaylist()));
    connect(m_configureActions[CopyPlaylist], SIGNAL(triggered()),
            this, SIGNAL(requireCopyCurrentPlaylist()));

    m_configureMenu->addAction(m_configureActions[ExportPlaylist]);
    m_configureMenu->addSeparator();
    m_configureMenu->addAction(m_configureActions[CopyPlaylist]);
}

void KNMusicPlaylistListViewEditor::setPlaylistListView(KNMusicPlaylistListView *playlistListView)
{
    m_playlistListView = playlistListView;
}

void KNMusicPlaylistListViewEditor::setPlaylistLoader(KNMusicPlaylistLoader *playlistLoader)
{
    m_playlistLoader=playlistLoader;
    //Do Retranslate.
    retranslate();
}
