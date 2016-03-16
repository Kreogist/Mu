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

#ifndef KNMUSICSTOREHOMEWIDGET_H
#define KNMUSICSTOREHOMEWIDGET_H

#include <QWidget>

class QLabel;
class KNMusicStoreAlbumList;
class KNMusicStoreBackend;
/*!
 * \brief The KNMusicStoreHomeWidget class is used to show the homepage image
 * and song information from the backend.
 */
class KNMusicStoreHomeWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreHomeWidget widget.
     * \param parent The parent widget pointer.
     */
    explicit KNMusicStoreHomeWidget(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set backend to home widget.
     * \param backend The music store backend.
     */
    void setBackend(KNMusicStoreBackend *backend);

protected:

private slots:
    void retranslate();

private:
    enum HomePageBlocks
    {
        NewMusicBlock,
        HomePageBlockCount
    };
    QLabel *m_blockTitle[HomePageBlockCount];
    KNMusicStoreAlbumList *m_newMusicList;
};

#endif // KNMUSICSTOREHOMEWIDGET_H
