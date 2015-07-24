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

#ifndef KNMUSICPLAYLIST_H
#define KNMUSICPLAYLIST_H

#include "knmusicplaylistbase.h"

class QSplitter;
class KNCategoryTab;
class KNEmptyStateWidget;
class KNMusicPlaylistList;
class KNMusicPlaylistViewer;
class KNMusicPlaylistManager;
class KNMusicPlaylist : public KNMusicPlaylistBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylist(QWidget *parent = 0);
    QAbstractButton *tab();

signals:

public slots:

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();

private:
    inline QSplitter *generateSplitter();
    KNCategoryTab *m_tab;
    KNEmptyStateWidget *m_container;
    KNMusicPlaylistList *m_playlistList;
    KNMusicPlaylistViewer *m_playlistViewer;
    KNMusicPlaylistManager *m_playlistManager;
};

#endif // KNMUSICPLAYLIST_H
