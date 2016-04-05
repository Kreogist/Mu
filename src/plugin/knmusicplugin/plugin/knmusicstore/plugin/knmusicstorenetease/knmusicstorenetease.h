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
#include <QFutureWatcher>
#include <QMutex>

#include "../../sdk/knmusicstoreutil.h"

#include "../../sdk/knmusicstorebackend.h"

class KNRestApiBase;
class KNMusicStoreSearchResult;
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

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::getSongDetail().
     */
    KNMusicStoreSongDetailInfo *getSongDetail() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::searchResultModel().
     */
    KNMusicStoreSearchModel *searchResultModel(int index) Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchHomeWidgetInfo().
     */
    void fetchHomeInfo() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchAlbumDetail().
     */
    void fetchAlbumDetail(const QVariant &albumId) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchSongDetail().
     */
    void fetchSongDetail(const QVariant &songId) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreBackend::fetchSearchResult().
     */
    void fetchSearchResult(const QString &keyword) Q_DECL_OVERRIDE;

private:
    struct SearchRequest
    {
        int type;
        int offset;
        QString keyword;
        QList<int> filter;
    };

    enum SearchResultField
    {
        NameField,
        ArtistField,
        AlbumField,
        DurationField,
        SearchResultFieldCount
    };

private slots:
    void updateLatestAlbumsList();
    void updateNeteaseList(QPointer<KNMusicStoreAlbumListModel> model,
                           const QString &listNo);
    void updateSearchResult(QPointer<KNMusicStoreSearchModel> model,
                            const SearchRequest &request);
    void updateAlbumDetail(QPointer<KNMusicStoreAlbumModel> albumDetail,
                           const QString &albumId);
    void updateSongDetail(QPointer<KNMusicStoreSongDetailInfo> songDetail,
                          const QString &songId);
    void onActionWatcherFinished();

private:
    enum NeteaseWorkingState
    {
        StateNull,
        StateHome,
        StateAlbumDetail,
        StateSongDetail,
        StateSearchResult
    };

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
    inline void quitHomeThreads();
    inline void quitWorkingThread(QFuture<void> &workThread);
    inline QNetworkRequest generateNeteaseRequest(const QString &url);
    inline QJsonObject getSongDetails(KNRestApiBase *curl,
                                      const QString &songId);
    inline QPixmap generateAlbumArt(const QByteArray &albumArtData,
                                    int width=StoreAlbumSize,
                                    int height=StoreAlbumSize);
    inline QString encryptedId(const QString &songId);
    inline void setDownloadUrl(const QJsonObject &songItem,
                               QString &urlOnline,
                               QString &urlHigh,
                               QString &urlLossless);
    inline QString getArtistNames(const QJsonObject &songData);
    inline QString getDuration(const QJsonObject &songData);

    QFuture<void> m_listThreads[NeteaseWorkThreadCount],
                  m_searchThreads[KNMusicStoreUtil::StoreSearchCategoryCount],
                  m_albumThread, m_songThread;
    QFutureWatcher<void> m_listThreadWatcher[NeteaseWorkThreadCount],
                         m_albumThreadWatcher, m_songThreadWatcher;
    QByteArray m_magicData;
    QMutex m_homeFeteching;
    int m_magicDataLength, m_homeLocking;
    KNMusicStoreAlbumListModel *m_listModel[NeteaseListCount];
    KNMusicStoreAlbumListModel *m_newAlbumModel, *m_hotSongModel;
    KNMusicStoreAlbumModel *m_albumDetail;
    KNMusicStoreSongDetailInfo *m_songDetail;
    KNMusicStoreSearchResult *m_searchResult;
    int m_launchingInstance;
};

#endif // KNMUSICSTORENETEASE_H
