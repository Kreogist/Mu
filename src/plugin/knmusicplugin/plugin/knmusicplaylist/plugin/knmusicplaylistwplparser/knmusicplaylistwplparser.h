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

#ifndef KNMUSICPLAYLISTWPLPARSER_H
#define KNMUSICPLAYLISTWPLPARSER_H

#include "../../sdk/knmusicplaylistparser.h"

/*!
 * \brief The KNMusicPlaylistWplParser class provides a Wpl format playlist
 * format parser.\n
 * Wpl playlist is the format which Microsoft Windows Media Player uses. It's
 * kinds of useless actually.
 */
class KNMusicPlaylistWplParser : public KNMusicPlaylistParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistWplParser object.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistWplParser(QObject *parent = 0);

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

#endif // KNMUSICPLAYLISTWPLPARSER_H
