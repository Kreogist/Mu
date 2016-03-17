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
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkCookie>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QImage>

#include "../../sdk/knmusicstoreutil.h"
#include "../../sdk/knmusicstorealbumlistmodel.h"

#include "knmusicstorenetease.h"

#include <QDebug>

KNMusicStoreNetease::KNMusicStoreNetease(QObject *parent) :
    KNMusicStoreBackend(parent),
    m_newAlbumModel(new KNMusicStoreAlbumListModel),
    m_hotSongModel(new KNMusicStoreAlbumListModel)
{
}

KNMusicStoreNetease::~KNMusicStoreNetease()
{
    //Remove the new album model.
    m_newAlbumModel->deleteLater();
    m_hotSongModel->deleteLater();
}

KNMusicStoreAlbumListModel *KNMusicStoreNetease::newAlbumModel()
{
    //Give back the model pointer.
    return m_newAlbumModel;
}

KNMusicStoreAlbumListModel *KNMusicStoreNetease::hotSongModel()
{
    //Give back the model pointer.
    return m_hotSongModel;
}

void KNMusicStoreNetease::fetchHomeWidgetInfo()
{
    //Prepare the response data.
    QByteArray responseData;
    //Get the new albums.
    if(get(generateNeteaseRequest("http://music.163.com/api/album/new?area=ALL&"
                                  "offset=0&total=true&limit=18"),
           responseData,
           false)==200)
    {
        //Clear the new album model.
        m_newAlbumModel->clear();
        //Get the album list of from the response data.
        QJsonObject albumListData=
                QJsonDocument::fromJson(responseData).object();
        //Get the album list from the album list data object.
        if(albumListData.contains("albums"))
        {
            //Parse the albums.
            QJsonArray albumList=albumListData.value("albums").toArray();
            //Check all items in the array.
            for(auto i=albumList.begin(); i!=albumList.end(); ++i)
            {
                //Translate the item into object.
                QJsonObject albumObject=(*i).toObject();
                //Check the properties are contained or not.
                if(albumObject.contains("name") &&
                        albumObject.contains("picUrl") &&
                        albumObject.contains("artist") &&
                        get(albumObject.value("picUrl").toString(),
                            responseData)==200)
                {
                    //Construct a album object.
                    KNMusicStoreUtil::StoreAlbumListItem currentAlbum;
                    //Set the album data.
                    currentAlbum.name=albumObject.value("name").toString();
                    currentAlbum.artist=
                            albumObject.value(
                                "artist").toObject().value("name").toString();
                    currentAlbum.albumArt=generateAlbumArt(responseData);
                    //Add current album to model.
                    m_newAlbumModel->appendItem(currentAlbum);
                }
            }
        }
    }
    //Get the new hot songs.
    if(get(generateNeteaseRequest("http://music.163.com/discover/toplist?"
                                  "id=3778678"),
           responseData,
           false)==200)
    {
        //Clear new artist model.
        m_hotSongModel->clear();
        //We will parse all the data from the song ids.
        QRegularExpression rex("/song\\?id=(\\d+)");
        QRegularExpressionMatchIterator i=rex.globalMatch(responseData);
        //Generate the song id lists.
        QStringList songIdList;
        //Check whether we could find next item.
        while(i.hasNext())
        {
            //Get the match result.
            QRegularExpressionMatch match=i.next();
            //Add song to song id list.
            songIdList.append(match.captured(1));
        }
        //Get song detail information.
        for(int songCount=18, i=0;
            i<songIdList.size() && songCount > 0;
            ++i)
        {
            //Get the song detail information.
            QJsonObject songDetail=getSongDetails(songIdList.at(i));
            //Check validation.
            if(songDetail.isEmpty())
            {
                //Ignore the current song.
                continue;
            }
            //Check the song data.
            if(songDetail.contains("songs"))
            {
                //Get the songs list
                QJsonArray songsArray=
                        songDetail.value("songs").toArray();
                //Check the size.
                if(songsArray.size()>0)
                {
                    //Get song data.
                    QJsonObject songData=songsArray.at(0).toObject();
                    //Check information and download album data.
                    if(songData.contains("name") &&
                            songData.contains("artists") &&
                            songData.contains("album") &&
                            get(songData.value("album").toObject().value(
                                    "picUrl").toString(),
                                responseData)==200)
                    {
                        //Reduce the counter.
                        --songCount;
                        //Construct a album object.
                        KNMusicStoreUtil::StoreAlbumListItem currentSong;
                        //Set data.
                        currentSong.name=songData.value("name").toString();
                        currentSong.albumArt=generateAlbumArt(responseData);
                        //Get artist list.
                        QJsonArray artistList=
                                songData.value("artists").toArray();
                        //Get the artist names.
                        QStringList artistNames;
                        for(auto j=0; j<artistList.size(); ++j)
                        {
                            //Add name to artist list.
                            artistNames.append(artistList.at(j).toObject(
                                                   ).value("name").toString());
                        }
                        currentSong.artist=artistNames.join(tr(", "));
                        //Add current song to model.
                        m_hotSongModel->appendItem(currentSong);
                    }
                }
            }
        }
    }
    qDebug()<<"Done!";
}

inline QNetworkRequest KNMusicStoreNetease::generateNeteaseRequest(
        const QString &url)
{
    //Generate the network request, initial it with the url.
    QNetworkRequest request;
    //Set default content type header.
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    //Set url of the request.
    request.setUrl(QUrl(url));
    //Set to header.
    request.setRawHeader("Cookie", "appver=2.0.2");
    //Set the "Referer" and "Origin".
    request.setRawHeader("Referer", "http://music.163.com/search/");
    request.setRawHeader("Original", "http://music.163.com");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Host", "music.163.com");
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) "
                         "AppleWebKit/537.36 (KHTML, like Gecko) "
                         "Chrome/33.0.1750.152 "
                         "Safari/537.36");
    //Give back the request.
    return request;
}

inline QJsonObject KNMusicStoreNetease::getSongDetails(const QString &songId)
{
    //Generate the response data.
    QByteArray responseData;
    //Get the song object.
    if(get(generateNeteaseRequest(QString("http://music.163.com/api/song/detail"
                                          "?ids=[%1]").arg(songId)),
        responseData,
        false)==200)
    {
        //If we could get the data, the return the json object.
        return QJsonDocument::fromJson(responseData).object();
    }
    //Or else return the null object.
    return QJsonObject();
}

inline QPixmap KNMusicStoreNetease::generateAlbumArt(
        const QByteArray &albumArtData)
{
    return QPixmap::fromImage(QImage::fromData(albumArtData)).scaled(
                StoreAlbumSize,
                StoreAlbumSize,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
}
