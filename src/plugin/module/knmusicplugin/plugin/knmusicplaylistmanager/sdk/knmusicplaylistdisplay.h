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
#ifndef KNMUSICPLAYLISTDISPLAY_H
#define KNMUSICPLAYLISTDISPLAY_H

#include <QWidget>

class QLabel;
class KNConnectionHandler;
class KNSideShadowWidget;
class KNOpacityAnimeButton;
class KNMusicTab;
class KNMusicPlaylistListItem;
class KNMusicPlaylistTreeView;
class KNMusicPlaylistDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistDisplay(QWidget *parent = 0);
    KNMusicPlaylistListItem *currentItem();
    void setMusicTab(KNMusicTab *tab);
    void searchText(const QString &text);

signals:
    void requireAddToPlaylist(int playlistRowIndex, QStringList fileList);

public slots:
    void updatePlaylistInfo();
    void onActionRemoveCurrent();
    void displayPlaylistItem(KNMusicPlaylistListItem *item);
    void scrollToSourceSongRow(const int &row);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionRowChanged();
    void onActionAddToPlaylist();
    void updateDetailInfo();
    void updatePlaylistTitle();

private:
    inline void initialLabels();
    inline void initialTreeView();
    inline void initialControls();
    QLabel *m_playlistTitle, *m_playlistDetail;
    KNSideShadowWidget *m_leftShadow;
    KNMusicPlaylistListItem *m_currentItem=nullptr;
    KNMusicPlaylistTreeView *m_playlistTreeView;
    KNOpacityAnimeButton *m_playPlaylist, *m_shufflePlaylist, *m_addToPlaylist;
    KNConnectionHandler *m_modelSignalHandler;
    QString m_songCount[3], m_minuateCount[3], m_searchResultIn,
            m_searchCount[3];
};

#endif // KNMUSICPLAYLISTDISPLAY_H
