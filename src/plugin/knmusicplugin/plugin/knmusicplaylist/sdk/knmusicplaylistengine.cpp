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
#include <QFileInfo>

#include "knmusicplaylistparser.h"

#include "knmusicplaylistengine.h"

KNMusicPlaylistEngine::KNMusicPlaylistEngine(QObject *parent) :
    QObject(parent)
{
}

void KNMusicPlaylistEngine::installPlaylistParser(KNMusicPlaylistParser *parser)
{
    //Change the working thread of the parser.
    parser->moveToThread(thread());
    //Change the parent relationship.
    parser->setParent(this);
    //Add the parser to the parser list.
    m_parsers.append(parser);
}

KNMusicPlaylistModel *KNMusicPlaylistEngine::read(const QString &filePath)
{
    //Initial a empty playlist model pointer.
    KNMusicPlaylistModel *playlistModel=nullptr;
    //Get the suffix of the file path.
    QString suffix="*."+QFileInfo(filePath).suffix().toLower();
    //Try to find the suffix whether the parser support to parse the suffix.
    for(auto i=m_parsers.begin(); i!=m_parsers.end(); ++i)
    {
        //Check whether the suffix is supported by the parser.
        if((*i)->suffix()==suffix)
        {
            //Use the current parser to parse the file.
            playlistModel=(*i)->read(filePath);
            //If the parser can parse this file, that's it.
            if(playlistModel)
            {
                //Give back the playlist model.
                return playlistModel;
            }
            //Or else, get out and tried all the parser.
            break;
        }
    }

    //Try to parse the file using all parsers in the parser list.
    for(auto i=m_parsers.begin(); i!=m_parsers.end(); ++i)
    {
        //Use the current parser to parse the file.
        playlistModel=(*i)->read(filePath);
        //If there's any one parser can parse this, that's it.
        if(playlistModel)
        {
            //Give back the playlist model.
            return playlistModel;
        }
    }
    //If none of the parsers could parse the file, return a nullptr.
    return nullptr;
}

bool KNMusicPlaylistEngine::write(KNMusicPlaylistModel *playlist,
                                  const QString &filePath,
                                  const QString &suffix)
{
    //Try to find the suffix in the parsers.
    for(auto i=m_parsers.begin(); i!=m_parsers.end(); ++i)
    {
        //Match the suffix in the playlist model.
        if((*i)->suffix().contains(suffix))
        {
            //Use the parser to write the playlist.
            return (*i)->write(playlist, filePath);
        }
    }
    //Or else, failed to write the playlist.
    return false;
}

bool KNMusicPlaylistEngine::write(KNMusicPlaylistModel *playlist,
                                  const QString &filePath,
                                  const int &parserIndex)
{
    //Use the parser to write the playlist to the file.
    return m_parsers.at(parserIndex)->write(playlist, filePath);
}

void KNMusicPlaylistEngine::getTypeAndSuffix(QStringList &types,
                                             QStringList &suffixs)
{
    //Clear the types and the suffixs data.
    types=QStringList();
    suffixs=QStringList();
    //Add data to the list.
    for(auto i=m_parsers.constBegin(); i!=m_parsers.constEnd(); ++i)
    {
        //Add types(description) data.
        types.append((*i)->description());
        //Add suffix data.
        suffixs.append((*i)->suffix());
    }
}

