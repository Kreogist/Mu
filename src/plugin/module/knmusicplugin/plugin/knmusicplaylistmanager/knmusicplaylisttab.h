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
#ifndef KNMUSICPLAYLISTTAB_H
#define KNMUSICPLAYLISTTAB_H

#include "knmusictab.h"

class QSplitter;
class KNMusicPlaylistDisplay;
class KNMusicPlaylistList;
class KNMusicPlaylistListItem;
class KNMusicPlaylistListView;
class KNMusicPlaylistListViewEditor;
class KNMusicPlaylistTab : public KNMusicTab
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistTab(QObject *parent = 0);
    ~KNMusicPlaylistTab();
    QString caption();
    QPixmap icon();
    QWidget *widget();

signals:
    void requireGeneratePlaylist(QString caption);
    void currentPlaylistChanged(const QModelIndex &current,
                                const QModelIndex &previous);

public slots:
    void displayPlaylistItem(KNMusicPlaylistListItem *item);
    void setPlaylistList(KNMusicPlaylistList *playlistList);
    void setCurrentPlaylist(const QModelIndex &index);
    void editPlaylistName(const QModelIndex &index);

private slots:
    void onActionAddPlaylist();

private:
    void initialPlaylistList();
    QSplitter *m_viewer;
    QWidget *m_playlistListViewer;
    KNMusicPlaylistDisplay *m_playlistDisplay;
    KNMusicPlaylistListView *m_playlistListView;
    KNMusicPlaylistListViewEditor *m_playlistListViewEditor;
};

#endif // KNMUSICPLAYLISTTAB_H
