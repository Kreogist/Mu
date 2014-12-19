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
#include <QDir>
#include <QUrl>
#include <QDomDocument>

#include "knmusicparser.h"
#include "knmusicmodelassist.h"
#include "../../sdk/knmusicplaylistlistitem.h"
#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicttplparser.h"

#include <QDebug>

KNMusicTTPLParser::KNMusicTTPLParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicTTPLParser::playlistType() const
{
    return tr("TTPlayer Playlist");
}

QString KNMusicTTPLParser::playlistSuffix() const
{
    return "*.ttpl";
}

bool KNMusicTTPLParser::parse(const QString &playlistFilePath,
                              KNMusicPlaylistListItem *playlistItem)
{
    //Open the playlist file first.
    QFile ttplFile(playlistFilePath);
    if(!ttplFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Use QDomDocument to parse the file.
    QDomDocument ttplDocument;
    if(!ttplDocument.setContent(&ttplFile, true))
    {
        //If there's any error, return false.
        return false;
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
        return false;
    }
    //Get the title of the playlist.
    playlistItem->setText(root.attribute("title"));
    //Get the track list data.
    QDomElement trackListData=root.firstChildElement("items");
    if(trackListData.isNull())
    {
        return false;
    }
    //Get the track list.
    QDomNodeList trackList=trackListData.childNodes();
    //Check the size and the record count is the same or not.
    if(trackListData.attribute("count").toInt()!=trackList.size())
    {
        return false;
    }
    //Get the model and the parser.
    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();
    KNMusicParser *parser=KNMusicGlobal::parser();
    //Load the tracks.
    for(int i=0, trackCount=trackList.size(); i<trackCount; i++)
    {
        //Get the current track.
        QDomElement currentTrack=trackList.at(i).toElement();
        //Check if the track item contains subtk tag.
        if(currentTrack.hasAttribute("subtk"))
        {
            //Means this is a track list, parse as a track list.
            QList<KNMusicDetailInfo> currentDetails;
            parser->parseTrackList(currentTrack.attribute("file"),
                                   currentDetails);
            //Get the track index.
            QString trackIndex=QString::number(currentTrack.attribute("subtk").toInt());
            //Find the track index in the list.
            for(QList<KNMusicDetailInfo>::iterator i=currentDetails.begin();
                i!=currentDetails.end();
                ++i)
            {
                //If we find the index, add to the playlist.
                if((*i).textLists[TrackNumber]==trackIndex)
                {
                    playlistModel->appendMusicRow(KNMusicModelAssist::generateRow(*i));
                }
            }
        }
        else
        {
            //Parse as a file.
            KNMusicDetailInfo currentDetail;
            parser->parseFile(currentTrack.attribute("file"),
                              currentDetail);
            //Add to playlist.
            playlistModel->appendMusicRow(KNMusicModelAssist::generateRow(currentDetail));
        }
    }
    //Set changed flag.
    playlistItem->setChanged(true);
    return true;
}

bool KNMusicTTPLParser::write(const QString &playlistFilePath,
                              KNMusicPlaylistListItem *playlistItem)
{
    QDomDocument ttplDocument;
    //Initial the root element.
    QDomElement root=ttplDocument.createElement("ttplaylist");
    root.setAttribute("title", playlistItem->text());
    root.setAttribute("version", "4");
    root.setAttribute("generator", "Kreogist Mu");
    ttplDocument.appendChild(root);

    //Initial the format.
    QDomElement format=ttplDocument.createElement("format");
    format.setAttribute("tagtitle", "%A - %T");
    format.setAttribute("deftitle", "%F");
    root.appendChild(format);

    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();
    //Write the item list.
    QDomElement items=ttplDocument.createElement("items");
    root.appendChild(items);
    items.setAttribute("count", QString::number(playlistModel->rowCount()));
    for(int i=0; i<playlistModel->rowCount(); i++)
    {
        //Generate the item element and set the information attribute.
        QDomElement item=ttplDocument.createElement("item");
        //Set information.
        QString songName=playlistModel->itemText(i, Name).toHtmlEscaped(),
                songArtist=playlistModel->itemText(i, Artist).toHtmlEscaped();
        item.setAttribute("title", songArtist + " - " + songName);
        item.setAttribute("SongName", songName);
        item.setAttribute("SongArtist", songArtist);
        item.setAttribute("len", QString::number(playlistModel->roleData(i, Time, Qt::UserRole).toLongLong()));
        //Check if it has a track file.
        QString trackFileTest=playlistModel->rowProperty(i, TrackFileRole).toString();
        if(trackFileTest.isEmpty())
        {
            item.setAttribute("file", QDir::toNativeSeparators(playlistModel->filePathFromRow(i)));
        }
        else
        {
            item.setAttribute("file", QDir::toNativeSeparators(trackFileTest));
            item.setAttribute("subtk", QString::number(playlistModel->itemText(i, TrackNumber).toInt()));
        }
        //Add the item to list.
        items.appendChild(item);
    }
    //Write the data to playlist file.
    return writePlaylistContentToFile(playlistFilePath,
                                      "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" +
                                      ttplDocument.toString(4));
}
