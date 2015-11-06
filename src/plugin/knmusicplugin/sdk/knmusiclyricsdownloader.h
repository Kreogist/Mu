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

#ifndef KNMUSICLYRICSDOWNLOADER_H
#define KNMUSICLYRICSDOWNLOADER_H

#include <QNetworkRequest>

#include "knutil.h"
#include "knmusicutil.h"
#include "knconnectionhandler.h"

#include <QObject>

using namespace MusicUtil;

class QNetworkAccessManager;
/*!
 * \brief The KNMusicLyricsDownloader class provides the interface function of a
 * lyrics download plugin and some handy function to download the lyrics.
 */
class KNMusicLyricsDownloader : public QObject
{
    Q_OBJECT
public:
    struct KNMusicLyricsDetails
    {
        QString title;          // The song title of the lyrics
        QString artist;         // The song artist of the lyrics
        QString lyricsData;     // The lyrics text data.
        int titleSimilarity;    // The similarity of the lyrics title to the
                                // title of the detail info.
        int artistSimilarity;   // The similarity of the lyrics artist.
        KNMusicLyricsDetails() :
            title(QString()),
            artist(QString()),
            lyricsData(QString()),
            titleSimilarity(-1),
            artistSimilarity(-1)
        {
        }
    };

    /*!
     * \brief Construct a KNMusicLyricsDownloader for all basic functions of
     * downloaders plugins.
     * \param parent The parent object.
     */
    explicit KNMusicLyricsDownloader(QObject *parent = 0);
    ~KNMusicLyricsDownloader();

    /*!
     * \brief Provide the name of the lyrics downloader.
     * \return The name of the downloader.
     */
    virtual QString downloaderName()=0;

    /*!
     * \brief Chagne the working thread of the downloader. Do NOT use
     * moveToThread() function, use this function instead.
     * \param thread The working thread of the downloader.
     */
    void setWorkingThread(QThread *thread);

    /*!
     * \brief This function provide how to compare a lyrics details. Check the
     * similarity of the title and artist.
     * \param lyricsDetailLeft The first lyrics detail.
     * \param lyricsDetailRight The second lyrics detail.
     * \return If the first lyrics detail is smaller than the second one, return
     * true.
     */
    static bool lyricsDetailLessThan(
            const KNMusicLyricsDetails &lyricsDetailLeft,
            const KNMusicLyricsDetails &lyricsDetailRight)
    {
        return (lyricsDetailLeft.titleSimilarity ==
                lyricsDetailRight.titleSimilarity)?
                        lyricsDetailLeft.artistSimilarity <
                            lyricsDetailRight.artistSimilarity:
                        lyricsDetailLeft.titleSimilarity <
                            lyricsDetailRight.titleSimilarity;
    }

signals:

public slots:
    /*!
     * \brief Download lyrics from the specific site according to the detail
     * info of a song.
     * \param detailInfo The detail info of a song.
     */
    virtual void downloadLyrics(const KNMusicDetailInfo &detailInfo,
                                QList<KNMusicLyricsDetails> &lyricsList)=0;

protected:
    /*!
     * \brief Process the original key words and remove the data which cannot be
     * appear in the URL.
     * \param keyword The prefer keyword.
     * \return The processed keyword data.
     */
    inline QString processKeywords(QString keyword)
    {
        //Clear some no used words. I don't know how these regexp works.
        //Magic, don't touch.
        keyword=keyword.toLower();
        keyword.replace('.', " ");
        keyword.replace('&', " ");
        //truncate all symbols
        keyword.replace(QRegExp("\\(.*?\\)|\\[.*?]|{.*?}|\\uff08.*?\\uff09"),
                        " ");
        keyword.replace(QRegExp("[-/:-@[-`{-~]+"), " ");
        return keyword;
    }

    /*!
     * \brief Save a lyrics with the title, artist and content data to the
     * lyrics list. It will automatically generate a lyrics detail structure and
     * calcualte the similarity of the title and artist.
     * \param title The title of lyrics.
     * \param artist The artist of lyrics.
     * \param content The content of the lyrics.
     * \param targetTitle The target title which will be used to calculate the
     * similarity.
     * \param targetArtist The target artist which will be used to calculate the
     * similarity.
     * \param lyricsList The lyrics detail list which the detail will be added
     * to.
     */
    inline void saveLyrics(const QString &title,
                           const QString &artist,
                           const QString &content,
                           const QString &targetTitle,
                           const QString &targetArtist,
                           QList<KNMusicLyricsDetails> &lyricsList)
    {
        //Generate the lyrics details.
        KNMusicLyricsDetails lyricsDetails;
        //Save the title and artist name.
        lyricsDetails.title=title;
        lyricsDetails.artist=artist;
        //Save the content data of the lyrics.
        lyricsDetails.lyricsData=content;
        //Calculate the similarity of the title and artist.
        lyricsDetails.titleSimilarity=
                KNUtil::similarity(lyricsDetails.title, targetTitle);
        lyricsDetails.artistSimilarity=
                KNUtil::similarity(lyricsDetails.artist, targetArtist);
        //Add to list.
        lyricsList.append(lyricsDetails);
    }

    /*!
     * \brief This is a override function.\n
     * Save a lyrics to the lyrics list. Calculate the similarity with a music
     * detail info.
     * \param detailInfo Provide the target title and artist which will be used
     * to calculate the similarity.
     * \param content The content of the lyrics.
     * \param lyricsDetails The lyrics detail struct. The title and artist
     * should be stored.
     * \param lyricsList The lyrics detail list which the detail will be added
     * to.
     */
    inline void saveLyrics(const KNMusicDetailInfo &detailInfo,
                           const QString &content,
                           KNMusicLyricsDetails &lyricsDetails,
                           QList<KNMusicLyricsDetails> &lyricsList)
    {
        //Save the content data of the lyrics.
        lyricsDetails.lyricsData=content;
        //Calculate the similarity of the title and artist.
        lyricsDetails.titleSimilarity=
                KNUtil::similarity(lyricsDetails.title,
                                   detailInfo.textLists[Name].toString());
        lyricsDetails.artistSimilarity=
                KNUtil::similarity(lyricsDetails.artist,
                                   detailInfo.textLists[Artist].toString());
        //Add to list.
        lyricsList.append(lyricsDetails);
    }

    /*!
     * \brief Http get request via given the request. This function works in
     * stucked way.
     * \param request The requst of the get request.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     */
    void get(const QNetworkRequest &request,
             QByteArray &responseData);

    /*!
     * \brief This is a override function.\n
     * Http get request via given the url, cookie and referer. This
     * function works in stucked way.\n
     * The parameters provided in this function is used to generate the request.
     * \param url The url of the get target.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     * \param cookie The cookie which will be use on the request header. By
     * default it can leave blank to use the default one provided by Qt.
     * \param referer The referer parameter.
     */
    inline void get(const QString &url,
                    QByteArray &responseData,
                    const QVariant &cookie=QVariant(),
                    const QString &referer=QString())
    {
        get(generateRequest(url, cookie, referer), responseData);
    }

    /*!
     * \brief Http post request via given the request. This function works in
     * stucked way.
     * \param request The request of the post request.
     * \param parameter The parameter which will be used when doing the post
     * request.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     */
    void post(QNetworkRequest request,
              const QByteArray &parameter,
              QByteArray &responseData);

    /*!
     * \brief This is a override function.\n
     * Http post request via given the request. This function works in stucked
     * way. The url, cookie and referer is used to generate the request header.
     * The 'parameter' is the parameter when doing the post.
     * \param url The url of the post target.
     * \param responseData The response data byte array which will be used to
     * receive the data from the get request.
     * \param parameter The parameter which will be used when doing the post
     * request.
     * \param cookie The cookie which will be use on the request header. By
     * default it can leave blank to use the default one provided by Qt.
     * \param referer The referer parameter.
     */
    inline void post(const QString &url,
                     QByteArray &responseData,
                     const QByteArray &parameter,
                     const QVariant &cookie=QVariant(),
                     const QString &referer=QString())
    {
        post(generateRequest(url, cookie, referer), parameter, responseData);
    }

    /*!
     * \brief Generate a request object by providing the url, cookie and the
     * referer.
     * \param url The request target url.
     * \param cookie The custom cookie. Default it is null, it will the default
     * cookie provided by Qt.
     * \param referer The custom referer. Default it's null, it will use no
     * referer to this request(Qt default).
     * \return The request object.
     */
    QNetworkRequest generateRequest(const QString &url,
                                    const QVariant &cookie=QVariant(),
                                    const QString &referer=QString());

private:
    QNetworkAccessManager *m_networkManager;
    KNConnectionHandler m_timeoutHandler;
};

#endif // KNMUSICLYRICSDOWNLOADER_H
