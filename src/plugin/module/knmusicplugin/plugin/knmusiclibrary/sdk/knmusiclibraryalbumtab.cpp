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

#include "kndropproxycontainer.h"
#include "knmusicsearchbase.h"
#include "knmusicsolomenubase.h"
#include "knmusicalbumview.h"
#include "knmusicalbumdetail.h"
#include "knmusiccategorymodel.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiclibrarymodel.h"

#include "knlocalemanager.h"

#include "knmusiclibraryalbumtab.h"

#include <QDebug>

KNMusicLibraryAlbumTab::KNMusicLibraryAlbumTab(QObject *parent) :
    KNMusicLibraryCategoryTab(parent)
{
    //Initial the drop proxy container.
    m_dropProxy=new KNDropProxyContainer(viewerWidget());
    setContentWidget(m_dropProxy);
    connect(m_dropProxy, &KNDropProxyContainer::dropProxyShow,
            this, &KNMusicLibraryAlbumTab::onActionTabShow);
    connect(m_dropProxy, &KNDropProxyContainer::dropProxyHide,
            this, &KNMusicLibraryAlbumTab::onActionTabHide);

    //Initial the layout for the container, only for auto resize splitter.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_dropProxy);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_dropProxy->setLayout(mainLayout);

    //Initial the album detail.
    m_albumDetail=new KNMusicAlbumDetail(m_dropProxy);
    //Initial the album view.
    m_albumView=new KNMusicAlbumView(m_dropProxy);
    m_albumView->setAlbumDetail(m_albumDetail);
    //Get the drop proxy container's focus proxy to album view.
    m_dropProxy->setFocusProxy(m_albumView);
    mainLayout->addWidget(m_albumView);

    //Initial the show in and search action.
    initialShowInAction();
    initialFindAction();
    //Connect retranslate request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicLibraryAlbumTab::retranslate);
    //Retranslate.
    retranslate();
}

QAction *KNMusicLibraryAlbumTab::showInAction()
{
    return m_showInAlbumTab;
}

QString KNMusicLibraryAlbumTab::caption()
{
    return tr("Albums");
}

QPixmap KNMusicLibraryAlbumTab::icon()
{
    return QPixmap(":/plugin/music/category/03_ablums.png");
}

void KNMusicLibraryAlbumTab::showInTab(QString filePath)
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
            //Ask to show the genre tab.
            emit requireShowTab();
            //Change the current category index.
            m_albumView->locateTo(categoryIndex);
            m_albumView->selectAlbum(proxyCategoryModel()->mapToSource(categoryIndex));
            //Set the details to display the index of the song.
            m_albumDetail->scrollToSourceRow(musicRow);
        }
    }
}

void KNMusicLibraryAlbumTab::retranslate()
{
    //Set the goto action caption.
    m_showInAlbumTab->setText(tr("Go to Album"));
    //Set the no album text.
    if(m_categoryModel!=nullptr)
    {
        //Set the no category text.
        m_categoryModel->setNoCategoryText(tr("No Album"));
    }
}

void KNMusicLibraryAlbumTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Do original set library model.
    KNMusicLibraryCategoryTab::setLibraryModel(model);
    //Save the library.
    m_musicLibrary=model;
    //Do connections.
    connect(m_dropProxy, &KNDropProxyContainer::requireAnalysisFiles,
            m_musicLibrary, &KNMusicLibraryModel::addFiles);
    //Set the music library to detail widget.
    m_albumDetail->setLibraryModel(model);
}

void KNMusicLibraryAlbumTab::setCategoryModel(KNMusicCategoryModel *model)
{
    //Save the model pointer.
    m_categoryModel=model;
    //Apply category model.
    KNMusicLibraryCategoryTab::setCategoryModel(m_categoryModel);
    //Update no category text.
    retranslate();

    //! This should be done in constructor, but setModel() is a virtual
    //! function, so we moved here.
    //Set the proxy model to album detail.
    m_albumDetail->setAlbumModel(m_categoryModel);
    //Set the proxy model to album view.
    m_albumView->setModel(proxyCategoryModel());
    proxyCategoryModel()->sort(0, Qt::AscendingOrder);
}

void KNMusicLibraryAlbumTab::onActionSearch(const QString &text)
{
    //Hide the detail and clear the selection first.
    m_albumDetail->flyAwayAlbumDetail();
    //Set the key word to do the search.
    proxyCategoryModel()->setFilterFixedString(text);
    //Update the album view.
    m_albumView->viewport()->update();
}

void KNMusicLibraryAlbumTab::onActionRequireSearch()
{
    //Set focus.
    KNMusicGlobal::musicSearch()->setSearchFocus(m_albumView);
}

void KNMusicLibraryAlbumTab::onActionShowInAlbum()
{
    showInTab(KNMusicGlobal::soloMenu()->currentFilePath());
}

void KNMusicLibraryAlbumTab::initialShowInAction()
{
    //Initial the action.
    m_showInAlbumTab=new QAction(this);
    connect(m_showInAlbumTab, SIGNAL(triggered()),
            this, SLOT(onActionShowInAlbum()));
}

void KNMusicLibraryAlbumTab::initialFindAction()
{
    //Initial the search action
    QAction *findAction=new QAction(m_albumView);
    findAction->setShortcut(QKeySequence(QKeySequence::Find));
    findAction->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(findAction, SIGNAL(triggered()), this, SLOT(onActionRequireSearch()));
    m_albumView->addAction(findAction);
}
