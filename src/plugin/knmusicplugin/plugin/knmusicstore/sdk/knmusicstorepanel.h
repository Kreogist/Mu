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
#ifndef KNMUSICSTOREPANEL_H
#define KNMUSICSTOREPANEL_H

#include <QVariant>

#include "knmousesensewidget.h"

class QLabel;
class KNAnimeLabelButton;
class KNMusicStoreBackend;
/*!
 * \brief The KNMusicStorePanel class provides the basic ports of the music
 * store panel. It also provides some widget that music store panel needs to
 * provide.
 */
class KNMusicStorePanel : public KNMouseSenseWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStorePanel widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStorePanel(QWidget *parent = 0);

    /*!
     * \brief Get the header label.
     * \return The header label pointer.
     */
    KNAnimeLabelButton *headerLabel();

    /*!
     * \brief Provides the bullet widget of this panel.
     * \return The bullet widget.
     */
    QLabel *bulletLabel();

signals:
    /*!
     * \brief When the network activity is started, this signal will be emitted.
     */
    void startNetworkActivity();

    /*!
     * \brief Ask to show the album with the given album Id.
     * \param albumId The album index.
     */
    void requireShowAlbum(QVariant albumId);

    /*!
     * \brief Ask to show the song detail info with the given song Id.
     * \param songId The song index.
     */
    void requireShowSong(QVariant songId);

    /*!
     * \brief Ask to search the keyword with specific keyword.
     * \param keyword The search keyword text.
     */
    void requireSearch(QString keyword);

public slots:
    /*!
     * \brief Set backend to home widget.
     * \param backend The music store backend.
     */
    virtual void setBackend(KNMusicStoreBackend *backend)=0;

private:
    QLabel *m_bulletWidget;
    KNAnimeLabelButton *m_headerLabel;
};

#endif // KNMUSICSTOREPANEL_H
