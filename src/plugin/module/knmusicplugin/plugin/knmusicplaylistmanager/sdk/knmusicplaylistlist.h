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
#ifndef KNMUSICPLAYLISTLIST_H
#define KNMUSICPLAYLISTLIST_H

#include <QStringList>

#include <QStandardItemModel>

class KNConnectionHandler;
class KNMusicPlaylistModel;
class KNMusicPlaylistListItem;
class KNMusicPlaylistListAssistant;
class KNMusicPlaylistList : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistList(QObject *parent = 0);
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent);
    KNMusicPlaylistModel *playlistModel(const int &row);
    KNMusicPlaylistListItem *playlistItem(const int &row);
    KNMusicPlaylistListItem *playlistItemFromIndex(const QModelIndex &index);
    QStringList playlistListData() const;
    void setPlaylistListData(const QStringList &playlistListData);

signals:
    void requireAddFileToPlaylist(int playlistRowIndex, QStringList fileList);
    void requireAddRowToPlaylist(int playlistRowIndex, QByteArray rowData);
    void requireCreatePlaylist(int preferRow, QStringList fileList);
    void requireCreatePlaylistRow(int preferRow, QByteArray rowData);
    void requireShowContent();
    void requireHideContent();

public slots:
    void appendPlaylist(QStandardItem *playlistItem);
    void insertPlaylist(const int &row, QStandardItem *playlistItem);
    void removePlaylist(const int &playlistItemRow);

private slots:
    void onActionItemChanged(QStandardItem *item);
    void onActionRowInserted(const QModelIndex &parent, int first, int last);
    void onActionRowRemoved(const QModelIndex &parent, int first, int last);

private:
    KNMusicPlaylistListAssistant *m_assistant;
    KNConnectionHandler *m_rowChangeHandle;
    QStringList m_playlistListData;
};

#endif // KNMUSICPLAYLISTLIST_H
