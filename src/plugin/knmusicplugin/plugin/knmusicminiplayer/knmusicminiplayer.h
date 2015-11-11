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

#ifndef KNMUSICMINIPLAYER_H
#define KNMUSICMINIPLAYER_H

#include "knmusicminiplayerbase.h"

class KNOpacityAnimeButton;
class KNMusicMiniPlayer : public KNMusicMiniPlayerBase
{
    Q_OBJECT
public:
    explicit KNMusicMiniPlayer(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::setBackend().
     */
    void setBackend(KNMusicBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::setNowPlaying().
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    KNOpacityAnimeButton *m_close;
};

#endif // KNMUSICMINIPLAYER_H
