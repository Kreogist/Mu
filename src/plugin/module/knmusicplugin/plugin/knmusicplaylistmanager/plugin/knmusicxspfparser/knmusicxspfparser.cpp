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
#include <QUrl>
#include <QDomDocument>

#include "../../sdk/knmusicplaylistlistitem.h"
#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicxspfparser.h"

#include <QDebug>

KNMusicXSPFParser::KNMusicXSPFParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
    ;
}

QString KNMusicXSPFParser::playlistType() const
{
    return tr("Vox playlist");
}

QString KNMusicXSPFParser::playlistSuffix() const
{
    return "*.xspf";
}

bool KNMusicXSPFParser::parse(const QString &playlistFilePath,
                              KNMusicPlaylistListItem *playlistItem)
{
    //Open the playlist file first.
    QFile xspfFile(playlistFilePath);
    if(!xspfFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Use QDomDocument to parse the file.
    QDomDocument xspfDocument;
    if(!xspfDocument.setContent(&xspfFile, true))
    {
        //If there's any error, return false.
        return false;
    }
    //Close the playlist file.
    xspfFile.close();
    //Get the root of the document, check the root.
    QDomElement root=xspfDocument.documentElement();
    if(root.tagName()!="playlist" ||
            !root.hasAttribute("version") ||
            root.attribute("version")!="1")
    {
        return false;
    }
    //Get the title of the playlist.
    playlistItem->setText(root.firstChildElement("title").text());
    //Get the track list data.
    QDomElement trackListData=root.firstChildElement("trackList");
    if(trackListData.isNull())
    {
        return false;
    }
    //Get the track list.
    QDomNodeList trackList=trackListData.childNodes();
    QStringList fileList;
    for(int i=0, trackCount=trackList.size(); i<trackCount; i++)
    {
        //Get the current track.
        QDomElement currentTrack=trackList.at(i).toElement();
        //Use QUrl to parse the location.
        QUrl currentUrl=QUrl(currentTrack.firstChildElement("location").text());
        //Get the track file path set to a music detail info.
        QFileInfo currentInfo(currentUrl.path());
        //Add to detail list.
        fileList.append(currentInfo.absoluteFilePath());
    }
    //Add files to playlist item model.
    playlistItem->playlistModel()->addFiles(fileList);
    //Set changed flags.
    playlistItem->setChanged(true);
    return true;
}
