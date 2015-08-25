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

#ifndef KNMUSICPLAYLISTPARSER_H
#define KNMUSICPLAYLISTPARSER_H

#include <QObject>
#include <QFile>

class KNMusicPlaylistModel;
/*!
 * \brief The KNMusicPlaylistParser class is the port class of the playlist
 * parser. A playlist parser can parse and write a type of the playlist file by
 * calling the read() and write() function.
 */
class KNMusicPlaylistParser : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistParser class.
     * \param parent The parent widget.
     */
    KNMusicPlaylistParser(QObject *parent = 0):QObject(parent){}

    /*!
     * \brief Get the description of the playlist type.
     * \return The playlist type description string.
     */
    virtual QString description()=0;

    /*!
     * \brief The playlist suffix. This will be used in the dialog filter.
     * \return The playlist suffix text.
     */
    virtual QString suffix()=0;

    /*!
     * \brief Read a playlist file according to the file path, and generate a
     * model. If there's any error occurs, it will return a nullptr pointer.
     * \param filePath The file path.
     * \return If the playlist file can be parsed successfully, it will return
     * the pointer of the playlist model. Or else it will return a nullptr.
     */
    virtual KNMusicPlaylistModel *read(const QString &filePath)=0;

    /*!
     * \brief Write a playlist model content to the file path in the model.
     * \param playlist The playlist model.
     * \return If write successfully, it will be true.
     */
    virtual bool write(KNMusicPlaylistModel *playlist,
                       const QString &filePath)=0;

signals:

public slots:

protected:
    inline bool writeContent(const QString &filePath,
                             const QString &content)
    {
        //Get the playlist file.
        QFile playlistFile(filePath);
        //Open it as write only mode, if the file cannot be written, return
        //false.
        if(!playlistFile.open(QIODevice::WriteOnly))
        {
            return false;
        }
        //Write the data, encode the text in UTF-8 codec.
        playlistFile.write(content.toUtf8());
        //Close the file.
        playlistFile.close();
        //Write successfully.
        return true;
    }
};

#endif // KNMUSICPLAYLISTPARSER_H
