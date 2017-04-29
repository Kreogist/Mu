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

#ifndef KNMUSICTRANSCODERBASE_H
#define KNMUSICTRANSCODERBASE_H

#include <QObject>

/*!
 * \brief The KNMusicTranscoderBase class provides a code state for the music
 * file transcode. It has the basic ports for all the transcoder backend.
 */
class KNMusicTranscoderBase : public QObject
{
    Q_OBJECT
public:
    enum TransCoderError
    {
        NoError,
        InputFailToOpen,
        InputFailToOpenStreamInfo,
        InputMoreThanOneAudioStream,
        InputFailToFindAudioStream,
        InputFailToFindCodec,
        InputFailToAllocDecContext,
        InputFailToCopyParameter,
        InputFailToOpenCodec,
        OutputFailToOpen,
        OutputFailToAllocFormatContext,
        OutputFailToFindFormat,
        OutputFailToFindEncoder,
        OutputFailToCreateNewStream,
        OutputFailToAllocEecContext,
        OutputFailToOpenCodec,
        OutputFailToInitStreamParameter,
        ResampleFailToAlloc,
        ResampleFailToOpen,
        FifoFailToAlloc
    };

    enum TranscoderEncoder
    {
        LossyAac,
        LossyAc3,
        LossyAdpcmAdx,
        LossyAdpcmG722,
        LossyAdpcmG726,
        LossyAdpcmImaWav,
        LossyAdpcmMs,
        LossyAdpcmSwf,
        LossyAdpcmYamaha,
        LossyEac3,
        LossyG7231,
        LossyIlbc,
        LossyMp2,
        LossyMp3,
        LossyNellymoser,
        LossyPcmAlaw,
        LossyPcmMulaw,
        LossyRa144,
        LossyRoqDpcm,
        LossyVorbis,
        LossyWmav1,
        LossyWmav2,
        LosslessAlac,
        LosslessDts,
        LosslessFlac,
        LosslessMlp,
        LosslessPcmF32be,
        LosslessPcmF32le,
        LosslessPcmF64be,
        LosslessPcmF64le,
        LosslessPcmS16be,
        LosslessPcmS16bePlanar,
        LosslessPcmS16le,
        LosslessPcmS16lePlanar,
        LosslessPcmS24be,
        LosslessPcmS24daud,
        LosslessPcmS24le,
        LosslessPcmS24lePlanar,
        LosslessPcmS32be,
        LosslessPcmS32le,
        LosslessPcmS32lePlanar,
        LosslessPcmS8,
        LosslessPcmS8Planar,
        LosslessPcmU16be,
        LosslessPcmU16le,
        LosslessPcmU24be,
        LosslessPcmU24le,
        LosslessPcmU32be,
        LosslessPcmU32le,
        LosslessPcmU8,
        LosslessS302m,
        LosslessTruehd,
        LosslessTta,
        LosslessWavpack
    };

    /*!
     * \brief Construct a KNMusicTranscoderBase object.
     * \param parent The parent object.
     */
    KNMusicTranscoderBase(QObject *parent = 0): QObject(parent){}

signals:
    /*!
     * \brief When a error is happend when transcoding the file, this signal
     * will be emit with the error information detail.
     * \param errorId The error index.
     * \param errorText The error text.
     */
    void transcodingError(int errorId, QString errorText);

public slots:
    /*!
     * \brief Set the input file path. If the transcoder could not open the
     * input file, return the error code.
     * \param filePath The input file path.
     * \param errorInfo The error information when error happend.
     * \return If the transcoder could open the file, return no error. Or else
     * the error code.
     */
    virtual int setInputFile(const QString &filePath,
                             QString &errorInfo)=0;

    /*!
     * \brief Set the output file path. If the transcoder could not open the
     * output file, return the error code.
     * \param filePath The output file path.
     * \param errorInfo The error information when error happend.
     * \return If the transcoder could open the file, return no error. Or else
     * the error code.
     */
    virtual int setOutputFile(const QString &filePath,
                              QString &errorInfo)=0;

    /*!
     * \brief Set the output file encoder.
     * \param encoder The encoder type.
     * \return If the transcoder could detect the encoder, return true. Or else
     * return false.
     */
    virtual bool setEncoder(TranscoderEncoder encoder)=0;

    /*!
     * \brief Start to execute the transcoder.
     */
    virtual void execute()=0;

    /*!
     * \brief Clear the transcoder caches.
     */
    virtual void clear()=0;
};

#endif // KNMUSICTRANSCODERBASE_H
