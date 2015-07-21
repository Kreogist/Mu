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
#include <QSplitter>

#include "kncategorytab.h"
#include "knemptystatewidget.h"

#include "sdk/knmusicplaylistemptyhint.h"
#include "sdk/knmusicplaylistlist.h"
#include "sdk/knmusicplaylistviewer.h"

#include "knlocalemanager.h"

#include "knmusicplaylist.h"

KNMusicPlaylist::KNMusicPlaylist(QWidget *parent) :
    KNMusicPlaylistBase(parent),
    m_tab(new KNCategoryTab(this)),
    m_container(new KNEmptyStateWidget(this)),
    m_playlistList(new KNMusicPlaylistList(this)),
    m_playlistViewer(new KNMusicPlaylistViewer(this))
{
    //Configure the tab.
    m_tab->setIcon(QIcon(":/plugin/music/playlist/icon.png"));

    //Generate the empty hint widget.
    KNMusicPlaylistEmptyHint *emptyHint=new KNMusicPlaylistEmptyHint(this);
    //Link the empty hint with the container.
    connect(emptyHint, &KNMusicPlaylistEmptyHint::requireAddPlaylist,
            m_container, &KNEmptyStateWidget::showContentWidget);
    //Set the empty widget and content widget.
    m_container->setEmptyWidget(emptyHint);
    //Get the splitter.
    QSplitter *contentContainer=generateSplitter();
    //Set the content widget as the splitter.
    m_container->setContentWidget(contentContainer);
    //Add the playlist list and playlist viewer to the splitter.
    contentContainer->addWidget(m_playlistList);
    contentContainer->addWidget(m_playlistViewer);

    //Link to retranslate.
    knI18n->link(this, &KNMusicPlaylist::retranslate);
    retranslate();
}

QAbstractButton *KNMusicPlaylist::tab()
{
    return m_tab;
}

void KNMusicPlaylist::resizeEvent(QResizeEvent *event)
{
    //Resize the tab widget.
    KNMusicPlaylistBase::resizeEvent(event);
    //Resize the container.
    m_container->resize(size());
}

void KNMusicPlaylist::retranslate()
{
    m_tab->setText(tr("Playlist"));
}

QSplitter *KNMusicPlaylist::generateSplitter()
{
    //Generate the splitter.
    QSplitter *splitter=new QSplitter(this);
    //Configure the splitter.
//    splitter->setHandleWidth(0);
    //Return the splitter.
    return splitter;
}
