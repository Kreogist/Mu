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
#include <QHash>
#include <QFileInfo>
#include <QUrl>
#include <QLinkedList>

#include "../../sdk/knmusicplaylistmodel.h"

#include "knmusicplaylistitunesxmlparser.h"

KNMusicPlaylistiTunesXMLParser::KNMusicPlaylistiTunesXMLParser(QObject *parent):
    KNMusicPlaylistParser(parent)
{
}

QString KNMusicPlaylistiTunesXMLParser::description()
{
    return tr("iTunes XML Playlist");
}

QString KNMusicPlaylistiTunesXMLParser::suffix()
{
    return "*.xml";
}

KNMusicPlaylistModel *KNMusicPlaylistiTunesXMLParser::read(
        const QString &filePath)
{
    //Get the plist file.
    QFile plistFile(filePath);
    //Open the playlist file first.
    if(!plistFile.open(QIODevice::ReadOnly))
    {
        return nullptr;
    }
    //Generate a dom document to parse the file.
    QDomDocument plistDocument;
    //Use QDomDocument to parse the file.
    if(!plistDocument.setContent(&plistFile, true))
    {
        //If there's any error, return nullptr.
        return nullptr;
    }
    //Close the playlist file.
    plistFile.close();
    //Get the 'plist' element from the document.
    QDomElement plistRoot=plistDocument.documentElement();
    //Check the 'plist' element.
    if(plistRoot.nodeName()!="plist" ||
                !plistRoot.hasAttribute("version") ||
                plistRoot.attribute("version")!="1.0")
    {
        return nullptr;
    }
    //Get the dict elements from the 'plist'.
    QDomNodeList root=plistRoot.elementsByTagName("dict");
    //Check the list data.
    if(root.isEmpty())
    {
        return nullptr;
    }
    //Pick out the 'dict' element.
    QDomNodeList dictElement=root.at(0).toElement().childNodes();
    //Check 'dict' childe nodes size.
    if(dictElement.isEmpty())
    {
        return nullptr;
    }
    //Initial the track info hash.
    QHash<QString, QString> musicLocateHash;
    QStringList playlistIndexList;
    //Prepare the playlist.
    KNMusicPlaylistModel *model=new KNMusicPlaylistModel(thread());
    //Get the first node.
    QDomElement keyNode=dictElement.at(0).toElement(),
                valueNode=keyNode.nextSiblingElement();
    //Check the current key node.
    while(!keyNode.isNull() && !keyNode.isNull())
    {
        //Check the name of the node.
        //For 'tracks':
        if(keyNode.text()=="Tracks")
        {
            //Check whether the child nodes is empty.
            if(!valueNode.childNodes().isEmpty())
            {
                //Get all the tracks information.
                QDomElement trackKeyNode=
                                valueNode.childNodes().at(0).toElement(),
                            trackValueNode=
                                trackKeyNode.nextSiblingElement();
                //Pick out all the tracks.
                while(!trackKeyNode.isNull() && !trackValueNode.isNull())
                {
                    //Parse the track dict information.
                    QDomElement songKeyNode=
                                  trackValueNode.childNodes().at(0).toElement(),
                                songValueNode=
                                  songKeyNode.nextSiblingElement();
                    //If all the information is valid.
                    while(!songKeyNode.isNull() && !songValueNode.isNull())
                    {
                        //Get the location of the song.
                        if(songKeyNode.text()=="Location")
                        {
                            //Get the url from the file.
                            //Use QUrl to translate it to a readable string.
                            QString rawUrlText=
                                    QUrl::fromPercentEncoding(
                                        songValueNode.text().toUtf8());
                            //Remove the protocol name.
                            if(rawUrlText.length()>17 &&
                                    rawUrlText.left(17)=="file://localhost/")
                            {
                                rawUrlText.remove(0, 17);
                            }
                            else
                            {
                                rawUrlText=QUrl(rawUrlText).path();
                            }
                            //Set the hash.
                            musicLocateHash.insert(
                                        trackKeyNode.text(),
                                        QFileInfo(rawUrlText).absoluteFilePath()
                                        );
                            break;
                        }
                        //Switch to the next song elements.
                        songKeyNode=songValueNode.nextSiblingElement();
                        songValueNode=songKeyNode.nextSiblingElement();
                    }
                    //Switch to the track elements.
                    trackKeyNode=trackValueNode.nextSiblingElement();
                    trackValueNode=trackKeyNode.nextSiblingElement();
                }
            }
        }
        //For 'Playlists'.
        else if(keyNode.text()=="Playlists")
        {
            //Check the node size.
            if(valueNode.childNodes().size()!=0)
            {
                //Get the dict of the playlist.
                QDomElement playlistDict=
                        valueNode.childNodes().at(0).toElement();
                //Check the size of playlist dict.
                if(playlistDict.childNodes().size()!=0)
                {
                    //Get the playlist keys.
                    QDomElement playlistKey=
                            playlistDict.childNodes().at(0).toElement(),
                                playlistValue=
                            playlistKey.nextSiblingElement();
                    //Read the playlist information.
                    while(!playlistKey.isNull() && !playlistValue.isNull())
                    {
                        //For 'Name' element, it stored the name of the
                        //playlist.
                        if(playlistKey.text()=="Name")
                        {
                            //Set the title to model.
                            model->setTitle(playlistValue.text());
                        }
                        //For 'Playlist Items' element, it stored the file path
                        //information of the songs.
                        else if(playlistKey.text()=="Playlist Items")
                        {
                            //Check the array is empty or not.
                            QDomNodeList itemsArray=playlistValue.childNodes();
                            if(!itemsArray.isEmpty())
                            {
                                //Get all the information from the items array.
                                for(int i=0; i<itemsArray.size(); i++)
                                {
                                    QDomElement currentTrack=
                                            itemsArray.at(i).toElement();
                                    if(currentTrack.childNodes().size()==2)
                                    {
                                        QDomElement trackElement=
                                                        currentTrack.
                                                        childNodes().
                                                        at(0).toElement(),
                                                    trackIndex=
                                                        trackElement.
                                                        nextSiblingElement();
                                        //Check if the track element is
                                        //'Track ID' we will save track info.
                                        if(trackElement.text()=="Track ID")
                                        {
                                            //Insert the track.
                                            playlistIndexList.append(
                                                        trackIndex.text());
                                        }
                                    }
                                }
                            }
                        }
                        //Switch to the next playlist key.
                        playlistKey=playlistValue.nextSiblingElement();
                        playlistValue=playlistKey.nextSiblingElement();
                    }
                }
            }
        }
        //Switch to the next elements.
        keyNode=valueNode.nextSiblingElement();
        valueNode=keyNode.nextSiblingElement();
    }
    //Prepare the file list.
    QStringList fileList;
    //Translate the index list to file list.
    for(auto i=playlistIndexList.constBegin();
        i!=playlistIndexList.constEnd();
        ++i)
    {
        //Get the file path according to the hash table.
        QString filePath=musicLocateHash.value((*i), QString());
        //If the file is not empty,
        if(!filePath.isEmpty())
        {
            //Add to file list.
            fileList.append(filePath);
        }
    }
    //Add the file list to model.
    model->appendFiles(fileList);
    //Set the changed flag.
    model->setChanged(true);
    //Give back the model.
    return model;
}

bool KNMusicPlaylistiTunesXMLParser::write(KNMusicPlaylistModel *playlist,
                                           const QString &filePath)
{
    //Generate the plist document.
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
    appendDictValue(plistDocument, dictElement, "Date",
                    QDateTime::currentDateTime());
    appendDictValue(plistDocument, dictElement, "Features", 5);
    appendDictValue(plistDocument, dictElement, "Show Content Ratings", true);

    //Generate database and song index list.
    QHash<QString, int> filePathIndex;
    QLinkedList<int> playlistIndexList;
    //Add pathes to hash keys.
    for(int i=0; i<playlist->rowCount(); i++)
    {
        //Get current path.
        QString currentPath=playlist->rowProperty(i, FilePathRole).toString();
        //Check the path in the index hash.
        int currentIndex=filePathIndex.value(currentPath, -1);
        //If we never insert this path to the index,
        if(currentIndex==-1)
        {
            //Get the new index.
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
        //Generate the track key value.
        QDomText trackKeyValue=
                plistDocument.createTextNode(QString::number(trackID));
        trackKey.appendChild(trackKeyValue);
        //Get the detail info.
        const KNMusicDetailInfo detailInfo=playlist->rowDetailInfo(currentRow);
        //Generate the dict.
        appendDictValue(plistDocument, trackDict, "Track ID",
                        trackID);
        appendDictValue(plistDocument, trackDict, "Name",
                        detailInfo.textLists[Name]);
        appendDictValue(plistDocument, trackDict, "Artist",
                        detailInfo.textLists[Artist]);
        appendDictValue(plistDocument, trackDict, "Album Artist",
                        detailInfo.textLists[AlbumArtist]);
        appendDictValue(plistDocument, trackDict, "Genre",
                        detailInfo.textLists[Genre]);
        appendDictValue(plistDocument, trackDict, "Kind",
                        detailInfo.textLists[Kind]);
        appendDictValue(plistDocument, trackDict, "Size",
                        detailInfo.size);
        appendDictValue(plistDocument, trackDict, "Total Time",
                        detailInfo.duration);
        appendDictValue(plistDocument, trackDict, "Track Number",
                        detailInfo.textLists[TrackNumber].toString().toInt());
        appendDictValue(plistDocument, trackDict, "Track Count",
                        detailInfo.textLists[TrackCount].toString().toInt());
        appendDictValue(plistDocument, trackDict, "Year",
                        detailInfo.textLists[Year].toString().toInt());
        appendDictValue(plistDocument, trackDict, "Date Modified",
                        detailInfo.dateModified);
        appendDictValue(plistDocument, trackDict, "Date Added",
                        detailInfo.dateAdded);
        appendDictValue(plistDocument, trackDict, "Bit Rate",
                        detailInfo.bitRate);
        appendDictValue(plistDocument, trackDict, "Sample Rate",
                        detailInfo.samplingRate);
        appendDictValue(plistDocument, trackDict, "Track Type", "File");
        QString fileLocate=QUrl::fromLocalFile(detailInfo.filePath).toString();
#ifdef Q_OS_WIN
        fileLocate.insert(7, "localhost");
#endif
        appendDictValue(plistDocument, trackDict, "Location",
                        QString(QUrl::toPercentEncoding(fileLocate, "/:")));
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
    appendDictValue(plistDocument, playlistDict, "Name", playlist->title());
    appendDictValue(plistDocument, playlistDict, "Playlist ID", "99");
    appendDictValue(plistDocument, playlistDict, "All Items", true);
    //Generate playlist items array.
    QDomElement playlistItemsKey=plistDocument.createElement("key"),
                playlistItemsArray=plistDocument.createElement("array");
    QDomText playlistItemsKeyValue=
                plistDocument.createTextNode("Playlist Items");
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
    return writeContent(filePath,
                        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE "
                        "plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\""
                        " \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
                        + plistDocument.toString(4));
}

inline void KNMusicPlaylistiTunesXMLParser::appendDictValue(
        QDomDocument &plistDocument,
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
        valueElementData=plistDocument.createTextNode(
                            QString::number(value.toInt()));
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
        valueElementData=plistDocument.createTextNode(
                            QString::number(value.toLongLong()));
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
