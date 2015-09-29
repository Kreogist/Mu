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
#include "knemptystatewidget.h"

#include "knmusiclibraryemptyhint.h"
#include "knmusiclibrarymodel.h"
#include "knmusiccategoryproxymodel.h"
#include "knmusiccategorymodelbase.h"

#include "knmusiclibrarycategorytab.h"

KNMusicLibraryCategoryTab::KNMusicLibraryCategoryTab(QWidget *parent) :
    KNMusicLibraryTab(parent),
    m_emptyStateWidget(new KNEmptyStateWidget(this)),
    m_emptyHint(new KNMusicLibraryEmptyHint(this)),
    m_categoryProxyModel(new KNMusicCategoryProxyModel(this))
{
    //Set the empty hint as the empty widget.
    m_emptyStateWidget->setEmptyWidget(m_emptyHint);
}

void KNMusicLibraryCategoryTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Link the library model.
    connect(m_emptyHint, &KNMusicLibraryEmptyHint::urlsDropped,
            model, &KNMusicLibraryModel::appendUrls);
    //Link the model response to the empyt state widget.
    connect(model, &KNMusicLibraryModel::libraryNotEmpty,
            m_emptyStateWidget, &KNEmptyStateWidget::showContentWidget);
    connect(model, &KNMusicLibraryModel::libraryEmpty,
            m_emptyStateWidget, &KNEmptyStateWidget::showEmptyWidget);
}

void KNMusicLibraryCategoryTab::setCategoryModel(
        KNMusicCategoryModelBase *model)
{
    //Default, we have to set the model to proxy model as source model.
    m_categoryProxyModel->setSourceModel(model);
}

void KNMusicLibraryCategoryTab::resizeEvent(QResizeEvent *event)
{
    //Do the original resize event.
    KNMusicLibraryTab::resizeEvent(event);
    //Make the empty state widget the same size as the widget.
    m_emptyStateWidget->resize(size());
}

void KNMusicLibraryCategoryTab::setContentWidget(QWidget *widget)
{
    //Set the content widget to the empty state widget.
    m_emptyStateWidget->setContentWidget(widget);
    //Give the widget to the focus proxy.
    m_emptyStateWidget->setFocusProxy(widget);
}

KNMusicCategoryProxyModel *KNMusicLibraryCategoryTab::categoryProxyModel()
{
    return m_categoryProxyModel;
}
