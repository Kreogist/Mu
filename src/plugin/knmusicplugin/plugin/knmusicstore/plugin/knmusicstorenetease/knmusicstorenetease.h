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
#include <QNetworkRequest>

#include "../../sdk/knmusicstoreutil.h"

#include "../../sdk/knmusicstorebackend.h"

class KNRestApiBase;
class KNMusicStoreAlbumModel;
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

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::albumDetailModel().
     */
    KNMusicStoreAlbumModel *albumDetailModel() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchHomeWidgetInfo().
     */
    void fetchHomeWidgetInfo() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchAlbumDetail().
     */
    void fetchAlbumDetail(const QString &albumId) Q_DECL_OVERRIDE;

private slots:
    void updateLatestAlbumsList();
    void updateNeteaseList(QPointer<KNMusicStoreAlbumListModel> model,
                           const QString &listNo);

private:
    inline QNetworkRequest generateNeteaseRequest(const QString &url);
    inline QJsonObject getSongDetails(KNRestApiBase *curl,
                                      const QString &songId);
    inline QPixmap generateAlbumArt(const QByteArray &albumArtData,
                                    int width=StoreAlbumSize,
                                    int height=StoreAlbumSize);
    inline QString timeToText(const double &time);
    inline QString encryptedId(const QString &songId);
    enum NeteaseLists
    {
        BillboardList,
        iTunesList,
        OriconList,
        NeteaseListCount
    };
    enum NeteaseWorkThreads
    {
        LatestAlbumList=NeteaseListCount,
        HotSongList,
        NeteaseWorkThreadCount
    };

    QFuture<void> m_listThreads[NeteaseListCount+2];
    QByteArray m_magicData;
    int m_magicDataLength;
    KNMusicStoreAlbumListModel *m_listModel[NeteaseListCount];
    KNMusicStoreAlbumListModel *m_newAlbumModel, *m_hotSongModel;
    KNMusicStoreAlbumModel *m_albumDetail;
};

#endif // KNMUSICSTORENETEASE_H
