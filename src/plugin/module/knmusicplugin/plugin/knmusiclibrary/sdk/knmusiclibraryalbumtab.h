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
#ifndef KNMUSICLIBRARYALBUMTAB_H
#define KNMUSICLIBRARYALBUMTAB_H

#include "knmusiclibrarycategorytab.h"

class KNDropProxyContainer;
class KNMusicAlbumView;
class KNMusicAlbumDetail;
class KNMusicLibraryAlbumTab : public KNMusicLibraryCategoryTab
{
    Q_OBJECT
public:
    explicit KNMusicLibraryAlbumTab(QObject *parent = 0);
    QAction *showInAction();
    QString caption();
    QPixmap icon();
    QWidget *widget();

signals:

public slots:
    void retranslate();
    void setLibraryModel(KNMusicLibraryModel *model);
    void setCategoryModel(KNMusicCategoryModel *model);
    void onActionSearch(const QString &text);

private slots:
    void onActionRequireSearch();
    void onActionShowInAlbum();

private:
    void initialShowInAction();
    void initialFindAction();
    QAction *m_showInAlbumTab;
    KNDropProxyContainer *m_container;
    KNMusicAlbumView *m_albumView;
    KNMusicAlbumDetail *m_albumDetail;
    KNMusicLibraryModel *m_musicLibrary;
    KNMusicCategoryModel *m_categoryModel;
};

#endif // KNMUSICLIBRARYALBUMTAB_H
