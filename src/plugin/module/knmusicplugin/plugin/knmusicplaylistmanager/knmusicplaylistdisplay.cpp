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
#include <QLabel>

#include "knmousesensewidget.h"
#include "knmusicplaylisttreeview.h"

#include "knmusicplaylistdisplay.h"

KNMusicPlaylistDisplay::KNMusicPlaylistDisplay(QWidget *parent) :
    KNDropProxyContainer(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Add layout.
    QBoxLayout *displayLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             this);
    displayLayout->setContentsMargins(0,0,0,0);
    displayLayout->setSpacing(0);
    setLayout(displayLayout);

    //Initial information container.
    KNMouseSenseWidget *infoContainer=new KNMouseSenseWidget(this);
    infoContainer->setContentsMargins(20,12,0,8);

    QBoxLayout *infoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          infoContainer);
    infoLayout->setContentsMargins(0,0,0,0);
    infoLayout->setSpacing(0);
    infoContainer->setLayout(infoLayout);

    displayLayout->addWidget(infoContainer);

    //Initial the information label.
    m_playlistTitle=new QLabel(this);
    QPalette captionPal=m_playlistTitle->palette();
    captionPal.setColor(QPalette::WindowText, QColor(255,255,255));
    m_playlistTitle->setPalette(captionPal);
    QFont titleFont=m_playlistTitle->font();
    titleFont.setPixelSize(17);
    titleFont.setBold(true);
    m_playlistTitle->setFont(titleFont);
    infoLayout->addWidget(m_playlistTitle);
    m_playlistInfo=new QLabel(this);
    m_playlistInfo->setPalette(captionPal);
    infoLayout->addWidget(m_playlistInfo);

    //Initial the tree view.
    m_playlistTreeView=new KNMusicPlaylistTreeView(this);
//    connect(m_playlistTreeView, &KNMusicPlaylistTreeView::requirePlayIndex,
//            this, &KNMusicPlaylistDisplay::requirePlayIndex);
//    connect(m_playlistTreeView, &KNMusicPlaylistTreeView::requireRemoveItem,
//            this, &KNMusicPlaylistDisplay::requireRemoveItem);

    displayLayout->addWidget(m_playlistTreeView, 1);
}
