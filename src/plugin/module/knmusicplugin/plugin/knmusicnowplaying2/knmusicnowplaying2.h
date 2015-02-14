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
#ifndef KNMUSICNOWPLAYING2_H
#define KNMUSICNOWPLAYING2_H

#include "knmusicnowplayingbase.h"

class KNMusicNowPlaying2 : public KNMusicNowPlayingBase
{
    Q_OBJECT
public:
    explicit KNMusicNowPlaying2(QObject *parent = 0);
    ~KNMusicNowPlaying2();

    //Set the backend for control.
    void setBackend(KNMusicBackend *backend);

    //Current playing information.
    KNMusicProxyModel *playingModel();
    KNMusicModel *playingMusicModel();
    QPersistentModelIndex currentPlayingIndex() const;
    KNMusicAnalysisItem currentAnalaysisItem() const;

    //Loop state.
    int loopState();

signals:
    void requirePlayRow(int row);

public slots:
    //Backup and restore current playing, for rename current playing.
    void backupCurrentPlaying();
    void restoreCurrentPlaying();

    //Clear current playing.
    void resetCurrentPlaying();

    //Restore configure.
    void restoreConfigure();

    //Locate the current index.
    void showCurrentIndexInOriginalTab();

    //The original proxy model may be used to display other data,
    //and we need to backup that model to a shadow model.
    void shadowPlayingModel();

    //Play a row in a proxy model.
    void playMusicRow(KNMusicProxyModel *model,
                      int row,
                      KNMusicTab *tab=nullptr);
    //Temporarily play several files.
    void playTemporaryFiles(const QStringList &filePaths);

    //Playlist controls.
    void playNext();
    void playPrevious();
    void changeLoopState();
    void setLoopState(const int &state);

    //Current song control.
    void setCurrentSongRating(const int &rating);

    //When a model is going to be delete, check if the model is the playing
    //model.
    void checkRemovedModel(KNMusicModel *model);
    //Finished playing controls.
    void onActionPlayingFinished();
    void onActionCantLoad();
    void onActionLoaded();

private slots:
    void retranslate();
    void applyPreference();
    //Play the specific row in the model.
    void playRow(const int &proxyRow);

private:
    //Common functions.
    inline void initialTemporaryModel();
    inline void initialShadowModel();
    inline void clearNowPlayingIcon();
    inline void clearShadowModel();

    inline int nextRow(int currentProxyRow, bool ignoreLoopMode=false);
    inline int prevRow(int currentProxyRow, bool ignoreLoopMode=false);

    //Infrastructure
    KNMusicBackend *m_backend=nullptr;
    QPixmap m_playingIcon, m_cantPlayIcon;
    KNConfigure *m_cacheConfigure, *m_musicConfigure;
    int m_loopMode=NoRepeat;
    qint64 m_backupPosition=-1;

    //Models.
    KNMusicProxyModel *m_playingModel=nullptr;
    KNMusicModel *m_playingMusicModel=nullptr;
    KNMusicProxyModel *m_shadowPlayingModel=nullptr,
                      *m_temporaryModel=nullptr;
    KNMusicSinglePlaylistModel *m_temporaryMusicModel;

    //Current playing items.
    QPersistentModelIndex m_currentPlayingIndex;
    KNMusicAnalysisItem m_currentPlayingAnalysisItem;
    KNMusicTab *m_currentTab=nullptr;

    //Flags.
    bool m_manualPlayed=false;
};

#endif // KNMUSICNOWPLAYING2_H
