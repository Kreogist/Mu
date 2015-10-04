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

#ifndef KNMUSICNOWPLAYING_H
#define KNMUSICNOWPLAYING_H

#include <random>

#include "knconnectionhandler.h"

#include "knmusicnowplayingbase.h"

class KNMusicTemporaryPlaylistModel;
class KNMusicNowPlaying : public KNMusicNowPlayingBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicNowPlaying object.
     * \param parent The parent object.
     */
    explicit KNMusicNowPlaying(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::setBackend().
     */
    void setBackend(KNMusicBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playingModel().
     */
    KNMusicProxyModel *playingModel() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playingMusicModel().
     */
    KNMusicModel *playingMusicModel() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::loopState().
     */
    int loopState() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playingIndex().
     */
    QPersistentModelIndex playingIndex() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playingItem().
     */
    KNMusicAnalysisItem playingItem() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playingTab().
     */
    KNMusicTab *playingTab() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::loadConfigure().
     */
    void loadConfigure() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::saveConfigure().
     */
    void saveConfigure() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::shadowPlayingModel().
     */
    void shadowPlayingModel() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playMusicRow().
     */
    void playMusicRow(KNMusicProxyModel *model,
                      int row,
                      KNMusicTab *tab) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playNext().
     */
    void playNext() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playPrevious().
     */
    void playPrevious() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::playTemporaryFiles().
     */
    void playTemporaryFiles(const QStringList &filePaths) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::changeLoopState().
     */
    void changeLoopState() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::setLoopState().
     */
    void setLoopState(const int &state) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::setPlayingRating().
     */
    void setPlayingRating(int rating) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicNowPlayingBase::onActionModelRemoved().
     */
    void onActionModelRemoved(KNMusicModel *model) Q_DECL_OVERRIDE;

private slots:
    void onActionBackendFinished();
    void onActionPlayingItemRemoved();
    void onActionLoadSuccess();
    void onActionLoadFailed();
    void playRow(const int &proxyRow);

private:
    inline void resetShadowModel();
    inline void resetCurrentPlaying();
    inline void resetCurrentPlayingModelData();
    inline void playNextRow(bool noLoopMode=false);

    KNMusicBackend *m_backend;
    KNMusicProxyModel *m_playingProxyModel,
                      *m_shadowPlayingModel,
                      *m_temporaryProxyPlaylist;
    KNMusicTemporaryPlaylistModel *m_temporaryPlaylist;
    KNMusicTab *m_playingTab;
    int m_loopState;

    QPersistentModelIndex m_playingIndex;
    KNMusicAnalysisItem m_playingAnalysisItem;

    std::mt19937 m_mersenneSeed;

    KNConnectionHandler m_backendConnections;
    bool m_manualPlayed;
};

#endif // KNMUSICNOWPLAYING_H
