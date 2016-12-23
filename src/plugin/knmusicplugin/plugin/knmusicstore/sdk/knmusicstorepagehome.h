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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREPAGEHOME_H
#define KNMUSICSTOREPAGEHOME_H

#include "knmusicstoreutil.h"

#include "knmusicstorepage.h"

using namespace MusicStoreUtil;

class QLabel;
class KNMusicStoreHomeListModel;
class KNMusicStoreHomeAlbumView;
class KNMusicStoreHomeSongView;
/*!
 * \brief The KNMusicStorePageHome class provides the default page for the music
 * store. These information include:
 *  1. The banner slides.
 *  2. Some ranking list.
 *  3. Some other links.
 */
class KNMusicStorePageHome : public KNMusicStorePage
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNMusicStorePageHome widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStorePageHome(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStorePage::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStorePage::setPageLabel().
     */
    void setPageLabel(int labelIndex, const QVariant &value) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStorePage::setBackend().
     */
    void setBackend(KNMusicStoreBackend *backend) Q_DECL_OVERRIDE;

private slots:
    void retranslate();

private:
    KNMusicStoreHomeListModel *m_homeListModel[HomeSongListCount];
    QLabel *m_titleLabel[HomeSongListCount];
    KNConnectionHandler m_backendConnection;
    KNMusicStoreHomeAlbumView *m_newAlbumView;
    KNMusicStoreHomeSongView *m_newSongView;
    int m_homeContentCounter;
    bool m_homeCounterClear;
};

#endif // KNMUSICSTOREPAGEHOME_H
