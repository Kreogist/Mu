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

#ifndef KNMUSICPLAYLISTMANAGER_H
#define KNMUSICPLAYLISTMANAGER_H

#include <QObject>

class QThread;
class KNMusicPlaylistModel;
class KNMusicPlaylistListModel;
class KNMusicPlaylistManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistManager(QObject *parent = 0);
    ~KNMusicPlaylistManager();

    KNMusicPlaylistListModel *playlistList();
    KNMusicPlaylistModel *playlist(const QModelIndex &index);

    QString playlistDirPath() const;
    void setPlaylistDirPath(const QString &playlistDirPath);
    void loadPlaylistList();
    bool loadPlaylist(const QString &filePath);

    bool isPlaylistListLoaded() const;
    void setPlaylistListLoaded(bool isPlaylistListLoaded);
    QModelIndex createPlaylist();

signals:
    void requireShowContent();
    void requireHideContent();

public slots:

private:
    QString generateTitle(const QString &preferName = QString());
    KNMusicPlaylistListModel *m_playlistList;
    QString m_playlistDirPath;
    bool m_isPlaylistListLoaded;
    QThread *m_workingThread;
};

#endif // KNMUSICPLAYLISTMANAGER_H
