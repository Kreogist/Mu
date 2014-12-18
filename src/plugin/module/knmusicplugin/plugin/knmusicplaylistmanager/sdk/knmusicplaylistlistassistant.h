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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef KNMUSICPLAYLISTLISTASSISTANT_H
#define KNMUSICPLAYLISTLISTASSISTANT_H

#include <QIcon>
#include <QStringList>

#include <QObject>

class KNMusicPlaylistListItem;
class KNMusicPlaylistListAssistant : public QObject
{
    Q_OBJECT
public:
    static KNMusicPlaylistListItem *generateBlankPlaylist(const QString &caption);
    static KNMusicPlaylistListItem *generatePlaylist(const QString &caption=QString());
    static bool parsePlaylistFile(const QString &playlistFilePath,
                                  QString &title,
                                  QJsonArray &playlistData);
    static QIcon playlistIcon();
    static void setPlaylistIcon(const QIcon &playlistIcon);
    static QString playlistFolderPath();
    static void setPlaylistFolderPath(const QString &playlistFolderPath);
    static void loadPlaylistDatabase(const QString &filePath,
                                     QStringList &data);
    static bool readPlaylist(const QString &filePath,
                             KNMusicPlaylistListItem *item);
    static void buildPlaylist(KNMusicPlaylistListItem *item);
    static bool writePlaylist(KNMusicPlaylistListItem *item);
    static void savePlaylistDatabase(const QString &filePath,
                                     const QStringList &data);
    static QString alloctPlaylistFilePath();

signals:

public slots:

private:
    explicit KNMusicPlaylistListAssistant(QObject *parent = 0);
    static QIcon m_playlistIcon;
    static QString m_playlistFolderPath;
    static int m_version;
};

#endif // KNMUSICPLAYLISTLISTASSISTANT_H
