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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICPLAYLISTENGINE_H
#define KNMUSICPLAYLISTENGINE_H

#include <QObject>

class KNMusicPlaylistModel;
class KNMusicPlaylistParser;
/*!
 * \brief The KNMusicPlaylistEngine class provides a simple management module
 * for all the parsers. It can automatically use all the parsers to parse one
 * file and give out the result.
 */
class KNMusicPlaylistEngine : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistEngine object.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistEngine(QObject *parent = 0);

    /*!
     * \brief Add one playlist parser to the playlist parser engine.
     * \param parser The parser object.
     */
    void installPlaylistParser(KNMusicPlaylistParser *parser);

    /*!
     * \brief Tried to read a file and parse it to KNMusicPlaylistModel.
     * \param filePath The file path.
     * \return If any of the parser parse the file sucessfully, return the
     * playlist model. Or else nullptr.
     */
    KNMusicPlaylistModel *read(const QString &filePath);

    /*!
     * \brief Tried to write the playlist using the a specific suffix.
     * \param playlist The playlist model pointer.
     * \param filePath The target file path.
     * \param suffix The suffix of the file. This will determine the parser
     * which will be used to write the file.
     * \return If the parser write the model successfully, it will be true.
     */
    bool write(KNMusicPlaylistModel *playlist,
               const QString &filePath,
               const QString &suffix);

    /*!
     * \brief A overload funciton of write().\n
     * Tried to write the playlist using the specific parser according to the
     * parser index.
     * \param playlist The playlist model pointer.
     * \param filePath The target file path.
     * \param parserIndex The index of the parser.
     * \return If the parser write the model successfully, it will be true.
     */
    bool write(KNMusicPlaylistModel *playlist,
               const QString &filePath,
               const int &parserIndex);

    /*!
     * \brief Get all the type and supported suffix from all the parsers.
     * \param types The types list.
     * \param suffixs The suffix list.
     */
    void getTypeAndSuffix(QStringList &types, QStringList &suffixs);

private:
    QList<KNMusicPlaylistParser *> m_parsers;
};

#endif // KNMUSICPLAYLISTENGINE_H
