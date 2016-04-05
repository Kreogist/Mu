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
#include <QJsonObject>
#include <QDateTime>
#include <QNetworkCookie>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QImage>
#include <QScopedPointer>
#include <QCryptographicHash>

#include "knrestapibase.h"

#include "knmusicutil.h"

#include "../../sdk/knmusicstorealbumlistmodel.h"
#include "../../sdk/knmusicstorealbummodel.h"
#include "../../sdk/knmusicstoresongdetailinfo.h"
#include "../../sdk/knmusicstoresearchresult.h"
#include "../../sdk/knmusicstoresearchmodel.h"

#include "knmusicstorenetease.h"

#include <QDebug>

#define ListMaximumSize 18

KNMusicStoreNetease::KNMusicStoreNetease(QObject *parent) :
    KNMusicStoreBackend(parent),
    m_magicData(QByteArray("3go8&$8*3*3h0k(2)2")),
    m_magicDataLength(m_magicData.size()),
    m_homeLocking(0),
    m_newAlbumModel(new KNMusicStoreAlbumListModel),
    m_hotSongModel(new KNMusicStoreAlbumListModel),
    m_albumDetail(new KNMusicStoreAlbumModel),
    m_songDetail(new KNMusicStoreSongDetailInfo),
    m_searchResult(new KNMusicStoreSearchResult),
    m_launchingInstance(StateNull)
{
    //Initial the list model.
    for(int i=0; i<NeteaseListCount; ++i)
    {
        m_listModel[i]=new KNMusicStoreAlbumListModel;
    }
    //Configure the watchers.
    for(int i=0; i<NeteaseWorkThreadCount; ++i)
    {
        //Link the watcher signals to the counter.
        connect(&m_listThreadWatcher[i], SIGNAL(finished()),
                this, SLOT(onActionWatcherFinished()));
    }
}

KNMusicStoreNetease::~KNMusicStoreNetease()
{
    //Remove the new album model and hot song model.
    delete m_newAlbumModel;
    m_newAlbumModel=nullptr;
    delete m_hotSongModel;
    m_hotSongModel=nullptr;
    //Remove album detail model.
    delete m_albumDetail;
    m_albumDetail=nullptr;
    //Remove the song detail information.
    delete m_songDetail;
    m_songDetail=nullptr;
    //Remove the search result model.
    delete m_searchResult;
    m_searchResult=nullptr;
    //Remove the list model.
    for(int i=0; i<NeteaseListCount; ++i)
    {
        //Reset the pointer.
        delete m_listModel[i];
        m_listModel[i]=nullptr;
    }

    //Quit all the working threads.
    // Home Threads.
    for(int i=0; i<NeteaseWorkThreadCount; ++i)
    {
        //Quit the home working threads.
        quitWorkingThread(m_listThreads[i]);
    }
    // Album Thread.
    quitWorkingThread(m_albumThread);
    quitWorkingThread(m_songThread);
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

int KNMusicStoreNetease::listCount()
{
    //Give back the list count.
    return NeteaseListCount;
}

QString KNMusicStoreNetease::listName(int listIndex)
{
    //Check the list index.
    switch (listIndex)
    {
    case BillboardList:
        return tr("Billboard Week List");
    case iTunesList:
        return tr("iTunes Hot List");
    case OriconList:
        return tr("Oricon Week List");
    default:
        return QString();
    }
}

KNMusicStoreAlbumListModel *KNMusicStoreNetease::listModel(int listIndex)
{
    Q_ASSERT(listIndex < NeteaseListCount && listIndex > -1);
    //Give back the pointer.
    return m_listModel[listIndex];
}

KNMusicStoreAlbumModel *KNMusicStoreNetease::albumDetailModel()
{
    //Give back the model.
    return m_albumDetail;
}

KNMusicStoreSongDetailInfo *KNMusicStoreNetease::getSongDetail()
{
    //Give back the song detail.
    return m_songDetail;
}

KNMusicStoreSearchModel *KNMusicStoreNetease::searchResultModel(int index)
{
    //Get the search result model.
    return m_searchResult->searchResultModel(index);
}

void KNMusicStoreNetease::fetchHomeInfo()
{
    //Check the launching state.
    switch(m_launchingInstance)
    {
    case StateNull:
        //Change the launching instance to state home.
        m_launchingInstance=StateHome;
        break;
    case StateHome:
        //Launching home fetch functions, do not need to launch it again.
        return;
    default:
        //When launching others, we cannot doing anything but wait.
        return;
    }

    //Update the latest albums.
    m_listThreads[LatestAlbumList]=
            QtConcurrent::run(this,
                              &KNMusicStoreNetease::updateLatestAlbumsList);

    //Update the hot songs list.
    m_listThreads[HotSongList]=
            QtConcurrent::run(this,
                              &KNMusicStoreNetease::updateNeteaseList,
                              m_hotSongModel,
                              QString("3778678"));
    //Update lists.
    m_listThreads[BillboardList]=
            QtConcurrent::run(this,
                              &KNMusicStoreNetease::updateNeteaseList,
                              m_listModel[BillboardList],
                              QString("60198"));
    m_listThreads[iTunesList]=
            QtConcurrent::run(this,
                              &KNMusicStoreNetease::updateNeteaseList,
                              m_listModel[iTunesList],
                              QString("11641012"));
    m_listThreads[OriconList]=
            QtConcurrent::run(this,
                              &KNMusicStoreNetease::updateNeteaseList,
                              m_listModel[OriconList],
                              QString("60131"));

    //Set update the future.
    for(int i=0; i<NeteaseWorkThreadCount; ++i)
    {
        //Set the watcher.
        m_listThreadWatcher[i].setFuture(m_listThreads[i]);
    }
}

void KNMusicStoreNetease::fetchAlbumDetail(const QVariant &albumId)
{
    //Check the launching state.
    switch(m_launchingInstance)
    {
    case StateNull:
        //Change the launching instance to state home.
        m_launchingInstance=StateAlbumDetail;
        break;
    case StateAlbumDetail:
        //Launching home fetch functions, do not need to launch it again.
        return;
    default:
        //When launching others, we cannot doing anything but wait.
        return;
    }

    //Update the album thread.
    m_albumThread=
            QtConcurrent::run(this,
                              &KNMusicStoreNetease::updateAlbumDetail,
                              m_albumDetail,
                              albumId.toString());
}

void KNMusicStoreNetease::fetchSongDetail(const QVariant &songId)
{
    //Check the launching state.
    switch(m_launchingInstance)
    {
    case StateNull:
        //Change the launching instance to state home.
        m_launchingInstance=StateSongDetail;
        break;
    case StateSongDetail:
        //Launching home fetch functions, do not need to launch it again.
        return;
    default:
        //When launching others, we cannot doing anything but wait.
        return;
    }

    //Update the album thread.
    m_songThread=
            QtConcurrent::run(this,
                              &KNMusicStoreNetease::updateSongDetail,
                              m_songDetail,
                              songId.toString());
}

void KNMusicStoreNetease::fetchSearchResult(const QString &keyword)
{
    QList<int> songSearchFilter;
    songSearchFilter.append(NameField);
    songSearchFilter.append(ArtistField);
    songSearchFilter.append(AlbumField);
    songSearchFilter.append(DurationField);
    //Construct search request.
    SearchRequest request;
    request.keyword=keyword;
    request.offset=0;
    request.type=1;
    request.filter=songSearchFilter;
    updateSearchResult(m_searchResult->searchResultModel(
                           KNMusicStoreUtil::CategorySong),
                       request);
}

void KNMusicStoreNetease::updateLatestAlbumsList()
{
    //Prepare the rest api.
    QScopedPointer<KNRestApiBase> curl;
    //Initial the rest api.
    curl.reset(new KNRestApiBase);
    //Prepare the response data.
    QByteArray responseData;
    //Get the new albums.
    if(curl->get(generateNeteaseRequest("http://music.163.com/api/album/new?"
                                        "area=ALL&offset=0&total=true&limit=" +
                                        QString::number(ListMaximumSize)),
                 responseData,
                 false)==200)
    {
        //Check pointer first.
        if(m_newAlbumModel==nullptr)
        {
            //If the model is failed, then ignore current work.
            return;
        }
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
                QJsonObject songObject=(*i).toObject();
                //Check the properties are contained or not.
                if(songObject.contains("name") &&
                        songObject.contains("picUrl") &&
                        songObject.contains("artist") &&
                        curl->get(songObject.value("picUrl").toString(),
                                  responseData)==200)
                {
                    //Construct a album object.
                    KNMusicStoreUtil::StoreAlbumListItem currentAlbum;
                    //Set the album data.
                    currentAlbum.name=songObject.value("name").toString();
                    currentAlbum.artist=
                            songObject.value(
                                "artist").toObject().value("name").toString();
                    currentAlbum.albumArt=generateAlbumArt(responseData);
                    currentAlbum.albumData=QString::number(
                                qint64(songObject.value("id").toDouble()));
                    //Check pointer first.
                    if(m_newAlbumModel==nullptr)
                    {
                        //If the model is failed, then ignore current work.
                        return;
                    }
                    //Add current album to model.
                    m_newAlbumModel->appendItem(currentAlbum);
                }
            }
        }
    }
}

void KNMusicStoreNetease::updateNeteaseList(
        QPointer<KNMusicStoreAlbumListModel> model,
        const QString &listNo)
{
    //Prepare the rest api.
    QScopedPointer<KNRestApiBase> curl;
    //Initial the rest api.
    curl.reset(new KNRestApiBase);
    //Prepare the response data.
    QByteArray responseData;
    //Get the new hot songs.
    if(curl->get(generateNeteaseRequest("http://music.163.com/discover/toplist?"
                                        "id="+QString(listNo)),
                 responseData,
                 false)==200)
    {
        //Check model first.
        if(model.isNull())
        {
            //Stop to downloading data.
            return;
        }
        //Clear new artist model.
        model.data()->clear();
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
        for(int songCount=ListMaximumSize, i=0;
            i<songIdList.size() && songCount > 0;
            ++i)
        {
            //Get the song detail information.
            QJsonObject songDetail=getSongDetails(curl.data(),
                                                  songIdList.at(i));
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
                            curl->get(songData.value("album").toObject().value(
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
                        currentSong.artist=getArtistNames(songData);
                        currentSong.albumData=
                                qint64(songData.value("id").toDouble());
                        //Check model first.
                        if(model.isNull())
                        {
                            //Stop to downloading data.
                            return;
                        }
                        //Add current song to model.
                        model.data()->appendItem(currentSong);
                    }
                }
            }
        }
    }
}

void KNMusicStoreNetease::updateSearchResult(
        QPointer<KNMusicStoreSearchModel> model,
        const SearchRequest &request)
{
    //Prepare the rest api.
    QScopedPointer<KNRestApiBase> curl;
    //Initial the rest api.
    curl.reset(new KNRestApiBase);
    //Prepare the response data.
    QByteArray responseData;
    //Get the search result.
    if(curl->post(generateNeteaseRequest(
                      "http://music.163.com/api/search/get/"
                      "?s="+request.keyword+"&"
                      "limit=20&" +
                      "type="+QString::number(request.type)+"&"
                      "offset="+QString::number(request.offset)),
                  QByteArray(),
                  responseData)==200)
    {
        //Check model first.
        if(model.isNull())
        {
            //Stop to downloading data.
            return;
        }
        //Parse the data.
        QJsonObject dataObject=QJsonDocument::fromJson(responseData).object();
        //Get the result.
        if(!dataObject.contains("result"))
        {
            //The data object is failed.
            return;
        }
        //Get the result object.
        dataObject=dataObject.value("result").toObject();
        //Get the song count, set it to the model.
        model->setTotalCount(dataObject.value("songCount").toInt());
        //Get the songs items array.
        QJsonArray searchItemList=dataObject.value("songs").toArray();
        //Generate the song id list and song data list.
        QList<QVariant> itemIdList;
        QList<QStringList> itemList;
        //Construct the song item.
        for(auto i=searchItemList.begin(); i!=searchItemList.end(); ++i)
        {
            //Get the song item.
            QJsonObject songData=(*i).toObject();
            //Check the song data.
            itemIdList.append(QString::number(
                              qint64(songData.value("id").toDouble())));
            //Insert the data to stringlist.
            QStringList itemInfoList;
            //Append data to item info list according to filter.
            for(int j=0; j<request.filter.size(); ++j)
            {
                switch(j)
                {
                case NameField:
                    itemInfoList.append(songData.value("name").toString());
                    break;
                case ArtistField:
                    itemInfoList.append(getArtistNames(songData));
                    break;
                case AlbumField:
                    itemInfoList.append(songData.value(
                                            "album").toObject().value(
                                            "name").toString());
                    break;
                case DurationField:
                    itemInfoList.append(getDuration(songData));
                    break;
                }
            }
            //Insert data to song list.
            itemList.append(itemInfoList);
        }
        //Check model.
        if(model.isNull())
        {
            //Failed.
            return;
        }
        //Set song list data and song id data.
        model->setSearchResult(itemList);
        model->setSearchResultId(itemIdList);
    }
}

void KNMusicStoreNetease::updateAlbumDetail(
        QPointer<KNMusicStoreAlbumModel> albumDetail,
        const QString &albumId)
{
    qDebug()<<"Album Fetch Start.";
    //Prepare the rest api.
    QScopedPointer<KNRestApiBase> curl;
    //Initial the rest api.
    curl.reset(new KNRestApiBase);
    //Prepare the response data.
    QByteArray responseData;
    //Get the album detail info.
    if(curl->get(generateNeteaseRequest(QString("http://music.163.com/api/"
                                                "album/%1/").arg(albumId)),
                 responseData,
                 false)==200)
    {
        qDebug()<<"Fetching song detail information.";
        //Parse the response data to JSON object.
        QJsonObject albumData=QJsonDocument::fromJson(responseData).object();
        //The only important data is the album data.
        if(!albumData.contains("album"))
        {
            //Failed to fetch album data.
            return;
        }
        //Get the album data.
        albumData=albumData.value("album").toObject();
        //Save the information to album model.
        if(albumDetail.isNull())
        {
            //Ignore the settings.
            return;
        }
        //Save the information to album detail model.
        albumDetail->setAlbumInfo(KNMusicStoreUtil::AlbumTitle,
                                  albumData.value("name").toString());
        albumDetail->setAlbumInfo(KNMusicStoreUtil::AlbumArtist,
                                  albumData.value("artist").toObject().value(
                                      "name").toString());
        albumDetail->setAlbumInfo(KNMusicStoreUtil::AlbumReleaseCompany,
                                  albumData.value("company").toString());
        albumDetail->setAlbumInfo(
                    KNMusicStoreUtil::AlbumReleaseTime,
                    QDateTime::fromMSecsSinceEpoch(
                        albumData.value("publishTime").toDouble()).toString(
                        "yyyy-MM-dd"));
        //Get the music song data.
        QJsonArray songArray=albumData.value("songs").toArray();
        //Loop for all information.
        for(int i=0; i<songArray.size(); ++i)
        {
            qDebug()<<"Fetching song"<<i;
            //Get the song object.
            QJsonObject songData=songArray.at(i).toObject();
            //Generate the song detail info.
            KNMusicStoreUtil::StoreSongItem item;
            //Save the song information.
            item.name=songData.value("name").toString();
            //Combine the artists names.
            item.artist=getArtistNames(songData);
            item.duration=getDuration(songData);
            item.index=songData.value("no").toInt();
            item.songData=qint64(songData.value("id").toDouble());
            //Set the download url.
            setDownloadUrl(songData,
                           item.urlOnline,
                           item.urlHigh,
                           item.urlLossless);
            //Check detail information.
            if(albumDetail.isNull())
            {
                //Ignore the information.
                return;
            }
            //Append item to album detail.
            albumDetail->appendItem(item);
        }
        qDebug()<<"Fetching album art.";
        //Download the image.
        if(curl->get(albumData.value("picUrl").toString(),
                     responseData)==200 &&
                (!albumDetail.isNull()))
        {
            //Save the album data.
            albumDetail->setAlbumArt(generateAlbumArt(responseData,
                                                      219, 219));
            //Emit finished signal.
            emit albumFetchComplete();
            qDebug()<<"Album Fetch Finished.";
            //Reset the launching instance to null.
            m_launchingInstance=StateNull;
        }
    }
}

void KNMusicStoreNetease::updateSongDetail(
        QPointer<KNMusicStoreSongDetailInfo> songDetail,
        const QString &songId)
{
    qDebug()<<"Song Fetch Start.";
    //Check song detail pointer.
    if(songDetail.isNull())
    {
        //Failed to fetch the song detail.
        return;
    }
    //Reset the song detail.
    songDetail->reset();
    //Prepare the rest api.
    QScopedPointer<KNRestApiBase> curl;
    //Initial the rest api.
    curl.reset(new KNRestApiBase);
    //Get the song detail.
    QJsonObject songInfo=getSongDetails(curl.data(), songId);
    //Check the pointer.
    if(songDetail.isNull())
    {
        //Song detail model has been removed.
        return;
    }
    //Parse the song detail.
    //Check the song data.
    if(!songInfo.contains("songs"))
    {
        //Failed to fetch the songs.
        return;
    }
    //Get the songs list
    QJsonArray songsArray=songInfo.value("songs").toArray();
    //Check the size.
    if(songsArray.isEmpty())
    {
        //Failed to fetch the songs.
        return;
    }
    //Get song data.
    QJsonObject songData=songsArray.at(0).toObject();
    //Prepare the album data and lyrics data.
    QByteArray albumArtData, lyricsData;
    //Check information and download album data & lyrics.
    if(songData.contains("album") &&
            curl->get(songData.value("album").toObject().value(
                          "picUrl").toString(),
                      albumArtData)==200 &&
            curl->get(generateNeteaseRequest(
                          QString("http://music.163.com/api/song/lyric?os=osx"
                                  "&id=%1&lv=-1&kv=-1&tv=-1").arg(
                              QString::number(
                                  qint64(songData.value("id").toDouble())))),
                      lyricsData)==200)
    {
        //Check the pointer.
        if(songDetail.isNull())
        {
            //Song detail model has been removed.
            return;
        }
        //Get the lyrics response data.
        QJsonObject lyricsResponse=QJsonDocument::fromJson(lyricsData).object();
        //Check the lyrics response data.
        if(lyricsResponse.contains("lrc"))
        {
            //Get the lrc object.
            lyricsResponse=lyricsResponse.value("lrc").toObject();
            //Get the lyrics data from the object.
            songDetail->setSongData(KNMusicStoreSongDetailInfo::Lyrics,
                                    lyricsResponse.value("lyric").toString());
        }
        //Set the pixmap data.
        songDetail->setAlbumArt(generateAlbumArt(albumArtData, 219, 219));
        //Save all the other information.
        songDetail->setSongData(KNMusicStoreSongDetailInfo::Name,
                                  songData.value("name").toString());
        {
            //Get artist list.
            QJsonArray artistList=songData.value("artists").toArray();
            //Get the artist information.
            QStringList artistNames, artistIds;
            for(auto j=0; j<artistList.size(); ++j)
            {
                //Get the current artist.
                QJsonObject artistObject=artistList.at(j).toObject();
                //Add name to artist list.
                artistNames.append(artistObject.value("name").toString());
                artistIds.append(QString::number(static_cast<qint64>(
                                         artistObject.value("id").toDouble())));
            }
            //Set the artist information.
            songDetail->setArtists(artistNames);
            songDetail->setArtistsId(artistIds);
        }
        //Get the album information.
        {
            //Get album object.
            QJsonObject albumObject=songData.value("album").toObject();
            //Get the album id and name.
            songDetail->setSongData(KNMusicStoreSongDetailInfo::AlbumName,
                                    albumObject.value("name").toString());
            songDetail->setSongData(
                        KNMusicStoreSongDetailInfo::AlbumId,
                        QString::number(
                            static_cast<qint64>(albumObject.value(
                                                    "id").toDouble())));
        }
        //Get the url information.
        {
            QString urlOnline, urlHigh, urlLossless;
            //Set the download url to cache string.
            setDownloadUrl(songData,
                           urlOnline,
                           urlHigh,
                           urlLossless);
            //Set the data to song detail.
            songDetail->setSongData(KNMusicStoreSongDetailInfo::OnlineUrl,
                                    urlOnline);
            songDetail->setSongData(KNMusicStoreSongDetailInfo::HighUrl,
                                    urlHigh);
            songDetail->setSongData(KNMusicStoreSongDetailInfo::LossLessUrl,
                                    urlLossless);
        }
        //Emit the complete signal.
        emit songFetchComplete();
        //Reset the launching instance to null.
        m_launchingInstance=StateNull;
    }
}

void KNMusicStoreNetease::onActionWatcherFinished()
{
    //Lock for the home widget.
    m_homeFeteching.lock();
    //Increase the counter.
    ++m_homeLocking;
    qDebug()<<m_homeLocking;
    //Check home locking counter.
    if(m_homeLocking==NeteaseWorkThreadCount)
    {
        //Emit the home fetching signal.
        emit homeFetchComplete();
        qDebug()<<"Finished!";
        //Reset the launching instance to null.
        m_launchingInstance=StateNull;
    }
    //Release the lock.
    m_homeFeteching.unlock();
}

inline void KNMusicStoreNetease::quitWorkingThread(QFuture<void> &workThread)
{
    //Check list threads working thread.
    if(workThread.isRunning())
    {
        //Cancel and wait for thread stop.
        workThread.cancel();
        //Wait for cancel.
        workThread.waitForFinished();
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

inline QJsonObject KNMusicStoreNetease::getSongDetails(KNRestApiBase *curl,
                                                       const QString &songId)
{
    //Generate the response data.
    QByteArray responseData;
    //Get the song object.
    if(curl->get(generateNeteaseRequest(QString("http://music.163.com/api/song/"
                                                "detail?ids=[%1]").arg(songId)),
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
        const QByteArray &albumArtData,
        int width,
        int height)
{
    return QPixmap::fromImage(QImage::fromData(albumArtData)).scaled(
                width,
                height,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
}

QString KNMusicStoreNetease::encryptedId(const QString &songId)
{
    //Based on https://github.com/yanunon/NeteaseCloudMusic
    //Translate song id into byte array.
    QByteArray rawId=songId.toLatin1(),
               encryptedData;
    //For all char in song id, do ^ with magic data.
    for(int i=0; i<rawId.size(); ++i)
    {
        //Encrypted the raw id.
        encryptedData.append(rawId.at(i) ^
                             m_magicData.at(i % m_magicDataLength));
    }
    //Do MD5 with the encrypted data. Encode the middle with Base64.
    QByteArray middle=
            QCryptographicHash::hash(encryptedData,
                                     QCryptographicHash::Md5).toBase64();
    //Remove the last byte.
    middle.resize(middle.size()-1);
    //Replace the specific byte.
    middle.replace('/', '_');
    middle.replace('+', '-');
    //Give back the result.
    return middle;
}

inline void KNMusicStoreNetease::setDownloadUrl(const QJsonObject &songItem,
                                                QString &urlOnline,
                                                QString &urlHigh,
                                                QString &urlLossless)
{
    //Save the online url for all first.
    QString songUrl=songItem.value("mp3Url").toString();
    urlOnline=songUrl;
    urlHigh=songUrl;
    urlLossless=songUrl;
    //Save the high quality url.
    if(songItem.contains("hMusic"))
    {
        //Get the high quality object.
        QJsonObject highMusic=songItem.value("hMusic").toObject();
        //Get the encrypted id.
        QString songId=QString::number(
                    highMusic.value("dfsId").toDouble()),
                encId=encryptedId(songId);
        //Then the url will be compose as
        QString highUrl=
                QString("http://m%1.music.126.net/%2/%3.mp3").arg(
                    QString::number((qrand() % 3)+1),
                    encId,
                    songId);
        //Save the high url for url high and url lossless.
        urlHigh=highUrl;
        urlLossless=highUrl;
    }
}

inline QString KNMusicStoreNetease::getArtistNames(const QJsonObject &songData)
{
    //Get the artist list.
    const QJsonArray &artists=songData.value("artists").toArray();
    //Generate artist names list.
    QStringList artistsNames;
    //Get the artist names.
    for(int j=0; j<artists.size(); ++j)
    {
        //Add name to artist names.
        artistsNames.append(artists.at(j).toObject().value("name").toString());
    }
    //Give back the join data.
    return artistsNames.join(", ");
}

QString KNMusicStoreNetease::getDuration(const QJsonObject &songData)
{
    return KNMusicUtil::msecondToString(songData.value("duration").toDouble());
}
