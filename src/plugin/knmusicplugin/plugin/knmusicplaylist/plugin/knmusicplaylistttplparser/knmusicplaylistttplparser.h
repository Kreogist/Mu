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

#ifndef KNMUSICPLAYLISTTTPLPARSER_H
#define KNMUSICPLAYLISTTTPLPARSER_H

#include "../../sdk/knmusicplaylistparser.h"

/*!
 * \brief The KNMusicPlaylistTtplParser class provides you a parser to parse the
 * TTPL(TTplayer Play List) format playlist. TTPlayer is one of the most famous
 * music player in China. But now it is belongs to Baidu. I really miss it and
 * it's still in my computer. I wrote this plugin to pay my respects.
 */
class KNMusicPlaylistTtplParser : public KNMusicPlaylistParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistTtplParser object.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistTtplParser(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicPlaylistParser::description().
     */
    QString description() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistParser::suffix().
     */
    QString suffix() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistParser::read().
     */
    KNMusicPlaylistModel *read(const QString &filePath) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistParser::write().
     */
    bool write(KNMusicPlaylistModel *playlist,
               const QString &filePath) Q_DECL_OVERRIDE;
};

#endif // KNMUSICPLAYLISTTTPLPARSER_H
