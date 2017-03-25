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

extern "C"
{
#include <libavformat/avformat.h>
#include <libavformat/version.h>
}

#include "knmusicffmpeganalysiser.h"

KNMusicFfmpegAnalysiser::KNMusicFfmpegAnalysiser(QObject *parent) :
    KNMusicAnalysiser(parent)
{
    //Register all the the muxers, demuxers and protocols.
    av_register_all();
}

bool KNMusicFfmpegAnalysiser::analysis(KNMusicDetailInfo &detailInfo)
{
    //Generate the format context pointer.
    AVFormatContext *formatContext=NULL;
    //Open the file with ffmpeg.
    if(avformat_open_input(
                &formatContext,
                QDir::toNativeSeparators(
                    detailInfo.filePath).toLocal8Bit().data(),
                NULL,
                NULL))
    {
        //If return any negative AVERROR on failure, then failed to analysis the
        //file.
        return false;
    }
    //Check the formatContext.
    if(!formatContext)
    {
        //Something wrong happened.
        //Close the format context.
        avformat_close_input(&formatContext);
        //Failed to analysis the file.
        return false;
    }
    //Find the stream info from the context.
    if(avformat_find_stream_info(formatContext, NULL)<0)
    {
        //We can't find even one stream in the file.
        //Close the format context.
        avformat_close_input(&formatContext);
        //Failed to analysis the file.
        return false;
    }
    //Check whether the codec parameter is defined, if so, then it will use the
    //latest version code of FFMpeg.
    //Here we use the FF_API_CODER_TYPE as the tester, the version later should
    //contains AVCodecParameters.
#ifdef FF_API_CODER_TYPE
    //Initial the audio stream context.
    AVCodecParameters *codecParameters=NULL;
    //Find the audio stream.
    for(unsigned int i=0; i<formatContext->nb_streams; ++i)
    {
        //Get the stream;
        AVStream *stream=formatContext->streams[i];
        //Check the validation of the stream.
        if(stream)
        {
            //Get the codec context of the stream.
            AVCodecParameters *parameters=stream->codecpar;
            //Check the context of the stream.
            if(parameters && parameters->codec_type==AVMEDIA_TYPE_AUDIO)
            {
                //Save the context.
                codecParameters=parameters;
                //Get the sample rate.
                detailInfo.samplingRate=codecParameters->sample_rate;
                //Break.
                break;
            }
        }
    }
#else
    //Use the old codec parameter of the stream. It is not recommend in the
    //latest ffmpeg.
    //Initial the audio stream context.
    AVCodecContext *codecParameters=NULL;
    //Find the audio stream.
    for(unsigned int i=0; i<formatContext->nb_streams; ++i)
    {
        //Get the stream;
        AVStream *stream=formatContext->streams[i];
        //Check the validation of the stream.
        if(stream)
        {
            //Get the codec context of the stream.
            AVCodecContext *context=stream->codec;
            //Check the context of the stream.
            if(context && context->codec_type==AVMEDIA_TYPE_AUDIO)
            {
                //Save the context.
                codecParameters=context;
                //Break.
                break;
            }
        }
    }
#endif
    //Check the audio stream index.
    if(!codecParameters)
    {
        //We can't find any audio stream in the file.
        //Close the format context.
        avformat_close_input(&formatContext);
        //Failed to analysis the file.
        return false;
    }
    //Now, everything is ready.
    //The duration which AVFormatContext providec is the duration of the stream,
    //in AV_TIME_BASE fractional seconds. We need to change it to ms.
    detailInfo.duration=formatContext->duration/(AV_TIME_BASE/1000);
    //Calculate the bit rate.
    detailInfo.bitRate=(double)detailInfo.size/detailInfo.duration*8+0.5;
    //Close the format context.
    avformat_close_input(&formatContext);
    //Analysis complete.
    return true;
}
