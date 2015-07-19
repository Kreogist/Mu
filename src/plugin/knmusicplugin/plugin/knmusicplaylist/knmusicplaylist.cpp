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
#include "kncategorytab.h"
#include "knemptystatewidget.h"

#include "sdk/knmusicplaylistemptyhint.h"

#include "knlocalemanager.h"

#include "knmusicplaylist.h"

KNMusicPlaylist::KNMusicPlaylist(QWidget *parent) :
    KNMusicPlaylistBase(parent),
    m_tab(new KNCategoryTab(this)),
    m_container(new KNEmptyStateWidget(this))
{
    //Generate the empty hint widget.
    KNMusicPlaylistEmptyHint *emptyHint=new KNMusicPlaylistEmptyHint(this);
    //Set the empty widget and content widget.
    m_container->setEmptyWidget(emptyHint);

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
    //Resize the shadow.
}

void KNMusicPlaylist::retranslate()
{
    m_tab->setText(tr("Playlist"));
}
