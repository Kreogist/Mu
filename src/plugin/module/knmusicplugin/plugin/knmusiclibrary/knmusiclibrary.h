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

class KNMusicLibraryModel;
class KNMusicLibraryTab;
class KNMusicLibrary : public KNMusicLibraryBase
{
    Q_OBJECT
public:
    explicit KNMusicLibrary(QObject *parent = 0);
    KNMusicTab *songTab();
    KNMusicTab *artistTab();
    KNMusicTab *albumTab();
    KNMusicTab *genreTab();

signals:

public slots:

private:
    void initialTabs();
    enum LibraryTabs
    {
        Songs,
        Artists,
        Albums,
        Genres,
        LibraryTabs
    };
    KNMusicLibraryModel *m_libraryModel;
    KNMusicLibraryTab *m_libraryTabs[LibraryTabs];
};

#endif // KNMUSICLIBRARY_H
