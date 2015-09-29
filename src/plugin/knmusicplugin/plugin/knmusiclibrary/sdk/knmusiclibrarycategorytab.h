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

#ifndef KNMUSICLIBRARYCATEGORYTAB_H
#define KNMUSICLIBRARYCATEGORYTAB_H

#include "knmusiclibrarytab.h"

class KNMusicCategoryModelBase;
class KNMusicCategoryProxyModel;
class KNEmptyStateWidget;
class KNMusicLibraryEmptyHint;
class KNMusicLibraryCategoryTab : public KNMusicLibraryTab
{
    Q_OBJECT
public:
    explicit KNMusicLibraryCategoryTab(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the library model to the tab, and it will link the library
     * model to the container.
     * \param model The library model.
     */
    void setLibraryModel(KNMusicLibraryModel *model) Q_DECL_OVERRIDE;

    /*!
     * \brief Set category model to the tab. You may need the proxy model.
     * \param model The category model.
     */
    virtual void setCategoryModel(KNMusicCategoryModelBase *model);

protected:
    /*!
     * \brief Reimplemented from KNMusicLibraryTab::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;


    /*!
     * \brief Get the category proxy model pointer for tree view widget to show
     * the category model.
     * \return The category proxy model pointer.
     */
    KNMusicCategoryProxyModel *categoryProxyModel();

    /*!
     * \brief Set the content widget of the tab.
     * \param widget The widget pointer.
     */
    void setContentWidget(QWidget *widget);

private:
    KNEmptyStateWidget *m_emptyStateWidget;
    KNMusicLibraryEmptyHint *m_emptyHint;
    KNMusicCategoryProxyModel *m_categoryProxyModel;
};

#endif // KNMUSICLIBRARYCATEGORYTAB_H
