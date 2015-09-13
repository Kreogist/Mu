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

#ifndef KNMUSICMAINPLAYER_H
#define KNMUSICMAINPLAYER_H

#include "knmusicmainplayerbase.h"

class KNOpacityAnimeButton;
/*!
 * \brief The KNMusicMainPlayer class is a default realize of the main music
 * player. It will use all the standard SDK file to build the main music player.
 * You can treat this as an example of the main player.
 */
class KNMusicMainPlayer : public KNMusicMainPlayerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMainPlayer widget.
     * \param parent The parent widget.
     */
    explicit KNMusicMainPlayer(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicPlayerBase::setBackend().
     */
    void setBackend(KNMusicBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlayerBase::setNowPlaying().
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    KNOpacityAnimeButton *m_hideMainPlayer;
};

#endif // KNMUSICMAINPLAYER_H
