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
#include <QSplitter>

#include "kndropproxycontainer.h"
#include "kncategorytab.h"
#include "knlocalemanager.h"

#include "knmusicsolomenubase.h"
#include "knmusiclibrarycategorydelegate.h"
#include "knmusicsearchbase.h"
#include "knmusicproxymodel.h"
#include "knmusiccategorylistviewbase.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiccategorymodelbase.h"
#include "knmusiccategorydisplay.h"
#include "knmusiclibrarymodel.h"

#include "knmusiclibraryartisttab.h"

#include <QDebug>

KNMusicLibraryArtistTab::KNMusicLibraryArtistTab(QWidget *parent) :
    KNMusicLibraryCategoryTab(parent),
    m_notEmptyCheck(QMetaObject::Connection()),
    m_currentSourceIndex(QModelIndex()),
    m_tab(new KNCategoryTab(this)),
    m_showInArtistTab(new QAction(this)),
    m_dropProxy(new KNDropProxyContainer(this)),
    m_artistList(new KNMusicCategoryListViewBase(m_dropProxy)),
    m_artistDisplay(new KNMusicCategoryDisplay(this, this)),
    m_categoryModel(nullptr),
    m_libraryModel(nullptr)
{
    //Configure the tab button.
    m_tab->setIcon(QIcon(":/plugin/music/category/artist.png"));
    //Configure the show in action.
    connect(m_showInArtistTab, &QAction::triggered,
            this, &KNMusicLibraryArtistTab::onActionShowInArtist);
    //Configure the artist display.
    m_artistDisplay->setCategoryColumn(Artist);
    //Configure the artist list.
    m_artistList->setTabOrder(m_artistList, m_artistDisplay);
    m_artistList->enabledSearch();
    m_artistList->setItemDelegate(
                new KNMusicLibraryCategoryDelegate(m_artistList));
    //Set the drop proxy widget to the content widget.
    setContentWidget(m_dropProxy);
    //Initial the layout for the container, only for auto resize splitter.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_dropProxy);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_dropProxy->setLayout(mainLayout);
    //Initial the main splitter.
    QSplitter *splitter=new QSplitter(m_dropProxy);
    splitter->setHandleWidth(0); //This is beautiful.
    splitter->setChildrenCollapsible(false);
    splitter->setFocusProxy(m_artistList);
    //Set drop proxy container widget's focus proxy to the splitter.
    m_dropProxy->setFocusProxy(splitter);
    //Add splitter to main splitter.
    mainLayout->addWidget(splitter);
    //Add widget to splitter.
    splitter->addWidget(m_artistList);
    splitter->addWidget(m_artistDisplay);
    //Set viewer properties after add widgets.
    splitter->setCollapsible(1, false);
    splitter->setStretchFactor(1, 1);

    //Link the locale manager.
    knI18n->link(this, &KNMusicLibraryArtistTab::retranslate);
    retranslate();
}

QAction *KNMusicLibraryArtistTab::showInAction()
{
    return m_showInArtistTab;
}

QAbstractButton *KNMusicLibraryArtistTab::tab()
{
    return m_tab;
}

void KNMusicLibraryArtistTab::showInTab(const KNMusicDetailInfo &detailInfo)
{
    //Check out the library model has been set or not.
    if(!m_libraryModel)
    {
        return;
    }
    //Clear up all the search result.
    clearSearchRequest();
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

void KNMusicLibraryArtistTab::showIndex(KNMusicModel *musicModel,
                                        const QModelIndex &index)
{
    //Check out the library model has been set or not.
    if((!m_libraryModel) || (!musicModel))
    {
        return;
    }
    //Clear the search result.
    clearSearchRequest();
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

void KNMusicLibraryArtistTab::setCategoryModel(KNMusicCategoryModelBase *model)
{
    //Check whether the category model has been set before.
    if(m_categoryModel!=nullptr)
    {
        //Ignore the next set.
        return;
    }
    //Save the category model.
    m_categoryModel=model;
    //Check the category model.
    if(m_categoryModel==nullptr)
    {
        //Ignore the null model.
        return;
    }
    //Do original set.
    KNMusicLibraryCategoryTab::setCategoryModel(model);
    //Link the artwork update signal.
    connect(m_categoryModel, &KNMusicCategoryModelBase::albumArtUpdate,
            [=](const QModelIndex &updatedIndex)
            {
                //Check out the updated index.
                if(updatedIndex==m_currentSourceIndex)
                {
                    //Update the category index.
                    updateDisplayArtwork(m_currentSourceIndex);
                }
            });
    //Update the model.
    retranslate();

    //! This should be done in constructor, but setModel() is a virtual
    //! function, so we moved here.
    //Set the proxy model to tree view.
    m_artistList->setModel(categoryProxyModel());
    //Connect the requirement here.
    connect(m_artistList, &KNMusicCategoryListViewBase::requireSearchCategory,
            this, &KNMusicLibraryArtistTab::onActionSearchCategory);
    //Set the default sort order.
    categoryProxyModel()->sort(0, Qt::AscendingOrder);
    //Link the selection model to display slot.
    connect(m_artistList->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this, &KNMusicLibraryArtistTab::onActionCategoryIndexChanged);
}

void KNMusicLibraryArtistTab::setLibraryModel(KNMusicLibraryModel *model)
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
    m_artistDisplay->setLibraryModel(m_libraryModel);
}

void KNMusicLibraryArtistTab::showEvent(QShowEvent *event)
{
    //Do original show event.
    KNMusicLibraryCategoryTab::showEvent(event);
    //Ensure we have any artist item, and check whether the current index is
    //vaild.
    checkCategorySelected();
    //Linked library not empty signal to category selected check.
    m_notEmptyCheck=connect(m_libraryModel,
                            &KNMusicLibraryModel::libraryNotEmpty,
                            this,
                            &KNMusicLibraryArtistTab::checkCategorySelected);
}

void KNMusicLibraryArtistTab::hideEvent(QHideEvent *event)
{
    //Do the original hide event.
    KNMusicLibraryCategoryTab::hideEvent(event);
    //Cut down the not empty check.
    disconnect(m_notEmptyCheck);
}

void KNMusicLibraryArtistTab::retranslate()
{
    //Update the tab text.
    m_tab->setText(tr("Artists"));
    //Set the search place holder text.
    m_artistList->setSearchPlaceHolderText(tr("Search in artist"));
    //Set the action caption.
    m_showInArtistTab->setText(tr("Go to Artist"));
    //Update the no category text.
    if(m_categoryModel!=nullptr)
    {
        //Set no category text.
        m_categoryModel->setNoCategoryText(tr("No Artist"));
        //Update the detail info, might update the translation.
        if(m_artistList->currentIndex().isValid())
        {
            //Use category index changed signal to update labels.
            onActionCategoryIndexChanged(m_artistList->currentIndex());
        }
    }
}

void KNMusicLibraryArtistTab::onActionShowInArtist()
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

void KNMusicLibraryArtistTab::onActionCategoryIndexChanged(
        const QModelIndex &index)
{
    //Save the current source index.
    m_currentSourceIndex=categoryProxyModel()->mapToSource(index);
    //CHeck whether it's the no item index.
    if(m_currentSourceIndex.row()==0)
    {
        //Ask the artist display to show the no category item.
        m_artistDisplay->showNoCategoryItem(m_categoryModel->noCategoryText());
        //Mission complete.
        return;
    }
    //Or else we have to show the category.
    //Set the category text.
    m_artistDisplay->setCategoryText(
                m_categoryModel->data(m_currentSourceIndex,
                                      Qt::DisplayRole).toString());
    //Set the category artwork.
    updateDisplayArtwork(m_currentSourceIndex);
}

void KNMusicLibraryArtistTab::onActionSearchCategory(const QString &text)
{
    //Generate a search block.
    KNMusicSearchBlock categoryBlock;
    //Configure the block.
    categoryBlock.value=text;
    //Set to the block list.
    QList<KNMusicSearchBlock> blockList;
    //Add to list.
    blockList.append(categoryBlock);
    //Set to category proxy model.
    categoryProxyModel()->setSearchBlocks(blockList);
    //Check out the proxy model.
    if(categoryProxyModel()->rowCount()==0)
    {
        //Hide all staffs.
        m_artistDisplay->hideAllStaffs();
        return;
    }
    //Check out the text is blank or not.
    if(text.isEmpty())
    {
        //Check the current index is valid or not.
        if(m_artistList->currentIndex().isValid())
        {
            //Let the select one to be the center of the category view.
            m_artistList->scrollTo(m_artistList->currentIndex());
            //Complete the mission.
            return;
        }
    }
    //Or else, select the first item in the proxy model.
    m_artistList->setCurrentIndex(categoryProxyModel()->index(0,0));
}

inline void KNMusicLibraryArtistTab::clearSearchRequest()
{
    //Clear up the music search.
    knMusicGlobal->search()->clear();
    //Hide the search bar.
    m_artistList->hideSearchBar();
    //Ensure show the current index.
    m_artistList->scrollTo(m_artistList->currentIndex(),
                           QAbstractItemView::PositionAtCenter);
}

inline void KNMusicLibraryArtistTab::updateDisplayArtwork(
        const QModelIndex &index)
{
    //Set the category icon right from the library model.
    m_artistDisplay->setCategoryIcon(
        m_libraryModel->artwork(
            m_categoryModel->data(
                index,
                KNMusicCategoryModelBase::CategoryArtworkKeyRole).toString()));
}

void KNMusicLibraryArtistTab::checkCategorySelected()
{
    //Check whether we have category to select, and is category selected or not.
    if(categoryProxyModel()->rowCount() > 0 &&
            !m_artistList->currentIndex().isValid())
    {
        //Select the first item in the proxy model.
        m_artistList->setCurrentIndex(categoryProxyModel()->index(0,0));
    }
}

inline void KNMusicLibraryArtistTab::showAndSelectRow(const int &musicRow)
{
    //Show and select the index row.
    QModelIndex categoryIndex=
            categoryProxyModel()->categoryIndex(
                m_libraryModel->index(musicRow, Artist).data(Qt::DisplayRole));
    //Check is the category index valid.
    if(categoryIndex.isValid())
    {
        //Change the current index of the artist list.
        m_artistList->setCurrentIndex(categoryIndex);
        m_artistList->scrollTo(categoryIndex,
                               QAbstractItemView::PositionAtCenter);
        //Set the display widget to show the index of the song.
        m_artistDisplay->scrollToSourceRow(musicRow);
    }
    //Ask to show the tab.
    emit requireShowTab();
}
