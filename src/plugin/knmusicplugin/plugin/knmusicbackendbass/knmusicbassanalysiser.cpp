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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "bass.h"

#include "knmusicbassanalysiser.h"

KNMusicBassAnalysiser::KNMusicBassAnalysiser(QObject *parent) :
    KNMusicAnalysiser(parent)
{
}

bool KNMusicBassAnalysiser::analysis(KNMusicDetailInfo &detailInfo)
{
    //Generate the analysis thread.
    DWORD analysisChannel=
    //This function is optimized for speed.
#ifdef Q_OS_WIN
            BASS_StreamCreateFile(FALSE,
                                  detailInfo.filePath.toStdWString().data(),
                                  0,
                                  0,
                                  BASS_MUSIC_DECODE | BASS_UNICODE);
#endif
#ifdef Q_OS_UNIX
            BASS_StreamCreateFile(FALSE,
                                  detailInfo.filePath.toStdWString().data(),
                                  0,
                                  0,
                                  BASS_MUSIC_DECODE);
#endif
    //Check the channel.
    if(NULL==analysisChannel)
    {
        return false;
    }
    //Parse the file.
    //Get the duration.
    QWORD byteLength=BASS_ChannelGetLength(analysisChannel, BASS_POS_BYTE);
    //Translate the byte length to second duration.
    double secondDuration=BASS_ChannelBytes2Seconds(analysisChannel,
                                                    byteLength);
    //Some file is too short, we need to force set the duration to 1 second.
    if(0==secondDuration)
    {
        secondDuration=1;
    }
    //Save the duration (unit: ms)
    detailInfo.duration=secondDuration*1000;
    //Save the bit rate.
    //Bitrate (Kbps)
    detailInfo.bitRate=(DWORD)(BASS_StreamGetFilePosition(analysisChannel,
                                                          BASS_FILEPOS_END)/
                               125/secondDuration+0.5);
    //Save the sampling rate.
    BASS_CHANNELINFO channelInfo;
    BASS_ChannelGetInfo(analysisChannel, &channelInfo);
    detailInfo.samplingRate=channelInfo.freq;
    //Free the channel after use.
    BASS_StreamFree(analysisChannel);
    return true;
}
