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

#ifndef KNMUSICSTORENETEASEBACKEND_H
#define KNMUSICSTORENETEASEBACKEND_H

#include <QNetworkAccessManager>
#include <QScopedPointer>
#include <QMap>

#include "knconnectionhandler.h"

#include "../../sdk/knmusicstorebackend.h"

/*!
 * \brief The KNMusicStoreNeteaseBackend class provides the music store backend
 * plugin for Netease cloud music. The API is not provided by the offcial
 * documentation. Those APIs come from the following projects:
 *   1. wu-nerd/dmusic-plugin-NeteaseCloudMusic
 */
class KNMusicStoreNeteaseBackend : public KNMusicStoreBackend
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreNeteaseBackend object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreNeteaseBackend(QObject *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplmented from KNMusicStoreBackend::showAlbum().
     */
    void showAlbum(const QString &albumInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplmented from KNMusicStoreBackend::showSingleSong().
     */
    void showSingleSong(const QString &songInfo) Q_DECL_OVERRIDE;

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onAlbumDetailReply(QNetworkReply *reply);

private:
    enum NeteaseReplyTypes
    {
        NeteaseAlbumDetails,
        NeteaseAlbumArt,
        NeteaseSingleLyricsText
    };
    inline QByteArray getRawData(QNetworkReply *reply);
    inline QNetworkRequest generateRequest();
    inline void resetManager();
    KNConnectionHandler m_accessManagerHandler;
    QScopedPointer<QNetworkAccessManager> m_accessManager;
    QMap<QNetworkReply *, int> m_replyMap;
};

#endif // KNMUSICSTORENETEASEBACKEND_H
