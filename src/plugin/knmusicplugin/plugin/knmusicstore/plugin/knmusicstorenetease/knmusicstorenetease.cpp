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
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkCookie>
#include <QImage>

#include "../../sdk/knmusicstoreutil.h"
#include "../../sdk/knmusicstorealbumlistmodel.h"

#include "knmusicstorenetease.h"

#include <QDebug>

KNMusicStoreNetease::KNMusicStoreNetease(QObject *parent) :
    KNMusicStoreBackend(parent),
    m_newAlbumModel(new KNMusicStoreAlbumListModel)
{
}

KNMusicStoreNetease::~KNMusicStoreNetease()
{
    //Remove the new album model.
    m_newAlbumModel->deleteLater();
}

KNMusicStoreAlbumListModel *KNMusicStoreNetease::newAlbumModel()
{
    //Give back the model pointer.
    return m_newAlbumModel;
}

void KNMusicStoreNetease::fetchHomeWidgetInfo()
{
    QByteArray responseData;
    if(get(generateNeteaseRequest("http://music.163.com/api/album/new?area=ALL&"
                                  "offset=0&total=true&limit=18"),
           responseData,
           false)==200)
    {
        //Get the album list of from the response data.
        QJsonObject albumListData=QJsonDocument::fromJson(responseData).object();
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
                    currentAlbum.albumArt=
                            QPixmap::fromImage(
                                QImage::fromData(responseData)).scaled(
                                StoreAlbumSize,
                                StoreAlbumSize,
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);
                    //Add current album to model.
                    m_newAlbumModel->appendAlbum(currentAlbum);
                }
            }
        }
    }
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
    request.setRawHeader("Referer", "http://music.163.com");
    request.setRawHeader("Original", "http://music.163.com");
    //Give back the request.
    return request;
}
