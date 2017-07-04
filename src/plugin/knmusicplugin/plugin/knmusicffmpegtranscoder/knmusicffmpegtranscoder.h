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

#ifndef KNMUSICFFMPEGTRANSCODER_H
#define KNMUSICFFMPEGTRANSCODER_H

#include <stdint.h>

#include "knmusictranscoderbase.h"

struct AVFormatContext;
struct AVCodecContext;
struct AVOutputFormat;
struct AVAudioFifo;
struct AVFrame;
struct AVPacket;
struct SwrContext;
/*!
 * \brief The KNMusicFfmpegTranscode class provides the transcode for the FFMpeg
 * transcoder. This plugin need to find the ffmpeg bin.
 */
class KNMusicFfmpegTranscoder : public KNMusicTranscoderBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicFfmpegTranscode object.
     * \param parent The parent object.
     */
    explicit KNMusicFfmpegTranscoder(QObject *parent = 0);
    ~KNMusicFfmpegTranscoder();

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicTranscoderBase::setInputFile().
     */
    int setInputFile(const QString &filePath,
                     QString &errorInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTranscoderBase::setOutputFile().
     */
    int setOutputFile(const QString &filePath,
                      QString &errorInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTranscoderBase::setEncoder().
     */
    bool setEncoder(TranscoderEncoder encoder) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTranscoderBase::execute().
     */
    void execute() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTranscoderBase::clear().
     */
    void clear() Q_DECL_OVERRIDE;

private:
    static const char *getErrorText(const int error);
    inline void clearAll();
    inline void clearInputCache();
    inline void clearOutputCache();
    inline void clearResamplerCache();
    inline void clearFifoCache();
    inline int initResampler();
    inline int initFifoBuffer();
    inline AVOutputFormat *getOutputFormat(const QString &formatName);
    inline int readDecodeConvertAndStore(int *finished);
    inline void clearDecodeCache(uint8_t **convertedInputSamples,
                                 AVFrame *inputFrame);
    inline int loadEncodeAndWrite();
    inline int initOutputFrame(AVFrame **frame, int frameSize);
    inline int encodeAudioFrame(AVFrame *frame, int *dataPresent);
    inline int initInputFrame(AVFrame **frame);
    inline void initPacket(AVPacket *packet);
    inline int decodeAudioFrame(AVFrame *frame, int *dataPresent,
                                int *finished);
    inline int initConvertedSamples(uint8_t ***convertedInputSamples,
                                    int frameSize);
    inline int convertSamples(const uint8_t **inputData,
                              uint8_t **convertedData,
                              const int frameSize,
                              SwrContext *resampleContext);
    inline int addSamplesToFifo(uint8_t **convertedInputSamples,
                                const int frameSize);
    QString m_outputFormat;
    int64_t m_timestamp;
    AVFormatContext *m_inputFormatContext, *m_outputFormatContext;
    AVCodecContext *m_inputCodecContext, *m_outputCodecContext;
    SwrContext *m_resampleContext;
    AVAudioFifo *m_fifo;
    int m_outputCodecId;
    int m_outputBitRate;
    int m_channels;
};

#endif // KNMUSICFFMPEGTRANSCODER_H
