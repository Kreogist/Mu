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

#ifndef KNMUSICSTOREPAGEALBUM_H
#define KNMUSICSTOREPAGEALBUM_H

#include "knmusicstorepage.h"

class QLabel;
class KNScrollLabel;
class KNHighLightLabel;
class KNMusicStoreListView;
/*!
 * \brief The KNMusicStorePageAlbum class provides the user interface for the
 * album data. Album is actually the same as the playlist. However, the user
 * interface is quite different from the playlist, and it is enough for a split
 * module.\n
 * This module contains the following information of one album:
 *   1. The name of the album.
 *   2. The artist of the album.
 */
class KNMusicStorePageAlbum : public KNMusicStorePage
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStorePageAlbum widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStorePageAlbum(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStorePage::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStorePage::setPageLabel().
     */
    void setPageLabel(int labelIndex, const QVariant &value) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStorePage::setBackend().
     */
    void setBackend(KNMusicStoreBackend *backend) Q_DECL_OVERRIDE;

private slots:
    void onAlbumRowCountChanged(int row);
    void onViewIndexClicked(const QModelIndex &index);

private:
    KNConnectionHandler m_backendConnection;
    KNScrollLabel *m_albumTitle;
    QLabel *m_albumArtist;
    KNHighLightLabel *m_albumArt;
    KNMusicStoreListView *m_albumView;
};

#endif // KNMUSICSTOREPAGEALBUM_H
