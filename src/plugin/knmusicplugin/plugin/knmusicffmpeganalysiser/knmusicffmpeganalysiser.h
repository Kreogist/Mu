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
 *    libavutil      54. 27.100 / 54. 27.100
 *    libavcodec     56. 41.100 / 56. 41.100
 *    libavformat    56. 36.100 / 56. 36.100
 *    libavdevice    56.  4.100 / 56.  4.100
 *    libavfilter     5. 16.101 /  5. 16.101
 *    libavresample   2.  1.  0 /  2.  1.  0
 *    libswscale      3.  1.101 /  3.  1.101
 *    libswresample   1.  2.100 /  1.  2.100
 *    libpostproc    53.  3.100 / 53.  3.100
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
