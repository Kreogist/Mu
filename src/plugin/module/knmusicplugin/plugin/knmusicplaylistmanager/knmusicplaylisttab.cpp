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
#include <QSplitter>
#include <QBoxLayout>

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

    //Initial the playlist list.
    initialPlaylistList();
    m_viewer->addWidget(m_playlistList);
    //Initial the playlist display.
    m_playlistDisplay=new KNMusicPlaylistDisplay(m_viewer);
    m_viewer->addWidget(m_playlistDisplay);
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

void KNMusicPlaylistTab::initialPlaylistList()
{
    //Initial the container.
    m_playlistList=new QWidget(m_viewer);
    //Set properties.
    m_playlistList->setContentsMargins(0,0,0,0);

    //Initial the layout.
    QBoxLayout *playlistListLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                  m_playlistList);
    playlistListLayout->setContentsMargins(0,0,0,0);
    playlistListLayout->setSpacing(0);
    m_playlistList->setLayout(playlistListLayout);

    //Initial the list view.
    m_playlistListView=new KNMusicPlaylistListView(m_playlistList);
    playlistListLayout->addWidget(m_playlistListView);

    //Initial the list editor.
    m_playlistListViewEditor=new KNMusicPlaylistListViewEditor(m_playlistList);
    playlistListLayout->addWidget(m_playlistListViewEditor);
}
