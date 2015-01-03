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
#include <QTextCodec>
#include <QRegularExpression>

#include "knmusicbaidulyrics.h"

#include <QDebug>

KNMusicBaiduLyrics::KNMusicBaiduLyrics(QObject *parent):
    KNMusicLyricsDownloader(parent)
{
    //Initial GBK codec.
    m_gbkCodec=QTextCodec::codecForName("GBK");
}

void KNMusicBaiduLyrics::downloadLyrics(const KNMusicDetailInfo &detailInfo,
                                        QList<KNMusicLyricsDetails> &lyricsList)
{
    //Generate the song info base url.
    QString url="http://box.zhangmen.baidu.com/x?title=" +
            processKeywords(detailInfo.textLists[Name]).toUtf8().toPercentEncoding() +
            "$$" +
            processKeywords(detailInfo.textLists[Artist]).toUtf8().toPercentEncoding() +
            "$$$$&op=12&count=1&format=json";
    QByteArray responseData;
    //Get the data.
    get(url, responseData);
    if(responseData.isEmpty())
    {
        return;
    }
    //Find lyric id.
    QString responseText=responseData;
    QRegularExpression lyricsID("<lrcid>([0-9]*)</lrcid>");
    QRegularExpressionMatchIterator i=lyricsID.globalMatch(responseText);
    if(!i.hasNext())
    {
        return;
    }
    //Use the first id.
    QString currentID=i.next().captured(1),
            lyricsUrl="http://box.zhangmen.baidu.com/bdlrc/" +
                      QString::number(currentID.toLongLong()/100) +
                      "/"+
                      currentID+
                      ".lrc";
    //Get the lyrics!
    get(lyricsUrl, responseData);
    if(responseData.isEmpty())
    {
        return;
    }
    //Change the codec from GBK to UTF-8.
//    return writeLyricsFile(detailInfo,
//                           m_gbkCodec->toUnicode(responseData));
}
