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
class KNMusicProxyModelPool;
class KNMusicSinglePlaylistModel;
class KNMusicNowPlaying : public KNMusicNowPlayingBase
{
    Q_OBJECT
public:
    explicit KNMusicNowPlaying(QObject *parent = 0);
    void setBackend(KNMusicBackend *backend);
    KNMusicProxyModel *playingModel();

signals:

public slots:
    void playNext();
    void playPrevious();
    void onActionPlayingFinished();
    void changeLoopState();
    void playTemporaryFiles(const QStringList &filePaths);
    void setPlayingModel(KNMusicProxyModel *model);
    void playMusic(const int &row);
    void playMusic(const QModelIndex &index);
    void checkRemovedModel(KNMusicModel *model);
    void checkRemovedIndex(const QModelIndex &index);

private:
    void resetPlayingItem();
    void resetPlayingModels();
    KNMusicBackend *m_backend=nullptr;
    KNMusicSinglePlaylistModel *m_temporaryModel;
    KNMusicModel *m_playingMusicModel=nullptr;
    KNMusicProxyModel *m_playingModel=nullptr, *m_temporaryProxyModel;
    KNMusicProxyModelPool *m_proxyModelPool;
    QPersistentModelIndex m_currentPlayingIndex;
    QPixmap m_playingIcon;
    int m_loopMode=NoRepeat;
};

#endif // KNMUSICNOWPLAYING_H
