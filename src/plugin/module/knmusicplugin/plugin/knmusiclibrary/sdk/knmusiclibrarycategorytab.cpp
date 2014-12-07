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
#include "knemptystatewidget.h"
#include "knmusiclibraryemptyhint.h"
#include "knmusiclibrarymodel.h"
#include "knmusiccategorymodel.h"
#include "knmusiccategoryproxymodel.h"

#include "knmusiclibrarycategorytab.h"

KNMusicLibraryCategoryTab::KNMusicLibraryCategoryTab(QObject *parent) :
    KNMusicLibraryTab(parent)
{
    //Initial the viewer widget.
    m_viewer=new KNEmptyStateWidget();
    connect(m_viewer, &KNEmptyStateWidget::aboutToBeShown,
            this, &KNMusicLibraryTab::requireLoadLibrary);

    //Initial the empty hint.
    m_emptyHint=new KNMusicLibraryEmptyHint(m_viewer);
    m_viewer->setEmptyWidget(m_emptyHint);

    //Initial the proxy model.
    m_proxyCategoryModel=new KNMusicCategoryProxyModel(this);
}

QWidget *KNMusicLibraryCategoryTab::widget()
{
    return m_viewer;
}

void KNMusicLibraryCategoryTab::setCategoryModel(KNMusicCategoryModel *model)
{
    //Set source model.
    m_proxyCategoryModel->setSourceModel(model);
}

void KNMusicLibraryCategoryTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Connect analysis requirement signal.
    connect(m_emptyHint, &KNMusicLibraryEmptyHint::requireAnalysisFiles,
            model, &KNMusicLibraryModel::addFiles);
    //Connect show and hide signal.
    connect(model, &KNMusicLibraryModel::libraryNotEmpty,
            m_viewer, &KNEmptyStateWidget::showContentWidget);
    connect(model, &KNMusicLibraryModel::libraryEmpty,
            m_viewer, &KNEmptyStateWidget::showEmptyWidget);
}

KNMusicCategoryProxyModel *KNMusicLibraryCategoryTab::proxyCategoryModel()
{
    return m_proxyCategoryModel;
}

void KNMusicLibraryCategoryTab::setContentWidget(QWidget *widget)
{
    m_viewer->setContentWidget(widget);
}

QWidget *KNMusicLibraryCategoryTab::viewerWidget()
{
    return m_viewer;
}
