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

#ifndef KNMUSICLIBRARYGENRETAB_H
#define KNMUSICLIBRARYGENRETAB_H

#include <QModelIndex>

#include "knmusiclibrarycategorytab.h"

class KNCategoryTab;
class KNMusicCategoryDisplay;
class KNMusicCategoryListViewBase;
class KNMusicLibraryDropProxy;
class KNMusicLibraryGenreTab : public KNMusicLibraryCategoryTab
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryGenreTab widget.
     * \param parent The parent widget.
     */
    explicit KNMusicLibraryGenreTab(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showInAction().
     */
    QAction *showInAction() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::tab().
     */
    QAbstractButton *tab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showInTab().
     */
    void showInTab(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showIndex().
     */
    void showIndex(KNMusicModel *musicModel,
                   const QModelIndex &index) Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::setCategoryModel().
     */
    void setCategoryModel(KNMusicCategoryModelBase *model) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::setLibraryModel().
     */
    void setLibraryModel(KNMusicLibraryModel *model) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void checkCategorySelected();
    void onActionShowInGenre();
    void onActionCategoryIndexChanged(const QModelIndex &index);
    void onActionSearchCategory(const QString &text);

private:
    inline void clearSearchRequest();
    inline void showAndSelectRow(const int &musicRow);
    QMetaObject::Connection m_notEmptyCheck;
    QModelIndex m_currentSourceIndex;
    KNCategoryTab *m_tab;
    QAction *m_showInGenreTab;
    KNMusicLibraryDropProxy *m_dropProxy;
    KNMusicCategoryListViewBase *m_genreList;
    KNMusicCategoryDisplay *m_genreDisplay;
    KNMusicCategoryModelBase *m_categoryModel;
    KNMusicLibraryModel *m_libraryModel;
};

#endif // KNMUSICLIBRARYGENRETAB_H
