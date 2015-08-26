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
#include <QDir>

#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicplaylistextm3uparser.h"

KNMusicPlaylistExtM3uParser::KNMusicPlaylistExtM3uParser(QObject *parent) :
    KNMusicPlaylistM3uParser(parent)
{
}

QString KNMusicPlaylistExtM3uParser::description()
{
    return tr("Extended M3U Playlist");
}

bool KNMusicPlaylistExtM3uParser::write(KNMusicPlaylistModel *playlist,
                                        const QString &filePath)
{
    //Generate the content cache.
    QString content;
    //Write the first line of ExtM3U.
    content.append("#EXTM3U\n");
    //Write all the file path to the content.
    for(int i=0; i<playlist->rowCount(); ++i)
    {
        //Add a blank line first.
        content.append("\n");
        //Add EXTINF information first.
        content.append("#EXTINF:"+
                       QString::number(
                           playlist->rowProperty(i, DurationRole).toLongLong()
                           /1000) + ", "+
                           textCheck(playlist->textData(i, Artist)) + " - " +
                           textCheck(playlist->textData(i, Name)) + "\n");
        //Add the file path to the content cache.
        content.append(QDir::toNativeSeparators(
                       playlist->rowProperty(i, FilePathRole).toString())+"\n");
    }
    //Write the content to file.
    return writeContent(filePath, content);
}

inline QString KNMusicPlaylistExtM3uParser::textCheck(const QString &text)
{
    return text.isEmpty()?"(Unknown)":text;
}
