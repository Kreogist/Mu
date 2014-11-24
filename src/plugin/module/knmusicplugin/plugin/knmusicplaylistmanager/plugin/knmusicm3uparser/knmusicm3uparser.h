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
#ifndef KNMUSICM3UPARSER_H
#define KNMUSICM3UPARSER_H

#include "../../sdk/knmusicplaylistparser.h"

class KNMusicM3UParser : public KNMusicPlaylistParser
{
    Q_OBJECT
public:
    explicit KNMusicM3UParser(QObject *parent = 0);
    QString playlistType() const;
    QString playlistSuffix() const;
    bool parse(const QString &playlistFilePath,
               KNMusicPlaylistListItem *playlistItem);

signals:

public slots:

};

#endif // KNMUSICM3UPARSER_H
