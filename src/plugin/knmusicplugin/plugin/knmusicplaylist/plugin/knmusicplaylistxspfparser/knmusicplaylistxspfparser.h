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

#ifndef KNMUSICPLAYLISTXSPFPARSER_H
#define KNMUSICPLAYLISTXSPFPARSER_H

#include "../../sdk/knmusicplaylistparser.h"

/*!
 * \brief The KNMusicPlaylistXspfParser class provides a Xspf format playlist
 * parser. It can parse a Xspf format playlist to a playlist model.
 */
class KNMusicPlaylistXspfParser : public KNMusicPlaylistParser
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistXspfParser(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicPlaylistXspfParser::description().
     */
    QString description() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistXspfParser::suffix().
     */
    QString suffix() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistXspfParser::read().
     */
    KNMusicPlaylistModel *read(const QString &filePath) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistXspfParser::write().
     */
    bool write(KNMusicPlaylistModel *playlist,
               const QString &filePath) Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // KNMUSICPLAYLISTXSPFPARSER_H
