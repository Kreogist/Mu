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

#include <QModelIndex>

#include "knmusictab.h"

class QSplitter;
class QStandardItem;
class KNEmptyStateWidget;
class KNMusicPlaylistEmptyHint;
class KNMusicPlaylistModel;
class KNMusicPlaylistDisplay;
class KNMusicPlaylistList;
class KNMusicPlaylistListItem;
class KNMusicPlaylistListView;
class KNMusicPlaylistListViewEditor;
class KNMusicPlaylistLoader;
class KNMusicPlaylistTab : public KNMusicTab
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistTab(QObject *parent = 0);
    ~KNMusicPlaylistTab();
    QString caption();
    QPixmap icon();
    QWidget *widget();
    void showIndexInModel(KNMusicModel *model,
                          const QModelIndex &index);
    KNMusicPlaylistModel *currentPlaylistModel();
    void cutLoadRequirement();
    void setPlaylistLoader(KNMusicPlaylistLoader *playlistLoader);

signals:
    void requireLocateIndexInModel(KNMusicModel *model, QModelIndex index);
    void requireLoadPlaylistList();
    void requireCreateFirstPlaylist(const QStringList &fileList);
    void requireImportPlaylist(const QStringList &playlistList);
    void requireExportPlaylist(const QString &filePath,
                               const QModelIndex &index);
    void requireGeneratePlaylist();
    void requireRemovePlaylist(const QModelIndex &index);
    void requireMakeCopy(int index);
    void currentPlaylistChanged(const QModelIndex &current,
                                const QModelIndex &previous);

public slots:
    void onActionSearch(const QString &text);
    void onActionRemoveCurrent();
    void onActionPlaylistItemChanged(QStandardItem *item);
    void displayPlaylistItem(KNMusicPlaylistListItem *item);
    void setPlaylistList(KNMusicPlaylistList *playlistList);
    void selectSourceRow(const int &row);
    void setCurrentPlaylist(const QModelIndex &index);
    void editPlaylistName(const QModelIndex &index);

private slots:
    void onActionCopyCurrent();

private:
    inline void initialPlaylistList();
    KNEmptyStateWidget *m_viewer;
    QSplitter *m_mainViewer;
    QWidget *m_playlistListViewer;
    KNMusicPlaylistList *m_playlistList;
    KNMusicPlaylistEmptyHint *m_emptyHint;
    KNMusicPlaylistDisplay *m_playlistDisplay;
    KNMusicPlaylistListView *m_playlistListView;
    KNMusicPlaylistListViewEditor *m_playlistListViewEditor;
};

#endif // KNMUSICPLAYLISTTAB_H
