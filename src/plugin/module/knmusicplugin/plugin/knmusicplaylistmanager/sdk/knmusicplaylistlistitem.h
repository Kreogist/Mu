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
#ifndef KNMUSICPLAYLISTLISTITEM_H
#define KNMUSICPLAYLISTLISTITEM_H

#include <QStandardItem>

class KNMusicPlaylistModel;
class KNMusicPlaylistListItem : public QStandardItem
{
public:
    explicit KNMusicPlaylistListItem();
    ~KNMusicPlaylistListItem();
    KNMusicPlaylistModel *playlistModel();
    QString playlistFilePath() const;
    void setPlaylistFilePath(const QString &playlistFilePath);
    bool changed() const;
    void setChanged(bool changed);

private:
    KNMusicPlaylistModel *m_playlistModel=nullptr;
    QString m_playlistFilePath;
    bool m_changed=false;
};

#endif // KNMUSICPLAYLISTLISTITEM_H
