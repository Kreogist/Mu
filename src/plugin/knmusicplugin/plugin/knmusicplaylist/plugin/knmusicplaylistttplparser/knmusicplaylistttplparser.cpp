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
#include <QFile>
#include <QDomDocument>

#include "knmusicparser.h"
#include "knmusicglobal.h"

#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicplaylistttplparser.h"

KNMusicPlaylistTtplParser::KNMusicPlaylistTtplParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicPlaylistTtplParser::description()
{
    return tr("TTPlayer Playlist");
}

QString KNMusicPlaylistTtplParser::suffix()
{
    return "*.ttpl";
}

KNMusicPlaylistModel *KNMusicPlaylistTtplParser::read(const QString &filePath)
{
    //Get the playlist file.
    QFile ttplFile(filePath);
    //Open the playlist file first.
    if(!ttplFile.open(QIODevice::ReadOnly))
    {
        //Failed to load the playlist.
        return nullptr;
    }
    //Generate the playlist parser.
    QDomDocument ttplDocument;
    //Use QDomDocument to parse the file.
    if(!ttplDocument.setContent(&ttplFile, true))
    {
        //If there's any error, return nullptr.
        return nullptr;
    }
    //Close the playlist file.
    ttplFile.close();
    //Get the root of the document, check the root.
    QDomElement root=ttplDocument.documentElement();
    //Only support for the latest ttplayer.
    if(root.tagName()!="ttplaylist" ||
            !root.hasAttribute("version") ||
            root.attribute("version")!="4")
    {
        //If there's any error, then return nullptr.
        return nullptr;
    }
    //Get the track list data.
    QDomElement trackListData=root.firstChildElement("items");
    //Check the track list data.
    if(trackListData.isNull())
    {
        return nullptr;
    }
    //Get the track list.
    QDomNodeList trackList=trackListData.childNodes();
    //Check the size and the record count is the same or not.
    if(trackListData.attribute("count").toInt()!=trackList.size())
    {
        return nullptr;
    }
    //Get the parser from music global.
    KNMusicParser *parser=knMusicGlobal->parser();
    //Check the validation of the parser.
    if(!parser)
    {
        return nullptr;
    }
    //Generate a playlist model.
    KNMusicPlaylistModel *model=new KNMusicPlaylistModel(thread());
    //Set the title of the playlist.
    model->setTitle(root.attribute("title"));
    //Load the tracks.
    for(int i=0, trackCount=trackList.size(); i<trackCount; i++)
    {
        //Get the current track.
        QDomElement currentTrack=trackList.at(i).toElement();
        //Check if the track item contains subtk tag.
        if(currentTrack.hasAttribute("subtk"))
        {
            //Means this is a track list, parse as a track list.
            QList<KNMusicAnalysisItem> currentDetails;
            parser->parseTrackList(currentTrack.attribute("file"),
                                   currentDetails);
            //Get the track index.
            QString trackIndex=
                    QString::number(currentTrack.attribute("subtk").toInt());
            //Find the track index in the list.
            for(auto i=currentDetails.begin(); i!=currentDetails.end(); ++i)
            {
                //If we find the index, add to the playlist.
                if((*i).detailInfo.textLists[TrackNumber].toString()
                        ==trackIndex)
                {
                    model->appendRow((*i).detailInfo);
                }
            }
        }
        else
        {
            //Parse as a file.
            KNMusicAnalysisItem item;
            parser->parseFile(currentTrack.attribute("file"), item);
            //Add to playlist.
            model->appendRow(item.detailInfo);
        }
    }
    //Set changed flag.
    model->setChanged(true);
    //Give back the model.
    return model;
}

bool KNMusicPlaylistTtplParser::write(KNMusicPlaylistModel *playlist,
                                      const QString &filePath)
{
    //Prepare the ttpl document.
    QDomDocument ttplDocument;
    //Generate the 'root' element.
    QDomElement root=ttplDocument.createElement("ttplaylist");
    //Set the attribute of the 'root' element.
    root.setAttribute("title", playlist->title());
    root.setAttribute("version", "4");
    root.setAttribute("generator", "Kreogist Mu");
    //Add 'root' element to document.
    ttplDocument.appendChild(root);

    //Generate the 'format' element.
    QDomElement format=ttplDocument.createElement("format");
    //Set the attribute of the 'format' element.
    format.setAttribute("tagtitle", "%A - %T");
    format.setAttribute("deftitle", "%F");
    //Add 'format' element to root.
    root.appendChild(format);

    //Generate the 'items' element.
    QDomElement items=ttplDocument.createElement("items");
    //Add 'items' element to root.
    root.appendChild(items);
    //Configure the 'count' property of the 'items' element.
    items.setAttribute("count", QString::number(playlist->rowCount()));
    //Add song information to 'items' element.
    for(int i=0; i<playlist->rowCount(); i++)
    {
        //Generate the item element and set the information attribute.
        QDomElement item=ttplDocument.createElement("item");
        //Set attributes.
        QString songName=playlist->textData(i, Name).toHtmlEscaped(),
                songArtist=playlist->textData(i, Artist).toHtmlEscaped();
        item.setAttribute("title", songArtist + " - " + songName);
        item.setAttribute("SongName", songName);
        item.setAttribute("SongArtist", songArtist);
        item.setAttribute("len",
                          QString::number(
                              playlist->rowProperty(i,
                                                    DurationRole).toLongLong())
                          );
        //Check if the current record is a track of another file.
        QString trackFile=playlist->rowProperty(i, TrackFileRole).toString();
        //If the track file is empty, then we only need to set the 'file'
        //attribute.
        if(trackFile.isEmpty())
        {
            item.setAttribute("file",
                              QDir::toNativeSeparators(
                                  playlist->rowProperty(i,
                                                        FilePathRole).toString()
                                  ));
        }
        else
        {
            //Set the 'file' attribute.
            item.setAttribute("file", QDir::toNativeSeparators(trackFile));
            //Set the 'track' attribute.
            item.setAttribute("subtk",
                              QString::number(
                                  playlist->textData(i,
                                                     TrackNumber).toInt()));
        }
        //Add the item to list.
        items.appendChild(item);
    }
    //Write the data to playlist file.
    return writeContent(filePath,
                        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone="
                        "\"yes\" ?>\n"+
                        ttplDocument.toString(4));
}
