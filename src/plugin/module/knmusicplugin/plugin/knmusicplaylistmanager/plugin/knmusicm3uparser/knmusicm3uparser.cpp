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
#include <QFileInfo>
#include <QTextStream>

#include "knmusicparser.h"
#include "knmusicmodelassist.h"
#include "../../sdk/knmusicplaylistlistitem.h"
#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicm3uparser.h"

KNMusicM3UParser::KNMusicM3UParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicM3UParser::playlistType() const
{
    return tr("M3U Playlist");
}

QString KNMusicM3UParser::playlistSuffix() const
{
    return "*.m3u";
}

bool KNMusicM3UParser::parse(const QString &playlistFilePath,
                             KNMusicPlaylistListItem *playlistItem)
{
    //Open the playlist file first.
    QFile m3uFile(playlistFilePath);
    if(!m3uFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Read all the file from the playlist.
    QTextStream m3uStream(&m3uFile);
    QString currentLine=m3uStream.readLine().simplified();
    QStringList availableFilePath;
    //Until we cannot read any more.
    while(!currentLine.isEmpty())
    {
        //Check is this line unuseable.
        if(currentLine.at(0)!='#')
        {
            //Check is the file available.
            QFileInfo currentFile(currentLine);
            //If exist, add it to file path list.
            if(currentFile.exists())
            {
                availableFilePath.append(currentFile.absoluteFilePath());
            }
        }
        //Read next line.
        currentLine=m3uStream.readLine().simplified();
    }
    //Close the file.
    m3uFile.close();
    //Check if the file is available.
    if(availableFilePath.isEmpty())
    {
        return false;
    }

    //Set the file name as the title.
    QFileInfo m3uFileInfo(m3uFile);
    playlistItem->setText(m3uFileInfo.baseName());
    //Get the model and the parser.
    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();
    KNMusicParser *parser=KNMusicGlobal::parser();
    //Parse all the file in the file list.
    for(QStringList::iterator currentFilePath=availableFilePath.begin();
        currentFilePath!=availableFilePath.end();
        ++currentFilePath)
    {
        KNMusicDetailInfo currentDetail;
        //Treat it as a music file, parse it.
        parser->parseFile(*currentFilePath, currentDetail);
        //Add this song to playlist.
        playlistModel->appendMusicRow(KNMusicModelAssist::generateRow(currentDetail));
    }
    return true;
}

bool KNMusicM3UParser::write(const QString &playlistFilePath,
                             KNMusicPlaylistListItem *playlistItem)
{
    return false;
}
