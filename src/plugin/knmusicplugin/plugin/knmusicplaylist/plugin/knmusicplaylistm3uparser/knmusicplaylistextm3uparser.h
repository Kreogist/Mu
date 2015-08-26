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

#ifndef KNMUSICPLAYLISTEXTM3UPARSER_H
#define KNMUSICPLAYLISTEXTM3UPARSER_H

#include "knmusicplaylistm3uparser.h"

/*!
 * \brief The KNMusicPlaylistExtM3uParser class provides a parser for extended
 * m3u file.
 */
class KNMusicPlaylistExtM3uParser : public KNMusicPlaylistM3uParser
{
    Q_OBJECT
public:
    /*!
     * \brief Constuct a KNMusicPlaylistExtM3uParser object.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistExtM3uParser(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicPlaylistM3uParser::description().
     */
    QString description() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlaylistM3uParser::write().
     */
    bool write(KNMusicPlaylistModel *playlist,
               const QString &filePath) Q_DECL_OVERRIDE;

private:
    inline QString textCheck(const QString &text);
};

#endif // KNMUSICPLAYLISTEXTM3UPARSER_H
