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
#include <QVariant>
#include <QJsonObject>

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
    enum MusicColumns
    {
        MusicRowState=MusicDataCount,
        MusicColumnCount
    };

    enum MusicPropertyRole
    {
        FilePathRole=Qt::UserRole,
        FileNameRole,
        StartPositionRole,
        ArtworkKeyRole,
        TrackFileRole,
        TrackIndexRole,
        CannotPlayFlagRole,
        DurationRole,
        FileSizeRole
    };
    enum PropertyListIndex
    {
        PropertyFilePath,
        PropertyFileName,
        PropertyCoverImageHash,
        PropertyBitRate,
        PropertySamplingRate,
        PropertySize,
        PropertyDuration,
        PropertyDateAdded,
        PropertyDateModified,
        PropertyLastPlayed,
        PropertyTrackFilePath,
        PropertyTrackIndex,
        PropertyStartPosition,
        PropertyListCount
    };
    enum MusicColumnRole
    {
        SortDataRole=Qt::UserRole
    };
    enum KNMusicPlayingState
    {
        Stopped,
        Playing,
        Paused
    };
    enum KNMusicLoopState
    {
        NoRepeat,
        RepeatAll,
        RepeatTrack,
        Shuffle,
        LoopCount
    };
    struct KNMusicDetailInfo
    {
        //Tag datas.
        QVariant textLists[MusicDataCount];
        QString fileName;           //Properties.
        QString filePath;
        QString trackFilePath;
        QDateTime dateModified;
        QDateTime dateLastPlayed;
        QDateTime dateAdded;
        quint64 size;
        qint64 startPosition;       //Music properties.
        qint64 duration;
        qint64 bitRate;
        qint64 samplingRate;
        QString coverImageHash;     //Image hash data.
        int trackIndex;
        bool cannotPlay;            //The cannot playing flag.
        //Initial the values
        KNMusicDetailInfo():
            startPosition(-1),
            duration(0),
            bitRate(0),
            samplingRate(0),
            trackIndex(-1),
            cannotPlay(false)
        {
        }
        //Equal operation.
        bool operator ==(const KNMusicDetailInfo &value)
        {
            return filePath==value.filePath &&
                    trackFilePath==value.trackFilePath &&
                    trackIndex==value.trackIndex;
        }
    };
    struct KNMusicAnalysisItem
    {
        KNMusicDetailInfo detailInfo;
        //Album art data.
        QMap<QString, QList<QByteArray>> imageData;
        QImage coverImage;
    };
    struct KNMusicListTrackDetailInfo
    {
        //Metadata map.
        QMap<int, QString> metaData;
        //Track time.
        qint64 startPosition;
        qint64 trackDuration;
        //File path.
        QString musicFilePath;
        //Track index.
        int index;
        //Initial values.
        KNMusicListTrackDetailInfo() :
            startPosition(-1),
            trackDuration(-1),
            index(-1)
        {
        }
    };
    struct KNMusicListDetailInfo
    {
        //Metadata map.
        QMap<int, QString> metaData;
        //Track list.
        QList<KNMusicListTrackDetailInfo> trackList;
    };
    struct KNMusicSearchBlock
    {
        //Value of the block.
        QVariant value;
        //Index of the column or the role.
        int index;
        //Actually there's two kinds of data, so using a bool to check the
        //whether the index means a column or a property.
        bool isColumn;
        //Initial value.
        KNMusicSearchBlock():
            value(QVariant()),
            index(-1),
            isColumn(true)
        {
        }
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
    static QString dateTimeToText(const QDateTime &dateTime)
    {
        return dateTime.toString("yyyy-MM-dd AP hh:mm");
    }

    /*!
     * \brief Translate a data string to a QDateTime class.
     * \param data The data string. The format is yyyyMMddHHmmss.
     * \return A QDateTime class parsed from the data string.
     */
    static QDateTime dataToDateTime(const QString &data)
    {
        return QDateTime::fromString(data, "yyyyMMddHHmmss");
    }

    /*!
     * \brief Translate a QDateTime class to a data string.
     * \param dateTime The date time object.
     * \return The data string.
     */
    static QString dateTimeToData(const QDateTime &dateTime)
    {
        return dateTime.toString("yyyyMMddHHmmss");
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

    /*!
     * \brief Convert a 32-bit integer to char array which in the inverse order.
     * \param rawTagData The char array pointer.
     * \param data The 32-bit number.
     */
    static void int32ToInverseChar(char *rawTagData, const quint32 &data)
    {
        rawTagData[3]=(data & 0xFF000000) >> 24;
        rawTagData[2]=(data & 0x00FF0000) >> 16;
        rawTagData[1]=(data & 0x0000FF00) >> 8;
        rawTagData[0]=(data & 0x000000FF);
    }

    /*!
     * \brief Convert a inverse char array to a 32-bit integer.
     * \param rawTagData The char array pointer.
     * \return The 32-bit integer.
     */
    static quint32 inverseCharToInt32(const char *rawTagData)
    {
        return (((quint32)rawTagData[3]<<24) & 0xFF000000) +
               (((quint32)rawTagData[2]<<16) & 0x00FF0000) +
               (((quint32)rawTagData[1]<<8)  & 0x0000FF00) +
               ( (quint32)rawTagData[0]      & 0x000000FF);
    }

    /*!
     * \brief Convert a 32-bit integer to a char array.
     * \param rawTagData The char array pointer.
     * \param data The 32-bit number.
     */
    static void int32ToChar(char *rawTagData, const quint32 &data)
    {
        rawTagData[0]=(data & 0xFF000000) >> 24;
        rawTagData[1]=(data & 0x00FF0000) >> 16;
        rawTagData[2]=(data & 0x0000FF00) >> 8;
        rawTagData[3]=(data & 0x000000FF);
    }

    /*!
     * \brief Convert a char array to a 32-bit integer.
     * \param rawTagData The char array pointer.
     * \return The 32-bit integer.
     */
    static quint32 charToInt32(const char *rawTagData)
    {
        return (((quint32)rawTagData[0]<<24) & 0xFF000000) +
               (((quint32)rawTagData[1]<<16) & 0x00FF0000) +
               (((quint32)rawTagData[2]<<8)  & 0x0000FF00) +
               ( (quint32)rawTagData[3]      & 0x000000FF);
    }

    /*!
     * \brief Translate a QJsonObject to a KNMusicDetailInfo class.
     * \param object The json object class.
     * \return The translated KNMusicDetailInfo class.
     */
    static MusicUtil::KNMusicDetailInfo objectToDetailInfo(
            const QJsonObject &object);

    /*!
     * \brief Translate a KNMusicDetailInfo to a QJsonObject class.
     * \param detailInfo The KNMusicDetailInfo class.
     * \return The translate QJsonObject class.
     */
    static QJsonObject detailInfoToObject(
            const MusicUtil::KNMusicDetailInfo &detailInfo);

private:
    KNMusicUtil();
    KNMusicUtil(const KNMusicUtil &);
};

#endif // KNMUSICUTIL

