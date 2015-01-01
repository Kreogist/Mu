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

#include <QPersistentModelIndex>

#include "knmusicnowplayingbase.h"

class KNMusicModel;
class KNMusicProxyModel;
class KNMusicSinglePlaylistModel;
class KNMusicNowPlaying : public KNMusicNowPlayingBase
{
    Q_OBJECT
public:
    explicit KNMusicNowPlaying(QObject *parent = 0);
    ~KNMusicNowPlaying();
    void setBackend(KNMusicBackend *backend);
    KNMusicProxyModel *playingModel();
    KNMusicModel *playingMusicModel();
    int loopState();
    QPersistentModelIndex currentPlayingIndex() const;

signals:

public slots:
    void showCurrentIndexInOriginalTab();
    void shadowPlayingModel();
    void resetCurrentPlaying();
    void restoreConfigure();
    void playNext();
    void playPrevious();
    void onActionPlayingFinished();
    void onActionCannotPlay();
    void setRating(const int &rating);
    void changeLoopState();
    void setLoopState(const int &state);
    void playTemporaryFiles(const QStringList &filePaths);
    void setPlayingModel(KNMusicProxyModel *model,
                         KNMusicTab *tab=nullptr);
    void playMusic(const int &row);
    void playMusic(const QModelIndex &index);
    void checkRemovedModel(KNMusicModel *model);

private:
    void saveConfigure();

    void playNextSong(bool cannotLoadFile=false);
    void resetPlayingItem();
    void resetPlayingModels();
    KNMusicBackend *m_backend=nullptr;
    KNMusicSinglePlaylistModel *m_temporaryModel;
    KNMusicModel *m_playingMusicModel=nullptr;
    KNMusicProxyModel *m_playingModel=nullptr,
                      *m_shadowPlayingModel,
                      *m_temporaryProxyModel;
    QPersistentModelIndex m_currentPlayingIndex;
    QPixmap m_playingIcon, m_cantPlayIcon;
    KNMusicTab *m_currentTab=nullptr;
    int m_loopMode=NoRepeat;
};

#endif // KNMUSICNOWPLAYING_H
