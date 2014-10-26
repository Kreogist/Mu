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
#include <QFileInfo>

#include "knmusicplaylistlistitem.h"

#include "knmusicplaylistlistitemassistant.h"

QIcon KNMusicPlaylistListItemAssistant::m_playlistIcon=QIcon();

KNMusicPlaylistListItemAssistant::KNMusicPlaylistListItemAssistant(QObject *parent) :
    QObject(parent)
{
    ;
}

QString KNMusicPlaylistListItemAssistant::playlistFolderPath()
{
    return m_playlistFolderPath;
}

void KNMusicPlaylistListItemAssistant::setPlaylistFolderPath(const QString &playlistFolderPath)
{
    m_playlistFolderPath = playlistFolderPath;
}

QIcon KNMusicPlaylistListItemAssistant::playlistIcon()
{
    return m_playlistIcon;
}

void KNMusicPlaylistListItemAssistant::setPlaylistIcon(const QIcon &playlistIcon)
{
    m_playlistIcon = playlistIcon;
}

QString KNMusicPlaylistListItemAssistant::alloctPlaylistFilePath()
{
    //Initial the playlist file path.
    QString baseName=QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"),
            playlistFilePath=m_playlistFolderPath+baseName+".mplst";
    //Test the file is exsist or not.
    QFileInfo playlistFile(playlistFilePath);
    if(playlistFile.exists())
    {
        //If the file exsist, change the file name, until not exsist.
        int count=1;
        while(playlistFile.exists())
        {
            playlistFilePath=m_playlistFolderPath+
                    baseName+" "+
                    QString::number(count++)+".mplst";
            playlistFile.setFile(playlistFilePath);
        }
    }
    return playlistFilePath;
}

KNMusicPlaylistListItem *KNMusicPlaylistListItemAssistant::generateBlankPlaylist(const QString &caption)
{
    //Generate a default item first.
    KNMusicPlaylistListItem *playlistItem=generatePlaylist(caption);
    //Alloct a file path.
    playlistItem->setPlaylistFilePath(alloctPlaylistFilePath());
    //Return the item.
    return playlistItem;
}

KNMusicPlaylistListItem *KNMusicPlaylistListItemAssistant::generatePlaylist(
        const QString &caption)
{
    //Generate a default playlist.
    KNMusicPlaylistListItem *playlistItem=new KNMusicPlaylistListItem();
    playlistItem->setIcon(m_playlistIcon);
    playlistItem->setText(caption);
    return playlistItem;
}
