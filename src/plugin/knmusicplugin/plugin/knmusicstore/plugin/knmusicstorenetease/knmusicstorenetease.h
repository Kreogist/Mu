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
#ifndef KNMUSICSTORENETEASE_H
#define KNMUSICSTORENETEASE_H

#include "../../sdk/knmusicstorebackend.h"

/*!
 * \brief The KNMusicStoreNetease class provides the netease plugin for download
 * music information from netease cloud music store.\n
 * This plugin might be abandon for any copyright reason.
 */
class KNMusicStoreNetease : public KNMusicStoreBackend
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreNetease plugin.
     * \param parent The parent object.
     */
    explicit KNMusicStoreNetease(QObject *parent = 0);
    ~KNMusicStoreNetease();

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::newAlbumModel().
     */
    KNMusicStoreAlbumListModel *newAlbumModel() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchHomeWidgetInfo().
     */
    void fetchHomeWidgetInfo() Q_DECL_OVERRIDE;

private:
    inline QNetworkRequest generateNeteaseRequest(const QString &url);
    KNMusicStoreAlbumListModel *m_newAlbumModel;
};

#endif // KNMUSICSTORENETEASE_H
