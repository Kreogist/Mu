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
#ifndef KNMUSICSTOREBACKEND_H
#define KNMUSICSTOREBACKEND_H

#include "knrestapibase.h"

class KNMusicStoreAlbumListModel;
/*!
 * \brief The KNMusicStoreBackend class provides the basic framework for a music
 * store plugin. It could get the data from a music store and update the data
 * into Kreogist format.
 */
class KNMusicStoreBackend : public KNRestApiBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a NMusicStoreBackend object.
     * \param parent The parent object.
     */
    KNMusicStoreBackend(QObject *parent = 0) : KNRestApiBase(parent){}

    /*!
     * \brief Provides the new album model.
     * \return The new album model.
     */
    virtual KNMusicStoreAlbumListModel *newAlbumModel()=0;

    /*!
     * \brief Provides the hotest music model.
     * \return The hotest music model.
     */
    virtual KNMusicStoreAlbumListModel *hotSongModel()=0;

signals:

public slots:
    /*!
     * \brief Get the home page information to local.
     */
    virtual void fetchHomeWidgetInfo()=0;
};

#endif // KNMUSICSTOREBACKEND_H
