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
extern "C"
{
#include "libavformat/avformat.h"
#include "libavformat/avio.h"

#include "libavcodec/avcodec.h"
#include "libavcodec/version.h"

#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"

#include "libswresample/swresample.h"
}

#include <QFileInfo>

#include "knmusicffmpegtranscoder.h"

#include <QDebug>

//From the commit f9b1cf15c233dde29f3659c903e32f74be4fdb95 of FFMpeg library.
//LibAV introduce the copy context API from 57.12, but FFMpeg introduce from
//57.33
#if LIBAVCODEC_VERSION_MAJOR > 57 || (LIBAVCODEC_VERSION_MAJOR==57 && \
    LIBAVCODEC_VERSION_MINOR>32)
#define FFMPEG_ENABLE_COPY_CONTEXT
#endif

/*
 * Most of these file is copy from the official example, transcode_aac.c
 * The code is change to fit with c++11.
 */
KNMusicFfmpegTranscoder::KNMusicFfmpegTranscoder(QObject *parent) :
    KNMusicTranscoderBase(parent),
    m_timestamp(0),
    m_inputFormatContext(nullptr),
    m_outputFormatContext(nullptr),
    m_inputCodecContext(nullptr),
    m_outputCodecContext(nullptr),
    m_outputCodecId(-1),
    m_outputBitRate(96000),
    m_channels(2)
{
    // Register all codecs and formats so that they can be used.
    av_register_all();
}

KNMusicFfmpegTranscoder::~KNMusicFfmpegTranscoder()
{
    //Clear the transcoder caches.
    clearAll();
}

int KNMusicFfmpegTranscoder::setInputFile(const QString &filePath,
                                          QString &errorInfo)
{
    //Get the basic parameter.
    AVCodec *inputCodec;
    int error=NoError;
    //Clear the input cache first.
    clearInputCache();
    //Open the input file to read from it.
    if((error=avformat_open_input(&m_inputFormatContext,
                                  filePath.toLocal8Bit().data(),
                                  NULL,
                                  NULL))<0)
    {
        //Clear the input format context
        m_inputFormatContext=nullptr;
        //Get the error information.
        errorInfo=getErrorText(error);
        //Give back the error.
        return InputFailToOpen;
    }
    //Get information on the input file (number of streams etc.).
    if((error=avformat_find_stream_info(m_inputFormatContext, NULL))<0)
    {
        //Wape all the input cache data.
        clearInputCache();
        //Get the error information.
        errorInfo=getErrorText(error);
        return InputFailToOpenStreamInfo;
    }
    //Find the audio stream.
    AVStream *audioStream=nullptr;
    unsigned int streamCount=m_inputFormatContext->nb_streams;
    for(unsigned int i=0; i<streamCount; ++i)
    {
#ifdef FFMPEG_ENABLE_COPY_CONTEXT
        //Check the stream type.
        AVCodecParameters *streamCodecParameter=
                m_inputFormatContext->streams[i]->codecpar;
        if(AVMEDIA_TYPE_AUDIO==streamCodecParameter->codec_type)
#else
        //Check the stream type.
        AVCodecContext *streamCodec=m_inputFormatContext->streams[i]->codec;
        if(AVMEDIA_TYPE_AUDIO==streamCodec->codec_type)
#endif
        {
            //If we have the audio stream.
            if(audioStream)
            {
                //Wape all the input cache data.
                clearInputCache();
                //Find more than one audio stream.
                return InputMoreThanOneAudioStream;
            }
            //Save the stream.
            audioStream=m_inputFormatContext->streams[i];
        }
    }
    //Check the audio stream.
    if(audioStream==nullptr)
    {
        //Wape all the input cache data.
        clearInputCache();
        //Cannot find any audio stream.
        errorInfo=getErrorText(error);
        return InputFailToFindAudioStream;
    }
    //Find a decoder for the audio stream.
#ifdef FFMPEG_ENABLE_COPY_CONTEXT
    AVCodecID codecID=audioStream->codecpar->codec_id;
#else
    AVCodecID codecID=audioStream->codec->codec_id;
#endif
    if(!(inputCodec=avcodec_find_decoder(codecID)))
    {
        //Wape all the input cache data.
        clearInputCache();
        //Failed to find audio codec.
        return InputFailToFindCodec;
    }
    //Allocate a new decoding context.
    m_inputCodecContext=avcodec_alloc_context3(inputCodec);
    if (!m_inputCodecContext)
    {
        //Wape all the input cache data.
        clearInputCache();
        //Failed to allocate the memory.
        return InputFailToAllocDecContext;
    }
    //Initialize the stream parameters with demuxer information.
#ifdef FFMPEG_ENABLE_COPY_CONTEXT
    error=avcodec_parameters_to_context(m_inputCodecContext,
                                        audioStream->codecpar);
#else
    error=avcodec_copy_context(m_inputCodecContext, audioStream->codec);
#endif
    if(error < 0)
    {
        //Wape all the input cache data.
        clearInputCache();
        return InputFailToCopyParameter;
    }
    //Open the decoder for the audio stream to use it later.
    if((error=avcodec_open2(m_inputCodecContext, inputCodec, NULL)) < 0)
    {
        //Wape all the input cache data.
        clearInputCache();
        return InputFailToOpenCodec;
    }
    //No error happened here.
    return 0;
}

int KNMusicFfmpegTranscoder::setOutputFile(const QString &filePath,
                                           QString &errorInfo)
{
    //Prepare the codec.
    QByteArray localeFileName=filePath.toLocal8Bit();
    AVIOContext *outputIoContext=nullptr;
    AVStream *stream=nullptr;
    AVCodec *outputCodec=nullptr;
    int error=NoError;
    //Clear output cache.
    clearOutputCache();
    //Open the output file to write to it.
    if((error=avio_open(&outputIoContext, localeFileName.data(),
                        AVIO_FLAG_WRITE))<0)
    {
        //Get the error info and give back the error info.
        errorInfo=getErrorText(error);
        return OutputFailToOpen;
    }
    //Create a new format context for the output container format.
    if (!(m_outputFormatContext=avformat_alloc_context()))
    {
        //Close the avio context.
        avio_close(outputIoContext);
        //Could not allocate output format context.
        return OutputFailToAllocFormatContext;
    }
    //Associate the output file (pointer) with the container format context.
    m_outputFormatContext->pb=outputIoContext;
    //Guess the desired container format based on the file extension.
    if(!(m_outputFormatContext->oformat=getOutputFormat(m_outputFormat)))
    {
        //Clear the caches.
        clearOutputCache();
        //Could not find output file format.
        return OutputFailToFindFormat;
    }
    av_strlcpy(m_outputFormatContext->filename, localeFileName.data(),
               sizeof(m_outputFormatContext->filename));
    //Find the encoder to be used by its name.
    if(!(outputCodec=avcodec_find_encoder((enum AVCodecID)m_outputCodecId)))
    {
        //Clear the caches.
        clearOutputCache();
        //Could not find the encoder.
        return OutputFailToFindEncoder;
    }
    //Create a new audio stream in the output file container.
    if(!(stream=avformat_new_stream(m_outputFormatContext, NULL)))
    {
        //Clear the caches.
        clearOutputCache();
        //Could not create new stream.
        return OutputFailToCreateNewStream;
    }
    //Allocate the encoding context.
    m_outputCodecContext=avcodec_alloc_context3(outputCodec);
    if(!m_outputCodecContext)
    {
        //Clear the caches.
        clearOutputCache();
        //Could not allocate an encoding context.
        return OutputFailToAllocEecContext;
    }
    /*
     * Set the basic encoder parameters.
     * The input file's sample rate is used to avoid a sample rate conversion.
     */
    m_outputCodecContext->channels = m_channels;
    m_outputCodecContext->channel_layout =
            av_get_default_channel_layout(m_channels);
    m_outputCodecContext->sample_rate = m_inputCodecContext->sample_rate;
    m_outputCodecContext->sample_fmt = outputCodec->sample_fmts[0];
    m_outputCodecContext->bit_rate = m_outputBitRate;
    //Allow the use of the experimental encoder.
    m_outputCodecContext->strict_std_compliance=FF_COMPLIANCE_EXPERIMENTAL;
    //Set the sample rate for the container.
    stream->time_base.den = m_inputCodecContext->sample_rate;
    stream->time_base.num = 1;
    // Some container formats (like MP4) require global headers to be present
    // Mark the encoder so that it behaves accordingly.
    if(m_outputFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
    {
        //Set the flag.
        m_outputCodecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    //Open the encoder for the audio stream to use it later.
    if((error=avcodec_open2(m_outputCodecContext, outputCodec, NULL)) < 0)
    {
        //Clear the cache.
        clearOutputCache();
        //Could not open output codec.
        errorInfo=getErrorText(error);
        return OutputFailToOpenCodec;
    }
    //Initial the parameters.
#ifdef FFMPEG_ENABLE_COPY_CONTEXT
    error=avcodec_parameters_from_context(stream->codecpar,
                                          m_outputCodecContext);
#else
    error=avcodec_copy_context(m_outputCodecContext, stream->codec);
#endif
    //Check error.
    if(error < 0)
    {
        //Clear the cache.
        clearOutputCache();
        //Could not initialize stream parameters.
        return OutputFailToInitStreamParameter;
    }
//    //PCM patches.
//    if(m_outputCodecId)
    return NoError;
}

bool KNMusicFfmpegTranscoder::setEncoder(TranscoderEncoder encoder)
{
    //Check the encoder.
    switch(encoder)
    {
    case LossyAac:
        m_outputCodecId=AV_CODEC_ID_AAC;
        m_outputFormat="adts";
        break;
    case LossyAc3:
        m_outputCodecId=AV_CODEC_ID_AC3;
        m_outputFormat="ac3";
        break;
    case LossyAdpcmAdx:
        m_outputCodecId=AV_CODEC_ID_ADPCM_ADX;
        m_outputFormat="adx";
        break;
    case LossyAdpcmG722:
        m_outputCodecId=AV_CODEC_ID_ADPCM_G722;
        m_outputFormat="g722";
        break;
    case LossyAdpcmG726:
        m_outputCodecId=AV_CODEC_ID_ADPCM_G726;
        m_outputFormat="flv";
        break;
    case LossyAdpcmImaWav:
        m_outputCodecId=AV_CODEC_ID_ADPCM_IMA_WAV;
        m_outputFormat="flv";
        break;
    case LossyAdpcmMs:
        m_outputCodecId=AV_CODEC_ID_ADPCM_MS;
        m_outputFormat="flv";
        break;
    case LossyAdpcmSwf:
        m_outputCodecId=AV_CODEC_ID_ADPCM_SWF;
        m_outputFormat="flv";
        break;
    case LossyAdpcmYamaha:
        m_outputCodecId=AV_CODEC_ID_ADPCM_YAMAHA;
        m_outputFormat="flv";
        break;
    case LossyEac3:
        m_outputCodecId=AV_CODEC_ID_EAC3;
        m_outputFormat="eac3";
        break;
    case LossyG7231:
        m_outputCodecId=AV_CODEC_ID_G723_1;
        m_outputFormat="g723_1";
        break;
    case LossyIlbc:
        m_outputCodecId=AV_CODEC_ID_ILBC;
        m_outputFormat="ilbc";
        break;
    case LossyMp2:
        m_outputCodecId=AV_CODEC_ID_MP2;
        m_outputFormat="mp2";
        break;
    case LossyMp3:
        m_outputCodecId=AV_CODEC_ID_MP3;
        m_outputFormat="mp3";
        break;
    case LossyNellymoser:
        m_outputCodecId=AV_CODEC_ID_NELLYMOSER;
        m_outputFormat="flv";
        break;
    case LossyPcmAlaw:
        m_outputCodecId=AV_CODEC_ID_PCM_ALAW;
        m_outputFormat="vob";
        break;
    case LossyPcmMulaw:
        m_outputCodecId=AV_CODEC_ID_PCM_MULAW;
        m_outputFormat="vob";
        break;
    case LossyRa144:
        m_outputCodecId=AV_CODEC_ID_RA_144;
        m_outputFormat="rm";
        break;
    case LossyRoqDpcm:
        m_outputCodecId=AV_CODEC_ID_ROQ_DPCM;
        m_outputFormat="roq";
        break;
    case LossyVorbis:
        m_outputCodecId=AV_CODEC_ID_VORBIS;
        m_outputFormat="ogg";
        break;
    case LossyWmav1:
        m_outputCodecId=AV_CODEC_ID_WMAV1;
        m_outputFormat="asf";
        break;
    case LossyWmav2:
        m_outputCodecId=AV_CODEC_ID_WMAV2;
        m_outputFormat="asf";
        break;
    case LosslessAlac:
        m_outputCodecId=AV_CODEC_ID_ALAC;
        m_outputFormat="ipod";
        break;
    case LosslessDts:
        m_outputCodecId=AV_CODEC_ID_DTS;
        m_outputFormat="dts";
        break;
    case LosslessFlac:
        m_outputCodecId=AV_CODEC_ID_FLAC;
        m_outputFormat="flac";
        break;
    case LosslessMlp:
        m_outputCodecId=AV_CODEC_ID_MLP;
        m_outputFormat="mlp";
        break;
    case LosslessPcmF32be:
        m_outputCodecId=AV_CODEC_ID_PCM_F32BE;
        m_outputFormat="f32be";
        break;
    case LosslessPcmF32le:
        m_outputCodecId=AV_CODEC_ID_PCM_F32LE;
        m_outputFormat="f32le";
        break;
    case LosslessPcmF64be:
        m_outputCodecId=AV_CODEC_ID_PCM_F64BE;
        m_outputFormat="f64be";
        break;
    case LosslessPcmF64le:
        m_outputCodecId=AV_CODEC_ID_PCM_F64LE;
        m_outputFormat="f64le";
        break;
    case LosslessPcmS16be:
        m_outputCodecId=AV_CODEC_ID_PCM_S16BE;
        m_outputFormat="s16be";
        break;
    case LosslessPcmS16bePlanar:
        m_outputCodecId=AV_CODEC_ID_PCM_S16BE_PLANAR;
        m_outputFormat="s16be";
        break;
    case LosslessPcmS16le:
        m_outputCodecId=AV_CODEC_ID_PCM_S16LE;
        m_outputFormat="s16le";
        break;
    case LosslessPcmS16lePlanar:
        m_outputCodecId=AV_CODEC_ID_PCM_S16LE_PLANAR;
        m_outputFormat="s16le";
        break;
    case LosslessPcmS24be:
        m_outputCodecId=AV_CODEC_ID_PCM_S24BE;
        m_outputFormat="s24be";
        break;
    case LosslessPcmS24le:
        m_outputCodecId=AV_CODEC_ID_PCM_S24LE;
        m_outputFormat="s24le";
        break;
    case LosslessPcmS24lePlanar:
        m_outputCodecId=AV_CODEC_ID_PCM_S24LE_PLANAR;
        m_outputFormat="s24le";
        break;
    case LosslessPcmS32be:
        m_outputCodecId=AV_CODEC_ID_PCM_S32BE;
        m_outputFormat="s32be";
        break;
    case LosslessPcmS32le:
        m_outputCodecId=AV_CODEC_ID_PCM_S32LE;
        m_outputFormat="s32le";
        break;
    case LosslessPcmS32lePlanar:
        m_outputCodecId=AV_CODEC_ID_PCM_S32LE_PLANAR;
        m_outputFormat="s32le";
        break;
    case LosslessPcmS8:
        m_outputCodecId=AV_CODEC_ID_PCM_S8;
        m_outputFormat="s8";
        break;
    case LosslessPcmS8Planar:
        m_outputCodecId=AV_CODEC_ID_PCM_S8_PLANAR;
        m_outputFormat="s8";
        break;
    case LosslessPcmU16be:
        m_outputCodecId=AV_CODEC_ID_PCM_U16BE;
        m_outputFormat="u16be";
        break;
    case LosslessPcmU16le:
        m_outputCodecId=AV_CODEC_ID_PCM_U16LE;
        m_outputFormat="u16le";
        break;
    case LosslessPcmS24daud:
        m_outputCodecId=AV_CODEC_ID_PCM_S24DAUD;
        m_outputFormat="daud";
        break;
    case LosslessPcmU24be:
        m_outputCodecId=AV_CODEC_ID_PCM_U24BE;
        m_outputFormat="u24be";
        break;
    case LosslessPcmU24le:
        m_outputCodecId=AV_CODEC_ID_PCM_U24LE;
        m_outputFormat="u24le";
        break;
    case LosslessPcmU32be:
        m_outputCodecId=AV_CODEC_ID_PCM_U32BE;
        m_outputFormat="u32be";
        break;
    case LosslessPcmU32le:
        m_outputCodecId=AV_CODEC_ID_PCM_U32LE;
        m_outputFormat="u32le";
        break;
    case LosslessPcmU8:
        m_outputCodecId=AV_CODEC_ID_PCM_U8;
        m_outputFormat="u8";
        break;
    case LosslessS302m:
        m_outputCodecId=AV_CODEC_ID_S302M;
        m_outputFormat="daud";
        break;
    case LosslessTruehd:
        m_outputCodecId=AV_CODEC_ID_TRUEHD;
        m_outputFormat="truehd";
        break;
    case LosslessTta:
        m_outputCodecId=AV_CODEC_ID_TTA;
        m_outputFormat="tta";
        break;
    case LosslessWavpack:
        m_outputCodecId=AV_CODEC_ID_WAVPACK;
        m_outputFormat="wv";
        break;
    default:
        m_outputCodecId=-1;
        m_outputFormat="";
        return false;
    }
    return true;
}

void KNMusicFfmpegTranscoder::execute()
{
    //Initial the resampler.
    if(initResampler()!=NoError)
    {
        //Failed.
        return;
    }
    //Initial the FIFO queue.
    if(initFifoBuffer()!=NoError)
    {
        //Failed.
        return;
    }
    //Reset the time stamp.
    m_timestamp=0;
    //Write the header of the output file container.
    int error;
    if((error=avformat_write_header(m_outputFormatContext, NULL)) < 0)
    {
        //Could not write output file header.
        emit transcodingError(TranscodeFailToWriteHeader, getErrorText(error));
        return;
    }
    // Loop as long as we have input samples to read or output samples
    // to write; abort as soon as we have neither.
    int finished = 0;
    while(!finished)
    {
        //Use the encoder's desired frame size for processing.
        int outputFrameSize=m_outputCodecContext->frame_size;
        //Reset the finish hint.
        finished=0;
        //Make sure that there is one frame worth of samples in the FIFO
        //buffer so that the encoder can do its work.
        //Since the decoder's and the encoder's frame size may differ, we
        //need to FIFO buffer to store as many frames worth of input samples
        //that they make up at least one frame worth of output samples.
        while(outputFrameSize==0 ||
              av_audio_fifo_size(m_fifo) < outputFrameSize)
        {
            // Decode one frame worth of audio samples, convert it to the
            // output sample format and put it into the FIFO buffer.
            if (readDecodeConvertAndStore(&finished))
            {
                //Error happened.
                clearFifoCache();
                clearResamplerCache();
                //Mission complete.
                return;
            }
            // If we are at the end of the input file, we continue
            // encoding the remaining audio samples to the output file.
            if (finished)
            {
                break;
            }
        }
        // If we have enough samples for the encoder, we encode them.
        // At the end of the file, we pass the remaining samples to
        // the encoder.
        while(av_audio_fifo_size(m_fifo) >= outputFrameSize ||
              (finished && av_audio_fifo_size(m_fifo) > 0))
        {
            // Take one frame worth of audio samples from the FIFO buffer,
            // encode it and write it to the output file.
            if(loadEncodeAndWrite())
            {
                //Error happened.
                clearFifoCache();
                clearResamplerCache();
                //Mission complete.
                return;
            }
        }
        // If we are at the end of the input file and have encoded
        // all remaining samples, we can exit this loop and finish.
        if(finished)
        {
            //Prepare the data written data.
            int dataWritten;
            //Flush the encoder as it may have delayed frames.
            do
            {
                //Encode the audio frame.
                if (encodeAudioFrame(NULL, &dataWritten))
                {
                    //Error happened.
                    clearFifoCache();
                    clearResamplerCache();
                    //Mission complete.
                    return;
                }
            }
            while (dataWritten);
            //Finished all the data.
            finished=1;
        }
    }
    //Write the trailer of the output file container.
    if((error = av_write_trailer(m_outputFormatContext)) < 0)
    {
        //Could not write output file trailer.
        emit transcodingError(TranscodeFailToWriteTailer, getErrorText(error));
    }
    //Clear the fifo and resampler.
    clearFifoCache();
    clearResamplerCache();
}

void KNMusicFfmpegTranscoder::clear()
{
    //Clear all the cache data.
    clearAll();
}

const char *KNMusicFfmpegTranscoder::getErrorText(const int error)
{
    //Generate the static error buffer for the text.
    static char errorBuffer[255];
    //Get the error string.
    av_strerror(error, errorBuffer, sizeof(errorBuffer));
    //Return the buffer.
    return errorBuffer;
}

inline void KNMusicFfmpegTranscoder::clearAll()
{
    //Clear all the fifo cache.
    clearFifoCache();
    //Clear all the resampler cache.
    clearResamplerCache();
    //Clear all the output file cache data.
    clearOutputCache();
    //Clear all the input file cache data.
    clearInputCache();
}

inline void KNMusicFfmpegTranscoder::clearInputCache()
{
    //Clear the input codec context.
    if(m_inputCodecContext)
    {
        //Free the codec context.
        avcodec_free_context(&m_inputCodecContext);
        //Reset the nullptr.
        m_inputCodecContext=nullptr;
    }
    //Clear the input format context data.
    if(m_inputFormatContext)
    {
        //Close the input context.
        avformat_free_context(m_inputFormatContext);
        //Reset the pointer.
        m_inputFormatContext=nullptr;
    }
}

inline void KNMusicFfmpegTranscoder::clearOutputCache()
{
    //Clear the codec context.
    if(m_outputCodecContext)
    {
        //Free the output context.
        avcodec_free_context(&m_outputCodecContext);
        //Reset the pointer.
        m_outputCodecContext=nullptr;
    }
    //Clear the output format context.
    if(m_outputFormatContext)
    {
        //Close the context.
        avio_closep(&m_outputFormatContext->pb);
        //Free the context.
        avformat_free_context(m_outputFormatContext);
        //Reset the pointer.
        m_outputFormatContext=nullptr;
    }
}

inline void KNMusicFfmpegTranscoder::clearResamplerCache()
{
    //Check the context.
    if(m_resampleContext)
    {
        //Free the memory.
        swr_free(&m_resampleContext);
        //Reset the pointer.
        m_resampleContext=nullptr;
    }
}

inline void KNMusicFfmpegTranscoder::clearFifoCache()
{
    //Check the fifo pointer.
    if (m_fifo)
    {
        //Free the fifo.
        av_audio_fifo_free(m_fifo);
        //Reset the pointer.
        m_fifo=nullptr;
    }
}

inline int KNMusicFfmpegTranscoder::initResampler()
{
    int error;
    /*
     * Create a resampler context for the conversion.
     * Set the conversion parameters.
     * Default channel layouts based on the number of channels
     * are assumed for simplicity (they are sometimes not detected
     * properly by the demuxer and/or decoder).
     */
    m_resampleContext=swr_alloc_set_opts(
                NULL,
                av_get_default_channel_layout(m_outputCodecContext->channels),
                m_outputCodecContext->sample_fmt,
                m_outputCodecContext->sample_rate,
                av_get_default_channel_layout(m_inputCodecContext->channels),
                m_inputCodecContext->sample_fmt,
                m_inputCodecContext->sample_rate,
                0, NULL);
    //Check the resample state.
    if (!m_resampleContext)
    {
        //Emit the error.
        emit transcodingError(ResampleFailToAlloc,
                              "Fail to allocate resample context memory.");
        //Could not allocate resample context.
        return ResampleFailToAlloc;
    }
    /*
     * Perform a sanity check so that the number of converted samples is
     * not greater than the number of samples to be converted.
     * If the sample rates differ, this case has to be handled differently
     */
    av_assert0(m_outputCodecContext->sample_rate==
               m_inputCodecContext->sample_rate);
    //Open the resampler with the specified parameters.
    if((error=swr_init(m_resampleContext)) < 0)
    {
        //Free the resample context.
        clearResamplerCache();
        //Emit the error.
        emit transcodingError(ResampleFailToOpen, getErrorText(error));
        //Could not open resample context.
        return ResampleFailToOpen;
    }
    //Complete.
    return NoError;
}

int KNMusicFfmpegTranscoder::initFifoBuffer()
{
    //Create the FIFO buffer based on the specified output sample format.
    if (!(m_fifo=av_audio_fifo_alloc(m_outputCodecContext->sample_fmt,
                                     m_outputCodecContext->channels, 1)))
    {
        //Emit error.
        emit transcodingError(FifoFailToAlloc,
                              "Failed to allocate memory for output context.");
        //Could not allocate FIFO.
        return FifoFailToAlloc;
    }
    //Complete.
    return NoError;
}

inline AVOutputFormat *KNMusicFfmpegTranscoder::getOutputFormat(
        const QString &formatName)
{
    //This function is basically copy the original guess function. However it
    //seems that the original guess function is not as good as what I expected.
    //Why not use QString to compare?
    //Generate the format pointer.
    AVOutputFormat *format=NULL;
    //Find the proper file type, loop until the last one.
    while((format=av_oformat_next(format))!=NULL)
    {
        //Check the format name is the same as the format name.
        if(formatName==QString(format->name))
        {
            //Give back the format pointer.
            return format;
        }
    }
    //Fail to get the format pointer.
    return NULL;
}

int KNMusicFfmpegTranscoder::readDecodeConvertAndStore(int *finished)
{
    //Temporary storage of the input samples of the frame read from the file.
    AVFrame *inputFrame=nullptr;
    //Temporary storage for the converted input samples.
    uint8_t **convertedInputSamples=nullptr;
    int dataPresent;
    //Initialize temporary storage for one input frame.
    if(initInputFrame(&inputFrame))
    {
        //Clean up the cache.
        clearDecodeCache(convertedInputSamples, inputFrame);
        //Return error exit.
        return AVERROR_EXIT;
    }
    //Decode one frame worth of audio samples.
    if(decodeAudioFrame(inputFrame, &dataPresent, finished))
    {
        //Clean up the cache.
        clearDecodeCache(convertedInputSamples, inputFrame);
        //Return error exit.
        return AVERROR_EXIT;
    }
    // If we are at the end of the file and there are no more samples
    // in the decoder which are delayed, we are actually finished.
    // This must not be treated as an error.
    if(*finished && !dataPresent)
    {
        //Clean up the cache.
        clearDecodeCache(convertedInputSamples, inputFrame);
        //Mission complete.
        return 0;
    }
    //If there is decoded data, convert and store it.
    if(dataPresent)
    {
        //Initialize the temporary storage for the converted input samples.
        if(initConvertedSamples(&convertedInputSamples,
                                inputFrame->nb_samples) ||
                //Convert the input samples to the desired output sample format.
                //This requires a temporary storage provided by converted
                //input samples.
                convertSamples((const uint8_t**)inputFrame->extended_data,
                               convertedInputSamples,
                               inputFrame->nb_samples,
                               m_resampleContext) ||
                //Add the converted input samples to the FIFO buffer for later
                //processing.
                addSamplesToFifo(convertedInputSamples,
                                 inputFrame->nb_samples))
        {
            //Clean up the cache.
            clearDecodeCache(convertedInputSamples, inputFrame);
            //Return error exit.
            return AVERROR_EXIT;
        }
    }
    //Clean up the cache.
    clearDecodeCache(convertedInputSamples, inputFrame);
    //Success.
    return 0;
}

inline void KNMusicFfmpegTranscoder::clearDecodeCache(
        uint8_t **convertedInputSamples,
        AVFrame *inputFrame)
{
    //Clean the input sample data.
    if (convertedInputSamples)
    {
        //Free the pointer.
        av_freep(&convertedInputSamples[0]);
        //Clear the memory.
        free(convertedInputSamples);
    }
    //Free the input frame.
    av_frame_free(&inputFrame);
}

inline int KNMusicFfmpegTranscoder::loadEncodeAndWrite()
{
    //Temporary storage of the output samples of the frame written to the file.
    AVFrame *outputFrame=nullptr;
    // Use the maximum number of possible samples per frame.
    // If there is less than the maximum possible frame size in the FIFO
    // buffer use this number. Otherwise, use the maximum possible frame size
    int frameSize=FFMIN(av_audio_fifo_size(m_fifo),
                        m_outputCodecContext->frame_size),
            dataWritten;
    //Check the frame size validation.
    if(frameSize==0)
    {
        //Because the frame_size could be 0 because some codecs set the flag
        //with AV_CODEC_CAP_VARIABLE_FRAME_SIZE.
        //When the codec set the flag with this, it will ask the user to provide
        //the frame size instead of the codec provides. e.g. all the PCM
        //implementation from FFMpeg.
        frameSize=av_audio_fifo_size(m_fifo);
    }
    qDebug()<<"Frame size is="<<frameSize;
    //Initialize temporary storage for one output frame.
    if(initOutputFrame(&outputFrame, frameSize))
    {
        //Error happened when allocating the frame.
        return AVERROR_EXIT;
    }
    // Read as many samples from the FIFO buffer as required to fill the frame.
    // The samples are stored in the frame temporarily.
    if (av_audio_fifo_read(m_fifo,
                           (void **)outputFrame->data,
                           frameSize) < frameSize)
    {
        //Could not read data from FIFO.
        //!FIXME.
        av_frame_free(&outputFrame);
        return AVERROR_EXIT;
    }
    //Encode one frame worth of audio samples.
    if (encodeAudioFrame(outputFrame, &dataWritten))
    {
        //Error when encoding.
        av_frame_free(&outputFrame);
        return AVERROR_EXIT;
    }
    //Free the frame.
    av_frame_free(&outputFrame);
    //Mission complete.
    return 0;
}

inline int KNMusicFfmpegTranscoder::initOutputFrame(AVFrame **frame,
                                                    int frameSize)
{
    int error;
    //Create a new frame to store the audio samples.
    if (!(*frame=av_frame_alloc()))
    {
        emit transcodingError(TranscodeFailToAllocWriteFrame,
                              "Fail to allocate write frame memory.");
        //Could not allocate output frame.
        return AVERROR_EXIT;
    }
    //Set the frame's parameters, especially its size and format.
    //av_frame_get_buffer needs this to allocate memory for the audio samples
    //of the frame.
    //Default channel layouts based on the number of channels are assumed for
    //simplicity.
    (*frame)->nb_samples     = frameSize;
    (*frame)->channel_layout = m_outputCodecContext->channel_layout;
    (*frame)->format         = m_outputCodecContext->sample_fmt;
    (*frame)->sample_rate    = m_outputCodecContext->sample_rate;
    // Allocate the samples of the created frame. This call will make sure that
    // the audio frame can hold as many samples as specified.
    if((error = av_frame_get_buffer(*frame, 0)) < 0)
    {
        //Could allocate output frame samples.
        emit transcodingError(TranscodeFailToGetWriteBuf, getErrorText(error));
        av_frame_free(frame);
        return error;
    }
    return 0;
}

int KNMusicFfmpegTranscoder::encodeAudioFrame(AVFrame *frame, int *dataPresent)
{
    //Packet used for temporary storage.
    AVPacket outputPacket;
    int error;
    //Initial the packet.
    initPacket(&outputPacket);
    //Set a timestamp based on the sample rate for the container.
    if (frame)
    {
        //Set the frame timestamp.
        frame->pts=m_timestamp;
        //Update the global time stamp.
        m_timestamp+=frame->nb_samples;
    }
    // Encode the audio frame and store it in the temporary packet.
    // The output audio stream encoder is used to do this.
    if ((error = avcodec_encode_audio2(m_outputCodecContext, &outputPacket,
                                       frame, dataPresent)) < 0)
    {
        //Could not encode frame.
        emit transcodingError(TranscodeFailToEncodeAudio, getErrorText(error));
        av_packet_unref(&outputPacket);
        return error;
    }
    //Write one audio frame from the temporary packet to the output file.
    if(*dataPresent)
    {
        //Write the frame data.
        if ((error = av_write_frame(m_outputFormatContext, &outputPacket)) < 0)
        {
            //Could not write frame.
            emit transcodingError(TranscodeFailToWriteFrame,
                                  getErrorText(error));
            av_packet_unref(&outputPacket);
            return error;
        }
        //Wipe up the data.
        av_packet_unref(&outputPacket);
    }
    //Success.
    return 0;
}

inline int KNMusicFfmpegTranscoder::initInputFrame(AVFrame **frame)
{
    //Load the frame.
    if(!(*frame = av_frame_alloc()))
    {
        //Could not allocate input frame.
        emit transcodingError(TranscodeFailToAllocReadFrame,
                              "Could not allocate input frame.");
        return AVERROR(ENOMEM);
    }
    //For else failed to allocate the frame.
    return 0;
}

inline void KNMusicFfmpegTranscoder::initPacket(AVPacket *packet)
{
    //Initial the packet via ffmpeg function.
    av_init_packet(packet);
    //Set the packet data and size so that it is recognized as being empty.
    packet->data=nullptr;
    packet->size=0;
}

inline int KNMusicFfmpegTranscoder::decodeAudioFrame(AVFrame *frame,
                                                     int *dataPresent,
                                                     int *finished)
{
    //Packet used for temporary storage.
    AVPacket inputPacket;
    int error;
    //Initial the packet.
    initPacket(&inputPacket);
    //Read one audio frame from the input file into a temporary packet.
    if ((error = av_read_frame(m_inputFormatContext, &inputPacket)) < 0)
    {
        //If we are at the end of the file, flush the decoder below.
        if (error == AVERROR_EOF)
        {
            qDebug()<<"Reach the EOF!";
            //Set the finished to be done.
            *finished = 1;
        }
        else
        {
            //Could not read frame.
            emit transcodingError(TranscodeFailToReadFrame,
                                  getErrorText(error));
            return error;
        }
    }
    qDebug()<<inputPacket.pts;
    // Decode the audio frame stored in the temporary packet.
    // The input audio stream decoder is used to do this.
    // If we are at the end of the file, pass an empty packet to the decoder
    // to flush it.
    // Reset the error code to 0.
    error = 0;
    // It is originally using avcodec_decode_audio4, however, it is deprecated.
//    if((error = avcodec_send_packet(m_inputCodecContext, &inputPacket)) < 0 ||
//            (error = avcodec_receive_frame(m_inputCodecContext, frame)) != 0)
    if ((error = avcodec_decode_audio4(m_inputCodecContext, frame,
                                       dataPresent, &inputPacket)) < 0)
    {
//        //Check the error code first.
//        if(error==AVERROR_EOF)
//        {
//            //Which means the codec reaches the end of the file.
//            //This should not be processed as an error.
//            (*dataPresent)=1;
//        }
//        else
//        {
            //Could not decode frame.
            emit transcodingError(TranscodeFailToDecReadFrame, getErrorText(error));
            av_packet_unref(&inputPacket);
            return error;
//        }
    }
    // If the decoder has not been flushed completely, we are not finished,
    // so that this function has to be called again.
    if (*finished && *dataPresent)
    {
        //Set finished to not complete.
        *finished = 0;
    }
    //Clear the input packet reference.
    av_packet_unref(&inputPacket);
    //Complete.
    return 0;
}

inline int KNMusicFfmpegTranscoder::initConvertedSamples(
        uint8_t ***convertedInputSamples,
        int frameSize)
{
    int error;
    // Allocate as many pointers as there are audio channels.
    // Each pointer will later point to the audio samples of the corresponding
    // channels (although it may be NULL for interleaved formats).
    if (!(*convertedInputSamples=
          (uint8_t **)calloc(m_outputCodecContext->channels,
                             sizeof(**convertedInputSamples))))
    {
        //Emit the error signal.
        emit transcodingError(TranscodeFailToAllocSampleArray,
                              "Fail to allocate converted sample input array.");
        //Could not allocate converted input sample pointers.
        return AVERROR(ENOMEM);
    }
    // Allocate memory for the samples of all channels in one consecutive
    // block for convenience.
    if ((error=av_samples_alloc(*convertedInputSamples, NULL,
                                m_outputCodecContext->channels,
                                frameSize,
                                m_outputCodecContext->sample_fmt, 0)) < 0)
    {
        //Emit the error signal.
        emit transcodingError(TranscodeFailToAllocSample,
                              "Fail to allocate converted sample inputs.");
        //Could not allocate converted input samples.
        getErrorText(error);
        av_freep(&(*convertedInputSamples)[0]);
        free(*convertedInputSamples);
        return error;
    }
    return 0;
}

inline int KNMusicFfmpegTranscoder::convertSamples(const uint8_t **inputData,
                                                   uint8_t **convertedData,
                                                   const int frameSize,
                                                   SwrContext *resampleContext)
{
    int error;
    //Convert the samples using the resampler.
    if ((error=swr_convert(resampleContext,
                           convertedData, frameSize,
                           inputData, frameSize)) < 0)
    {
        //Could not convert input samples.
        emit transcodingError(TranscodeFailToCovertSample, getErrorText(error));
        return error;
    }
    //Complete.
    return 0;
}

inline int KNMusicFfmpegTranscoder::addSamplesToFifo(
        uint8_t **convertedInputSamples, const int frameSize)
{
    int error;
    // Make the FIFO as large as it needs to be to hold both,
    // the old and the new samples.
    if((error=av_audio_fifo_realloc(m_fifo,
                                    av_audio_fifo_size(m_fifo) + frameSize))<0)
    {
        //Could not reallocate FIFO.
        emit transcodingError(TranscodeFailToReallocFifo,
                              "Fail to add to expand fifo memory.");
        return error;
    }
    //Store the new samples in the FIFO buffer.
    if(av_audio_fifo_write(m_fifo,
                           (void **)convertedInputSamples,
                           frameSize) < frameSize)
    {
        //Could not write data to FIFO.
        emit transcodingError(TranscodeFailToWriteFifo,
                              "Fail to add to sample to fifo.");
        return AVERROR_EXIT;
    }
    return 0;
}
