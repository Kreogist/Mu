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
#ifndef KNMUSICLIBRARYGENRETAB_H
#define KNMUSICLIBRARYGENRETAB_H

#include "knmusiclibrarycategorytab.h"

class QSplitter;
class KNMusicGenreModel;
class KNDropProxyContainer;
class KNMusicCategoryDisplay;
class KNMusicCategoryListViewBase;
class KNMusicLibraryGenreTab : public KNMusicLibraryCategoryTab
{
    Q_OBJECT
public:
    explicit KNMusicLibraryGenreTab(QObject *parent = 0);
    QAction *showInAction();
    QString caption();
    QPixmap icon();

signals:

public slots:
    void retranslate();
    void setLibraryModel(KNMusicLibraryModel *model);
    void setCategoryModel(KNMusicCategoryModel *model);
    void onActionSearch(const QString &text);

protected slots:
    void onActionCategoryIndexChanged(const QModelIndex &index);
    void onActionTabShow();
    void onActionTabHide();

private slots:
    void onActionRequireSearch();
    void onActionShowInGenre();
    void checkCategorySelected();

private:
    void initialShowInAction();
    void initialFindAction();
    QAction *m_showInGenre;
    KNDropProxyContainer *m_dropProxy;
    QSplitter *m_splitter;
    KNMusicLibraryModel *m_musicLibrary;
    KNMusicGenreModel *m_categoryModel;
    KNMusicCategoryListViewBase *m_genreList;
    KNMusicCategoryDisplay *m_genreDisplay;
    QMetaObject::Connection m_notEmptyCheck;
};

#endif // KNMUSICLIBRARYGENRETAB_H
