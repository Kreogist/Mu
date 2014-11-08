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
#include <QDir>

#include "knffmpegglobal.h"

#include "knmusicffmpeganalysiser.h"

KNMusicFFMpegAnalysiser::KNMusicFFMpegAnalysiser(QObject *parent) :
    KNMusicAnalysiser(parent)
{
    //Initial the global to make sure the FFMpeg has been instanced.
    KNFFMpegGlobal::instance();
}

bool KNMusicFFMpegAnalysiser::analysis(const QString &filePath,
                                       KNMusicDetailInfo &detailInfo)
{
    int audioStream=-1;
    AVFormatContext *formatContext=NULL;
    AVCodecContext *codecContext=NULL;
    //Open the file with ffmpeg.
    if(avformat_open_input(&formatContext,
                           QDir::toNativeSeparators(filePath).toLocal8Bit().data(),
                           NULL,
                           NULL)!=0)
    {
        //Open failed.
        return false;
    }
    //Check whether we can find the stream info from the context.
    if(avformat_find_stream_info(formatContext, NULL)<0)
    {
        //We can't find even one stream
        return false;
    }
    //Find the audio stream.
    for(unsigned int i=0; i<formatContext->nb_streams; i++)
    {
        if(formatContext->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
        {
            audioStream=i;
            break;
        }
    }
    //Check is there audio stream:
    if(-1==audioStream)
    {
        //No audio stream find.
        return false;
    }
    //Get the audio codec.
    codecContext=formatContext->streams[audioStream]->codec;
    AVCodec *codec=avcodec_find_decoder(codecContext->codec_id);
    //Check is there a codec can we used.
    if(codecContext==NULL)
    {
        return false;
    }
    //Open the codec context using the codec.
    if(avcodec_open2(codecContext, codec, NULL)<0)
    {
        return false;
    }

    //Now, everything is ready.
    //The duration which AVFormatContext providec is the duration of the stream,
    //in AV_TIME_BASE fractional seconds. We need to change it to ms.
    detailInfo.duration=formatContext->duration/(AV_TIME_BASE/1000);
    //Get the sample rate.
    detailInfo.samplingRate=codecContext->sample_rate;
    //Calculate the bit rate.
    detailInfo.bitRate=(double)detailInfo.size/detailInfo.duration*8+0.5;

    //Close the file.
    avformat_close_input(&formatContext);

    return true;
}
