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

#ifndef KNMUSICSTOREBASE_H
#define KNMUSICSTOREBASE_H

#include "knmusictab.h"

/*!
 * \brief The KNMusicStoreBase class provides all the basic ports for the online
 * music store. The music store should be a information collection. Currently,
 * it will collect all the music from the plugins and provide information to
 * user.\n
 * Basiclly, a music store tab should provide a home widget. All the other
 * widget is not forced required.
 */
class KNMusicStoreBase : public KNMusicTab
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreBase tab widget.
     * \param parent The parent object.
     */
    KNMusicStoreBase(QWidget *parent = 0) : KNMusicTab(parent) {}

signals:

public slots:

};

#endif // KNMUSICSTOREBASE_H
