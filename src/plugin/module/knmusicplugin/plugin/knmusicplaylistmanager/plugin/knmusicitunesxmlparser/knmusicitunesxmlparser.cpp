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
#include <QUrl>
#include <QFileInfo>
#include <QDomDocument>

#include "knmusicparser.h"
#include "knmusicmodelassist.h"
#include "../../sdk/knmusicplaylistlistitem.h"
#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicitunesxmlparser.h"

#include <QDebug>

KNMusiciTunesXMLParser::KNMusiciTunesXMLParser(QObject *parent) :
    KNMusicPlaylistParser(parent)
{
}

QString KNMusiciTunesXMLParser::playlistType() const
{
    return tr("iTunes XML Playlist");
}

QString KNMusiciTunesXMLParser::playlistSuffix() const
{
    return "*.xml";
}

bool KNMusiciTunesXMLParser::parse(const QString &playlistFilePath,
                                   KNMusicPlaylistListItem *playlistItem)
{
    //Open the playlist file first.
    QFile plistFile(playlistFilePath);
    if(!plistFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Use QDomDocument to parse the file.
    QDomDocument plistDocument;
    if(!plistDocument.setContent(&plistFile, true))
    {
        //If there's any error, return false.
        return false;
    }
    //Close the playlist file.
    plistFile.close();
    //Get the root of the document, check the root.
    QDomNodeList root=plistDocument.elementsByTagName("dict");
    if(root.isEmpty())
    {
        return false;
    }
    QDomNodeList dictElement=root.at(0).toElement().childNodes();
    //Check dict childe nodes size.
    if(dictElement.isEmpty())
    {
        return false;
    }
    //Initial the track info hash.
    QHash<QString, QString> musicLocateHash;
    QStringList playlistIndexList;
    //Get the first node.
    QDomElement keyNode=dictElement.at(0).toElement(),
                valueNode=keyNode.nextSiblingElement();
    while(!keyNode.isNull() && !keyNode.isNull())
    {
        if(keyNode.text()=="Tracks")
        {
            if(!valueNode.childNodes().isEmpty())
            {
                //Get all the tracks information.
                QDomElement trackKeyNode=valueNode.childNodes().at(0).toElement(),
                            trackValueNode=trackKeyNode.nextSiblingElement();
                while(!trackKeyNode.isNull() && !trackValueNode.isNull())
                {
                    //Parse the track dict information.
                    QDomElement songKeyNode=trackValueNode.childNodes().at(0).toElement(),
                                songValueNode=songKeyNode.nextSiblingElement();
                    while(!songKeyNode.isNull() && !songValueNode.isNull())
                    {
                        if(songKeyNode.text()=="Location")
                        {
                            QFileInfo currentFileInfo(
                                        QUrl(QUrl::fromPercentEncoding(songValueNode.text().toUtf8())).path());
                            //Set the hash.
                            musicLocateHash.insert(trackKeyNode.text(), currentFileInfo.absoluteFilePath());
                            break;
                        }
                        //Get the next elements.
                        songKeyNode=songValueNode.nextSiblingElement();
                        if(songKeyNode.isNull())
                        {
                            break;
                        }
                        songValueNode=songKeyNode.nextSiblingElement();
                    }
                    //Get the next elements.
                    trackKeyNode=trackValueNode.nextSiblingElement();
                    if(trackKeyNode.isNull())
                    {
                        break;
                    }
                    trackValueNode=trackKeyNode.nextSiblingElement();
                }
            }
        }
        else if(keyNode.text()=="Playlists")
        {
            if(valueNode.childNodes().size()!=0)
            {
                QDomElement playlistDict=valueNode.childNodes().at(0).toElement();
                if(playlistDict.childNodes().size()!=0)
                {
                    QDomElement playlistKey=playlistDict.childNodes().at(0).toElement(),
                                playlistValue=playlistKey.nextSiblingElement();
                    //Read the playlist information.
                    while(!playlistKey.isNull() && !playlistValue.isNull())
                    {
                        if(playlistKey.text()=="Name")
                        {
                            playlistItem->setText(playlistValue.text());
                        }
                        else if(playlistKey.text()=="Playlist Items")
                        {
                            //Check the array is empty or not.
                            QDomNodeList itemsArray=playlistValue.childNodes();
                            if(!itemsArray.isEmpty())
                            {
                                for(int i=0; i<itemsArray.size(); i++)
                                {
                                    QDomElement currentTrack=itemsArray.at(i).toElement();
                                    if(currentTrack.childNodes().size()==2)
                                    {
                                        QDomElement trackElement=currentTrack.childNodes().at(0).toElement(),
                                                    trackIndex=trackElement.nextSiblingElement();
                                        if(trackElement.text()=="Track ID")
                                        {
                                            //Insert the track.
                                            playlistIndexList.append(trackIndex.text());
                                        }
                                    }
                                }
                            }
                        }
                        //Get the playlist key.
                        playlistKey=playlistValue.nextSiblingElement();
                        if(playlistKey.isNull())
                        {
                            break;
                        }
                        playlistValue=playlistKey.nextSiblingElement();
                    }
                }
            }
        }
        //Get the next elements.
        keyNode=valueNode.nextSiblingElement();
        if(keyNode.isNull())
        {
            break;
        }
        valueNode=keyNode.nextSiblingElement();
    }
    //Check the result.
    if(musicLocateHash.isEmpty() || playlistIndexList.isEmpty())
    {
        return false;
    }
    QStringList musicFileList;
    //Prepare the file list.
    while(!playlistIndexList.isEmpty())
    {
        QString currentFilePath=musicLocateHash.value(playlistIndexList.takeFirst(),
                                                      QString());
        if(!currentFilePath.isEmpty())
        {
            musicFileList.append(currentFilePath);
        }
    }
    //Add to the playlist model;
    playlistItem->playlistModel()->addFiles(musicFileList);
    //Set the changed flag.
    playlistItem->setChanged(true);
    return true;
}

bool KNMusiciTunesXMLParser::write(const QString &playlistFilePath,
                                   KNMusicPlaylistListItem *playlistItem)
{
    ;
}
