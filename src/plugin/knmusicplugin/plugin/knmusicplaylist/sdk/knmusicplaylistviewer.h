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

#ifndef KNMUSICPLAYLISTVIEWER_H
#define KNMUSICPLAYLISTVIEWER_H

#include <QWidget>

class QLabel;
class KNConnectionHandler;
class KNSideShadowWidget;
class KNMusicPlaylistModel;
class KNMusicPlaylistTreeView;
class KNMusicPlaylistViewer : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistViewer(QWidget *parent = 0);
    ~KNMusicPlaylistViewer();
    void setPlaylist(KNMusicPlaylistModel *model);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void retranslate();
    void onActionModelRowCountChanged();
    void onActionSearch();

private:
    inline void updateDetailInfo();
    KNMusicPlaylistTreeView *m_treeView;
    QLabel *m_title, *m_detail;
    KNSideShadowWidget *m_leftShadow;
    KNConnectionHandler *m_modelLinkHandler;

    QString m_songCount[3],
            m_minuateCount[3],
            m_hourCount[2],
            m_hourCountWithoutMinuate[2],
            m_searchCount[3];
    QString m_searchResultIn;
};

#endif // KNMUSICPLAYLISTVIEWER_H
