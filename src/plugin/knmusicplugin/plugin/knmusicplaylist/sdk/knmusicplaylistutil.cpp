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
#include <QJsonArray>

#include "knmusicutil.h"

#include "knmusicplaylistutil.h"

KNMusicDetailInfo KNMusicPlaylistUtil::objectToDetailInfo(
        const QJsonObject &object)
{
    //Generate a detail info struct.
    KNMusicDetailInfo detailInfo;
    //Get the text data from the object.
    QJsonArray textData=object.value("Text").toArray();
    //Check the size of the text data before. Never trust any user input.
    if(textData.size()==MusicDataCount)
    {
        //Write the data to text lists.
        for(int i=0; i<MusicDataCount; i++)
        {
            //Save the text caption.
            detailInfo.textLists[i]=textData.at(i).toString();
        }
    }
    //Read the other appendix data.
    detailInfo.filePath=object.value("FilePath").toString();
    detailInfo.fileName=object.value("FileName").toString();
    detailInfo.trackFilePath=object.value("TrackFilePath").toString();
    detailInfo.trackIndex=object.value("TrackIndex").toInt();
    detailInfo.startPosition=object.value("StartPosition").toDouble();
    detailInfo.size=object.value("Size").toDouble();
    detailInfo.dateModified=
            KNMusicUtil::dataToDateTime(object.value("DateModified").toString());
    detailInfo.dateAdded=
            KNMusicUtil::dataToDateTime(object.value("DateAdded").toString());
    detailInfo.dateLastPlayed=
            KNMusicUtil::dataToDateTime(object.value("LastPlayed").toString());
    detailInfo.duration=object.value("Time").toDouble();
    detailInfo.bitRate=object.value("BitRate").toDouble();
    detailInfo.samplingRate=object.value("SampleRate").toDouble();
    //Retranslate some text for multi-locale support.
    detailInfo.textLists[DateModified]=
            KNMusicUtil::dateTimeToText(detailInfo.dateModified);
    detailInfo.textLists[DateAdded]=
            KNMusicUtil::dateTimeToText(detailInfo.dateAdded);
    detailInfo.textLists[LastPlayed]=
            KNMusicUtil::dateTimeToText(detailInfo.dateLastPlayed);
    //Detail info loaded complete.
    return detailInfo;
}
