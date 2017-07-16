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

#include <QNetworkReply>

#include "knutil.h"
#include "knmusicutil.h"

#include "knrestapibase.h"

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

    /*!
     * \brief Chagne the working thread of the downloader. Do NOT use
     * moveToThread() function, use this function instead.
     * \param thread The working thread of the downloader.
     */
    void setWorkingThread(QThread *thread);

signals:
    /*!
     * \brief When one lyrics search mission is completed, this signal will be
     * emitted.
     * \param identifier The request object identifier.
     * \param detailInfo The detail info of the music.
     * \param lyricsList The lyrics data search result list.
     */
    void missionComplete(int identifier,
                         KNMusicDetailInfo detailInfo,
                         QList<KNMusicLyricsDetails> lyricsList);

public slots:
    /*!
     * \brief Download lyrics from the specific site according to the detail
     * info of a song.
     * \param identifier The request source identifier
     * \param detailInfo The detail info of a song.
     */
    void downloadLyrics(uint identifier,
                        const KNMusicDetailInfo &detailInfo);

    /*!
     * \brief Clear the download cache of one request source.
     * \param identifier The identifier of the request source.
     */
    void cancel(uint identifier);

protected:
    struct KNMusicReplyData
    {
        QByteArray result;
        QVariant user;
    };

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
     * \brief Save a lyrics into the current request source.
     * \param identifier The request identifier.
     * \param title The title of the lyrics file.
     * \param artist The artist of the lyrics file.
     * \param content The content of the lyrics.
     */
    void saveLyrics(uint identifier,
                    const QString &title,
                    const QString &artist,
                    const QString &content);

    /*!
     * \brief When one mission is complete, this function should be called.
     * \param identifier The source identifier.
     */
    void completeRequest(uint identifier);

    /*!
     * \brief This is the initial step of the whole download session.
     * \param identifier The identifier.
     * \param detailInfo The music information.
     */
    virtual void initialStep(uint identifier,
                             const KNMusicDetailInfo &detailInfo)=0;

    /*!
     * \brief Process the next step. The current step is an integer which is
     * guaranteed larger than 0.
     * \param identifier The identifier of the source.
     * \param currentStep The current step index.
     * \param replyCaches The data of the reply caches.
     */
    virtual void processStep(uint identifier,
                             int currentStep,
                             const QList<KNMusicReplyData> &replyCaches)=0;

    /*!
     * \brief Set the reply total count of the current step.
     * \param identifier The request source identifier.
     * \param replyCount The count of the replies.
     */
    void setReplyCount(uint identifier, int replyCount);

    /*!
     * \brief Do a HTTP GET request, and save the data.
     * \param identifier The source identifier.
     * \param url The url of the GET request.
     * \param user Any data that need to be provided when giving the reply.
     */
    void get(uint identifier,
             const QNetworkRequest &request,
             const QVariant &user=QVariant());

    /*!
     * \brief This is an override function. Do a HTTP GET request, and save the
     * data. This will use the default request, so only need to provide a URL.
     * \param identifier The source identifier.
     * \param url The url of the GET request.
     * \param user Any data that need to be provided when giving the reply.
     */
    void get(uint identifier, const QString &url,
             const QVariant &user=QVariant());

    /*!
     * \brief Do a HTTP POST request, and save the data.
     * \param identifier The source identifier.
     * \param request The post request object.
     * \param content The content need to be posted.
     * \param user Any data that need to be provided when giving the reply.
     */
    void post(uint identifier,
              const QNetworkRequest &request,
              const QByteArray &content,
              const QVariant &user=QVariant());

private slots:
    void onTimeoutCount();
    void onReplyFinished(QNetworkReply *reply);

private:
    struct KNMusicLyricsStep
    {
        QList<KNMusicLyricsDetails> lyricsList;
        QList<QNetworkReply *> replies;
        QList<KNMusicReplyData> replyCaches;
        QList<QVariant> replyUserCaches;
        KNMusicDetailInfo detailInfo;
        int currentStep, replyCount;
        KNMusicLyricsStep() :
            currentStep(0),
            replyCount(0)
        {
        }
    };

    struct KNMusicLyricsRequestSource
    {
        KNMusicLyricsStep currentStep;
        bool isWorking;
        KNMusicLyricsRequestSource() :
            isWorking(false)
        {
        }
    };

    struct KNMusicRequestData
    {
        QVariant user;
        uint identifier;
        int timeout;
        KNMusicRequestData() :
            identifier(0),
            timeout(0)
        {
        }
    };

    QNetworkAccessManager *m_accessManager;
    QMap<QNetworkReply *, KNMusicRequestData> m_replyMap;
    QMap<uint, KNMusicLyricsRequestSource> m_sourceMap;
    QTimer *m_timeoutCounter;
};

#endif // KNMUSICLYRICSDOWNLOADER_H
