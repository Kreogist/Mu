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
#ifndef KNMUSICMAINPLAYER_H
#define KNMUSICMAINPLAYER_H

#include "knmusicmainplayerbase.h"

class QLabel;
class QGridLayout;
class KNMusicMainPlayer : public KNMusicMainPlayerBase
{
public:
    explicit KNMusicMainPlayer(QWidget *parent = 0);
    ~KNMusicMainPlayer();

private slots:
    void retranslate();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    enum InformationElements
    {
        ElementTitle,
        ElementArtist,
        ElementAlbum,
        ElementGenre,
        ElementYear,
        ElementQuality,
        ElementLocation,
        InformationElementsCount
    };

    inline void initialAlbumArt();
    //Album art widgets.
    QLabel *m_albumArt;

    inline void initialInformationPanel();
    //Information layouts.
    QGridLayout *m_informationPanelLayout;
    QLabel *m_informationElementCaptions[InformationElementsCount],
           *m_informationElements[InformationElementsCount];

    inline void initialLyricsPanel();
    //Lyrics panel.

    inline void initialPlaylistPanel();
    inline void initialControlPanel();

};

#endif // KNMUSICMAINPLAYER_H
