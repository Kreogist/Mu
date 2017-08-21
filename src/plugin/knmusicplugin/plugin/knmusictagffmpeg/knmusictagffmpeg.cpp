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
extern "C"
{
#include "libavformat/avformat.h"
#include "libavutil/dict.h"
}

#include "knmusictagffmpeg.h"

#include <QDebug>

KNMusicTagFfmpeg::KNMusicTagFfmpeg(QObject *parent) :
    KNMusicTagParser(parent)
{
    //Register all the formats.
    av_register_all();
}

QString KNMusicTagFfmpeg::tagParserName()
{
    return "ffmpeg";
}

bool KNMusicTagFfmpeg::parseTag(QFile &musicFile,
                               QDataStream &musicDataStream,
                               KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(musicFile)
    Q_UNUSED(musicDataStream)
    //Prepare the format context.
    AVFormatContext *formatContext = NULL;
    int result;
    //Use the FFMpeg to open the file.
    if((result = avformat_open_input(
            &formatContext,
            analysisItem.detailInfo.filePath.toLocal8Bit().data(),
            NULL, NULL)))
    {
        //Failed to open the file.
        return false;
    }
    //Read the data from the context.
    KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Get the information.
    AVDictionary *metadata=formatContext->metadata;
    setMetadata(metadata, "title", detailInfo, Name);
    setMetadata(metadata, "album", detailInfo, Album);
    setMetadata(metadata, "album_artist", detailInfo, AlbumArtist);
    setMetadata(metadata, "artist", detailInfo, Artist);
    setMetadata(metadata, "comment", detailInfo, Comments);
    setMetadata(metadata, "composer", detailInfo, Composer);
    setMetadata(metadata, "genre", detailInfo, Genre);
    setMetadata(metadata, "track", detailInfo, TrackNumber);
    setMetadata(metadata, "disc", detailInfo, DiscNumber);
    //Close the input file.
    avformat_free_context(formatContext);
    //Complete.
    return true;
}

bool KNMusicTagFfmpeg::writeTag(const KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(analysisItem);
    return false;
}

bool KNMusicTagFfmpeg::parseAlbumArt(KNMusicAnalysisItem &analysisItem)
{
    Q_UNUSED(analysisItem);
    return false;
}

bool KNMusicTagFfmpeg::writable() const
{
    return false;
}

bool KNMusicTagFfmpeg::writeCoverImage() const
{
    return false;
}

inline void KNMusicTagFfmpeg::setMetadata(AVDictionary *dictionary,
                                          const char *key,
                                          KNMusicDetailInfo &detailInfo,
                                          int tagIndex)
{
    //Get the dictionary entry.
    AVDictionaryEntry *tag = av_dict_get(dictionary, key, NULL, 0);
    //Check the tag pointer.
    if(!tag)
    {
        //Ignore for the NULL tag.
        return;
    }
    //Save the data.
    switch(tagIndex)
    {
    case DiscNumber:
        //Get the disc text.
        setSeparatorData(tag->value, detailInfo, DiscNumber, DiscCount);
        break;
    case TrackNumber:
        //Get the track number.
        setSeparatorData(tag->value, detailInfo, TrackNumber, TrackCount);
        break;
    default:
        //Simply set the value as the data of the text.
        setTextData(detailInfo.textLists[tagIndex],
                    QString(tag->value).simplified());
        break;
    }
}

void KNMusicTagFfmpeg::setSeparatorData(const QString &rawData,
                                        KNMusicDetailInfo &detailInfo,
                                        int firstIndex,
                                        int secondIndex)
{
    //Find the '/' separator.
    int separator=rawData.indexOf('/');
    if(separator==-1)
    {
        //Cannot find the separator, it should be a single number.
        detailInfo.textLists[firstIndex]=rawData;
    }
    else
    {
        //The data should be in format First part/Second part.
        detailInfo.textLists[firstIndex]=rawData.left(separator);
        detailInfo.textLists[secondIndex]=rawData.mid(separator+1);
    }
}
