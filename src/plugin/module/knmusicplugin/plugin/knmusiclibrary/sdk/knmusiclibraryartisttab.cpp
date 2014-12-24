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
#include <QSplitter>
#include <QBoxLayout>
#include <QAction>

#include "kndropproxycontainer.h"
#include "knmusicsearchbase.h"
#include "knmusiccategorymodel.h"
#include "knmusiccategorydisplay.h"
#include "knmusiclibrarymodel.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiccategorylistviewbase.h"
#include "knmusicsolomenubase.h"

#include "knlocalemanager.h"

#include "knmusiclibraryartisttab.h"

#include <QDebug>

KNMusicLibraryArtistTab::KNMusicLibraryArtistTab(QObject *parent) :
    KNMusicLibraryCategoryTab(parent)
{
    //Initial the drop proxy container.
    m_dropProxy=new KNDropProxyContainer(viewerWidget());
    setContentWidget(m_dropProxy);
    connect(m_dropProxy, &KNDropProxyContainer::dropProxyShow,
            this, &KNMusicLibraryArtistTab::onActionTabShow);
    connect(m_dropProxy, &KNDropProxyContainer::dropProxyHide,
            this, &KNMusicLibraryArtistTab::onActionTabHide);

    //Initial the layout for the container, only for auto resize splitter.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_dropProxy);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_dropProxy->setLayout(mainLayout);

    //Initial the main splitter.
    m_splitter=new QSplitter(m_dropProxy);
    m_splitter->setHandleWidth(0); //This is beautiful.
    m_splitter->setChildrenCollapsible(false);
    //Set drop proxy container widget's focus proxy to the splitter.
    m_dropProxy->setFocusProxy(m_splitter);
    mainLayout->addWidget(m_splitter);

    //Initial the list.
    m_artistList=new KNMusicCategoryListViewBase(m_dropProxy);
    //Set the splitter's focus proxy to the list.
    m_splitter->setFocusProxy(m_artistList);
    m_splitter->addWidget(m_artistList);

    //Initial the category display.
    m_artistDisplay=new KNMusicCategoryDisplay(m_dropProxy);
    m_artistDisplay->setCategoryColumn(Artist);
    //Set the tab order.
    m_artistList->setTabOrder(m_artistList, m_artistDisplay);
    //Add to widget.
    m_splitter->addWidget(m_artistDisplay);

    //Set viewer properties after add widgets.
    m_splitter->setCollapsible(1, false);
    m_splitter->setStretchFactor(1, 1);

    //Initial the show in and search action.
    initialShowInAction();
    initialFindAction();

    //Connect retranslate request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicLibraryArtistTab::retranslate);
    //Retranslate.
    retranslate();
}

QAction *KNMusicLibraryArtistTab::showInAction()
{
    return m_showInArtistTab;
}

QString KNMusicLibraryArtistTab::caption()
{
    return tr("Artists");
}

QPixmap KNMusicLibraryArtistTab::icon()
{
    return QPixmap(":/plugin/music/category/02_artists.png");
}

void KNMusicLibraryArtistTab::showInTab(QString filePath)
{
    //Clear the search result.
    KNMusicGlobal::musicSearch()->search("");
    //Get the row of the file.
    int musicRow=m_musicLibrary->rowFromFilePath(filePath);
    //If the row is available.
    if(musicRow!=-1)
    {
        //Get the genre name of the row.
        QModelIndex categoryIndex=
                proxyCategoryModel()->categoryIndex(
                    m_musicLibrary->itemText(musicRow,
                                             m_categoryModel->categoryIndex()));
        //Check is the catgeory vaild.
        if(categoryIndex.isValid())
        {
            //Change the current category index.
            m_artistList->setCurrentIndex(categoryIndex);
            m_artistList->scrollTo(categoryIndex,
                                   QAbstractItemView::PositionAtCenter);
            //Set the details to display the index of the song.
            m_artistDisplay->scrollToSourceRow(musicRow);
            //Ask to show the genre tab.
            emit requireShowTab();
        }
    }
}

void KNMusicLibraryArtistTab::retranslate()
{
    //Set the action caption.
    m_showInArtistTab->setText(tr("Go to Artist"));
    //Set the no category text.
    if(m_categoryModel!=nullptr)
    {
        m_categoryModel->setNoCategoryText(tr("No Artist"));
        //Update the detail info, might update the translation.
        if(m_artistList->currentIndex().isValid())
        {
            onActionCategoryIndexChanged(m_artistList->currentIndex());
        }
    }
}

void KNMusicLibraryArtistTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Do original set library model.
    KNMusicLibraryCategoryTab::setLibraryModel(model);
    //Save the library.
    m_musicLibrary=model;
    //Link analysis request connections.
    connect(m_dropProxy, &KNDropProxyContainer::requireAnalysisFiles,
            m_musicLibrary, &KNMusicLibraryModel::addFiles);
    //Set the model.
    m_artistDisplay->setLibraryModel(m_musicLibrary);
}

void KNMusicLibraryArtistTab::setCategoryModel(KNMusicCategoryModel *model)
{
    //Save the model pointer.
    m_categoryModel=model;
    //Apply category model.
    KNMusicLibraryCategoryTab::setCategoryModel(m_categoryModel);
    //Link the artwork update signal.
    connect(m_categoryModel, &KNMusicCategoryModel::categoryAlbumArtUpdate,
            this, &KNMusicLibraryArtistTab::onActionCategoryAlbumArtUpdate);
    //Update the model.
    retranslate();

    //! This should be done in constructor, but setModel() is a virtual
    //! function, so we moved here.
    //Set the proxy model to tree view.
    m_artistList->setModel(proxyCategoryModel());
    proxyCategoryModel()->sort(0, Qt::AscendingOrder);
    connect(m_artistList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicLibraryArtistTab::onActionCategoryIndexChanged);
}

void KNMusicLibraryArtistTab::onActionSearch(const QString &text)
{
    //Search in artist model.
    proxyCategoryModel()->setFilterFixedString(text);
    //We want to track the result index.
    QModelIndex resultIndex=m_artistList->currentIndex();
    //Check whether there's select item in the treeview.
    if(!resultIndex.isValid())
    {
        //Check it's there any other choice of the index.
        if(proxyCategoryModel()->rowCount()>0)
        {
            //New category index.
            resultIndex=proxyCategoryModel()->index(0,0);
            //Set the current index to the first item of the category model.
            m_artistList->setCurrentIndex(resultIndex);
        }
        else
        {
            return;
        }
    }
    //Trace the result index.
    m_artistList->scrollTo(resultIndex,
                           QAbstractItemView::PositionAtCenter);
}

void KNMusicLibraryArtistTab::onActionTabShow()
{
    //Do the original show request.
    KNMusicLibraryCategoryTab::onActionTabShow();
    //Ensure we have any artist item, and check whether the current index is vaild.
    checkCategorySelected();
    //Linked library not empty signal to category selected check.
    m_notEmptyCheck=connect(m_musicLibrary, &KNMusicLibraryModel::libraryNotEmpty,
                            this, &KNMusicLibraryArtistTab::checkCategorySelected);
}

void KNMusicLibraryArtistTab::onActionTabHide()
{
    //Do the original hide request.
    KNMusicLibraryCategoryTab::onActionTabHide();
    //Cut down the not empty check.
    disconnect(m_notEmptyCheck);
}

void KNMusicLibraryArtistTab::onActionRequireSearch()
{
    //Set focus.
    KNMusicGlobal::musicSearch()->setSearchFocus(m_artistList);
}

void KNMusicLibraryArtistTab::onActionShowInArtist()
{
    showInTab(KNMusicGlobal::soloMenu()->currentFilePath());
}

void KNMusicLibraryArtistTab::onActionCategoryAlbumArtUpdate(const QModelIndex &updatedIndex)
{
    //Update the background when the album art update if the index is the
    //current index.
    if(updatedIndex==m_currentSourceIndex)
    {
        setCategoryArtwork(m_currentSourceIndex);
    }
}

void KNMusicLibraryArtistTab::checkCategorySelected()
{
    //Check whether we have category to select, and is category selected or not.
    if(proxyCategoryModel()->rowCount()>0 &&
            !m_artistList->currentIndex().isValid())
    {
        m_artistList->setCurrentIndex(proxyCategoryModel()->index(0,0));
    }
}

void KNMusicLibraryArtistTab::initialShowInAction()
{
    //Initial the artist tab.
    m_showInArtistTab=new QAction(this);
    connect(m_showInArtistTab, SIGNAL(triggered()),
            this, SLOT(onActionShowInArtist()));
}

void KNMusicLibraryArtistTab::initialFindAction()
{
    //Initial the search action
    QAction *findAction=new QAction(m_dropProxy);
    findAction->setShortcut(QKeySequence(QKeySequence::Find));
    findAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(findAction, SIGNAL(triggered()), this, SLOT(onActionRequireSearch()));
    m_dropProxy->addAction(findAction);
}

void KNMusicLibraryArtistTab::setCategoryArtwork(const QModelIndex &categoryIndex)
{
    //Set the artwork.
    m_artistDisplay->setCategoryIcon(
                m_musicLibrary->artwork(m_categoryModel->data(categoryIndex,
                                                              CategoryArtworkKeyRole).toString()));
}

void KNMusicLibraryArtistTab::onActionCategoryIndexChanged(const QModelIndex &index)
{
    m_currentSourceIndex=proxyCategoryModel()->mapToSource(index);
    //Check is the no category item.
    if(m_currentSourceIndex.row()==0)
    {
        m_artistDisplay->showNoCategoryItem(m_categoryModel->noCategoryText());
        return;
    }
    //Set the category text.
    m_artistDisplay->setCategoryText(
                m_categoryModel->data(m_currentSourceIndex,
                                      Qt::DisplayRole).toString());
    //Set the category artwork.
    setCategoryArtwork(m_currentSourceIndex);
}
