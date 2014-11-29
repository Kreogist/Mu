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
#ifndef KNMUSICLIBRARY_H
#define KNMUSICLIBRARY_H

#include "knmusiclibrarybase.h"

class QThread;
class KNMusicCategoryModel;
class KNMusicLibraryDatabase;
class KNMusicLibraryModel;
class KNMusicLibraryTab;
class KNMusicLibraryCategoryTab;
class KNMusicLibrary : public KNMusicLibraryBase
{
    Q_OBJECT
public:
    explicit KNMusicLibrary(QObject *parent = 0);
    ~KNMusicLibrary();
    KNMusicTab *songTab();
    KNMusicTab *artistTab();
    KNMusicTab *albumTab();
    KNMusicTab *genreTab();

signals:

public slots:

private:
    void initialSongTab();
    void initialArtistTab();
    void initialAlbumTab();
    void initialGenreTab();
    enum CategoryTabs
    {
        TabArtists,
        TabAlbums,
        TabGenres,
        CategoryTabsCount
    };
    QThread *m_libraryDatabaseThread;
    KNMusicLibraryDatabase *m_libraryDatabase;
    KNMusicLibraryModel *m_libraryModel;
    KNMusicLibraryTab *m_librarySongTab;
    KNMusicCategoryModel *m_categoryModel[CategoryTabsCount];
    KNMusicLibraryCategoryTab *m_libraryTabs[CategoryTabsCount];
};

#endif // KNMUSICLIBRARY_H
