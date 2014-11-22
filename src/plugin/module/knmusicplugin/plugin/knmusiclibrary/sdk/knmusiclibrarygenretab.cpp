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
#include "knmusicgenremodel.h"
#include "knmusiccategorydisplay.h"
#include "knmusiclibrarymodel.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiccategorylistviewbase.h"
#include "knmusicglobal.h"
#include "knmusicsolomenubase.h"

#include "knlocalemanager.h"

#include "knmusiclibrarygenretab.h"

KNMusicLibraryGenreTab::KNMusicLibraryGenreTab(QObject *parent) :
    KNMusicLibraryCategoryTab(parent)
{
    //Initial the drop proxy container.
    m_container=new KNDropProxyContainer;
    connect(m_container, &KNDropProxyContainer::dropProxyShow,
            this, &KNMusicLibraryGenreTab::onActionTabShow);
    connect(m_container, &KNDropProxyContainer::dropProxyHide,
            this, &KNMusicLibraryGenreTab::onActionTabHide);

    //Initial the layout for the container, only for auto resize splitter.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_container);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_container->setLayout(mainLayout);

    //Initial the main splitter.
    m_splitter=new QSplitter(m_container);
    m_splitter->setHandleWidth(0); //This is beautiful.
    m_splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(m_splitter);

    //Initial the list.
    m_genreList=new KNMusicCategoryListViewBase(m_container);
    m_splitter->addWidget(m_genreList);

    //Initial the category display.
    m_genreDisplay=new KNMusicCategoryDisplay(m_container);
    m_genreDisplay->setCategoryColumn(Genre);
    m_splitter->addWidget(m_genreDisplay);

    //Set viewer properties after add widgets.
    m_splitter->setCollapsible(1, false);
    m_splitter->setStretchFactor(1, 1);

    //Initial the show in action.
    initialShowInAction();

    //Connect retranslate request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicLibraryGenreTab::retranslate);
    //Retranslate.
    retranslate();
}

QAction *KNMusicLibraryGenreTab::showInAction()
{
    return m_showInGenre;
}

QString KNMusicLibraryGenreTab::caption()
{
    return tr("Genres");
}

QPixmap KNMusicLibraryGenreTab::icon()
{
    return QPixmap(":/plugin/music/category/04_genres.png");
}

QWidget *KNMusicLibraryGenreTab::widget()
{
    return m_container;
}

void KNMusicLibraryGenreTab::retranslate()
{
    m_showInGenre->setText(tr("Go to Genre"));
}

void KNMusicLibraryGenreTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Save the library.
    m_musicLibrary=model;
    //Do connections.
    connect(m_container, &KNDropProxyContainer::requireAnalysisFiles,
            m_musicLibrary, &KNMusicLibraryModel::addFiles);
    //Set the model.
    m_genreDisplay->setLibraryModel(m_musicLibrary);
}

void KNMusicLibraryGenreTab::setCategoryModel(KNMusicCategoryModel *model)
{
    //Save the model pointer.
    m_categoryModel=static_cast<KNMusicGenreModel *>(model);
    //Set the no category text.
    m_categoryModel->setNoCategoryText(tr("No Genre"));
    //Apply category model.
    KNMusicLibraryCategoryTab::setCategoryModel(m_categoryModel);

    //! This should be done in constructor, but setModel() is a virtual
    //! function, so we moved here.
    //Set the proxy model to tree view.
    m_genreList->setModel(proxyCategoryModel());
    proxyCategoryModel()->sort(0, Qt::AscendingOrder);
    connect(m_genreList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicLibraryGenreTab::onActionCategoryIndexChanged);
}

void KNMusicLibraryGenreTab::onActionSearch(const QString &text)
{
    ;
}

void KNMusicLibraryGenreTab::onActionCategoryIndexChanged(const QModelIndex &index)
{
    QModelIndex categoryIndex=proxyCategoryModel()->mapToSource(index);
    //Check is the no category item.
    if(categoryIndex.row()==0)
    {
        m_genreDisplay->showNoCategoryItem(m_categoryModel->noCategoryText());
        return;
    }
    QString genreName=m_categoryModel->data(categoryIndex,
                                            Qt::DisplayRole).toString();
    //Set the category text.
    m_genreDisplay->setCategoryText(genreName);
    //Set the artwork.
    m_genreDisplay->setCategoryIcon(m_categoryModel->genreIcon(genreName));
}

void KNMusicLibraryGenreTab::onActionTabShow()
{
    //Do the original show request.
    KNMusicLibraryCategoryTab::onActionTabShow();
    //Ensure we have any artist item, and check whether the current index is vaild.
    if(proxyCategoryModel()->rowCount()>0 &&
            !m_genreList->currentIndex().isValid())
    {
        m_genreList->setCurrentIndex(proxyCategoryModel()->index(0,0));
    }
}

void KNMusicLibraryGenreTab::onActionShowInGenre()
{
    //Get the row of the file.
    int musicRow=m_musicLibrary->rowFromFilePath(KNMusicGlobal::soloMenu()->currentFilePath());
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
            m_genreList->setCurrentIndex(categoryIndex);
            m_genreList->scrollTo(categoryIndex,
                                  QAbstractItemView::PositionAtCenter);
            //Set the details to display the index of the song.
            m_genreDisplay->scrollToSourceRow(musicRow);
            //Ask to show the genre tab.
            emit requireShowTab();
        }
    }
}

void KNMusicLibraryGenreTab::initialShowInAction()
{
    //Initial the show in action.
    m_showInGenre=new QAction(this);
    connect(m_showInGenre, SIGNAL(triggered()),
            this, SLOT(onActionShowInGenre()));
}
