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
#include <QSplitter>

#include "knfiledialog.h"
#include "kncategorytab.h"
#include "knemptystatewidget.h"
#include "knlocalemanager.h"
#include "knnotification.h"

// SDKs
#include "knmusicnowplayingbase.h"
#include "knmusicsearchbase.h"

// SDKs of playlists
#include "sdk/knmusicplaylistemptyhint.h"
#include "sdk/knmusicplaylistlist.h"
#include "sdk/knmusicplaylistlistmodel.h"
#include "sdk/knmusicplaylistviewer.h"
#include "sdk/knmusicplaylistmanager.h"
#include "sdk/knmusicplaylistmodel.h"
#include "sdk/knmusicplaylistlistview.h"

// Plugins
#include "plugin/knmusicplaylistitunesxmlparser/knmusicplaylistitunesxmlparser.h"
#include "plugin/knmusicplaylistxspfparser/knmusicplaylistxspfparser.h"
#include "plugin/knmusicplaylistwplparser/knmusicplaylistwplparser.h"
#include "plugin/knmusicplaylistttplparser/knmusicplaylistttplparser.h"
#include "plugin/knmusicplaylistm3uparser/knmusicplaylistm3uparser.h"
#include "plugin/knmusicplaylistm3uparser/knmusicplaylistm3u8parser.h"
#include "plugin/knmusicplaylistm3uparser/knmusicplaylistextm3uparser.h"

#include "knmusicglobal.h"

#include "knmusicplaylist.h"

#include <QDebug>

KNMusicPlaylist::KNMusicPlaylist(QWidget *parent) :
    KNMusicPlaylistBase(parent),
    m_tab(new KNCategoryTab(this)),
    m_container(new KNEmptyStateWidget(this)),
    m_playlistList(new KNMusicPlaylistList(this)),
    m_floatPlaylistList(new KNMusicPlaylistListView()),
    m_playlistViewer(new KNMusicPlaylistViewer(this, this)),
    m_playlistManager(new KNMusicPlaylistManager(this))
{
    //Configure the tab.
    m_tab->setIcon(QIcon(":/plugin/music/playlist/icon.png"));
    //Set the playlist folder path.
    m_playlistManager->setPlaylistDirPath(
                knMusicGlobal->musicLibraryPath()+"/Playlist");
    //Set the playlist list model to the playlist list.
    m_playlistList->setPlaylistList(m_playlistManager->playlistList());
    m_floatPlaylistList->setModel(m_playlistManager->playlistList());
    //Link float playlist list loading request.
    connect(m_floatPlaylistList,
            &KNMusicPlaylistListView::requireInitialPlaylist,
            this, &KNMusicPlaylist::initialPlaylistList);

    //Install the playlist parser.
    m_playlistManager->installPlaylistParser(
                                            new KNMusicPlaylistiTunesXMLParser);
    m_playlistManager->installPlaylistParser(new KNMusicPlaylistXspfParser);
    m_playlistManager->installPlaylistParser(new KNMusicPlaylistWplParser);
    m_playlistManager->installPlaylistParser(new KNMusicPlaylistTtplParser);
    m_playlistManager->installPlaylistParser(new KNMusicPlaylistM3uParser);
    m_playlistManager->installPlaylistParser(new KNMusicPlaylistM3u8Parser);
    m_playlistManager->installPlaylistParser(new KNMusicPlaylistExtM3uParser);
    //Start running the engine.
    m_playlistManager->startParseEngine();

    //Link the GUI with che playlist manager.
    connect(m_playlistManager, &KNMusicPlaylistManager::requireShowContent,
            m_container, &KNEmptyStateWidget::showContentWidget);
    connect(m_playlistManager, &KNMusicPlaylistManager::requireHideContent,
            m_container, &KNEmptyStateWidget::showEmptyWidget);
    connect(m_playlistManager,
            &KNMusicPlaylistManager::requireShowAndRenamePlaylist,
            this, &KNMusicPlaylist::showAndRenamePlaylist);
    //Link playlist list to playlist manager.
    connect(m_playlistList, &KNMusicPlaylistList::requireCreatePlaylist,
            this, &KNMusicPlaylist::onActionCreatePlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireImportPlaylists,
            this, &KNMusicPlaylist::onActionImportPlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireExportPlaylist,
            this, &KNMusicPlaylist::onActionExportPlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireCopyPlaylist,
            this, &KNMusicPlaylist::onActionCopyPlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireRemovePlaylist,
            this, &KNMusicPlaylist::onActionRemovePlaylist);
    connect(m_playlistList, &KNMusicPlaylistList::requireShowPlaylist,
            this, &KNMusicPlaylist::onActionShowPlaylist);

    //Generate the empty hint widget.
    KNMusicPlaylistEmptyHint *emptyHint=new KNMusicPlaylistEmptyHint(this);
    //Link the empty hint with the container.
    connect(emptyHint, &KNMusicPlaylistEmptyHint::requireAddPlaylist,
            this, &KNMusicPlaylist::onActionCreatePlaylist);
    connect(emptyHint, &KNMusicPlaylistEmptyHint::requireImportPlaylists,
            this, &KNMusicPlaylist::onActionImportPlaylist);
    //Set the empty widget and content widget.
    m_container->setEmptyWidget(emptyHint);
    //Get the splitter.
    QSplitter *contentContainer=new QSplitter(this);
    //Configure the splitter.
    contentContainer->setHandleWidth(0);
    contentContainer->setChildrenCollapsible(false);
    //Set the content widget as the splitter.
    m_container->setContentWidget(contentContainer);
    //Add the playlist list and playlist viewer to the splitter.
    contentContainer->addWidget(m_playlistList);
    contentContainer->addWidget(m_playlistViewer);
    //Set the default stretch factor.
    contentContainer->setStretchFactor(1, 1);

    //Link to retranslate.
    knI18n->link(this, &KNMusicPlaylist::retranslate);
    retranslate();
}

QAbstractButton *KNMusicPlaylist::tab()
{
    return m_tab;
}

void KNMusicPlaylist::showIndex(KNMusicModel *musicModel,
                                const QModelIndex &index)
{
    //Check the music model.
    if(musicModel==nullptr)
    {
        return;
    }
    //Clear up the search model.
    if(knMusicGlobal->search())
    {
        knMusicGlobal->search()->clear();
    }
    //Find the music model.
    //Get the playlist list first.
    KNMusicPlaylistListModel *playlistList=m_playlistManager->playlistList();
    //Get the model index of the music model.
    QModelIndex playlistIndex=
            playlistList->playlistIndex(
                static_cast<KNMusicPlaylistModel *>(musicModel));
    //Check validatation of the model index.
    if(playlistIndex.isValid())
    {
        //Select the music data.
        m_playlistList->showPlaylist(playlistIndex);
        //Select the index of the playlist index.
        m_playlistViewer->selectSong(index);
        //Ask to show the tab.
        emit requireShowTab();
    }
}

QWidget *KNMusicPlaylist::playlistFloatList()
{
    //Give the float list widget.
    return m_floatPlaylistList;
}

KNMusicModel *KNMusicPlaylist::musicModel(const QString &identifier)
{
    //Check the initial flag.
    if(!m_playlistManager->isPlaylistListLoaded())
    {
        //Initial the playlist list.
        initialPlaylistList();
    }
    //Check the identifier through the list.
    return m_playlistManager->playlist(identifier);
}

KNMusicProxyModel *KNMusicPlaylist::proxyMusicModel(const QString &identifier)
{
    //Check the identifier.
    return ("ProxyModel/Playlist"==identifier)?m_playlistViewer->proxyModel():
                                               nullptr;
}

void KNMusicPlaylist::showEvent(QShowEvent *event)
{
    //Initial the playlist list when the widget is shown.
    initialPlaylistList();
    //Do the original show event.
    KNMusicPlaylistBase::showEvent(event);
}

void KNMusicPlaylist::resizeEvent(QResizeEvent *event)
{
    //Resize the tab widget.
    KNMusicPlaylistBase::resizeEvent(event);
    //Resize the container.
    m_container->resize(size());
}

void KNMusicPlaylist::retranslate()
{
    //Update the tab title.
    m_tab->setText(tr("Playlist"));

    //Update the error message.
    m_cannotDeleteMessage=tr("Failed to delete the playlist file: \n"
                             "\n"
                             "%1");
}

void KNMusicPlaylist::initialPlaylistList()
{
    //If the playlist list has never been load before, load it.
    if(m_playlistManager->isPlaylistListLoaded())
    {
        return;
    }
    //Load the playlist list.
    m_playlistManager->loadPlaylistList();
    //Get the playlist list temporarily.
    KNMusicPlaylistListModel *playlistList=m_playlistManager->playlistList();
    //Check if there's any playlist exist in the playlist list, select the
    //first one.
    if(playlistList->rowCount()>0)
    {
        //Select the first index.
        m_playlistList->showPlaylist(playlistList->index(0));
    }
}

void KNMusicPlaylist::showAndRenamePlaylist(const QModelIndex &playlistIndex)
{
    //When this function is called, we need to switch to playlist tab.
    emit requireShowTab();
    //Set focus to playlist list.
    m_playlistList->setFocus(Qt::MouseFocusReason);
    //Show that playlist.
    m_playlistList->showPlaylist(playlistIndex);
    //Ask the list view to rename it.
    m_playlistList->renamePlaylist(playlistIndex);
}

void KNMusicPlaylist::onActionShowPlaylist(const QModelIndex &index)
{
    //Show the playlist in viewer.
    m_playlistViewer->setPlaylist(m_playlistManager->playlist(index));
}

void KNMusicPlaylist::onActionCreatePlaylist()
{
    //Generate a empty playlist model in the playlist manager, and get the index
    //of the model.
    QModelIndex playlistIndex=m_playlistManager->createPlaylist();
    //Show and rename the playlist.
    showAndRenamePlaylist(playlistIndex);
    //Make the container switch to the content widget.
    m_container->showContentWidget();
}

void KNMusicPlaylist::onActionImportPlaylist()
{
    //Generate a file dialog.
    QFileDialog importPlaylists(this);
    //Set the file mode and the name filters.
    importPlaylists.setFileMode(QFileDialog::ExistingFiles);
    importPlaylists.setNameFilters(m_playlistManager->playlistFilter());
    //Launch the selector.
    if(importPlaylists.exec() &&
            !importPlaylists.selectedFiles().isEmpty())
    {
        //Import all the selected files to the playlist manager.
        QModelIndex firstPlaylistIndex=
            m_playlistManager->importPlaylists(importPlaylists.selectedFiles());
        //Check whether the index is valid.
        if(firstPlaylistIndex.isValid())
        {
            //Show that playlist.
            m_playlistList->showPlaylist(firstPlaylistIndex);
            //Make the container switch to the content widget.
            m_container->showContentWidget();
        }
    }
}

void KNMusicPlaylist::onActionRemovePlaylist()
{
    //Get the current model from the viewer.
    if(!m_playlistViewer->playlist())
    {
        //Ignore when the playlist model is nullptr.
        return;
    }
    //Get the model which is going to be removed.
    KNMusicPlaylistModel *model=m_playlistViewer->playlist();
    //Check the now playing first.
    if(knMusicGlobal->nowPlaying())
    {
        //Ask the now playing to check the model which is going to be delete.
        knMusicGlobal->nowPlaying()->onActionModelRemoved(model);
    }
    //Remove the playlist file.
    //Check the existence of the file.
    if(QFile::exists(model->filePath()))
    {
        //Remove the file.
        if(!QFile::remove(model->filePath()))
        {
            //Push a notification.
            knNotification->pushOnly(
                        tr("Error"),
                        m_cannotDeleteMessage.arg(model->filePath()));
            //Stop here.
            return;
        }
    }
    //Remove the playlist model.
    m_playlistManager->removePlaylist(model);
}

void KNMusicPlaylist::onActionExportPlaylist()
{
    //Get the current playlist model form the viewer.
    KNMusicPlaylistModel *playlist=m_playlistViewer->playlist();
    //Check the current playlist model.
    if(!playlist)
    {
        return;
    }
    //Generate a file dialog.
    QFileDialog exportPlaylist(this);
    //Set the file mode and the name filters.
    exportPlaylist.setFileMode(QFileDialog::AnyFile);
    exportPlaylist.setAcceptMode(QFileDialog::AcceptSave);
    //Set the default playlist file name.
    exportPlaylist.selectFile(playlist->title());
    //Get the export filter.
    QStringList fileFilter=m_playlistManager->playlistFilter();
    //Remove the all support one.
    fileFilter.removeFirst();
    //Set the export filter.
    exportPlaylist.setNameFilters(fileFilter);
    //Launch the export file.
    if(exportPlaylist.exec() &&
            !exportPlaylist.selectedFiles().isEmpty())
    {
        //Get the index of the selected filter.
        int selectedParserIndex=
                fileFilter.indexOf(exportPlaylist.selectedNameFilter());
        //Check the parser index.
        if(selectedParserIndex==-1)
        {
            //Something wrong happened, we cannot continue.
            return;
        }
        //Export the file.
        m_playlistManager->exportPlaylist(
                    playlist,
                    exportPlaylist.selectedFiles().first(),
                    selectedParserIndex);
    }
}

void KNMusicPlaylist::onActionCopyPlaylist()
{
    //Check the current select playlist.
    if(!m_playlistList->currentIndex().isValid())
    {
        //Ignore the invalid request.
        return;
    }
    //Ask to copy the current selected playlist.
    QModelIndex playlistIndex=m_playlistManager->copyPlaylist(
                m_playlistList->currentIndex());
    //Show and rename the playlist.
    showAndRenamePlaylist(playlistIndex);
}
