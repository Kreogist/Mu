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

#ifndef KNMUSICALBUMLISTVIEW_H
#define KNMUSICALBUMLISTVIEW_H

#include "knmusiclistviewbase.h"

class KNMusicTab;
class KNMusicModel;
class KNMusicProxyModel;
/*!
 * \brief The KNMusicAlbumListView class is a special music list view for album
 * views. It will support set category text to show a special album data.\n
 * It will force to select the album.
 */
class KNMusicAlbumListView : public KNMusicListViewBase
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicAlbumListView widget.
     * \param parent The parent widget.
     * \param tab The music tab widget of this widget should belongs to.
     */
    explicit KNMusicAlbumListView(QWidget *parent = 0,
                                  KNMusicTab *tab = 0);

signals:

public slots:
    /*!
     * \brief Set the album title to filter all the song.
     * \param text The album title.
     */
    void setCategoryText(const QVariant &text);

};

#endif // KNMUSICALBUMLISTVIEW_H
