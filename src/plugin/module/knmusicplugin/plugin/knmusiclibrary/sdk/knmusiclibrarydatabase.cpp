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
#include "knglobal.h"
#include "knmusicparser.h"
#include "knmusicmodelassist.h"

#include "knmusiclibrarydatabase.h"

#include <QDebug>

KNMusicLibraryDatabase::KNMusicLibraryDatabase(QObject *parent) :
    KNJSONDatabase(parent)
{
    m_global=KNGlobal::instance();
}

void KNMusicLibraryDatabase::updateMusicRow(const int &row,
                                            const QList<QStandardItem *> &musicRow)
{
//    QJsonObject currentObject;
//    //Generate the object for the new row.
//    generateObject(musicRow, currentObject);
//    //Replace the object in the database.
//    replace(row, currentObject);
}

void KNMusicLibraryDatabase::removeMusicRow(const int &row)
{
    //Remove the music row.
    removeAt(row);
}

inline void KNMusicLibraryDatabase::generateObject(const QList<QStandardItem *> &musicRow,
                                                   QJsonObject &musicObject)
{
    musicObject.insert("Name", musicRow.at(Name)->text());
    musicObject.insert("Album", musicRow.at(Album)->text());
    musicObject.insert("AlbumArtist", musicRow.at(AlbumArtist)->text());
    musicObject.insert("AlbumRating", musicRow.at(AlbumRating)->text());
    musicObject.insert("Artist", musicRow.at(Artist)->text());
    musicObject.insert("BeatsPerMinuate", musicRow.at(BeatsPerMinuate)->text());
    musicObject.insert("BitRate", musicRow.at(BitRate)->data(Qt::UserRole).toInt());
    musicObject.insert("Category", musicRow.at(Category)->text());
    musicObject.insert("Comments", musicRow.at(Comments)->text());
    musicObject.insert("Composer", musicRow.at(Composer)->text());
    musicObject.insert("DateAdded",
                       KNMusicModelAssist::dateTimeToDataString(
                           musicRow.at(DateAdded)->data(Qt::UserRole)));
    musicObject.insert("DateModified",
                       KNMusicModelAssist::dateTimeToDataString(
                           musicRow.at(DateModified)->data(Qt::UserRole)));
    musicObject.insert("Description", musicRow.at(Description)->text());
    musicObject.insert("DiscCount", musicRow.at(DiscCount)->text());
    musicObject.insert("DiscNumber", musicRow.at(DiscNumber)->text());
    musicObject.insert("Genre", musicRow.at(Genre)->text());
    musicObject.insert("Kind", musicRow.at(Kind)->text());
    musicObject.insert("LastPlayed",
                       KNMusicModelAssist::dateTimeToDataString(
                           musicRow.at(LastPlayed)->data(Qt::UserRole)));
    musicObject.insert("Plays", musicRow.at(Plays)->text());
    musicObject.insert("Rating", musicRow.at(Rating)->text());
    musicObject.insert("SampleRate", musicRow.at(SampleRate)->data(Qt::UserRole).toInt());
    musicObject.insert("Size",
                       QString::number(musicRow.at(Size)->data(Qt::UserRole).toLongLong()));
    musicObject.insert("Time",
                       QString::number(musicRow.at(Time)->data(Qt::UserRole).toLongLong()));
    musicObject.insert("TrackCount", musicRow.at(TrackCount)->text());
    musicObject.insert("TrackNumber", musicRow.at(TrackNumber)->text());
    musicObject.insert("Year", musicRow.at(Year)->text());

    //Write properties.
    QStandardItem *propertyItem=musicRow.at(Name);
    musicObject.insert("FilePath", propertyItem->data(FilePathRole).toString());
    musicObject.insert("FileName", propertyItem->data(FileNameRole).toString());
    musicObject.insert("ArtworkKeyRole", propertyItem->data(ArtworkKeyRole).toString());
    QString trackFilePath=propertyItem->data(TrackFileRole).toString();
    if(!trackFilePath.isEmpty())
    {
        musicObject.insert("TrackFilePath", trackFilePath);
        musicObject.insert("TrackIndex", propertyItem->data(TrackIndexRole).toInt());
        musicObject.insert("StartPosition",
                           QString::number(propertyItem->data(StartPositionRole).toLongLong()));
    }
}
