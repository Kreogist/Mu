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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QPixmap>
#include <QTimerEvent>
#include <QTimer>

#include "knmusiclrcparser.h"
#include "knmusicutil.h"

#include "../../sdk/knmusicstoreglobal.h"

#include "knmusicstoreneteasebackend.h"

#include <QDebug>

#define DefaultTimeoutLimit     30 //Default seconds for timeout.
#define DefaultPipelineLimit    32 //Default links for pipeline.

KNMusicStoreNeteaseBackend::KNMusicStoreNeteaseBackend(QObject *parent) :
    KNMusicStoreBackend(parent),
    m_timeout(new QTimer(this)),
    m_timeoutLimit(DefaultTimeoutLimit),
    m_pipelineLimit(DefaultPipelineLimit)
{
    setObjectName("MusicStoreNeteaseBackend");
    //Initial the list urls.
    // New albums, special API.
    m_listUrls[ListNewAlbum]="http://music.163.com/api/album/new?area=ALL&"
                             "offset=%1&total=true&limit=%2";
    // New songs, actually the New song ranking.
    m_listUrls[ListNewSongs]="http://music.163.com/discover/toplist?id=3779629";
    // Billboard, Oricon and iTunes ranking, the discover toplist API.
    m_listUrls[ListBillboard]="http://music.163.com/discover/toplist?id=60198";
    m_listUrls[ListOricon]="http://music.163.com/discover/toplist?id=60131";
    m_listUrls[ListItunes]="http://music.163.com/discover/toplist?id=11641012";
    // Top song ranking, actually the hot song ranking.
    m_listUrls[ListTopSongs]="http://music.163.com/discover/toplist?id=3778678";

    //Configure the timeout timer.
    m_timeout->setInterval(1000);
    m_timeout->setTimerType(Qt::VeryCoarseTimer);
    connect(m_timeout, &QTimer::timeout,
            this, &KNMusicStoreNeteaseBackend::onTimeoutTick);
}

KNMusicStoreNeteaseBackend::~KNMusicStoreNeteaseBackend()
{
    //Stop the ticking.
    stopTimeoutTick();
}

void KNMusicStoreNeteaseBackend::setWorkingThread(QThread *thread)
{
    //Do original moving.
    KNMusicStoreBackend::setWorkingThread(thread);
}

void KNMusicStoreNeteaseBackend::showHome()
{
    //Reset the network access manager.
    resetManager();
    //Fetch the list before HomeSongListCount.
    //Insert the GET request.
    //Because the data could be changed, this part we won't use loop to do this.
    insertRequest(m_listUrls[ListNewAlbum].arg("0", "32"),
                  NeteaseGet, NeteaseHomeListNewAlbum);
    insertRequest(m_listUrls[ListNewSongs],
                  NeteaseGet, NeteaseHomeListNewSongs);
    insertRequest(m_listUrls[ListBillboard],
                  NeteaseGet, NeteaseHomeListBillboard);
//    insertRequest(m_listUrls[ListOricon],
//                  NeteaseGet, NeteaseHomeListOricon);
//    insertRequest(m_listUrls[ListItunes],
//                  NeteaseGet, NeteaseHomeListItunes);
//    insertRequest(m_listUrls[ListTopSongs],
//                  NeteaseGet, NeteaseHomeListTopSongs);
    //Increase Internet counter.
    emit requireAddConnectionCount(3);
}

void KNMusicStoreNeteaseBackend::showAlbum(const QString &albumInfo)
{
    //The album info should be one string which is the album id.
    //The access url is:
    //  http://music.163.com/api/artist/(album id)
    //Reset the network access manager.
    resetManager();
    //Get the data.
    insertRequest("http://music.163.com/api/album/"+albumInfo,
                  NeteaseGet, NeteaseAlbumDetails);
    //Increase Internet counter.
    emit requireAddConnectionCount(1);
}

void KNMusicStoreNeteaseBackend::showSingleSong(const QString &songInfo)
{
    //The song information should be one string which is the song id.
    //The access url is:
    //  http://music.163.com/api/song/detail/?id=(song id)&ids=[(song id)]
    //Reset the network access manager.
    resetManager();
    //Get the data.
    insertRequest("http://music.163.com/api/song/detail/?id="+songInfo+
                  "&ids=["+songInfo+"]",
                  NeteaseGet, NeteaseSingleDetails);
    //Increase Internet counter.
    emit requireAddConnectionCount(1);
}

void KNMusicStoreNeteaseBackend::setTimeout(int seconds)
{
    //Save the timeout settings.
    m_timeoutLimit=seconds;
}

void KNMusicStoreNeteaseBackend::onReplyFinished(QNetworkReply *reply)
{
    //Reduce reply counter.
    emit requireReduceConnectionCount(1);
    //Check the caching request list.
    if(!m_queueRequest.isEmpty())
    {
        //Take the first cache request.
        NetworkRequestItem requestItem=m_queueRequest.takeFirst();
        //Launch the first cache request.
        launchRequest(requestItem.request,
                      requestItem.requestType,
                      requestItem.replyType);
    }
    //Check reply state.
    if(!m_replyTimeout.contains(reply))
    {
        //The reply is timeout, error.
        //!FIXME: show error.
        reply->deleteLater();
        return;
    }
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
    //Find the reply operation removed the pointer from the reply map and
    //timeout ticking map.
    m_replyMap.remove(reply);
    m_replyTimeout.remove(reply);
    //Check the operation.
    switch(replyOperation)
    {
    case NeteaseHomeListNewAlbum:
    case NeteaseHomeListNewSongs:
    case NeteaseHomeListBillboard:
    case NeteaseHomeListOricon:
    case NeteaseHomeListItunes:
    case NeteaseHomeListTopSongs:
        //Call the home page processing slots.
        onHomeListReply(replyOperation, reply);
        break;
    case NeteaseHomeListNewAlbumArt:
        //Call the home page new album art list processing.
        onHomeNewArtworkReply(reply, m_albumArtworkList, HomeNewAlbumArt);
        break;
    case NeteaseHomeListNewSongArt:
        //Call the home page new song art list processing.
        onHomeNewArtworkReply(reply, m_songArtworkList, HomeNewSongArt);
        break;
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
    case NeteaseSingleDetails:
        onSingleDetailReply(reply);
        break;
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

void KNMusicStoreNeteaseBackend::onHomeListReply(int listType,
                                                 QNetworkReply *reply)
{
    //Check the list type.
    switch(listType)
    {
    case NeteaseHomeListNewAlbum:
    {
        //Translate the reply data as a json object.
        QJsonObject homeListData=
                QJsonDocument::fromJson(reply->readAll()).object();
        //Get the album information.
        QJsonArray albumList=homeListData.value("albums").toArray(),
                   albumDataList;
        //Reset the album art list.
        m_albumArtworkList=QList<uint>();
        m_albumArtworkList.reserve(32);
        for(int i=0; i<32; ++i)
        {
            //Append null value: 0.
            m_albumArtworkList.append(0);
        }
        //Increase the counter.
        emit requireAddConnectionCount(albumList.size());
        //Loop and construct the album data.
        for(int i=0; i<albumList.size(); ++i)
        {
            //Translate the value to object, prepare the value object.
            QJsonObject albumObject=albumList.at(i).toObject(), albumData;
            //Insert the data to album data.
            albumData.insert("name", albumObject.value("name"));
            albumData.insert("custom", QString::number(
                                 quint64(albumObject.value("id").toDouble())));
            albumData.insert("artist", albumObject.value(
                                 "artist").toObject().value("name").toString());
            //Insert the album data to album data list.
            albumDataList.append(albumData);
            //Get the album art url.
            QString albumArtworkUrl=albumObject.value("picUrl").toString();
            //Save the url in the list.
            m_albumArtworkList.replace(i, qHash(albumArtworkUrl));
            //Fetch the album object value.
            insertRequest(albumArtworkUrl,
                          NeteaseGet, NeteaseHomeListNewAlbumArt,
                          false);
        }
        //Emit the set data function.
        emit requireSetHome(HomeNewAlbumData, albumDataList);
        break;
    }
    case NeteaseHomeListNewSongs:
        //Update the home page information.
        emit requireSetHome(
                    HomeNewSongData,
                    getSongDataList(reply, 32, true, &m_songArtworkList));
        break;
    case NeteaseHomeListBillboard:
        //Update the home page information.
        emit requireSetHome(HomeBillboardList,
                            getSongDataList(reply, 10, false, nullptr));
        break;
    }
}

void KNMusicStoreNeteaseBackend::onHomeNewArtworkReply(QNetworkReply *reply,
        QList<uint> &urlMap,
        int albumArtRequestType)
{
    //Load QPixmap from the reply.
    QPixmap homeAlbumArtPixmap;
    //Get the reply data.
    homeAlbumArtPixmap.loadFromData(reply->readAll());
    //Check the pixmap.
    if(homeAlbumArtPixmap.isNull())
    {
        //Ignore the invalid album art data.
        return;
    }
    //Construct the album structure.
    KNMusicStoreHomeUpdateArtwork homeArtworkData;
    //Set the index.
    homeArtworkData.index=urlMap.indexOf(qHash(reply->url().toString()));
    //Check index is valid or not.
    if(homeArtworkData.index==-1)
    {
        //Ignore the invalid reply data.
        return;
    }
    //Set the pixmap.
    homeArtworkData.artwork=homeAlbumArtPixmap;
    //Replace the list data.
    urlMap.replace(homeArtworkData.index, 0);
    //Change the album art data.
    emit requireSetHome(albumArtRequestType,
                        QVariant::fromValue(homeArtworkData));
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
        songItem.insert("custom", QString::number(
                            (quint64)songRawItem.value("id").toDouble()));
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
    insertRequest(albumObject.value("picUrl").toString(),
                  NeteaseGet, NeteaseAlbumArt, false);
    //Increase Internet counter.
    emit requireAddConnectionCount(1);
}

void KNMusicStoreNeteaseBackend::onSingleDetailReply(QNetworkReply *reply)
{
    //The song info is an json object of the song.
    QJsonObject songObject=QJsonDocument::fromJson(reply->readAll()).object(),
                songMetadata;
    {
        //Translate the raw data to json array.
        QJsonArray songInfoList=songObject.value("songs").toArray();
        //Check the song info list.
        if(songInfoList.isEmpty())
        {
            //Error occurs.
            //! FIXME: display error.
            return;
        }
        //Pick out the song object from the raw data.
        songObject=songInfoList.at(0).toObject();
    }
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
    //Most of the information is already in the song object.
    emit requireSetSingleSong(SingleMetadata, songMetadata);
    //Increase Internet counter.
    emit requireAddConnectionCount(2);
    //Fetch the album art.
    insertRequest(albumObject.value("picUrl").toString(),
                  NeteaseGet, NeteaseSingleAlbumArt, false);
    //Get the lyrics of the song.
    insertRequest("http://music.163.com/api/song/lyric?os=osx&id="+
                  QString::number(qint64(songObject.value("id").toDouble()))+
                  "&lv=-1&kv=-1&tv=-1",
                  NeteaseGet, NeteaseSingleLyricsText);
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

void KNMusicStoreNeteaseBackend::onTimeoutTick()
{
    //Get all the keys of time out.
    QList<QNetworkReply *> replyList=m_replyTimeout.keys();
    //Check the list number.
    if(replyList.isEmpty())
    {
        //Stop the tick.
        stopTimeoutTick();
        //Mission complete.
        return;
    }
    //Check all the reply in the waiting list.
    for(auto i : replyList)
    {
        //Check the i is null or not.
        if(i==nullptr)
        {
            //Remove the current item.
            m_replyTimeout.remove(i);
            //Continue.
            continue;
        }
        //Get the reply timeout.
        if(m_replyTimeout.value(i) > m_timeoutLimit)
        {
            //Timeout, abort the reply.
            //Remove the item from the map.
            m_replyTimeout.remove(i);
            //Notice: do not need to delete later. In the implementation, it
            //will emit finish signal, all the data will be processed there.
            i->abort();
            //Go to next.
            continue;
        }
        //Increase the counter.
        m_replyTimeout.insert(i, m_replyTimeout.value(i)+1);
    }
}

inline void KNMusicStoreNeteaseBackend::insertRequest(const QString &url,
                                                      int requestType,
                                                      int replyType,
                                                      bool useHeader)
{
    //Prepare an empty request.
    QNetworkRequest neteaseRequest;
    //Check the use header.
    if(useHeader)
    {
        //Use the generate request.
        neteaseRequest=generateRequest();
        //Set the url.
        neteaseRequest.setUrl(QUrl(url));
    }
    else
    {
        //Normally, for no use header request is to download files.
        neteaseRequest=QNetworkRequest(QUrl::fromEncoded(url.toLocal8Bit()));
    }
    //Check the reply running list.
    if(m_replyMap.size()>=m_pipelineLimit)
    {
        //Throw the reply in the cache list.
        NetworkRequestItem requestItem;
        //Construct the item.
        requestItem.request=neteaseRequest;
        requestItem.requestType=requestType;
        requestItem.replyType=replyType;
        //Insert the item.
        m_queueRequest.append(requestItem);
        //Complete.
        return;
    }
    //Launch the request.
    launchRequest(neteaseRequest, requestType, replyType);
}

inline void KNMusicStoreNeteaseBackend::launchRequest(
        const QNetworkRequest &request,
        int requestType,
        int replyType)
{
    //Create the reply pointer.
    QNetworkReply *reply=nullptr;
    //Check the request type.
    switch(requestType)
    {
    case NeteaseGet:
        //Get the reply pointer.
        reply=m_accessManager->get(request);
        //Get the request, insert the request in the map.
        m_replyMap.insert(reply, replyType);
        break;
    }
    //Insert the timeout counter.
    m_replyTimeout.insert(reply, 0);
    //Start timeout checking.
    startTimeoutTick();
}

inline QJsonArray KNMusicStoreNeteaseBackend::getSongDataList(
        QNetworkReply *reply,
        int maximumItem,
        bool fetchAlbum,
        QList<uint> *artworkList)
{
    //Prepare the song object list.
    QJsonArray songList;
    {
        //Read the html content.
        QByteArray htmlRawContent=reply->readAll();
        //Search the song-list-pre-cache id inside the raw content.
        int divStart=
                htmlRawContent.indexOf("<div id=\"song-list-pre-cache\"");
        //Check the position.
        if(divStart==-1)
        {
            //Cannot find the data.
            //! FIXME: data error.
            return songList;
        }
        //Search the end div from the position.
        int divEnd=
                htmlRawContent.indexOf("</div>", divStart);
        //Check the position of div end.
        if(divEnd==-1)
        {
            //Kidding me?
            //!FIXME: Unknown error, data cannot be paired.
            return songList;
        }
        //Do the search again, this time, it should be what we want.
        divEnd=htmlRawContent.indexOf("</div>", divEnd+6);
        //Check the div end position.
        if(divEnd==-1)
        {
            //Kidding me?
            //!FIXME: Unknown error, data cannot be paired.
            return songList;
        }
        //Parse the div part only.
        htmlRawContent=htmlRawContent.mid(divStart, divEnd-divStart+6);
        //Inside the div part, there is a textarea tag, it stores the JSON
        //format song list.
        int textAreaStart=htmlRawContent.indexOf("<textarea");
        if(textAreaStart==-1)
        {
            //! FIXME: No textarea.
            return songList;
        }
        //Find the first > of the tag.
        int textAreaEnd=htmlRawContent.indexOf(">", textAreaStart);
        if(textAreaEnd==-1)
        {
            //! FIXME: No textarea tag end.
            return songList;
        }
        //Remove the previous data.
        htmlRawContent=htmlRawContent.mid(textAreaEnd+1);
        //Search the end tag.
        textAreaStart=htmlRawContent.indexOf("</textarea>");
        if(textAreaStart==-1)
        {
            //! FIXME: No textarea.
            return songList;
        }
        //Remove the end tag data.
        htmlRawContent=htmlRawContent.left(textAreaStart);
        //Parse the json format data.
        songList=QJsonDocument::fromJson(htmlRawContent).array();
    }
    //Prepare the custom data.
    QJsonArray songDataList;
    //Check the song list size.
    int songDataSize=qMin(maximumItem, songList.size());
    //Check the fetch album switch.
    if(fetchAlbum)
    {
        //Increase the counter.
        emit requireAddConnectionCount(songDataSize);
        //Reset the album art list.
        artworkList->clear();
        artworkList->reserve(songDataSize);
        for(int i=0; i<songDataSize; ++i)
        {
            //Append null value: 0.
            artworkList->append(0);
        }
    }
    //Loop and construct the album data.
    for(int i=0; i<songDataSize; ++i)
    {
        //Translate the value to object, prepare the value object.
        QJsonObject songObject=songList.at(i).toObject(), songData;
        //Insert the data to album data.
        songData.insert("name", songObject.value("name"));
        songData.insert("custom", QString::number(
                             quint64(songObject.value("id").toDouble())));
        //Combine the artist name
        QJsonArray artistList=songObject.value("artists").toArray();
        QString artistName;
        for(auto j : artistList)
        {
            //Each j is an object.
            QJsonObject artistObject=j.toObject();
            //Check whether we need to add sperator or not.
            if(!artistName.isEmpty())
            {
                //Append comma.
                artistName.append(", ");
            }
            //Append artist name.
            artistName.append(artistObject.value("name").toString());
        }
        //Translate the album object.
        QJsonObject albumObject=songObject.value("album").toObject();
        //Append artist and album data.
        songData.insert("artist-album", artistName+ " - " +
                        albumObject.value("name").toString());
        //Insert the album data to album data list.
        songDataList.append(songData);
        //Check the switch.
        if(fetchAlbum)
        {
            //Get the song artwork url.
            QString songArtworkUrl=albumObject.value("picUrl").toString();
            //Save the url in the list.
            artworkList->replace(i, qHash(songArtworkUrl));
            //Fetch the album object value.
            insertRequest(songArtworkUrl,
                          NeteaseGet, NeteaseHomeListNewSongArt,
                          false);
        }
    }
    //Complete the song data list.
    return songDataList;
}

inline QNetworkRequest KNMusicStoreNeteaseBackend::generateRequest()
{
    //Build the request.
    QNetworkRequest request;
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Accept-Language",
                         "zh-CN,zh;q=0.8,gl;q=0.6,zh-TW;q=0.4");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Content-Type",
                         "application/x-www-form-urlencoded");
    request.setRawHeader("Host", "music.163.com");
    request.setRawHeader("Referer", "http://music.163.com/search/");
    //Fake User-Agent hack to Safari.
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_5) "
                         "AppleWebKit/601.7.8 (KHTML, like Gecko) "
                         "Version/9.1.3 Safari/537.86.7");
    return request;
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
    //Check reply map size.
    if(!m_replyMap.isEmpty())
    {
        //Reduce the counter.
        emit requireReduceConnectionCount(m_replyMap.size());
        //Clear the map.
        m_replyMap.clear();
        m_replyTimeout.clear();
    }
    //Link the access manager.
    m_accessManagerHandler.append(
                connect(m_accessManager.data(),
                        &QNetworkAccessManager::finished,
                        this, &KNMusicStoreNeteaseBackend::onReplyFinished,
                        //Use queue connection for using event loop.
                        Qt::QueuedConnection));
}

inline void KNMusicStoreNeteaseBackend::startTimeoutTick()
{
    //Start the timeout ticking.
    m_timeout->start();
}

inline void KNMusicStoreNeteaseBackend::stopTimeoutTick()
{
    //Stop the time out ticking.
    m_timeout->stop();
}
