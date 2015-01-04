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
#include <QNetworkCookie>
#include <QUrl>

#include "knmusicneteaselyrics.h"

#include <QDebug>

KNMusicNeteaseLyrics::KNMusicNeteaseLyrics(QObject *parent):
    KNMusicLyricsDownloader(parent)
{
    ;
}

void KNMusicNeteaseLyrics::downloadLyrics(const KNMusicDetailInfo &detailInfo,
                                        QList<KNMusicLyricsDetails> &lyricsList)
{
    //Generate cookies.
    QNetworkCookie neteaseCookie;
    neteaseCookie.setName("appver");
    neteaseCookie.setValue("2.0.2;");
    QVariant neteaseCookieData;
    neteaseCookieData.setValue(neteaseCookie);
    //Generate search url.
    QString musicSearchURL="http://music.163.com/api/search/get/";
    QByteArray responseData, parameter;
    parameter.append("limit=20&offset=0&type=1&s="+
                     QUrl::toPercentEncoding(detailInfo.textLists[Name]));
    qDebug()<<parameter;
    post(musicSearchURL,
         responseData,
         parameter,
         neteaseCookieData,
         "http://music.163.com/");
    qDebug()<<responseData;
}
