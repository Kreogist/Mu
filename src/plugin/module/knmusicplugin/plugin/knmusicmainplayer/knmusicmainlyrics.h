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
#ifndef KNMUSICMAINLYRICS_H
#define KNMUSICMAINLYRICS_H

#include "knmusicscrolllyrics.h"

class QLabel;
class QTimeLine;
class KNPreferenceItemGlobal;
class KNMusicGlobal;
class KNMusicLyricsManager;
class KNMusicMainLyrics : public KNMusicScrollLyrics
{
    Q_OBJECT
public:
    explicit KNMusicMainLyrics(QWidget *parent = 0);
    ~KNMusicMainLyrics();
    void setHeaderPlayer(KNMusicHeaderPlayerBase *player);

signals:

public slots:
    void onActionLyricsUpdate();

private:
    KNMusicLyricsManager *m_lyricsManager;
    KNMusicHeaderPlayerBase *m_player;
};

#endif // KNMUSICMAINLYRICS_H
