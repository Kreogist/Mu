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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

#include "knmusiclyricsdownloader.h"

#define TimerInterval 3000
#define MaxTimeoutCount 5

KNMusicLyricsDownloader::KNMusicLyricsDownloader(QObject *parent) :
    QObject(parent),
    m_accessManager(new QNetworkAccessManager()),
    m_timeoutCounter(new QTimer())
{
    //Configure the timer.
    m_timeoutCounter->setInterval(TimerInterval);
    connect(m_timeoutCounter, &QTimer::timeout,
            this, &KNMusicLyricsDownloader::onTimeoutCount);
    //Link the access manager finish reply to the slot.
    connect(m_accessManager, &QNetworkAccessManager::finished,
            this, &KNMusicLyricsDownloader::onReplyFinished);
}

KNMusicLyricsDownloader::~KNMusicLyricsDownloader()
{
    //Stop the counter first.
    m_timeoutCounter->stop();
    //Delete the access manager.
    m_accessManager->deleteLater();
    //Delete the timer.
    m_timeoutCounter->deleteLater();
}

void KNMusicLyricsDownloader::setWorkingThread(QThread *thread)
{
    //Move the downloader to the thread.
    moveToThread(thread);
    //Move the children to the thread.
    m_accessManager->moveToThread(thread);
    m_timeoutCounter->moveToThread(thread);
}

void KNMusicLyricsDownloader::downloadLyrics(
        uint identifier,
        const KNMusicDetailInfo &detailInfo)
{
    //Check whether the identifier appears in the map or not.
    if(m_sourceMap.contains(identifier))
    {
        //Pick out the request source from the map.
        KNMusicLyricsRequestSource source=m_sourceMap.value(identifier);
        //Check the source working state.
        if(source.isWorking)
        {
            //Need to cancel the source first.
            return;
        }
        //Create the new step.
        KNMusicLyricsStep request;
        //Save the detail info as the current working info.
        request.detailInfo=detailInfo;
        //Update the source.
        source.currentStep=request;
        //Set the working flag.
        source.isWorking=true;
        //Update the source to the map.
        m_sourceMap.insert(identifier, source);
        //Start the first step.
        initialStep(identifier, detailInfo);
    }
    else
    {
        //Start the new queue for the request source.
        KNMusicLyricsRequestSource source;
        //Save the detail info as the current working info.
        source.currentStep.detailInfo=detailInfo;
        //Set the working flag.
        source.isWorking=true;
        //Append the source to the map.
        m_sourceMap.insert(identifier, source);
        //Start the first step.
        initialStep(identifier, detailInfo);
    }
}

void KNMusicLyricsDownloader::cancel(uint identifier)
{
    //Check whether the identifier appears in the map or not.
    if(!m_sourceMap.contains(identifier))
    {
        //Ignore the invalid request.
        return;
    }
    //Pick out the request source from the map.
    KNMusicLyricsRequestSource source=m_sourceMap.value(identifier);
    //Check the working state.
    if(!source.isWorking)
    {
        //Source is not actually working.
        return;
    }
    //Pick out the current step.
    KNMusicLyricsStep &currentStep=source.currentStep;
    //Remove all the replies from the map.
    for(auto reply : currentStep.replies)
    {
        //Remove i from the reply map.
        m_replyMap.remove(reply);
        //Abort the request.
        reply->abort();
    }
    //Clear the current step.
    source.currentStep=KNMusicLyricsStep();
    source.isWorking=false;
    //Update the source.
    m_sourceMap.insert(identifier, source);
}

void KNMusicLyricsDownloader::saveLyrics(uint identifier,
                                         const QString &title,
                                         const QString &artist,
                                         const QString &content)
{
    //Get the source request.
    //Check identifier existance.
    if(!m_sourceMap.contains(identifier))
    {
        //Ignore the invalid request.
        return;
    }
    //Pick out the request source from the map.
    KNMusicLyricsRequestSource source=m_sourceMap.value(identifier);
    //Get the current step.
    KNMusicLyricsStep &currentStep=source.currentStep;
    //Okay, we could find the detail info here.
    const KNMusicDetailInfo &detailInfo=currentStep.detailInfo;
    //Save the content data of the lyrics.
    KNMusicLyricsDetails lyricsDetail;
    lyricsDetail.title=title;
    lyricsDetail.artist=artist;
    lyricsDetail.lyricsData=content;
    //Calculate the similarity of the title and artist.
    lyricsDetail.titleSimilarity=
            KNUtil::similarity(lyricsDetail.title,
                               detailInfo.textLists[Name].toString());
    lyricsDetail.artistSimilarity=
            KNUtil::similarity(lyricsDetail.artist,
                               detailInfo.textLists[Artist].toString());
    //Add to list.
    currentStep.lyricsList.append(lyricsDetail);
    //Update the source map.
    m_sourceMap.insert(identifier, source);
}

void KNMusicLyricsDownloader::completeRequest(uint identifier)
{
    //Check identifier existance.
    if(!m_sourceMap.contains(identifier))
    {
        //Ignore the invalid request.
        return;
    }
    //Pick out the request source from the map.
    KNMusicLyricsRequestSource source=m_sourceMap.value(identifier);
    //Update the working state.
    source.isWorking=false;
    //Update the map.
    m_sourceMap.insert(identifier, source);
    //Emit the signal for the data.
    emit missionComplete(identifier, source.currentStep.detailInfo,
                         source.currentStep.lyricsList);
}

void KNMusicLyricsDownloader::setReplyCount(uint identifier, int replyCount)
{
    //Check identifier existance.
    if(!m_sourceMap.contains(identifier))
    {
        //Ignore the invalid request.
        return;
    }
    //Pick out the request source from the map.
    KNMusicLyricsRequestSource source=m_sourceMap.value(identifier);
    //The reply count could only be set when it is 0 and list is empty.
    if(!source.currentStep.replies.isEmpty() ||
            source.currentStep.replyCount!=0)
    {
        //To protect the current working data.
        return;
    }
    //Update the reply count.
    source.currentStep.replyCount=replyCount;
    //Update the source map.
    m_sourceMap.insert(identifier, source);
}

void KNMusicLyricsDownloader::get(uint identifier,
                                  const QNetworkRequest &request,
                                  const QVariant &user)
{
    //Check identifier existance.
    if(!m_sourceMap.contains(identifier))
    {
        //Ignore the invalid request.
        return;
    }
    //Pick out the request source from the map.
    KNMusicLyricsRequestSource source=m_sourceMap.value(identifier);
    //Get the current step.
    KNMusicLyricsStep &currentStep=source.currentStep;
    //Start to get the data.
    QNetworkReply *reply=m_accessManager->get(request);
    //Append the reply to the reply map.
    KNMusicRequestData replyData;
    replyData.identifier=identifier;
    replyData.user=user;
    m_replyMap.insert(reply, replyData);
    //Append the reply to the reply list.
    currentStep.replies.append(reply);
    //Update the map.
    m_sourceMap.insert(identifier, source);
    //Star the timeout timer.
    m_timeoutCounter->start();
}

void KNMusicLyricsDownloader::get(uint identifier, const QString &url,
                                  const QVariant &user)
{
    //Construct the request.
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    //Do the HTTP GET operation.
    get(identifier, request, user);
}

void KNMusicLyricsDownloader::post(uint identifier,
                                   const QNetworkRequest &request,
                                   const QByteArray &content,
                                   const QVariant &user)
{
    //Check identifier existance.
    if(!m_sourceMap.contains(identifier))
    {
        //Ignore the invalid request.
        return;
    }
    //Pick out the request source from the map.
    KNMusicLyricsRequestSource source=m_sourceMap.value(identifier);
    //Get the current step.
    KNMusicLyricsStep &currentStep=source.currentStep;
    //Construct the request.
    //Start to get the data.
    QNetworkReply *reply=m_accessManager->post(request, content);
    //Append the reply to the reply map.
    KNMusicRequestData replyData;
    replyData.identifier=identifier;
    replyData.user=user;
    m_replyMap.insert(reply, replyData);
    //Append the reply to the reply list.
    currentStep.replies.append(reply);
    //Update the map.
    m_sourceMap.insert(identifier, source);
    //Star the timeout timer.
    m_timeoutCounter->start();
}

void KNMusicLyricsDownloader::onTimeoutCount()
{
    //Check all the reply in the map, increase its counter.
    QList<QNetworkReply *> replyList=m_replyMap.keys();
    //Check the reply list size.
    if(replyList.isEmpty())
    {
        //No need to start the counter.
        m_timeoutCounter->stop();
    }
    //Check all the reply in the list.
    for(auto reply:replyList)
    {
        //Check the reply timeout in the map.
        KNMusicRequestData requestData=m_replyMap.value(reply);
        //Increase the counter.
        ++requestData.timeout;
        //Update the request data.
        m_replyMap.insert(reply, requestData);
        //Check the counter.
        if(MaxTimeoutCount==requestData.timeout)
        {
            //Cancel the data.
            //It will still emit the finish signal, and the finished slot will
            //process its data.
            reply->abort();
        }
    }
}

void KNMusicLyricsDownloader::onReplyFinished(QNetworkReply *reply)
{
    //Check the reply data in the reply map.
    if(!m_replyMap.contains(reply))
    {
        //Ignore the reply which is not in the map (cancelled reply).
        return;
    }
    //Pick out the hash data of the identifier.
    KNMusicRequestData &&requestData=m_replyMap.take(reply);
    uint identifier=requestData.identifier;
    //Get the request source struct.
    KNMusicLyricsRequestSource requestSource=m_sourceMap.value(identifier);
    //Check the current step data.
    KNMusicLyricsStep &currentStep=requestSource.currentStep;
    if(!currentStep.replies.contains(reply))
    {
        //Ignore the reply, the reply should be cancelled.
        return;
    }
    //Read all the data from the reply.
    KNMusicReplyData replyData;
    replyData.result=reply->readAll();
    replyData.user=requestData.user;
    currentStep.replyCaches.append(replyData);
    //Remove the reply from the list.
    currentStep.replies.removeOne(reply);
    //Reduce the count.
    --currentStep.replyCount;
    //Check the reply list size, when all the reply has arrived.
    if(currentStep.replyCount==0)
    {
        //Stop the counter.
        m_timeoutCounter->stop();
        //Increase the step index.
        ++currentStep.currentStep;
        //Save the reply caches.
        QList<KNMusicReplyData> replyResults=currentStep.replyCaches;
        //Clear the reply caches.
        currentStep.replyCaches=QList<KNMusicReplyData>();
        //Update the request source.
        m_sourceMap.insert(identifier, requestSource);
        //Move to next step, however, this will be the responsibility of the
        //specific downloader.
        processStep(identifier, currentStep.currentStep, replyResults);
    }
    else
    {
        //Simply update the request source.
        m_sourceMap.insert(identifier, requestSource);
    }
}
