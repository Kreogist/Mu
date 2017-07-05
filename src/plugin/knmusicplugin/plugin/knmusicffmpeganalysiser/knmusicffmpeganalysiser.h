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

#ifndef KNMUSICFFMPEGANALYSISER_H
#define KNMUSICFFMPEGANALYSISER_H

#include "knmusicanalysiser.h"

/*
 * Aug 23th, 2015:
 *  This version is re-written under Mac OS X 10.9 with FFMpeg installed via
 * Home Brew. The version of FFMpeg and the libraries are:
 *    libavutil      55. 58.100 / 55. 58.100
 *    libavcodec     57. 89.100 / 57. 89.100
 *    libavformat    57. 71.100 / 57. 71.100
 *    libavdevice    57.  6.100 / 57.  6.100
 *    libavfilter     6. 82.100 /  6. 82.100
 *    libavresample   3.  5.  0 /  3.  5.  0
 *    libswscale      4.  6.100 /  4.  6.100
 *    libswresample   2.  7.100 /  2.  7.100
 *    libpostproc    54.  5.100 / 54.  5.100
 */

/*!
 * \brief The KNMusicFfmpegAnalysiser class provides a KNMusicAnalysiser
 * realization using the famous library FFMpeg.
 */
class KNMusicFfmpegAnalysiser : public KNMusicAnalysiser
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicFfmpegAnalysiser object.
     * \param parent The parent object.
     */
    explicit KNMusicFfmpegAnalysiser(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicAnalysiser::analysis().
     */
    bool analysis(KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;
};

#endif // KNMUSICFFMPEGANALYSISER_H
