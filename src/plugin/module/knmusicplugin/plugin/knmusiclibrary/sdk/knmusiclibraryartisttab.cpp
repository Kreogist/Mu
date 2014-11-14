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
#include "knmusiccategoryproxymodel.h"
#include "knmusiccategorylistviewbase.h"

#include "knmusiclibraryartisttab.h"

KNMusicLibraryArtistTab::KNMusicLibraryArtistTab(QObject *parent) :
    KNMusicLibraryCategoryTab(parent)
{
    //Initial the drop proxy container.
    m_widget=new KNDropProxyContainer;
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_widget);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_widget->setLayout(mainLayout);
    m_splitter=new QSplitter(m_widget);
    mainLayout->addWidget(m_splitter);
    m_artistList=new KNMusicCategoryListViewBase(m_widget);
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
    return m_widget;
}

void KNMusicLibraryArtistTab::setLibraryModel(KNMusicLibraryModel *model)
{
    ;
}

void KNMusicLibraryArtistTab::setCategoryModel(KNMusicCategoryModel *model)
{
    //Apply category model.
    KNMusicLibraryCategoryTab::setCategoryModel(model);
    //Set the proxy model to tree view.
    m_artistList->setModel(proxyCategoryModel());
}

void KNMusicLibraryArtistTab::onActionSearch(const QString &text)
{
    ;
}
