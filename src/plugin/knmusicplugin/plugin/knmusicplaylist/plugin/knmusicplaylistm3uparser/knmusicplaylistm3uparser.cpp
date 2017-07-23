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
#include <QTextStream>
#include <QFileInfo>

#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicplaylistm3uparser.h"

KNMusicPlaylistM3uParser::KNMusicPlaylistM3uParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicPlaylistM3uParser::description()
{
    return tr("M3U Playlist");
}

QString KNMusicPlaylistM3uParser::suffix()
{
    return "*.m3u";
}

KNMusicPlaylistModel *KNMusicPlaylistM3uParser::read(const QString &filePath)
{
    return readPlaylist(filePath);
}

bool KNMusicPlaylistM3uParser::write(KNMusicPlaylistModel *playlist,
                                     const QString &filePath)
{
    //Generate the content cache.
    QString content;
    //Write all the file path to the content.
    for(int i=0; i<playlist->rowCount(); ++i)
    {
        //Add the file path to the content cache.
        content.append(playlist->rowProperty(i, FilePathRole).toString()+"\n");
    }
    //Write the content to file.
    return writeContent(filePath, content);
}

KNMusicPlaylistModel *KNMusicPlaylistM3uParser::readPlaylist(
        const QString &filePath,
        const char *codec)
{
    //Get the m3u file.
    QFile m3uFile(filePath);
    //Open the playlist file first.
    if(!m3uFile.open(QIODevice::ReadOnly))
    {
        //Failed to open the file.
        return nullptr;
    }
    //Generate the text stream.
    QTextStream m3uStream(&m3uFile);
    //Check if there's a specific codec is told to use.
    if(codec!=nullptr)
    {
        m3uStream.setCodec(codec);
    }
    //Read all the file from the playlist.
    QStringList fileLines=m3uStream.readAll().split(QRegExp("[\n\r]"),
                                                    QString::SkipEmptyParts),
                availableFilePath;
    //Close the file.
    m3uFile.close();
    //Get the directory path of the m3u file.
    QString baseDirectory=QFileInfo(m3uFile).absolutePath();
    //Parse each line of the file.
    for(auto i=fileLines.constBegin(); i!=fileLines.constEnd(); ++i)
    {
        //Check is this line comment.
        if((*i).at(0)=='#')
        {
            continue;
        }
        //Save the current record.
        QString recordFilePath=(*i);
        //Check the existence of the curren file path.
        if(!QFileInfo::exists(recordFilePath))
        {
            //It might be relative path.
            //Add the base directory in front of the record.
            recordFilePath.prepend(baseDirectory+"/");
            //Check the new path.
            if(!QFileInfo::exists(recordFilePath))
            {
                continue;
            }
        }
        //Add the valid file path to the available file path.
        availableFilePath.append(recordFilePath);
    }
    //Clear the original file data.
    fileLines.clear();
    //Check the available file path list is empty or not.
    if(availableFilePath.isEmpty())
    {
        return nullptr;
    }
    //Generate the playlist.
    KNMusicPlaylistModel *model=new KNMusicPlaylistModel(thread());
    //Set the file name without suffix as title.
    model->setTitle(QFileInfo(m3uFile).completeBaseName());
    //Add all the file to the model.
    model->appendFiles(availableFilePath);
    //Set the changed flags.
    model->setChanged(true);
    //Give back the model.
    return model;
}
