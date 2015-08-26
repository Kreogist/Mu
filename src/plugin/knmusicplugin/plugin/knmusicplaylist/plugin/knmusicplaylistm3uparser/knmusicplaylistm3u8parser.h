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

#ifndef KNMUSICPLAYLISTM3U8PARSER_H
#define KNMUSICPLAYLISTM3U8PARSER_H

#include "knmusicplaylistm3uparser.h"

/*!
 * \brief The KNMusicPlaylistM3u8Parser class is a parser for M3U8 format
 * playlist. Actually it's the same as m3u. But only force use UTF-8 to read the
 * file.
 */
class KNMusicPlaylistM3u8Parser : public KNMusicPlaylistM3uParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistM3u8Parser object.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistM3u8Parser(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicPlaylistXspfParser::description().
     */
    QString description() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistXspfParser::suffix().
     */
    QString suffix() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistM3uParser::read().
     */
    KNMusicPlaylistModel *read(const QString &filePath) Q_DECL_OVERRIDE;
};

#endif // KNMUSICPLAYLISTM3U8PARSER_H
