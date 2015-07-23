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
#include "knutil.h"

#include "knmusicplaylistmodel.h"
#include "knmusicplaylistlistmodel.h"

#include "knmusicplaylistmanager.h"

KNMusicPlaylistManager::KNMusicPlaylistManager(QObject *parent) :
    QObject(parent),
    m_playlistList(new KNMusicPlaylistListModel(this))
{
}

KNMusicPlaylistListModel *KNMusicPlaylistManager::playlistList()
{
    return m_playlistList;
}

void KNMusicPlaylistManager::createPlaylist()
{
    //Generate the playlist.
    KNMusicPlaylistModel *model=new KNMusicPlaylistModel(m_playlistList);
    //
    ;
}

QString KNMusicPlaylistManager::generateTitle(const QString &preferName)
{
    //Check whether the prefer name won't need to be used to change.
    if(!preferName.isEmpty() &&
            !m_playlistList->findTitle(preferName))
    {
        return preferName;
    }
    //Now we need to generate a name. Start counting at 2, because you have
    //already get the 'Playlist', and the new one should be name as 'Playlist 2'
    quint32 counter=2;
    //Treat the prefer name as the base name.
    QString baseName=preferName;
    //Generate the base name of the file.
    if(preferName.isEmpty())
    {
        //Then it should be a 'New playlist'.
        baseName=tr("New Playlist");
        //If we are here, there may no other playlist named this, check this
        //name first.
        if(!m_playlistList->findTitle(baseName))
        {
            return baseName;
        }
    }
    else
    {
        //Check is the caption's format is like "Title 2"
        int lastSpacePos=preferName.lastIndexOf(' ');
        //If we cannot find the space, means this cannot be a format like that.
        if(lastSpacePos!=-1)
        {
            //Tried to translate the last part to a number.
            //Get the content.
            QString numerousContent=preferName.mid(lastSpacePos+1);
            //Set a bool varible to save the translate result.
            bool translateResult=false;
            //Tried to do the translation.
            int lastIndex=numerousContent.toInt(&translateResult);
            //If it DOES have a number, replace the base name and the counter.
            if(translateResult)
            {
                //Save the new base name.
                baseName=preferName.left(lastSpacePos);
                //Save the new counter.
                counter=lastIndex+1;
            }
        }
    }
    //Link the base name and the counter.
    QString countedName=baseName + " " + QString::number(counter);
    //Check if the counted name can be found in the list, if it cannot be find,
    //then add the counter, relink the name, and check it again.
    while(!m_playlistList->findTitle(countedName))
    {
        //Add counter.
        counter++;
        //Relink the name.
        countedName=baseName + " " + QString::number(counter);
    }
    //Now the counted name should be the valid name.
    return countedName;
}

QString KNMusicPlaylistManager::playlistDirPath() const
{
    return m_playlistDirPath;
}

void KNMusicPlaylistManager::setPlaylistDirPath(const QString &playlistDirPath)
{
    m_playlistDirPath = playlistDirPath;
}
