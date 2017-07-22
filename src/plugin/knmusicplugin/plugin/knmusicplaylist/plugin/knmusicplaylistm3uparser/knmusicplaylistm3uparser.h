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

#ifndef KNMUSICPLAYLISTM3UPARSER_H
#define KNMUSICPLAYLISTM3UPARSER_H

#include "../../sdk/knmusicplaylistparser.h"

/*!
 * \brief The KNMusicPlaylistM3uParser class provides a m3u playlist parser. M3U
 * is a computer file format that contains multimedia playlists. It has some
 * other branch version: like m3u8, extm3u.
 */
class KNMusicPlaylistM3uParser : public KNMusicPlaylistParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistM3uParser object.
     * \param parent The given parent.
     */
    explicit KNMusicPlaylistM3uParser(QObject *parent = 0);

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

protected:
    /*!
     * \brief This function can parse a m3u format playlist to a playlist model.
     * The codec can be assigned. Or leave blank using the default settings of
     * QTextCodec.
     * \param filePath The file path.
     * \param codec The codec name.
     * \return If the playlist file can be parsed successfully, it will return
     * the pointer of the playlist model. Or else it will return a nullptr.
     */
    KNMusicPlaylistModel *readPlaylist(const QString &filePath,
                                       const char *codec=nullptr);
};

#endif // KNMUSICPLAYLISTM3UPARSER_H
