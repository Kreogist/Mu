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
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "knmusicplaylistloader.h"

KNMusicPlaylistLoader::KNMusicPlaylistLoader(QObject *parent) :
    QObject(parent)
{
}

KNMusicPlaylistLoader::~KNMusicPlaylistLoader()
{
    //Delete all parsers.
    qDeleteAll(m_parsers);
    m_parsers.clear();
}

void KNMusicPlaylistLoader::installPlaylistParser(KNMusicPlaylistParser *parser)
{
    m_parsers.append(parser);
}

bool KNMusicPlaylistLoader::parsePlaylist(const QString &filePath,
                                          QString &playlistTitle,
                                          QStringList &detailList)
{
    //Try to parse the file using all parsers.
    for(auto i=m_parsers.begin();
        i!=m_parsers.end();
        i++)
    {
        //If there's any one parser can parse this, that's it.
        if((*i)->parse(filePath, playlistTitle, detailList))
        {
            return true;
        }
    }
    return false;
}

void KNMusicPlaylistLoader::getPlaylistTypeAndSuffix(QStringList &types,
                                                     QStringList &suffixs)
{
    //Clear the list.
    types.clear();
    suffixs.clear();
    //Set the data.
    for(auto i=m_parsers.begin();
        i!=m_parsers.end();
        i++)
    {
        types.append((*i)->playlistType());
        suffixs.append((*i)->playlistSuffix());
    }
}
