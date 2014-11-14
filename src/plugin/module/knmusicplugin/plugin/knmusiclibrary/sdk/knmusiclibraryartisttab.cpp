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

#include "kndropproxycontainer.h"
#include "knmusiccategorymodel.h"
#include "knmusiclibrarymodel.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiccategorylistviewbase.h"

#include "knmusiclibraryartisttab.h"

KNMusicLibraryArtistTab::KNMusicLibraryArtistTab(QObject *parent) :
    KNMusicLibraryCategoryTab(parent)
{
    //Initial the drop proxy container.
    m_container=new KNDropProxyContainer;
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_container);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_container->setLayout(mainLayout);
    m_splitter=new QSplitter(m_container);
    mainLayout->addWidget(m_splitter);
    m_artistList=new KNMusicCategoryListViewBase(m_container);
    m_splitter->addWidget(m_artistList);
}

QString KNMusicLibraryArtistTab::caption()
{
    return tr("Artists");
}

QPixmap KNMusicLibraryArtistTab::icon()
{
    return QPixmap(":/plugin/music/category/02_artists.png");
}

QWidget *KNMusicLibraryArtistTab::widget()
{
    return m_container;
}

void KNMusicLibraryArtistTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Do connections.
    connect(m_container, &KNDropProxyContainer::requireAnalysisFiles,
            model, &KNMusicLibraryModel::addFiles);
}

void KNMusicLibraryArtistTab::setCategoryModel(KNMusicCategoryModel *model)
{
    //Set the no category text.
    model->setNoCategoryText(tr("No Artist"));
    //Apply category model.
    KNMusicLibraryCategoryTab::setCategoryModel(model);

    //! This should be done in constructor, but setModel() is a virtual
    //! function, so we moved here.
    //Set the proxy model to tree view.
    m_artistList->setModel(proxyCategoryModel());
    proxyCategoryModel()->sort(0, Qt::AscendingOrder);
}

void KNMusicLibraryArtistTab::onActionSearch(const QString &text)
{
    ;
}
