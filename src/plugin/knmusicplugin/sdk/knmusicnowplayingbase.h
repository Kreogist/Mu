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

#ifndef KNMUSICNOWPLAYINGBASE_H
#define KNMUSICNOWPLAYINGBASE_H

#include <QPersistentModelIndex>

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

class KNMusicBackend;
class KNMusicProxyModel;
class KNMusicModel;
class KNMusicTab;
/*!
 * \brief The KNMusicNowPlayingBase class is a port class for music proxy model
 * managing. It is used to control the current playing index of the specific
 * proxy model. And send the information to backend to play the file.
 */
class KNMusicNowPlayingBase : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicNowPlayingBase object with a given parent.
     * \param parent The parent object.
     */
    KNMusicNowPlayingBase(QObject *parent = 0):QObject(parent){}

    /*!
     * \brief Set the music playing backend.
     * \param backend The backend class.
     */
    virtual void setBackend(KNMusicBackend *backend)=0;

    /*!
     * \brief The current playing music proxy model.
     * \return The music proxy model pointer.
     */
    virtual KNMusicProxyModel *playingModel()=0;

    /*!
     * \brief The current playing music model. Actually it's the music model of
     * the proxy model.
     * \return The music proxy model.
     */
    virtual KNMusicModel *playingMusicModel()=0;

    /*!
     * \brief Get the using loop state of the playing mode.
     * \return The current loop state.
     */
    virtual int loopState()=0;

    /*!
     * \brief Get the current playing index from the current playing proxy
     * model. It's a persistent model index, so it will track the index even if
     * the index changed.
     * \return The music index. If there's no music playing, it will be a
     * invalid index.
     */
    virtual QPersistentModelIndex playingIndex()=0;

    /*!
     * \brief Get the analysis item of the current playing music.
     * \return The KNMusicAnalysisItem parsed before playing the music.
     */
    virtual KNMusicAnalysisItem playingItem()=0;

    /*!
     * \brief Get the playing music tab.
     * \return The music tab pointer. If there's no music playing it should be
     * return a nullptr.
     */
    virtual KNMusicTab *playingTab()=0;

signals:
    /*!
     * \brief When the now playing is resetted, this signal will be emitted.
     */
    void nowPlayingReset();

    /*!
     * \brief When the playing model is changed, this signal will be emitted.
     */
    void nowPlayingModelChanged(KNMusicProxyModel *model);

    /*!
     * \brief When the current playing index is changed, this signal will be
     * emitted.
     * \param analysisItem The new analysis item which is generated before
     * playing.
     */
    void nowPlayingChanged(KNMusicAnalysisItem analysisItem);

    /*!
     * \brief When the loop state is changed, this signal will be emitted.
     * \param state The new loop state index.
     */
    void loopStateChanged(int state);

public slots:
    /*!
     * \brief Reset the now playing manager. To clear up the current playing
     * index.
     */
    virtual void reset()=0;

    /*!
     * \brief Load configure from the music configure object.
     */
    virtual void loadConfigure()=0;

    /*!
     * \brief Save the configure to the music configure object.
     */
    virtual void saveConfigure()=0;

    /*!
     * \brief Make a shadow copy of the proxy music model. When especially the
     * playlist tree view changing the music model.
     */
    virtual void shadowPlayingModel()=0;

    /*!
     * \brief Play a music row in the model.
     * \param model The proxy model.
     * \param row The specific row index.
     * \param tab The music tab.
     */
    virtual void playMusicRow(KNMusicProxyModel *model,
                              int row,
                              KNMusicTab *tab)=0;

    /*!
     * \brief Play the next music row according to the loop state.
     */
    virtual void playNext()=0;

    /*!
     * \brief Play the previous music row according to the loop state.
     */
    virtual void playPrevious()=0;

    /*!
     * \brief Play some music files temporarily. This will add those files to a
     * temporary music model and then play it right then.
     * \param filePaths The music file path list.
     */
    virtual void playTemporaryFiles(const QStringList &filePaths)=0;

    /*!
     * \brief Change the loop state to the next state.
     */
    virtual void changeLoopState()=0;

    /*!
     * \brief Set a specific loop state.
     * \param state The loop state.
     */
    virtual void setLoopState(const int &state)=0;

    /*!
     * \brief Set rating for the current playing item.
     * \param rating The rating of the music. Range from 0 to 5.
     */
    virtual void setPlayingRating(int rating)=0;

    /*!
     * \brief When a music model is being removed, called this function to avoid
     * that now playing is using that music model. If it is using, now playing
     * module will reset.
     * \param model The model pointer.
     */
    virtual void onActionModelRemoved(KNMusicModel *model)=0;
};

#endif // KNMUSICNOWPLAYINGBASE_H
