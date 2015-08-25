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
class KNMusicPlaylistParser;
class KNMusicPlaylistModel;
class KNMusicPlaylistEngine;
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
    bool savePlaylist(KNMusicPlaylistModel *model, QString filePath=QString());
    QModelIndex importPlaylist(const QString &filePath);
    /*!
     * \brief Import a dozon of playlists for one time.
     * \param filePaths The file paths.
     * \return If any of the playlist file is import successfully, return the
     * first model index of the playlist. Or else a null model index.
     */
    QModelIndex importPlaylists(const QStringList &filePaths);

    bool exportPlaylist(KNMusicPlaylistModel *model,
                        const QString &filePath,
                        const int &parserIndex);

    bool isPlaylistListLoaded() const;
    void setPlaylistListLoaded(bool isPlaylistListLoaded);
    QModelIndex createPlaylist();

    /*!
     * \brief Add one playlist parser to the playlist parser engine.
     * \param parser The parser object.
     */
    void installPlaylistParser(KNMusicPlaylistParser *parser);

    QStringList playlistFilter();

signals:
    void requireShowContent();
    void requireHideContent();

public slots:

private:
    QString generateTitle(const QString &preferName = QString());
    KNMusicPlaylistListModel *m_playlistList;
    QString m_playlistDirPath;
    bool m_isPlaylistListLoaded;
    KNMusicPlaylistEngine *m_playlistEngine;
    QThread *m_workingThread;
};

#endif // KNMUSICPLAYLISTMANAGER_H
