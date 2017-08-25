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
#include <QAction>
#include <QBoxLayout>

#include "knemptystatewidget.h"
#include "kncategorytab.h"
#include "knlocalemanager.h"

#include "knmusicsearchbase.h"
#include "knmusicproxymodel.h"
#include "knmusicglobal.h"
#include "knmusiclibrarydropproxy.h"
#include "knmusiclibrarymodel.h"
#include "knmusiclibraryemptyhint.h"
#include "knmusiclibrarytreeview.h"
#include "knmusicsolomenubase.h"
#include "knmusicsearchbase.h"

#include "knmusiclibrarysongtab.h"

#include <QDebug>

KNMusicLibrarySongTab::KNMusicLibrarySongTab(QWidget *parent) :
    KNMusicLibraryTab(parent),
    m_tab(new KNCategoryTab(this)),
    m_emptyStateWidget(new KNEmptyStateWidget(this)),
    m_dropProxy(new KNMusicLibraryDropProxy(this)),
    m_emptyHint(new KNMusicLibraryEmptyHint(this)),
    m_libraryModel(nullptr),
    m_treeView(new KNMusicLibraryTreeView(this, this)),
    m_showInSongTab(new QAction(this))
{
    //Set properties.
    setFocusProxy(m_dropProxy);
    //Configure the tab button.
    m_tab->setIcon(QIcon(":/plugin/music/category/song.png"));
    //Configure the tree view.
    m_treeView->proxyModel()->setIdentifier("ProxyModel/Library/Song");
    //Configure the tree view.
    connect(m_treeView, &KNMusicLibraryTreeView::startDraggingSong,
            this, &KNMusicLibrarySongTab::requireShowPlaylistList);
    connect(m_treeView, &KNMusicLibraryTreeView::endDraggingSong,
            this, &KNMusicLibrarySongTab::requireHidePlaylistList);
    //Configure the show in action.
    connect(m_showInSongTab, &QAction::triggered,
            this, &KNMusicLibrarySongTab::onActionShowInSong);
    //Configure the drop proxy widget.
    m_dropProxy->setFocusProxy(m_treeView);
    m_dropProxy->installEventFilter(m_treeView);
    //Initial the layout for drop proxy.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_dropProxy);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_dropProxy->setLayout(mainLayout);
    //Add tree view to drop proxy.
    mainLayout->addWidget(m_treeView);
    //Link the empty hint connections.
    connect(m_emptyHint, &KNMusicLibraryEmptyHint::requireAddToLibrary,
            this, &KNMusicLibrarySongTab::requireAddToLibrary);
    //Configure the empty state widget.
    m_emptyStateWidget->setEmptyWidget(m_emptyHint);
    m_emptyStateWidget->setContentWidget(m_dropProxy);
    m_emptyStateWidget->showEmptyWidget();

    //Link the search.
    connect(knMusicGlobal->search(), &KNMusicSearchBase::requireSearch,
            this, &KNMusicLibrarySongTab::onActionSearch);

    //Link the retranslate.
    knI18n->link(this, &KNMusicLibrarySongTab::retranslate);
    retranslate();
}

QAbstractButton *KNMusicLibrarySongTab::tab()
{
    return m_tab;
}

QAction *KNMusicLibrarySongTab::showInAction()
{
    return m_showInSongTab;
}

void KNMusicLibrarySongTab::showInTab(const KNMusicDetailInfo &detailInfo)
{
    //Check out the library model has been set or not.
    if(!m_libraryModel)
    {
        return;
    }
    //Clear up all the search result.
    knMusicGlobal->search()->clear();
    //Find the detail info in the model.
    int musicRow=m_libraryModel->detailInfoRow(detailInfo);
    //Check out the music row.
    if(musicRow==-1)
    {
        return;
    }
    //Show and select the music row.
    showAndSelectRow(musicRow);
}

void KNMusicLibrarySongTab::showIndex(KNMusicModel *musicModel,
                                      const QModelIndex &index)
{
    //Check out the library model has been set or not.
    if((!m_libraryModel) || (!musicModel))
    {
        return;
    }
    //Clear the search result.
    knMusicGlobal->search()->clear();
    //Check out the music model is the same as the library model.
    if(musicModel==m_libraryModel)
    {
        //Show and select the index row.
        showAndSelectRow(index.row());
        //Mission complete.
        return;
    }
    //Or else we have to find the detail info in the model.
    showInTab(musicModel->rowDetailInfo(index.row()));
}

void KNMusicLibrarySongTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Check whether the library model has been set before.
    if(m_libraryModel!=nullptr)
    {
        //Ignore the next set.
        return;
    }
    //Save the model.
    m_libraryModel=model;
    //Check the library model first.
    if(m_libraryModel==nullptr)
    {
        //We will do nothing if the library model is null.
        return;
    }
    //Set the library model.
    m_dropProxy->setLibraryModelPointer((qint64)m_libraryModel);
    //Give the library model to the tree view.
    m_treeView->setMusicModel(m_libraryModel);
    //Link the analysis requirement to the library model.
    connect(m_dropProxy, &KNMusicLibraryDropProxy::urlsDropped,
            m_libraryModel, &KNMusicLibraryModel::appendUrls);
    connect(m_emptyHint, &KNMusicLibraryEmptyHint::urlsDropped,
            m_libraryModel, &KNMusicLibraryModel::appendUrls);
    //Link the library model signal response to the widgets.
    connect(m_libraryModel, &KNMusicLibraryModel::libraryNotEmpty,
            m_emptyStateWidget, &KNEmptyStateWidget::showContentWidget);
    connect(m_libraryModel, &KNMusicLibraryModel::libraryEmpty,
            m_emptyStateWidget, &KNEmptyStateWidget::showEmptyWidget);
    //Set the default sort state.
    m_treeView->sortByColumn(Name, Qt::AscendingOrder);
}

void KNMusicLibrarySongTab::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    KNMusicLibraryTab::resizeEvent(event);
    //Resize the empty state widget.
    m_emptyStateWidget->resize(size());
}

void KNMusicLibrarySongTab::retranslate()
{
    //Update the tab text.
    m_tab->setText(tr("Songs"));
    //Update the action text.
    m_showInSongTab->setText(tr("Go to Songs"));
}

void KNMusicLibrarySongTab::onActionShowInSong()
{
    //Get the solo menu action.
    KNMusicSoloMenuBase *soloMenu=knMusicGlobal->soloMenu();
    //Check out the proxy model pointer.
    if(soloMenu->triggeredModel()!=nullptr)
    {
        //Get the triggered proxy model.
        KNMusicProxyModel *proxyModel=soloMenu->triggeredModel();
        //Check out the music model.
        showIndex(proxyModel->musicModel(),
                  proxyModel->mapToSource(soloMenu->triggeredIndex()));
    }
}

void KNMusicLibrarySongTab::onActionSearch()
{
    //Check whether the proxy model is nullptr.
    if(m_treeView->proxyModel()==nullptr)
    {
        //Ignore the search request.
        return;
    }
    //Set the search rules to the proxy model.
    m_treeView->proxyModel()->setSearchBlocks(knMusicGlobal->search()->rules());
}

inline void KNMusicLibrarySongTab::showAndSelectRow(int musicRow)
{
    //Simply scroll to the special position.
    m_treeView->scrollToSourceRow(musicRow);
    //Ask to show the tab.
    emit requireShowTab();
}

