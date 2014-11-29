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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "knglobal.h"

#include "knmusicglobal.h"
#include "knmusicmodelassist.h"
#include "knmusicplaylistmodel.h"
#include "knmusicplaylistlistitem.h"

#include "knmusicplaylistlistassistant.h"

#include <QDebug>

using namespace KNMusic;

QIcon KNMusicPlaylistListAssistant::m_playlistIcon=QIcon();
QString KNMusicPlaylistListAssistant::m_playlistFolderPath=QString();
int KNMusicPlaylistListAssistant::m_version=3;

KNMusicPlaylistListAssistant::KNMusicPlaylistListAssistant(QObject *parent) :
    QObject(parent)
{
}

QString KNMusicPlaylistListAssistant::playlistFolderPath()
{
    return m_playlistFolderPath;
}

void KNMusicPlaylistListAssistant::setPlaylistFolderPath(
        const QString &playlistFolderPath)
{
    m_playlistFolderPath = KNGlobal::ensurePathAvaliable(playlistFolderPath);
}

void KNMusicPlaylistListAssistant::loadPlaylistDatabase(const QString &filePath,
                                                        QStringList &data)
{
    //Clear the data first.
    data=QStringList();
    //Open the list file.
    QFile playlistListFile(filePath);
    if(!playlistListFile.exists() ||
            !playlistListFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //Load the data from file.
    QJsonDocument playlistListData=
            QJsonDocument::fromJson(playlistListFile.readAll());
    //CLose the file ASAP.
    playlistListFile.close();
    QJsonObject playlistListObject=playlistListData.object();
    //Check the version, we can only load one version.
    if(playlistListObject["Version"].toInt()!=m_version)
    {
        return;
    }
    //Set the data to the string list.
    QJsonArray playlistData=playlistListObject["Playlists"].toArray();
    while(!playlistData.isEmpty())
    {
        data.append(playlistData.takeAt(0).toString());
    }
}

bool KNMusicPlaylistListAssistant::readPlaylist(const QString &filePath,
                                                KNMusicPlaylistListItem *item)
{
    QFile playlistFile(filePath);
    //If you cannot open it, return false.
    if(!playlistFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    //Read the document from file.
    QJsonDocument playlistDocument=
            QJsonDocument::fromJson(playlistFile.readAll());
    //Close the playlist ASAP.
    playlistFile.close();
    //Get the playlist object.
    QJsonObject playlistObject=playlistDocument.object();
    if(playlistObject.isEmpty())
    {
        return false;
    }
    //Check the playlist version.
    if(playlistObject["Version"].toInt()!=m_version)
    {
        return false;
    }
    //Initial the items.
    item->setData(playlistObject["Name"].toString(), Qt::DisplayRole);
    QFileInfo playlistFileInfo(playlistFile);
    item->setPlaylistFilePath(playlistFileInfo.absoluteFilePath());
    //Set the data to the string list.
    QJsonArray playlistContent=playlistObject["Songs"].toArray();
    //Initial the model.
    for(auto i=playlistContent.begin();
        i!=playlistContent.end();
        ++i)
    {
        //Prepare line data.
        QJsonObject musicItem=(*i).toObject();
        KNMusicDetailInfo currentInfo;
        //Get the text data.
        QJsonArray textData=musicItem["Text"].toArray();
        for(int i=0; i<MusicDataCount; i++)
        {
            currentInfo.textLists[i]=textData.at(i).toString();
        }
        //Read appendix data.
        currentInfo.filePath=musicItem["FilePath"].toString();
        currentInfo.fileName=musicItem["FileName"].toString();
        currentInfo.trackFilePath=musicItem["TrackFilePath"].toString();
        currentInfo.startPosition=musicItem["StartPosition"].toInt();
        currentInfo.size=musicItem["Size"].toInt();
        currentInfo.dateModified=
                KNMusicModelAssist::dataStringToDateTime(
                    musicItem["DateModified"].toString());
        currentInfo.dateAdded=
                KNMusicModelAssist::dataStringToDateTime(
                    musicItem["DateAdded"].toString());
        currentInfo.lastPlayed=
                KNMusicModelAssist::dataStringToDateTime(
                    musicItem["LastPlayed"].toString());
        currentInfo.duration=musicItem["Time"].toInt();
        currentInfo.bitRate=musicItem["BitRate"].toDouble();
        currentInfo.samplingRate=musicItem["SampleRate"].toDouble();
        //Here we need to update some info for multi-locale.
        currentInfo.textLists[DateModified]=
                KNMusicModelAssist::dateTimeToString(currentInfo.dateModified);
        currentInfo.textLists[DateAdded]=
                KNMusicModelAssist::dateTimeToString(currentInfo.dateAdded);
        currentInfo.textLists[LastPlayed]=
                KNMusicModelAssist::dateTimeToString(currentInfo.lastPlayed);
        //Insert the music row.
        item->playlistModel()->appendMusicRow(
                    KNMusicModelAssist::generateRow(currentInfo));
    }
    return true;
}

bool KNMusicPlaylistListAssistant::writePlaylist(KNMusicPlaylistListItem *item)
{
    //We still need to check this item.
    if(item==nullptr)
    {
        return false;
    }
    //Try to open the file as write mode.
    QFile playlistFile(item->playlistFilePath());
    if(!playlistFile.open(QIODevice::WriteOnly))
    {
        return false;
    }
    //Create the playlist content.
    QJsonArray playlistContent;
    KNMusicPlaylistModel *playlistModel=item->playlistModel();
    //Check if the playlist model null. If not, save data.
    if(playlistModel!=nullptr)
    {
        for(int row=0, songSize=playlistModel->rowCount();
            row<songSize;
            ++row)
        {
            //Initial the music item.
            QJsonObject musicItem;
            //Save the text data.
            QJsonArray textData;
            for(int j=0; j<MusicDataCount; j++)
            {
                textData.append(playlistModel->itemText(row, j));
            }
            musicItem["Text"]=textData;
            //Save appendix data.
            musicItem["FilePath"]=
                    playlistModel->rowProperty(row, FilePathRole).toString();
            musicItem["FileName"]=
                    playlistModel->rowProperty(row, FileNameRole).toString();
            musicItem["TrackFilePath"]=
                    playlistModel->rowProperty(row, TrackFileRole).toString();
            musicItem["StartPosition"]=
                    playlistModel->rowProperty(row, StartPositionRole).toInt();
            musicItem["Size"]=
                    playlistModel->roleData(row, Size, Qt::UserRole).toInt();
            musicItem["DateModified"]=
                    KNMusicModelAssist::dateTimeToDataString(
                        playlistModel->roleData(row,
                                                DateModified,
                                                Qt::UserRole));
            musicItem["DateAdded"]=
                    KNMusicModelAssist::dateTimeToDataString(
                        playlistModel->roleData(row,
                                                DateAdded,
                                                Qt::UserRole));
            musicItem["LastPlayed"]=
                    KNMusicModelAssist::dateTimeToDataString(
                        playlistModel->roleData(row,
                                                LastPlayed,
                                                Qt::UserRole));
            musicItem["Time"]=
                    playlistModel->roleData(row, Time, Qt::UserRole).toInt();
            musicItem["BitRate"]=
                    playlistModel->roleData(row, BitRate, Qt::UserRole).toInt();
            musicItem["SampleRate"]=
                    playlistModel->roleData(row, SampleRate, Qt::UserRole).toInt();
            //Add to playlist content.
            playlistContent.append(musicItem);
        }
    }
    //Create the playlist object.
    QJsonObject playlistObject;
    //Set the data of the playlist.
    playlistObject["Version"]=m_version;
    playlistObject["Name"]=item->data(Qt::DisplayRole).toString();
    playlistObject["Songs"]=playlistContent;
    //Create playlist document
    QJsonDocument playlistDocument;
    playlistDocument.setObject(playlistObject);
    //Write document data to the file.
    playlistFile.write(playlistDocument.toJson());
    //Close the file.
    playlistFile.close();
    return true;
}

void KNMusicPlaylistListAssistant::savePlaylistDatabase(const QString &filePath,
                                                        const QStringList &data)
{
    //Initial the list file.
    QFile playlistListFile(filePath);
    //Ensure the file path exist.
    QFileInfo playlistListFileInfo(playlistListFile);
    KNGlobal::ensurePathAvaliable(playlistListFileInfo.absolutePath());
    //Open the file as write only mode.
    if(!playlistListFile.open(QIODevice::WriteOnly))
    {
        return;
    }
    //Generate the Json Document.
    QJsonObject playlistListObject;
    playlistListObject["Version"]=m_version;
    playlistListObject["Playlists"]=QJsonArray::fromStringList(data);
    QJsonDocument playlistListData;
    playlistListData.setObject(playlistListObject);
    //Write playlist list data to the file.
    playlistListFile.write(playlistListData.toJson());
    //Close file for writing.
    playlistListFile.close();
}

QIcon KNMusicPlaylistListAssistant::playlistIcon()
{
    return m_playlistIcon;
}

void KNMusicPlaylistListAssistant::setPlaylistIcon(const QIcon &playlistIcon)
{
    m_playlistIcon = playlistIcon;
}

QString KNMusicPlaylistListAssistant::alloctPlaylistFilePath()
{
    //Initial the playlist file path.
    QString baseName=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"),
            playlistFilePath=m_playlistFolderPath+"/"+baseName+".mplst";
    //Test the file is exsist or not.
    QFileInfo playlistFile(playlistFilePath);
    if(playlistFile.exists())
    {
        //If the file exsist, change the file name, until not exsist.
        int count=1;
        while(playlistFile.exists())
        {
            playlistFilePath=m_playlistFolderPath+"/"+
                    baseName+" "+
                    QString::number(count++)+".mplst";
            playlistFile.setFile(playlistFilePath);
        }
    }
    return playlistFilePath;
}

KNMusicPlaylistListItem *KNMusicPlaylistListAssistant::generateBlankPlaylist(const QString &caption)
{
    //Generate a default item first.
    KNMusicPlaylistListItem *playlistItem=generatePlaylist(caption);
    //Alloct a file path.
    playlistItem->setPlaylistFilePath(alloctPlaylistFilePath());
    //Return the item.
    return playlistItem;
}

KNMusicPlaylistListItem *KNMusicPlaylistListAssistant::generatePlaylist(
        const QString &caption)
{
    //Generate a default playlist.
    KNMusicPlaylistListItem *playlistItem=new KNMusicPlaylistListItem();
    playlistItem->setIcon(m_playlistIcon);
    playlistItem->setText(caption);
    return playlistItem;
}
