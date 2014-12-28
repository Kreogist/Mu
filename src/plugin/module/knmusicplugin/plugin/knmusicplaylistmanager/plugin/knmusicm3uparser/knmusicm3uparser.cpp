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

#include <QDebug>

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
    QStringList fileLines=m3uStream.readAll().split(QRegExp("[\n\r]"),
                                                    QString::SkipEmptyParts),
                availableFilePath;
    //Until we cannot read any more.
    for(QStringList::iterator currentLine=fileLines.begin();
        currentLine!=fileLines.end();
        ++currentLine)
    {
        //Check is this line unuseable.
        if((*currentLine).at(0)=='#')
        {
            continue;
        }
        //Check is the file available.
        QFileInfo currentFile(*currentLine);
        //If exist, add it to file path list.
        if(currentFile.exists())
        {
            availableFilePath.append(currentFile.absoluteFilePath());
        }
    }
    //Clear the no use data.
    fileLines.clear();
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
        KNMusicAnalysisItem analysisItem;
        //Treat it as a music file, parse it.
        parser->parseFile(*currentFilePath, analysisItem);
        //Add this song to playlist.
        playlistModel->appendMusicRow(
                    KNMusicModelAssist::generateRow(analysisItem.detailInfo));
    }
    return true;
}

bool KNMusicM3UParser::write(const QString &playlistFilePath,
                             KNMusicPlaylistListItem *playlistItem)
{
    QString playlistContent;
    //Get the model.
    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();
    //Write all the file path to the content.
    for(int i=0; i<playlistModel->rowCount(); i++)
    {
        playlistContent.append(playlistModel->filePathFromRow(i) + "\n");
    }
    //Write the content to file.
    return writePlaylistContentToFile(playlistFilePath, playlistContent);
}
