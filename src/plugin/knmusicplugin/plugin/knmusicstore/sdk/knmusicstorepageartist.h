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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREPAGEARTIST_H
#define KNMUSICSTOREPAGEARTIST_H

#include <QWidget>

/*!
 * \brief The KNMusicStorePageArtist class provides the page for the artist.\n
 * Originally, it should be treat the same as the playlist. However, it is quite
 * different. The artist widget contains the following information of one artist
 * provided from the backend:\n
 *  1. Hot songs of the artist.
 *  2. Albums of the artist.
 * This essentially change the properties of the widget. So the page needs a
 * tab widget to change the category of these two parts.
 */
class KNMusicStorePageArtist : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicStorePageArtist(QWidget *parent = 0);

signals:

public slots:
};

#endif // KNMUSICSTOREPAGEARTIST_H
