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
#ifndef KNMUSICPLAYLISTMANAGER_H
#define KNMUSICPLAYLISTMANAGER_H

#include <QIcon>

#include "knmusicplaylistmanagerbase.h"

class KNMusicPlaylistLoader;
class KNMusicPlaylistTab;
class KNMusicPlaylistList;
class KNMusicPlaylistListItem;
class KNMusicPlaylistManager : public KNMusicPlaylistManagerBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistManager(QObject *parent = 0);
    ~KNMusicPlaylistManager();
    KNMusicTab *categoryTab();

signals:

public slots:

private slots:
    void onActionLoadPlaylistList();
    void onActionAddPlaylist(const QString &caption);
    void onActionAddToPlaylist(const int &row,
                               const QStringList &filePaths);
    void onActionAddRowToPlaylist(const int &row);
    void onActionRemovePlaylist(const QModelIndex &index);
    void onActionCreatePlaylist(const int &row,
                                const QStringList &filePaths);
    void onActionCreateRowPlaylist(const int &row);
    void onActionCurrentPlaylistChanged(const QModelIndex &current,
                                        const QModelIndex &previous);

private:
    void initialPlaylistLoader();
    void saveChangedPlaylist();
    bool importPlaylistFromFile(const QString &filePath);
    KNMusicPlaylistLoader *m_loader;
    KNMusicPlaylistTab *m_playlistTab;
    KNMusicPlaylistList *m_playlistList;
    KNMusicNowPlayingBase *m_nowPlaying;
    QString m_playlistDatabasePath;
};

#endif // KNMUSICPLAYLISTMANAGER_H
