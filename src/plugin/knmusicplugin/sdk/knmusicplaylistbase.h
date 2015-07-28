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

#ifndef KNMUSICPLAYLISTBASE_H
#define KNMUSICPLAYLISTBASE_H

#include "knmusictab.h"

/*!
 * \brief The KNMusicPlaylistBase class is the port class of the playlist
 * modules. It provides all the ports of a playlist tab should contains. This
 * class should be implement and realize these functions.
 */
class KNMusicPlaylistBase : public KNMusicTab
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistBase class with give parent.
     * \param parent The parent widget.
     */
    KNMusicPlaylistBase(QWidget *parent = 0):KNMusicTab(parent){}

signals:

public slots:
};

#endif // KNMUSICPLAYLISTBASE_H
