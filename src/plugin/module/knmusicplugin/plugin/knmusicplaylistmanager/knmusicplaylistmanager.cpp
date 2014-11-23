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
#include <QFile>

#include "plugin/knmusicxspfparser/knmusicxspfparser.h"

#include "sdk/knmusicplaylistloader.h"
#include "sdk/knmusicplaylistlistitem.h"
#include "sdk/knmusicplaylistlistassistant.h"
#include "sdk/knmusicplaylisttab.h"
#include "sdk/knmusicplaylistlist.h"
#include "sdk/knmusicplaylistmodel.h"

#include "knmusicglobal.h"
#include "knmusicnowplayingbase.h"

#include "knmusicplaylistmanager.h"

#include <QDebug>

KNMusicPlaylistManager::KNMusicPlaylistManager(QObject *parent) :
    KNMusicPlaylistManagerBase(parent)
{
    //Set the playlist icon.
    KNMusicPlaylistListAssistant::setPlaylistIcon(
                QIcon(":/plugin/music/playlist/playlist.png"));
    //Set the playlist folder path.
    KNMusicPlaylistListAssistant::setPlaylistFolderPath(
                KNMusicGlobal::musicLibraryPath()+"/Playlist");
    //Initial the default database path.
    m_playlistDatabasePath=KNMusicPlaylistListAssistant::playlistFolderPath() +
                           "/Playlist.mudb";

    //Initial the playlist loader.
    initialPlaylistLoader();
    //Initial playlist viewer UI.
    m_playlistTab=new KNMusicPlaylistTab(this);
    //Generate the playlist list.
    m_playlistList=new KNMusicPlaylistList(this);
    //Set the playlist list and loader.
    m_playlistTab->setPlaylistList(m_playlistList);
    m_playlistTab->setPlaylistLoader(m_playlistLoader);

    //Link the UI's request.
    connect(m_playlistTab, &KNMusicPlaylistTab::requireCreateFirstPlaylist,
            [=](const QStringList &filePath)
            {
                onActionCreatePlaylist(0, filePath);
            });
    connect(m_playlistTab, &KNMusicPlaylistTab::requireLoadPlaylistList,
            this, &KNMusicPlaylistManager::onActionLoadPlaylistList);
    connect(m_playlistTab, &KNMusicPlaylistTab::requireGeneratePlaylist,
            this, &KNMusicPlaylistManager::onActionAddPlaylist);
    connect(m_playlistTab, &KNMusicPlaylistTab::requireMakeCopy,
            this, &KNMusicPlaylistManager::onActionCopyPlaylist);
    connect(m_playlistTab, &KNMusicPlaylistTab::requireImportPlaylist,
            this, &KNMusicPlaylistManager::onActionImportPlaylist);
    connect(m_playlistTab, &KNMusicPlaylistTab::requireRemovePlaylist,
            this, &KNMusicPlaylistManager::onActionRemovePlaylist);
    connect(m_playlistTab, &KNMusicPlaylistTab::currentPlaylistChanged,
            this, &KNMusicPlaylistManager::onActionCurrentPlaylistChanged);
    //When the data of playlist list has been changed, update the detail.
    connect(m_playlistList, &KNMusicPlaylistList::itemChanged,
            m_playlistTab, &KNMusicPlaylistTab::onActionPlaylistItemChanged);
    //Connect add to item request.
    connect(m_playlistList, &KNMusicPlaylistList::requireAddToPlaylist,
            this, &KNMusicPlaylistManager::onActionAddToPlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireAddRowsToPlaylist,
            this, &KNMusicPlaylistManager::onActionAddRowToPlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireCreatePlaylist,
            this, &KNMusicPlaylistManager::onActionCreatePlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireCreateRowsPlaylist,
            this, &KNMusicPlaylistManager::onActionCreateRowPlaylist);
}

KNMusicPlaylistManager::~KNMusicPlaylistManager()
{
    //Save all the playlist changes first.
    saveChangedPlaylist();
    //Check if it has been loaded.
    if(m_playlistListLoaded)
    {
        //Save the playlist list data.
        KNMusicPlaylistListAssistant::savePlaylistDatabase(m_playlistDatabasePath,
                                                           m_playlistList->playlistListData());
    }
}

KNMusicTab *KNMusicPlaylistManager::categoryTab()
{
    return m_playlistTab;
}

void KNMusicPlaylistManager::onActionLoadPlaylistList()
{
    //Disconnect require signal.
    disconnect(m_playlistTab, &KNMusicPlaylistTab::requireLoadPlaylistList,
               this, &KNMusicPlaylistManager::onActionLoadPlaylistList);
    //Break the load requirement.
    m_playlistTab->cutLoadRequirement();
    //Initial a blank playlist files.
    QStringList rawFiles, playlistFiles;
    //Load the raw playlist files.
    KNMusicPlaylistListAssistant::loadPlaylistDatabase(m_playlistDatabasePath,
                                                       rawFiles);
    //Load all the files.
    while(!rawFiles.isEmpty())
    {
        QString currentFile=rawFiles.takeFirst();
        //If we can load this file, add to the playlist list.
        if(importPlaylistFromFile(currentFile))
        {
            playlistFiles.append(currentFile);
        }
    }
    //Set the playlist list data to the list.
    m_playlistList->setPlaylistListData(playlistFiles);
    //Check whether there's any playlist in the model.
    //If so, display the first playlist.
    if(m_playlistList->rowCount()!=0)
    {
        m_playlistTab->setCurrentPlaylist(m_playlistList->index(0,0));
    }
    //Set the flag.
    m_playlistListLoaded=true;
}

void KNMusicPlaylistManager::onActionAddPlaylist()
{
    createBlankPlaylist();
}

void KNMusicPlaylistManager::onActionAddToPlaylist(const int &row,
                                                   const QStringList &filePaths)
{

    //Get the playlist item.
    KNMusicPlaylistListItem *playlistItem=m_playlistList->playlistItem(row);
    //Add files to the item.
    playlistItem->playlistModel()->addFiles(filePaths);
}

void KNMusicPlaylistManager::onActionAddRowToPlaylist(const int &row)
{
    //Get the playlist item.
    KNMusicPlaylistListItem *playlistItem=m_playlistList->playlistItem(row);
    //Call the add drag function in the model to the item.
    playlistItem->playlistModel()->appendDragMusicRows();
}

void KNMusicPlaylistManager::onActionRemovePlaylist(const QModelIndex &index)
{
    int playlistItemRow=index.row();
    //Try to remove the item's file.
    QFile playlistFile(m_playlistList->playlistItem(playlistItemRow)->playlistFilePath());
    if(playlistFile.exists())
    {
        //Remove the playlist file.
        if(!playlistFile.remove())
        {
            //!FIXME: We need to tell user that we cannot remove the file.
            return;
        }
    }
    //Ask now playing to check the model.
    KNMusicGlobal::nowPlaying()->checkRemovedModel(
                m_playlistList->playlistModel(playlistItemRow));
    //Remove that row.
    m_playlistList->removePlaylist(playlistItemRow);
}

void KNMusicPlaylistManager::onActionImportPlaylist(QStringList playlistPaths)
{
    while(!playlistPaths.isEmpty())
    {
        importPlaylistFromFile(playlistPaths.takeFirst());
    }
}

void KNMusicPlaylistManager::onActionCopyPlaylist(const int &index)
{
    //Get the playlist item.
    KNMusicPlaylistListItem *playlistItem=m_playlistList->playlistItem(index);
    //Save that plalist item first
    KNMusicPlaylistListAssistant::writePlaylist(playlistItem);
    //Get a new file name.
    QString copiedFilePath=KNMusicPlaylistListAssistant::alloctPlaylistFilePath();
    //Copy the file in playlist item.
    if(QFile::copy(playlistItem->playlistFilePath(), copiedFilePath))
    {
        //Import that file into playlist list.
        playlistItem=importPlaylistFromFile(copiedFilePath);
        //Check is import successful or not.
        if(playlistItem!=nullptr)
        {
            //Change the playlist name.
            playlistItem->setText(generatePlaylistName(playlistItem->text()));
            //Display the copied playlist.
            m_playlistTab->setCurrentPlaylist(playlistItem->index());
        }
    }
}

void KNMusicPlaylistManager::onActionCreatePlaylist(const int &row,
                                                    const QStringList &filePaths)
{
    //Genreate a blank playlist first.
    KNMusicPlaylistListItem *playlistItem=createBlankPlaylist(row);
    //Add the file to playlist.
    playlistItem->playlistModel()->addFiles(filePaths);
}

void KNMusicPlaylistManager::onActionCreateRowPlaylist(const int &row)
{
    //Genreate a blank playlist first.
    KNMusicPlaylistListItem *playlistItem=createBlankPlaylist(row);
    //Append the drag music rows.
    playlistItem->playlistModel()->appendDragMusicRows();
}

void KNMusicPlaylistManager::onActionCurrentPlaylistChanged(const QModelIndex &current,
                                                            const QModelIndex &previous)
{
    Q_UNUSED(previous)
    //Display the current index playlist.
    //Ask UI to display the playlist item.
    m_playlistTab->displayPlaylistItem(
                m_playlistList->playlistItemFromIndex(current));
}

void KNMusicPlaylistManager::initialPlaylistLoader()
{
    //Initial the loader.
    m_playlistLoader=new KNMusicPlaylistLoader(this);
    //Install all the plugins.
    m_playlistLoader->installPlaylistParser(new KNMusicXSPFParser);
}

void KNMusicPlaylistManager::saveChangedPlaylist()
{
    for(int i=0; i<m_playlistList->rowCount(); i++)
    {
        //Check the item need to save or not.
        KNMusicPlaylistListItem *currentItem=m_playlistList->playlistItem(i);
        if(currentItem->changed())
        {
            KNMusicPlaylistListAssistant::writePlaylist(currentItem);
        }
    }
}

KNMusicPlaylistListItem *KNMusicPlaylistManager::importPlaylistFromFile(const QString &filePath)
{
    KNMusicPlaylistListItem *playlistItem=
            KNMusicPlaylistListAssistant::generatePlaylist();
    //!FIXME: Here we just load the playlist, but I want to load it dymanicly.
    //Using the mu playlist parser first to parse it.
    if(KNMusicPlaylistListAssistant::readPlaylist(filePath, playlistItem))
    {
        //If we can parse it, means it's a standard playlist.
        m_playlistList->appendPlaylist(playlistItem);
        return playlistItem;
    }
    //Parse other type of the data.
    QString playlistTitle;
    QStringList playlistFiles;
    if(m_playlistLoader->parsePlaylist(filePath, playlistTitle, playlistFiles))
    {
        //Set the title.
        playlistItem->setText(playlistTitle);
        //Analysis files.
        playlistItem->playlistModel()->addFiles(playlistFiles);
        //Add to playlist list.
        m_playlistList->appendPlaylist(playlistItem);
        //Set to current playlist.
        m_playlistTab->setCurrentPlaylist(playlistItem->index());
        return playlistItem;
    }
    //Delete the no used item.
    delete playlistItem;
    return nullptr;
}

QString KNMusicPlaylistManager::generatePlaylistName(const QString &preferName)
{
    //Check is prefer name has been used in the list or not.
    if(!preferName.isEmpty() &&
            m_playlistList->match(m_playlistList->index(0,0),
                                  Qt::DisplayRole,
                                  preferName).isEmpty())
    {
        return preferName;
    }
    //Now we need to generate a new plalist name.
    //The base name of the caption.
    QString baseName;
    //Start counting at two, because you have already got baseName playlist if
    //you need this counter.
    int sameFileCounter=2;
    //Genreate the base name.
    if(preferName.isEmpty())
    {
        //Set the base name here.
        baseName=tr("New Playlist");
        //If we are here, there might no other playlist named this, do the check
        //once more.
        if(m_playlistList->match(m_playlistList->index(0,0),
                                 Qt::DisplayRole,
                                 baseName).isEmpty())
        {
            return baseName;
        }
    }
    else
    {
        //Check is the caption's format is like "XXX 2"
        int lastSpace=preferName.lastIndexOf(" ");
        if(lastSpace==-1)
        {
            baseName=preferName;
        }
        else
        {
            //Get the right content and try to tranlsate it to caption.
            QString rightContent=preferName.mid(lastSpace+1);
            bool translateSuccess=false;
            int lastIndex=rightContent.toInt(&translateSuccess);
            //If we have been find a number there, set the counter to that number.
            if(translateSuccess)
            {
                baseName=preferName.left(lastSpace);
                sameFileCounter=lastIndex+1;
            }
            else
            {
                baseName=preferName;
            }
        }
    }
    //If it has been taken add count after it.
    QString linkedName=baseName + " " + QString::number(sameFileCounter);
    while(!m_playlistList->match(m_playlistList->index(0,0),
                                Qt::DisplayRole,
                                linkedName).isEmpty())
    {
        sameFileCounter++;
        linkedName=baseName + " " + QString::number(sameFileCounter);
    }
    return linkedName;
}

KNMusicPlaylistListItem *KNMusicPlaylistManager::createBlankPlaylist(const int &row,
                                                                     const QString &caption)
{
    //Create blank playlist.
    KNMusicPlaylistListItem *playlistItem=
            KNMusicPlaylistListAssistant::generateBlankPlaylist(
                caption.isEmpty()?generatePlaylistName():caption);
    //Insert the playlist item to the prefer row, or else append to the end.
    if(row>0 && row<m_playlistList->rowCount())
    {
        m_playlistList->insertPlaylist(row, playlistItem);
    }
    else
    {
        m_playlistList->appendPlaylist(playlistItem);
    }
    //Set the new playlist to the current playlist.
    m_playlistTab->setCurrentPlaylist(playlistItem->index());
    //Let user rename it automatically.
    m_playlistTab->editPlaylistName(playlistItem->index());
    //Return this item.
    return playlistItem;
}
