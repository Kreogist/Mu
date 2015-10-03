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

#ifndef KNMUSICNOWPLAYINGLISTVIEW_H
#define KNMUSICNOWPLAYINGLISTVIEW_H

#include <QListView>

class KNMusicModel;
class KNMusicProxyModel;
class KNMusicNowPlayingListView : public QListView
{
    Q_OBJECT
public:
    explicit KNMusicNowPlayingListView(QWidget *parent = 0);

    /*!
     * \brief Get the proxy of the model.
     * \return The proxy music model pointer.
     */
    KNMusicProxyModel *proxyModel();

    /*!
     * \brief Get the music model of the model. It will simply return the
     * managing music model of the proxy model.
     * \return The music model pointer.
     */
    KNMusicModel *musicModel();

signals:

public slots:

private slots:
    void onActionActivate(const QModelIndex &index);

private:
    inline void playIndex(const QModelIndex &index);
};

#endif // KNMUSICNOWPLAYINGLISTVIEW_H
