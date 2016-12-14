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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>

#include "knmusiclrcparser.h"
#include "knmusicutil.h"

#include "../../sdk/knmusicstoreglobal.h"
#include "../../sdk/knmusicstoreutil.h"

#include "knmusicstoreneteasebackend.h"

using namespace MusicStoreUtil;

#include <QDebug>

KNMusicStoreNeteaseBackend::KNMusicStoreNeteaseBackend(QObject *parent) :
    KNMusicStoreBackend(parent)
{
    setObjectName("MusicStoreNeteaseBackend");
}

void KNMusicStoreNeteaseBackend::showAlbum(const QString &albumInfo)
{
    //The album info should be one string which is the album id.
    //The access url is:
    //  http://music.163.com/api/artist/(album id)
    //Reset the network access manager.
    resetManager();
    //Construct the request.
    QNetworkRequest albumRequest=generateRequest();
    //Configure the accept encoding.
    albumRequest.setRawHeader("Accept-Encoding", "plain-text");
    //Set the url.
    albumRequest.setUrl(QUrl("http://music.163.com/api/album/"+albumInfo));
    //Get the request, insert the request in the map.
    m_replyMap.insert(m_accessManager->get(albumRequest), NeteaseAlbumDetails);
    //Increase Internet counter.
    knMusicStoreGlobal->addConnectionCounter(1);
}

void KNMusicStoreNeteaseBackend::showSingleSong(const QString &songInfo)
{
    //The song info is an json object of the song.
    QJsonObject songObject=QJsonDocument::fromJson(songInfo.toUtf8()).object(),
                songMetadata;
    //Construct the song metadata.
    songMetadata.insert("name", songObject.value("name"));
    emit requireSetNavigatorItem(PageSingleSong,
                                 songObject.value("name").toString());
    //Alias.
    QJsonArray alias=songObject.value("alias").toArray();
    QString subheading;
    //Join all the data in alias.
    for(auto i : alias)
    {
        //Append data to subheading.
        subheading.append(i.toString());
        subheading.append(" ");
    }
    songMetadata.insert("subheading", subheading);
    //Artist information.
    QJsonArray artists=songObject.value("artists").toArray(),
            artistName, artistList;
    //Go through the artist list.
    for(auto i : artists)
    {
        //Get the artist object.
        QJsonObject artistObject=i.toObject();
        //Insert the artist name.
        artistName.append(artistObject.value("name").toString());
        //Insert the artist list.
        artistList.append(QString(QJsonDocument(artistObject).toJson()));
    }
    //Set the artist info.
    songMetadata.insert("artist", artistName);
    songMetadata.insert("artist_meta", artistList);
    //Album information.
    QJsonObject albumObject=songObject.value("album").toObject();
    songMetadata.insert("album", albumObject.value("name").toString());
    songMetadata.insert("album_meta",
                        QString::number(
                            qint64(albumObject.value("id").toDouble())));
    //Fetch the album art.
    QString albumArtUrl=albumObject.value("picUrl").toString();
    QNetworkRequest albumArtRequest(QUrl::fromEncoded(
                                        albumArtUrl.toLocal8Bit()));
    //Get the request, insert the request in the map.
    m_replyMap.insert(m_accessManager->get(albumArtRequest),
                      NeteaseSingleAlbumArt);
    //Most of the information is already in the song object.
    emit requireSetSingleSong(SingleMetadata, songMetadata);
    //Get the lyrics of the song.
    QNetworkRequest singleLyrics=generateRequest();
    singleLyrics.setUrl(QUrl("http://music.163.com/api/song/lyric?os=osx&id="+
                             QString::number(
                                 qint64(songObject.value("id").toDouble()))+
                             "&lv=-1&kv=-1&tv=-1"));
    m_replyMap.insert(m_accessManager->get(singleLyrics),
                      NeteaseSingleLyricsText);
    //Increase Internet counter.
    knMusicStoreGlobal->addConnectionCounter(2);
}

void KNMusicStoreNeteaseBackend::onReplyFinished(QNetworkReply *reply)
{
    //Reduce reply counter.
    knMusicStoreGlobal->reduceConnectionCounter(1);
    //Check the reply in the hash list.
    int replyOperation=m_replyMap.value(reply, -1);
    //Check the operation.
    if(replyOperation==-1)
    {
        //Delete the reply.
        reply->deleteLater();
        //Ignore the operation.
        return;
    }
    //Find the reply operation removed the pointer from the replay map.
    m_replyMap.remove(reply);
    //Check the operation.
    switch(replyOperation)
    {
    case NeteaseAlbumDetails:
        //Album detail information reply.
        onAlbumDetailReply(reply);
        break;
    case NeteaseAlbumArt:
    {
        //Album art fetched.
        //Load QPixmap from the result.
        QPixmap albumArtPixmap;
        //Load the data to a pixmap.
        QByteArray replyData=reply->readAll();
        albumArtPixmap.loadFromData(replyData);
        //Emit the album set data.
        emit requireSetAlbum(AlbumArt, QVariant(albumArtPixmap));
        break;
    }
    case NeteaseSingleLyricsText:
        //Single song lyrics reply.
        onSingleLyricsReply(reply);
        break;
    case NeteaseSingleAlbumArt:
    {
        //Album art fetched.
        //Load QPixmap from the result.
        QPixmap albumArtPixmap;
        //Load the data to a pixmap.
        QByteArray replyData=reply->readAll();
        albumArtPixmap.loadFromData(replyData);
        //Emit the album set data.
        emit requireSetSingleSong(SingleAlbumArt, QVariant(albumArtPixmap));
        break;
    }
    default:
        break;
    }
    //Delete the reply.
    reply->deleteLater();
}

void KNMusicStoreNeteaseBackend::onAlbumDetailReply(QNetworkReply *reply)
{
    //Get the raw data from the reply.
    QJsonObject albumObject=QJsonDocument::fromJson(reply->readAll()).object(),
                metadata;
    //The inside album object is the target object.
    albumObject=albumObject.value("album").toObject();
    //Change the album tab name.
    QString albumName=albumObject.value("name").toString();
    emit requireSetNavigatorItem(PageAlbum, albumName);
    //Construct the metadata object.
    metadata.insert("name", albumName);
    metadata.insert("artist", albumObject.value("artist"
                                                ).toObject().value("name"));
    //Set the album data.
    QJsonArray songList, rawSongList=albumObject.value("songs").toArray();
    //Construct the song object.
    for(auto i : rawSongList)
    {
        //Translate each i to object.
        QJsonObject songItem, songRawItem=i.toObject();
        //Backup the raw item.
        songItem.insert("custom", QString(QJsonDocument(songRawItem).toJson(
                            QJsonDocument::Compact)));
        //Insert song item data to song list.
        songItem.insert("index", QString::number(
                            songRawItem.value("no").toInt()) + ".");
        songItem.insert("name", songRawItem.value("name").toString());
        //Calculate the duration.
        songItem.insert("duration",
                        KNMusicUtil::msecondToString(
                            songRawItem.value("duration").toDouble()));
        //Get artists data.
        QJsonArray artists=songRawItem.value("artists").toArray();
        QStringList artistList;
        //For all the artist item in the list.
        for(auto j : artists)
        {
            //Add data to artist lists.
            artistList.append(j.toObject().value("name").toString());
        }
        //Set the value.
        songItem.insert("artist", artistList.join(", "));
        songItem.insert("artistId", QString(QJsonDocument(artists).toJson(
                            QJsonDocument::Compact)));
        //Append to song list.
        songList.append(songItem);
    }
    //Add song list to metadata.
    metadata.insert("songs", songList);
    //Set the meta data.
    emit requireSetAlbum(AlbumMetadata, metadata);
    //Fetch the album art.
    QString albumArtUrl=albumObject.value("picUrl").toString();
    QNetworkRequest albumArtRequest(QUrl::fromEncoded(
                                        albumArtUrl.toLocal8Bit()));
    //Get the request, insert the request in the map.
    m_replyMap.insert(m_accessManager->get(albumArtRequest),
                      NeteaseAlbumArt);
    //Increase Internet counter.
    knMusicStoreGlobal->addConnectionCounter(1);
}

void KNMusicStoreNeteaseBackend::onSingleLyricsReply(QNetworkReply *reply)
{
    //Read the reply data.
    QJsonObject lyricsObject=QJsonDocument::fromJson(reply->readAll()).object();
    //All the data is in its lrc object.
    QString lrcLyrics;
    {
        //Get the raw text.
        QJsonObject lrcObject=lyricsObject.value("lrc").toObject();
        //Check whether it contains lyric key.
        if(lrcObject.contains("lyric"))
        {
            //Add text to LRC lyrics.
            lrcLyrics.append(lrcObject.value("lyric").toString());
        }
        //Get the translate text.
        lrcObject=lyricsObject.value("tlyric").toObject();
        //Check whether it contains lyric key.
        if(lrcObject.contains("lyric"))
        {
            //Add translated text to LRC lyrics.
            lrcLyrics.append(lrcObject.value("lyric").toString());
        }
    }
    //Parse the lyrics.
    QStringList textList;
    {
        //Prepare the position list.
        QList<qint64> positionList;
        knMusicStoreGlobal->lrcParser()->parseText(lrcLyrics,
                                                   positionList,
                                                   textList);
    }
    //Combine the text list with next line.
    emit requireSetSingleSong(SingleLyrics, textList.join('\n'));
}

inline QNetworkRequest KNMusicStoreNeteaseBackend::generateRequest()
{
    //Build the request.
    QNetworkRequest candidateRequest;
    candidateRequest.setRawHeader("Accept", "*/*");
    candidateRequest.setRawHeader("Accept-Language",
                                  "zh-CN,zh;q=0.8,gl;q=0.6,zh-TW;q=0.4");
    candidateRequest.setRawHeader("Connection", "keep-alive");
    candidateRequest.setRawHeader("Content-Type",
                                  "application/x-www-form-urlencoded");
    candidateRequest.setRawHeader("Host", "music.163.com");
    candidateRequest.setRawHeader("Referer", "http://music.163.com/search/");
    //UA change to Safari.
    candidateRequest.setRawHeader(
                "User-Agent",
                "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) "
                "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.152 "
                "Safari/537.36");
    return candidateRequest;
}

inline void KNMusicStoreNeteaseBackend::resetManager()
{
    //Check the reply map and access manager.
    if(m_replyMap.isEmpty() && (!m_accessManager.isNull()))
    {
        //When the map is empty, it means there is no pending request.
        //And the access manager is allocated, it won't need to reset.
        return;
    }
    //Break down the original link.
    m_accessManagerHandler.disconnectAll();
    //Reset the manager pointer.
    m_accessManager.reset(new QNetworkAccessManager());
    //Clear the reply map.
    QList<QNetworkReply *> replyList=m_replyMap.keys();
    //Recover the reply list memory.
    qDeleteAll(replyList);
    //Clear the map.
    m_replyMap.clear();
    //Link the access manager.
    m_accessManagerHandler.append(
                connect(m_accessManager.data(),
                        &QNetworkAccessManager::finished,
                        this, &KNMusicStoreNeteaseBackend::onReplyFinished,
                        //Use queue connection for using event loop.
                        Qt::QueuedConnection));
}
