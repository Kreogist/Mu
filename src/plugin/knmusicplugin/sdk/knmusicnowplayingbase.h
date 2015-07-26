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

signals:
    void nowPlayingReset();
    void nowPlayingChanged(KNMusicAnalysisItem analysisItem);
    void loopStateChanged(int state);

public slots:
    virtual void reset()=0;

    virtual void loadConfigure()=0;
    virtual void saveConfigure()=0;

    virtual void shadowPlayingModel()=0;

    virtual void playMusicRow(KNMusicProxyModel *model,
                              int row,
                              KNMusicTab *tab)=0;

    virtual void playNext()=0;
    virtual void playPrevious()=0;

    virtual void playTemporaryFiles(const QStringList &filePaths)=0;

    virtual void changeLoopState()=0;
    virtual void setLoopState(const int &state)=0;

    virtual void setPlayingRating(int rating)=0;

    virtual void onActionModelRemoved(KNMusicModel *model)=0;
};

#endif // KNMUSICNOWPLAYINGBASE_H
