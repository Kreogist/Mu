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
#include <QAction>
#include <QBoxLayout>

#include "knlocalemanager.h"
#include "kndropproxycontainer.h"
#include "kncategorytab.h"

#include "knmusicsearchbase.h"
#include "knmusicsolomenubase.h"
#include "knmusicproxymodel.h"
#include "knmusicglobal.h"
#include "knmusicalbumview.h"
#include "knmusicalbumdetail.h"
#include "knmusicalbummodel.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiclibrarymodel.h"

#include "knmusiclibraryalbumtab.h"

#include <QDebug>

KNMusicLibraryAlbumTab::KNMusicLibraryAlbumTab(QWidget *parent) :
    KNMusicLibraryCategoryTab(parent),
    m_tab(new KNCategoryTab(this)),
    m_showInAlbumTab(new QAction(this)),
    m_dropProxy(new KNDropProxyContainer(this)),
    m_albumDetail(new KNMusicAlbumDetail(m_dropProxy, this)),
    m_albumView(new KNMusicAlbumView(m_dropProxy)),
    m_albumModel(nullptr),
    m_libraryModel(nullptr)
{
    //Configure the tab button.
    m_tab->setIcon(QIcon(":/plugin/music/category/ablum.png"));
    //Configure the show in action.
    connect(m_showInAlbumTab, &QAction::triggered,
            this, &KNMusicLibraryAlbumTab::onActionShowInAlbum);
    //Configure the drop proxy container.
    m_dropProxy->setFocusProxy(m_albumView);
    setContentWidget(m_dropProxy);
    //Configure the album view.
    m_albumView->setAlbumDetail(m_albumDetail);

    //Initial the layout for the container, only for auto resize splitter.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_dropProxy);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_dropProxy->setLayout(mainLayout);
    //Add the album view to the main layout.
    mainLayout->addWidget(m_albumView);

    //Link retranslate.
    knI18n->link(this, &KNMusicLibraryAlbumTab::retranslate);
    retranslate();
}

QAction *KNMusicLibraryAlbumTab::showInAction()
{
    return m_showInAlbumTab;
}

QAbstractButton *KNMusicLibraryAlbumTab::tab()
{
    return m_tab;
}

void KNMusicLibraryAlbumTab::showInTab(const KNMusicDetailInfo &detailInfo)
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

void KNMusicLibraryAlbumTab::showIndex(KNMusicModel *musicModel,
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

void KNMusicLibraryAlbumTab::setCategoryModel(KNMusicCategoryModelBase *model)
{
    //Check whether we set it before.
    if(m_albumModel!=nullptr)
    {
        return;
    }
    //Save category model pointer.
    m_albumModel=static_cast<KNMusicAlbumModel *>(model);
    //Check out the album model.
    if(m_albumModel==nullptr)
    {
        return;
    }

    //Do original set.
    KNMusicLibraryCategoryTab::setCategoryModel(model);
    //Link the artwork update signal.
    connect(m_albumModel, &KNMusicAlbumModel::albumRemoved,
            m_albumDetail, &KNMusicAlbumDetail::onActionAlbumRemoved);
    connect(m_albumModel, &KNMusicAlbumModel::albumArtUpdate,
            m_albumDetail, &KNMusicAlbumDetail::onActionAlbumArtUpdate);
    //Update the model.
    retranslate();

    //This should be done in constructor, but setModel() is a virtual
    //function, so we moved here.
    //Set the proxy model to album view.
    m_albumView->setModel(categoryProxyModel());
    //Set the default sort order.
    categoryProxyModel()->sort(0, Qt::AscendingOrder);
}

void KNMusicLibraryAlbumTab::setLibraryModel(KNMusicLibraryModel *model)
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

    //Do original set library model.
    KNMusicLibraryCategoryTab::setLibraryModel(m_libraryModel);
    //Link analysis request connections.
    connect(m_dropProxy, &KNDropProxyContainer::urlsDropped,
            m_libraryModel, &KNMusicLibraryModel::appendUrls);
    //Set the model to display.
    m_albumDetail->setLibraryModel(m_libraryModel);
}

void KNMusicLibraryAlbumTab::setAlbumArtHash(
        QHash<QString, QVariant> *hashAlbumArt)
{
    //Give the hash album art to album detail.
    m_albumDetail->setAlbumArtHash(hashAlbumArt);
}

void KNMusicLibraryAlbumTab::retranslate()
{
    //Update the tab text.
    m_tab->setText(tr("Albums"));
    //Set the action caption.
    m_showInAlbumTab->setText(tr("Go to Albums"));
    //Update the no category text.
    if(m_albumModel!=nullptr)
    {
        //Set no category text.
        m_albumModel->setNoCategoryText(tr("No Album"));
        //Update the detail info, might update the translation.
        if(m_albumView->currentIndex().isValid())
        {
            //Use category index changed signal to update labels.
            m_albumDetail->updateAlbumCaptions();
        }
    }
}

void KNMusicLibraryAlbumTab::onActionShowInAlbum()
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

inline void KNMusicLibraryAlbumTab::showAndSelectRow(const int &musicRow)
{
    //Show and select the index row.
    QModelIndex &&categoryIndex=
            categoryProxyModel()->categoryIndex(
                m_libraryModel->index(musicRow, Album).data(Qt::DisplayRole));
    //Check is the category index valid.
    if(categoryIndex.isValid())
    {
        //Change the current index of the album view.
        m_albumView->locateTo(categoryIndex);
        m_albumView->selectAlbum(
                    categoryProxyModel()->mapToSource(categoryIndex));
        //Set the details to display the index of the song.
        m_albumDetail->scrollToSourceRow(musicRow);
    }
    //Ask to show the tab.
    emit requireShowTab();
}
