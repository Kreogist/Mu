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

#ifndef KNMUSICSTOREBASE_H
#define KNMUSICSTOREBASE_H

#include "knmusictab.h"

/*!
 * \brief The KNMusicStoreBase class provides the basic ports for a music store
 * plugin. This plugin has to provide a widget to display the music information
 * from several music store backend plugin.
 */
class KNMusicStoreBase : public KNMusicTab
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreBase widget.
     * \param parent The parent widget pointer.
     */
    KNMusicStoreBase(QWidget *parent = 0) : KNMusicTab(parent){}

signals:
    /*!
     * \brief Ask backend to fetch home information.
     */
    void requireFetchHome();

public slots:
    /*!
     * \brief Load all music store plugins.
     */
    virtual void loadPlugins()=0;
};

#endif // KNMUSICSTOREBASE_H
