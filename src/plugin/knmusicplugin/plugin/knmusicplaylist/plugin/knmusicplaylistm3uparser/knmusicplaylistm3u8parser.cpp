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

#include "knmusicplaylistm3u8parser.h"

KNMusicPlaylistM3u8Parser::KNMusicPlaylistM3u8Parser(QObject *parent) :
    KNMusicPlaylistM3uParser(parent)
{
}

QString KNMusicPlaylistM3u8Parser::description()
{
    return tr("UTF-8 M3U Playlist");
}

QString KNMusicPlaylistM3u8Parser::suffix()
{
    return "*.m3u8";
}

KNMusicPlaylistModel *KNMusicPlaylistM3u8Parser::read(const QString &filePath)
{
    return readPlaylist(filePath, "UTF-8");
}

