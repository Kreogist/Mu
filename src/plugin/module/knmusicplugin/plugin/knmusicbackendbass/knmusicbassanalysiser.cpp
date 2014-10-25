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
#include "bass.h"

#include "knmusicbassanalysiser.h"

KNMusicBassAnalysiser::KNMusicBassAnalysiser(QObject *parent) :
    KNMusicAnalysiser(parent)
{
}

bool KNMusicBassAnalysiser::analysis(const QString &filePath,
                                     KNMusicDetailInfo &detailInfo)
{
    DWORD infoCollectChannel;
    //This function is optimized for speed.
#ifdef Q_OS_WIN32
    infoCollectChannel=
                BASS_StreamCreateFile(FALSE,
                                      filePath.toStdWString().data(),
                                      0,
                                      0,
                                      BASS_MUSIC_DECODE | BASS_UNICODE);
#endif
#ifdef Q_OS_UNIX
    infoCollectChannel=
                BASS_StreamCreateFile(FALSE,
                                      filePath.toStdString().data(),
                                      0,
                                      0,
                                      BASS_MUSIC_DECODE);
#endif
    if(infoCollectChannel)
    {
        //Duration
        QWORD byteLength=
                BASS_ChannelGetLength(infoCollectChannel, BASS_POS_BYTE);
        double secondDuration=BASS_ChannelBytes2Seconds(infoCollectChannel,
                                                        byteLength);
        //Some file is too short, we need to force set the duration to 1 second.
        if(0==secondDuration)
        {
            secondDuration=1;
        }
        //Bitrate (Kbps)
        detailInfo.bitRate=(DWORD)(BASS_StreamGetFilePosition(infoCollectChannel, BASS_FILEPOS_END)/
                                   125/secondDuration+0.5);
        //Duration (ms)
        detailInfo.duration=secondDuration*1000;
        //Sampling rate (Hz)
        BASS_CHANNELINFO channelInfo;
        BASS_ChannelGetInfo(infoCollectChannel,&channelInfo);
        detailInfo.samplingRate=channelInfo.freq;
        BASS_StreamFree(infoCollectChannel);
        return true;
    }
    return false;
}
