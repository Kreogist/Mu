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

#include <QObject>

class KNMusicStoreAlbumModel;
class KNMusicStoreAlbumListModel;
class KNMusicStoreSongDetailInfo;
class KNMusicStoreSongListModel;
class KNMusicStoreSearchModel;
/*!
 * \brief The KNMusicStoreBackend class provides the basic framework for a music
 * store plugin. It could get the data from a music store and update the data
 * into Kreogist format.
 */
class KNMusicStoreBackend : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a NMusicStoreBackend object.
     * \param parent The parent object.
     */
    KNMusicStoreBackend(QObject *parent = 0) : QObject(parent){}

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

    /*!
     * \brief Get how many list of this backend could provide.
     * \return Provided list number.
     */
    virtual int listCount()=0;

    /*!
     * \brief Get the name of the list.
     * \param listIndex List index.
     * \return The list name.
     */
    virtual QString listName(int listIndex)=0;

    /*!
     * \brief Get the list album model.
     * \param listIndex List index.
     * \return The list model.
     */
    virtual KNMusicStoreAlbumListModel *listModel(int listIndex)=0;

    /*!
     * \brief Get the album detail information model.
     * \return The album detail information model.
     */
    virtual KNMusicStoreAlbumModel *albumDetailModel()=0;

    /*!
     * \brief Get the song detail information mode.
     * \return The song detail object information.
     */
    virtual KNMusicStoreSongDetailInfo *getSongDetail()=0;

    /*!
     * \brief Get the search result model pointer.
     * \param index The index of the model.
     * \return The search result model pointer.
     */
    virtual KNMusicStoreSearchModel *searchResultModel(int index)=0;

signals:
    /*!
     * \brief When home information has been fetched complete, this signal will
     * be emitted.
     */
    void homeFetchComplete();

    /*!
     * \brief When the album information has been fetchde complete, this signal
     * will be emitted.
     */
    void albumFetchComplete();

    /*!
     * \brief When the song information has been fetchde complete, this signal
     * will be emitted.
     */
    void songFetchComplete();

public slots:
    /*!
     * \brief Get the home page information to local.
     */
    virtual void fetchHomeInfo()=0;

    /*!
     * \brief Get the album detail information to local.
     * \param albumId The album index.
     */
    virtual void fetchAlbumDetail(const QVariant &albumId)=0;

    /*!
     * \brief Get the song detail information to local.
     * \param songId Song index.
     */
    virtual void fetchSongDetail(const QVariant &songId)=0;

    /*!
     * \brief Get the specific search result information to local.
     * \param keyword The keyword of search request.
     */
    virtual void fetchSearchResult(const QString &keyword)=0;
};

#endif // KNMUSICSTOREBACKEND_H
