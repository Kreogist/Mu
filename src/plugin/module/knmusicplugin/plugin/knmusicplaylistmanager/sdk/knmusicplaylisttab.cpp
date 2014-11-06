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
#include <QBoxLayout>
#include <QItemSelectionModel>
#include <QSplitter>

#include "knemptystatewidget.h"
#include "knmusicplaylistemptyhint.h"
#include "knmusicplaylistlistitem.h"
#include "knmusicplaylistlist.h"
#include "knmusicplaylistlistview.h"
#include "knmusicplaylistlistvieweditor.h"
#include "knmusicplaylistdisplay.h"

#include "knmusicplaylisttab.h"

#include <QDebug>

KNMusicPlaylistTab::KNMusicPlaylistTab(QObject *parent) :
    KNMusicTab(parent)
{
    //Initial the viewer.
    m_viewer=new KNEmptyStateWidget();
    connect(m_viewer, &KNEmptyStateWidget::aboutToBeShown,
            this, &KNMusicPlaylistTab::requireLoadPlaylistList);

    //Initial the empty hint widget.
    m_emptyHint=new KNMusicPlaylistEmptyHint(m_viewer);
    connect(m_emptyHint, &KNMusicPlaylistEmptyHint::requireAnalysisFiles,
            this, &KNMusicPlaylistTab::requireCreateFirstPlaylist);
    m_viewer->setEmptyWidget(m_emptyHint);

    //Initial the viewer.
    m_mainViewer=new QSplitter(m_viewer);
    //Set viewer properties.
    m_mainViewer->setContentsMargins(0,0,0,0);
    m_mainViewer->setHandleWidth(0); //This is beautiful.
    m_mainViewer->setChildrenCollapsible(false);
    m_viewer->setContentWidget(m_mainViewer);
    m_viewer->showEmptyWidget();

    //Initial the playlist list.
    initialPlaylistList();
    m_mainViewer->addWidget(m_playlistListViewer);
    //Initial the playlist display.
    m_playlistDisplay=new KNMusicPlaylistDisplay(m_mainViewer);
    m_mainViewer->addWidget(m_playlistDisplay);

    //Set viewer properties after add widgets.
    m_mainViewer->setCollapsible(1, false);
    m_mainViewer->setStretchFactor(1, 1);
}

KNMusicPlaylistTab::~KNMusicPlaylistTab()
{
    if(m_mainViewer->parent()==0)
    {
        delete m_mainViewer;
    }
}

QString KNMusicPlaylistTab::caption()
{
    return tr("Playlist");
}

QPixmap KNMusicPlaylistTab::icon()
{
    return QPixmap(":/plugin/music/category/05_playlists.png");
}

QWidget *KNMusicPlaylistTab::widget()
{
    return m_viewer;
}

KNMusicPlaylistModel *KNMusicPlaylistTab::currentPlaylistModel()
{
    KNMusicPlaylistListItem *currentItem=m_playlistDisplay->currentItem();
    return currentItem==nullptr?nullptr:currentItem->playlistModel();
}

void KNMusicPlaylistTab::cutLoadRequirement()
{
    disconnect(m_viewer, &KNEmptyStateWidget::aboutToBeShown,
               this, &KNMusicPlaylistTab::requireLoadPlaylistList);
}

void KNMusicPlaylistTab::onActionRemoveCurrent()
{
    //Check is current index available.
    if(!m_playlistListView->currentIndex().isValid())
    {
        return;
    }
    //Ask playlist display to backup header and set null model.
    m_playlistDisplay->onActionRemoveCurrent();
    //Send signal.
    emit requireRemovePlaylist(m_playlistListView->currentIndex());
}

void KNMusicPlaylistTab::onActionPlaylistItemChanged(QStandardItem *item)
{
    KNMusicPlaylistListItem *changedItem=
            static_cast<KNMusicPlaylistListItem *>(item);
    if(changedItem!=nullptr && m_playlistDisplay->currentItem()==changedItem)
    {
        m_playlistDisplay->updatePlaylistInfo();
    }
}

void KNMusicPlaylistTab::displayPlaylistItem(KNMusicPlaylistListItem *item)
{
    m_playlistDisplay->displayPlaylistItem(item);
}

void KNMusicPlaylistTab::setPlaylistList(KNMusicPlaylistList *playlistList)
{
    //Save the pointer.
    m_playlistList=playlistList;
    //Set the playlist model.
    m_playlistListView->setModel(m_playlistList);
    //Connect when the row count changed.
    connect(m_playlistList, &KNMusicPlaylistList::requireShowContent,
            m_viewer, &KNEmptyStateWidget::showContentWidget);
    connect(m_playlistList, &KNMusicPlaylistList::requireHideContent,
            m_viewer, &KNEmptyStateWidget::showEmptyWidget);
    //Connect current changed signal.
    connect(m_playlistListView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicPlaylistTab::currentPlaylistChanged);
}

void KNMusicPlaylistTab::setCurrentPlaylist(const QModelIndex &index)
{
    //Check index available first.
    if(!index.isValid())
    {
        return;
    }
    //Set current index.
    m_playlistListView->setCurrentIndex(index);
}

void KNMusicPlaylistTab::editPlaylistName(const QModelIndex &index)
{
    m_playlistListView->edit(index);
}

void KNMusicPlaylistTab::onActionCopyCurrent()
{
    emit requireMakeCopy(m_playlistListView->currentIndex().row());
}

void KNMusicPlaylistTab::initialPlaylistList()
{
    //Initial the container.
    m_playlistListViewer=new QWidget(m_mainViewer);
    //Set properties.
    m_playlistListViewer->setContentsMargins(0,0,0,0);

    //Initial the layout.
    QBoxLayout *playlistListLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                  m_playlistListViewer);
    playlistListLayout->setContentsMargins(0,0,0,0);
    playlistListLayout->setSpacing(0);
    m_playlistListViewer->setLayout(playlistListLayout);

    //Initial the list view.
    m_playlistListView=new KNMusicPlaylistListView(m_playlistListViewer);
    playlistListLayout->addWidget(m_playlistListView);

    //Initial the list editor.
    m_playlistListViewEditor=new KNMusicPlaylistListViewEditor(m_playlistListViewer);
    connect(m_playlistListViewEditor, &KNMusicPlaylistListViewEditor::requireAddPlaylist,
            this, &KNMusicPlaylistTab::requireGeneratePlaylist);
    connect(m_playlistListViewEditor, &KNMusicPlaylistListViewEditor::requireImportPlaylist,
            this, &KNMusicPlaylistTab::requireImportPlaylist);
    connect(m_playlistListViewEditor, &KNMusicPlaylistListViewEditor::requireRemoveCurrentPlaylist,
            this, &KNMusicPlaylistTab::onActionRemoveCurrent);
    connect(m_playlistListViewEditor, &KNMusicPlaylistListViewEditor::requireCopyCurrentPlaylist,
            this, &KNMusicPlaylistTab::onActionCopyCurrent);
    playlistListLayout->addWidget(m_playlistListViewEditor);
}
