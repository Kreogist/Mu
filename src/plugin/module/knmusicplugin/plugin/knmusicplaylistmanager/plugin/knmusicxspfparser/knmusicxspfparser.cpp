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
#include <QDateTime>
#include <QTimeZone>

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

bool KNMusicXSPFParser::write(const QString &playlistFilePath,
                              KNMusicPlaylistListItem *playlistItem)
{
    QDomDocument xspfDocument;
    //Initial the root element.
    QDomElement root=xspfDocument.createElement("playlist");
    root.setAttribute("xmlns", "http://xspf.org/ns/0/");
    root.setAttribute("version", "1");
    xspfDocument.appendChild(root);

    //Write the playlist information.
    QDomElement titleElement=xspfDocument.createElement("title"),
                creatorElement=xspfDocument.createElement("creator"),
                dateElement=xspfDocument.createElement("date");
    //Get the current time and time zone text.
    QDateTime generateTime=QDateTime::currentDateTime();
    int timeZoneOffset=generateTime.timeZone().standardTimeOffset(generateTime),
            hour=qAbs(timeZoneOffset)/3600;
    QString timeZoneText=hour<10?"0"+QString::number(hour):QString::number(hour);
    timeZoneText=timeZoneOffset>-1?"+"+timeZoneText+":00":"-"+timeZoneText+":00";
    QDomText titleTextElement=xspfDocument.createTextNode(playlistItem->text()),
             creatorTextElement=xspfDocument.createTextNode("VOX"),
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

    //Write the tracklist.
    QDomElement trackListElement=xspfDocument.createElement("trackList");
    root.appendChild(trackListElement);
    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();
    for(int i=0; i<playlistModel->rowCount(); i++)
    {
        //Generate the track element and information elements.
        QDomElement currentTrack=xspfDocument.createElement("track"),
                    location=xspfDocument.createElement("location"),
                    title=xspfDocument.createElement("title"),
                    trackNum=xspfDocument.createElement("trackNum"),
                    duration=xspfDocument.createElement("duration");
        QDomText locationText=xspfDocument.createTextNode(
                    QUrl::toPercentEncoding(playlistModel->filePathFromRow(i))),
                 titleText=xspfDocument.createTextNode(playlistModel->itemText(i, Name)),
                 trackNumText=xspfDocument.createTextNode(QString::number(i+1)),
                 durationText=xspfDocument.createTextNode(
                             QString::number(playlistModel->roleData(i, Time, Qt::UserRole).toLongLong()));
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
    return writePlaylistContentToFile(playlistFilePath,
                                      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"+
                                      xspfDocument.toString(4));
}
