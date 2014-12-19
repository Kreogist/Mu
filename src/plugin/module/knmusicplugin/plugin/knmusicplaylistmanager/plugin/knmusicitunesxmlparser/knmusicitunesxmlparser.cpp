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
#include <QLinkedList>
#include <QUrl>
#include <QFileInfo>

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
    QDomElement plistRoot=plistDocument.documentElement();
    if(plistRoot.nodeName()!="plist" ||
            !plistRoot.hasAttribute("version") ||
            plistRoot.attribute("version")!="1.0")
    {
        return false;
    }
    //Get the dict elements.
    QDomNodeList root=plistRoot.elementsByTagName("dict");
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
                            QString rawUrlText=QUrl::fromPercentEncoding(songValueNode.text().toUtf8());
                            if(rawUrlText.length()>17 &&
                                    rawUrlText.left(17)=="file://localhost/")
                            {
                                rawUrlText.remove(0, 17);
                            }
                            else
                            {
                                rawUrlText=QUrl(rawUrlText).path();
                            }
                            QFileInfo currentFileInfo(rawUrlText);
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
    QDomDocument plistDocument;
    //Initial the plist element.
    QDomElement plistRoot=plistDocument.createElement("plist");
    plistRoot.setAttribute("version", "1.0");
    plistDocument.appendChild(plistRoot);

    //Initial the dict element.
    QDomElement dictElement=plistDocument.createElement("dict");
    plistRoot.appendChild(dictElement);
    appendDictValue(plistDocument, dictElement, "Major Version", 1);
    appendDictValue(plistDocument, dictElement, "Minor Version", 1);
    appendDictValue(plistDocument, dictElement, "Date", QDateTime::currentDateTime());
    appendDictValue(plistDocument, dictElement, "Features", 5);
    appendDictValue(plistDocument, dictElement, "Show Content Ratings", true);

    //Generate database and song index list.
    QHash<QString, int> filePathIndex;
    QLinkedList<int> playlistIndexList;
    KNMusicPlaylistModel *playlistModel=playlistItem->playlistModel();
    for(int i=0; i<playlistModel->rowCount(); i++)
    {
        QString currentPath=playlistModel->filePathFromRow(i);
        int currentIndex=filePathIndex.value(currentPath, -1);
        //Check if we have already have the path in the index.
        if(currentIndex==-1)
        {
            currentIndex=i;
            //Insert the path to the hash.
            filePathIndex.insert(currentPath, currentIndex);
        }
        //Append the list.
        playlistIndexList.append(currentIndex);
    }

    //Output the database info to dict.
    //Initial the elements.
    QDomElement tracksKey=plistDocument.createElement("key"),
                tracksDict=plistDocument.createElement("dict");
    QDomText tracksKeyValue=plistDocument.createTextNode("Tracks");
    tracksKey.appendChild(tracksKeyValue);
    //Add to dict elements.
    dictElement.appendChild(tracksKey);
    dictElement.appendChild(tracksDict);
    //Write database info.
    QList<int> songIndexList=filePathIndex.values();
    while(!songIndexList.isEmpty())
    {
        int currentRow=songIndexList.takeFirst(),
                trackID=currentRow+100;
        //Generate current row key and dict.
        QDomElement trackKey=plistDocument.createElement("key"),
                    trackDict=plistDocument.createElement("dict");
        QDomText trackKeyValue=plistDocument.createTextNode(QString::number(trackID));
        trackKey.appendChild(trackKeyValue);
        //Generate the dict.
        appendDictValue(plistDocument, trackDict, "Track ID", trackID);
        appendDictValue(plistDocument, trackDict, "Name", playlistModel->itemText(currentRow, Name));
        appendDictValue(plistDocument, trackDict, "Artist", playlistModel->itemText(currentRow, Artist));
        appendDictValue(plistDocument, trackDict, "Album Artist", playlistModel->itemText(currentRow, AlbumArtist));
        appendDictValue(plistDocument, trackDict, "Genre", playlistModel->itemText(currentRow, Genre));
        appendDictValue(plistDocument, trackDict, "Kind", playlistModel->itemText(currentRow, Kind));
        appendDictValue(plistDocument, trackDict, "Size", playlistModel->roleData(currentRow, Size, Qt::UserRole).toLongLong());
        appendDictValue(plistDocument, trackDict, "Total Time", playlistModel->roleData(currentRow, Time, Qt::UserRole).toLongLong());
        appendDictValue(plistDocument, trackDict, "Track Number", playlistModel->itemText(currentRow, TrackNumber).toInt());
        appendDictValue(plistDocument, trackDict, "Track Count", playlistModel->itemText(currentRow, TrackCount).toInt());
        appendDictValue(plistDocument, trackDict, "Year", playlistModel->itemText(currentRow, Year).toInt());
        appendDictValue(plistDocument, trackDict, "Date Modified", playlistModel->roleData(currentRow, DateModified, Qt::UserRole).toDateTime());
        appendDictValue(plistDocument, trackDict, "Date Added", playlistModel->roleData(currentRow, DateAdded, Qt::UserRole).toDateTime());
        appendDictValue(plistDocument, trackDict, "Bit Rate", playlistModel->roleData(currentRow, BitRate, Qt::UserRole).toLongLong());
        appendDictValue(plistDocument, trackDict, "Sample Rate", playlistModel->roleData(currentRow, SampleRate, Qt::UserRole).toLongLong());
        appendDictValue(plistDocument, trackDict, "Track Type", "File");
        QString fileLocate=QUrl::fromLocalFile(playlistModel->filePathFromRow(currentRow)).toString();
#ifdef Q_OS_WIN32
        fileLocate.insert(7, "localhost");
#endif
        appendDictValue(plistDocument, trackDict, "Location", QString(QUrl::toPercentEncoding(fileLocate, "/:")));
        appendDictValue(plistDocument, trackDict, "File Folder Count", -1);
        appendDictValue(plistDocument, trackDict, "Library Folder Count", -1);
        //Add the key and dict to track.
        tracksDict.appendChild(trackKey);
        tracksDict.appendChild(trackDict);
    }

    //Output the playlist info to dict.
    QDomElement playlistKey=plistDocument.createElement("key"),
                playlistArray=plistDocument.createElement("array");
    QDomText playlistKeyValue=plistDocument.createTextNode("Playlists");
    playlistKey.appendChild(playlistKeyValue);
    //Add the key and array to dict element.
    dictElement.appendChild(playlistKey);
    dictElement.appendChild(playlistArray);
    //Generate current playlist information.
    QDomElement playlistDict=plistDocument.createElement("dict");
    playlistArray.appendChild(playlistDict);
    //Set playlist information to the dict.
    appendDictValue(plistDocument, playlistDict, "Name", playlistItem->text());
    appendDictValue(plistDocument, playlistDict, "Playlist ID", "99");
    appendDictValue(plistDocument, playlistDict, "All Items", true);
    //Generate playlist items array.
    QDomElement playlistItemsKey=plistDocument.createElement("key"),
                playlistItemsArray=plistDocument.createElement("array");
    QDomText playlistItemsKeyValue=plistDocument.createTextNode("Playlist Items");
    playlistItemsKey.appendChild(playlistItemsKeyValue);
    //Add to current playlist dict.
    playlistDict.appendChild(playlistItemsKey);
    playlistDict.appendChild(playlistItemsArray);
    //Generate Track ID dicts.
    while(!playlistIndexList.isEmpty())
    {
        QDomElement currentTrackDict=plistDocument.createElement("dict");
        appendDictValue(plistDocument,
                        currentTrackDict,
                        "Track ID",
                        100+playlistIndexList.takeFirst());
        //Add the dict to the array.
        playlistItemsArray.appendChild(currentTrackDict);
    }
    return writePlaylistContentToFile(playlistFilePath,
                                      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                      "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"+
                                      plistDocument.toString(4));
}

void KNMusiciTunesXMLParser::appendDictValue(QDomDocument &plistDocument,
                                             QDomElement &dict,
                                             const QString &key,
                                             const QVariant &value)
{
    QDomElement keyElement=plistDocument.createElement("key"),
                valueElement;
    QDomText keyElementData=plistDocument.createTextNode(key),
             valueElementData;
    switch(value.type())
    {
    case QVariant::Int:
        valueElement=plistDocument.createElement("integer");
        valueElementData=plistDocument.createTextNode(QString::number(value.toInt()));
        break;
    case QVariant::DateTime:
        valueElement=plistDocument.createElement("date");
        valueElementData=plistDocument.createTextNode(
                    value.toDateTime().toString("yyyy-MM-ddThh:mm:ssZ"));
        break;
    case QVariant::Bool:
        valueElement=plistDocument.createElement(value.toBool()?"true":"false");
        break;
    case QVariant::String:
        valueElement=plistDocument.createElement("string");
        valueElementData=plistDocument.createTextNode(value.toString());
        break;
    case QVariant::LongLong:
        valueElement=plistDocument.createElement("integer");
        valueElementData=plistDocument.createTextNode(QString::number(value.toLongLong()));
        break;
    default:
        //I don't support others!
        return;
    }
    //Set the data to key and value element.
    keyElement.appendChild(keyElementData);
    if(!valueElement.isNull())
    {
        valueElement.appendChild(valueElementData);
    }
    //Add data to dict.
    dict.appendChild(keyElement);
    dict.appendChild(valueElement);
}
