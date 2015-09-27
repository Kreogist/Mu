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

#include "knjsondatabase.h"

#include "knmusiclibrarymodel.h"

KNMusicLibraryModel::KNMusicLibraryModel(QObject *parent) :
    KNMusicModel(parent)
{
    ;
}

void KNMusicLibraryModel::recoverModel()
{
    //Check out the row count first, if there's any data then we have to ignore
    //this calling.
    if(rowCount()>0)
    {
        return;
    }
    //Read the database information.
    m_database->read();
    //Initial the total duration.
    quint64 totalDuration=0;
    //Start to insert data to the model.
    beginInsertRows(QModelIndex(),
                    0,
                    m_database->size() - 1);
    //Read the data through the database.
    for(auto i=m_database->begin(); i!=m_database->end(); ++i)
    {
        //Generate the detail info.
        KNMusicDetailInfo turboDetailInfo=generateDetailInfo((*i).toArray());
        //Append it to the model.
        appendDetailInfo(turboDetailInfo);
        //Calcualte the total duration.
        totalDuration+=turboDetailInfo.duration;
    }
    //Set the total duration.
    initialTotalDuration(totalDuration);
    //End to insert data.
    endInsertRows();
    //Because this operation change the row count, the row count changed signal
    //will be emitted.
    emit rowCountChanged();
}

KNMusicDetailInfo KNMusicLibraryModel::generateDetailInfo(
        const QJsonArray &dataArray)
{
    //Generate a new detail info.
    KNMusicDetailInfo detailInfo;
    //Get the information and property array.
    QJsonArray textArray=dataArray.at(0).toArray(),
               propertyArray=dataArray.at(1).toArray();
    //Set the detail info data.
    for(int i=0; i<MusicDataCount; ++i)
    {
        //Save the text list.
        detailInfo.textLists[i]=textArray.at(i).toString();
    }
    //Save the property data.
    detailInfo.filePath=propertyArray.at(PropertyFilePath).toString();
    //Check out the existance of the file path.
    if(!QFileInfo::exists(detailInfo.filePath))
    {
        //Set the cannot play flag to true if the file isn't exist.
        detailInfo.cannotPlay=true;
    }
    detailInfo.fileName=propertyArray.at(PropertyFileName).toString();
    detailInfo.trackFilePath=propertyArray.at(PropertyTrackFilePath).toString();
    detailInfo.trackIndex=propertyArray.at(PropertyTrackIndex).toInt();
    detailInfo.coverImageHash=
            propertyArray.at(PropertyCoverImageHash).toString();
    detailInfo.startPosition=
            propertyArray.at(PropertyStartPosition).toString().toLongLong();
    detailInfo.size=
            propertyArray.at(PropertySize).toString().toLongLong();
    detailInfo.dateModified=
            KNMusicUtil::dataToDateTime(
                propertyArray.at(PropertyDateModified).toString());
    detailInfo.dateAdded=
            KNMusicUtil::dataToDateTime(
                propertyArray.at(PropertyDateAdded).toString());
    detailInfo.dateLastPlayed=
            KNMusicUtil::dataToDateTime(
                propertyArray.at(PropertyLastPlayed).toString());
    detailInfo.duration=
            propertyArray.at(PropertyDuration).toString().toLongLong();
    detailInfo.bitRate=propertyArray.at(PropertyBitRate).toInt();
    detailInfo.samplingRate=propertyArray.at(PropertySamplingRate).toInt();
    return detailInfo;
}
KNJsonDatabase *KNMusicLibraryModel::database() const
{
    return m_database;
}

void KNMusicLibraryModel::setDatabase(KNJsonDatabase *database)
{
    m_database = database;
}

