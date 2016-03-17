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

#include <QtConcurrent/QtConcurrent>
#include <QJsonObject>

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

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::hotSongModel().
     */
    KNMusicStoreAlbumListModel *hotSongModel() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::listCount().
     */
    int listCount() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::listName().
     */
    QString listName(int listIndex) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::listModel().
     */
    KNMusicStoreAlbumListModel *listModel(int listIndex)  Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchHomeWidgetInfo().
     */
    void fetchHomeWidgetInfo() Q_DECL_OVERRIDE;

private slots:
    void updateLatestAlbumsList();

private:
    inline void updateNeteaseList(KNMusicStoreAlbumListModel *model,
                                  const QString &listUrl);
    inline QNetworkRequest generateNeteaseRequest(const QString &url);
    inline QJsonObject getSongDetails(const QString &songId);
    inline QPixmap generateAlbumArt(const QByteArray &albumArtData);
    enum NeteaseLists
    {
        BillboardList,
        iTunesList,
        OriconList,
        NeteaseListCount
    };

    QFuture<void> m_listThreads[NeteaseListCount+2];
    KNMusicStoreAlbumListModel *m_listModel[NeteaseListCount];
    KNMusicStoreAlbumListModel *m_newAlbumModel, *m_hotSongModel;
};

#endif // KNMUSICSTORENETEASE_H