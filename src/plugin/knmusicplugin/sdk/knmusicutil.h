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

#ifndef KNMUSICUTIL
#define KNMUSICUTIL

#include <QString>
#include <QDateTime>
#include <QImage>
#include <QMap>
#include <QList>
#include <QByteArray>

namespace MusicUtil
{
    enum MusicDatas
    {
        Name,
        Album,
        AlbumArtist,
        AlbumRating,
        Artist,
        BeatsPerMinuate,
        BitRate,
        Category,
        Comments,
        Composer,
        DateAdded,
        DateModified,
        Description,
        DiscCount,
        DiscNumber,
        Genre,
        Kind,
        LastPlayed,
        Plays,
        Rating,
        SampleRate,
        Size,
        Time,
        TrackCount,
        TrackNumber,
        Year,
        MusicDataCount
    };
    struct KNMusicDetailInfo
    {
        //Properties.
        int rating=0;
        int trackIndex=-1;
        QString fileName;
        QString filePath;
        QString trackFilePath;
        QDateTime dateModified;
        QDateTime dateLastPlayed;
        QDateTime dateAdded;
        quint64 size;
        //Music properties.
        qint64 startPosition=-1;
        qint64 duration=0;
        qint64 bitRate=0;
        qint64 samplingRate=0;
        //Image hash data.
        QString coverImageHash;
        //Tag datas.
        QString textLists[MusicDataCount];
    };
    struct KNMusicAnalysisItem
    {
        KNMusicDetailInfo detailInfo;
        //Album art data.
        QImage coverImage;
        QMap<QString, QList<QByteArray>> imageData;
    };
    struct KNMusicListTrackDetailInfo
    {
        //Track index.
        int index;
        //Track time.
        qint64 startPosition=-1;
        qint64 trackDuration=-1;
        //Metadata map.
        QMap<int, QString> metaData;
    };
    struct KNMusicListDetailInfo
    {
        //Properties.
        QString musicFilePath;
        //Metadata map.
        QMap<int, QString> metaData;
        //Track list.
        QList<KNMusicListTrackDetailInfo> trackList;
    };
}

class KNMusicUtil
{
public:
    /*!
     * \brief Translate a QData time function to a readable string. The format
     * will be yyyy-MM-dd AP hh:mm.
     * \param dateTime The date time object.
     * \return The translated string.
     */
    static QString dateTimeToString(const QDateTime &dateTime)
    {
        return dateTime.toString("yyyy-MM-dd AP hh:mm");
    }

    /*!
     * \brief Translate a msecond qint64 number to readable string. The format
     * will be mm:ss. The surplus msecond will be dropped.
     * \param msecond The msecond qint64 number.
     * \return The translated string.
     */
    static QString msecondToString(const qint64 &msecond)
    {
        //Translate m-second to second first.
        qint64 second=msecond/1000;
        //Calculate the second(ss) part.
        QString secondText=QString::number(second%60);
        //If second is 0-9, add a 0 before the number.
        //The minuate part(mm) will be the second divided by 60.
        return secondText.length()==1?
                    QString::number(second/60)+":0"+secondText:
                    QString::number(second/60)+":"+secondText;
    }

private:
    KNMusicUtil();
    KNMusicUtil(const KNMusicUtil &);
};

#endif // KNMUSICUTIL

