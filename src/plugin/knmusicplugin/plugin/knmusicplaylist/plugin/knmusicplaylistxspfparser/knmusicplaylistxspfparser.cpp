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
#include <QDomDocument>
#include <QFileInfo>
#include <QTimeZone>
#include <QUrl>

#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicplaylistxspfparser.h"

KNMusicPlaylistXspfParser::KNMusicPlaylistXspfParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicPlaylistXspfParser::description()
{
    return tr("Vox playlist");
}

QString KNMusicPlaylistXspfParser::suffix()
{
    return QString("*.xspf");
}

KNMusicPlaylistModel *KNMusicPlaylistXspfParser::read(const QString &filePath)
{
    //Open the playlist file first.
    QFile xspfFile(filePath);
    //Open it in read only mode.
    if(!xspfFile.open(QIODevice::ReadOnly))
    {
        //Open failed, return null.
        return nullptr;
    }
    //Generate the dom document.
    QDomDocument xspfDocument;
    //Use QDomDocument to parse the file.
    if(!xspfDocument.setContent(&xspfFile, true))
    {
        //If there's any error, return null.
        return nullptr;
    }
    //Close the playlist file.
    xspfFile.close();
    //Get the root of the document.
    QDomElement root=xspfDocument.documentElement();
    //Check the root.
    if(root.tagName()!="playlist" ||
                !root.hasAttribute("version") ||
                root.attribute("version")!="1")
    {
        return nullptr;
    }
    //Get the track list data.
    QDomElement trackListData=root.firstChildElement("trackList");
    //Check the track list data.
    if(trackListData.isNull())
    {
        return nullptr;
    }
    //Generate a playlist model.
    KNMusicPlaylistModel *playlistModel=new KNMusicPlaylistModel(thread());
    //Set the title of the playlist model.
    playlistModel->setTitle(root.firstChildElement("title").text());
    //Get the track list.
    QDomNodeList trackList=trackListData.childNodes();
    QStringList fileList;
    //Parse all the track from the track list. Get the file path.
    for(int i=0, trackCount=trackList.size(); i<trackCount; ++i)
    {
        //Get the current track.
        QDomElement currentTrack=trackList.at(i).toElement();
        //Use QUrl to parse the location.
        QUrl currentUrl=QUrl(currentTrack.firstChildElement("location").text());
        //Get the track file path set to a music detail info, add the file to
        //file list.
        fileList.append(QFileInfo(currentUrl.path()).absoluteFilePath());
    }
    //Add the file list to the playlist model.
    playlistModel->appendFiles(fileList);
    //Set the changed flags.
    playlistModel->setChanged(true);
    //Give back the playlist model.
    return playlistModel;
}

bool KNMusicPlaylistXspfParser::write(KNMusicPlaylistModel *playlist,
                                      const QString &filePath)
{
    //Generate the main content of the playlist.
    QDomDocument xspfDocument;
    //Initial the root element.
    QDomElement root=xspfDocument.createElement("playlist");
    //Set attributes.
    root.setAttribute("xmlns", "http://xspf.org/ns/0/");
    root.setAttribute("version", "1");
    //Add to root.
    xspfDocument.appendChild(root);

    //Write the playlist information.
    QDomElement titleElement=xspfDocument.createElement("title"),
                    creatorElement=xspfDocument.createElement("creator"),
            dateElement=xspfDocument.createElement("date");
    //Get the current time and time zone text.
    QDateTime generateTime=QDateTime::currentDateTime();
    int timeZoneOffset=generateTime.timeZone().standardTimeOffset(generateTime),
            hour=qAbs(timeZoneOffset)/3600;
    QString timeZoneText=
            hour<10?"0"+QString::number(hour):QString::number(hour);
    timeZoneText=
            timeZoneOffset>-1?"+"+timeZoneText+":00":"-"+timeZoneText+":00";
    QDomText titleTextElement=xspfDocument.createTextNode(playlist->title()),
             creatorTextElement=xspfDocument.createTextNode("Kreogist Mu"),
             dateTextElement=xspfDocument.createTextNode(
                generateTime.toString("yyyy-MM-ddThh:mm:ss")+timeZoneText);
    //Add the text element to info elements.
    titleElement.appendChild(titleTextElement);
    creatorElement.appendChild(creatorTextElement);
    dateElement.appendChild(dateTextElement);
    //Add to root element.
    root.appendChild(titleElement);
    root.appendChild(creatorElement);
    root.appendChild(dateElement);

    //Write the tracklist to the dom document.
    QDomElement trackListElement=xspfDocument.createElement("trackList");
    root.appendChild(trackListElement);
    //Go through the model.
    for(int i=0; i<playlist->rowCount(); ++i)
    {
        //Generate the track element.
        QDomElement currentTrack=xspfDocument.createElement("track"),
                    location=xspfDocument.createElement("location"),
                    title=xspfDocument.createElement("title"),
                    trackNum=xspfDocument.createElement("trackNum"),
                    duration=xspfDocument.createElement("duration");
        //Generate the track element data.
        QDomText locationText=xspfDocument.createTextNode(
                    QUrl::toPercentEncoding(
                        playlist->rowProperty(i, FilePathRole).toString())),
                titleText=xspfDocument.createTextNode(
                    playlist->textData(i, Name)),
                trackNumText=xspfDocument.createTextNode(QString::number(i+1)),
                durationText=xspfDocument.createTextNode(
                    QString::number(
                        playlist->rowProperty(i,
                                              DurationRole).toLongLong()));
        //Add the text element to info elements.
        location.appendChild(locationText);
        title.appendChild(titleText);
        trackNum.appendChild(trackNumText);
        duration.appendChild(durationText);
        //Add the information elements to track element.
        currentTrack.appendChild(location);
        currentTrack.appendChild(title);
        currentTrack.appendChild(trackNum);
        currentTrack.appendChild(duration);
        //Add to track list.
        trackListElement.appendChild(currentTrack);
    }
    //Write the data to playlist file.
    return writeContent(filePath,
                        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"+
                        xspfDocument.toString(4));
}
