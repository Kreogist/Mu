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
#include <QBoxLayout>
#include <QItemSelectionModel>
#include <QSplitter>

#include "knmusicplaylistlistitem.h"
#include "knmusicplaylistlist.h"
#include "knmusicplaylistlistview.h"
#include "knmusicplaylistlistvieweditor.h"
#include "knmusicplaylistdisplay.h"

#include "knmusicplaylisttab.h"

#include <QDebug>

KNMusicPlaylistTab::KNMusicPlaylistTab(QObject *parent) :
    KNMusicTab(parent)
{
    //Initial the viewer.
    m_viewer=new QSplitter();
    //Set viewer properties.
    m_viewer->setContentsMargins(0,0,0,0);
    m_viewer->setHandleWidth(0); //This is beautiful.
    m_viewer->setChildrenCollapsible(false);

    //Initial the playlist list.
    initialPlaylistList();
    m_viewer->addWidget(m_playlistListViewer);
    //Initial the playlist display.
    m_playlistDisplay=new KNMusicPlaylistDisplay(m_viewer);
    m_viewer->addWidget(m_playlistDisplay);

    //Set viewer properties after add widgets.
    m_viewer->setCollapsible(1, false);
    m_viewer->setStretchFactor(1, 1);
}

KNMusicPlaylistTab::~KNMusicPlaylistTab()
{
    if(m_viewer->parent()==0)
    {
        delete m_viewer;
    }
}

QString KNMusicPlaylistTab::caption()
{
    return tr("Playlist");
}

QPixmap KNMusicPlaylistTab::icon()
{
    return QPixmap(":/plugin/music/category/05_playlists.png");
}

QWidget *KNMusicPlaylistTab::widget()
{
    return m_viewer;
}

KNMusicPlaylistModel *KNMusicPlaylistTab::currentPlaylistModel()
{
    KNMusicPlaylistListItem *currentItem=m_playlistDisplay->currentItem();
    return currentItem==nullptr?nullptr:currentItem->playlistModel();
}

void KNMusicPlaylistTab::onActionPlaylistItemChanged(QStandardItem *item)
{
    KNMusicPlaylistListItem *changedItem=(KNMusicPlaylistListItem *)item;
    if(m_playlistDisplay->currentItem()==changedItem)
    {
        m_playlistDisplay->updatePlaylistInfo();
    }
}

void KNMusicPlaylistTab::displayPlaylistItem(KNMusicPlaylistListItem *item)
{
    m_playlistDisplay->displayPlaylistItem(item);
}

void KNMusicPlaylistTab::setPlaylistList(KNMusicPlaylistList *playlistList)
{
    m_playlistListView->setModel(playlistList);
    connect(m_playlistListView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicPlaylistTab::currentPlaylistChanged);
}

void KNMusicPlaylistTab::setCurrentPlaylist(const QModelIndex &index)
{
    m_playlistListView->setCurrentIndex(index);
}

void KNMusicPlaylistTab::editPlaylistName(const QModelIndex &index)
{
    m_playlistListView->edit(index);
}

void KNMusicPlaylistTab::onActionAddPlaylist()
{
    emit requireGeneratePlaylist(tr("New Playlist"));
}

void KNMusicPlaylistTab::initialPlaylistList()
{
    //Initial the container.
    m_playlistListViewer=new QWidget(m_viewer);
    //Set properties.
    m_playlistListViewer->setContentsMargins(0,0,0,0);

    //Initial the layout.
    QBoxLayout *playlistListLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                  m_playlistListViewer);
    playlistListLayout->setContentsMargins(0,0,0,0);
    playlistListLayout->setSpacing(0);
    m_playlistListViewer->setLayout(playlistListLayout);

    //Initial the list view.
    m_playlistListView=new KNMusicPlaylistListView(m_playlistListViewer);
    playlistListLayout->addWidget(m_playlistListView);

    //Initial the list editor.
    m_playlistListViewEditor=new KNMusicPlaylistListViewEditor(m_playlistListViewer);
    connect(m_playlistListViewEditor, &KNMusicPlaylistListViewEditor::requireAddPlaylist,
            this, &KNMusicPlaylistTab::onActionAddPlaylist);
    playlistListLayout->addWidget(m_playlistListViewEditor);
}
