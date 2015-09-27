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

#ifndef KNMUSICLIBRARY_H
#define KNMUSICLIBRARY_H

#include <QThread>

#include "knmusiclibrarybase.h"

class KNJsonDatabase;
class KNMusicLibraryModel;
class KNMusicLibraryTab;
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
    QThread m_databaseThread, m_imageThread;

    QString m_libraryPath;
    KNJsonDatabase *m_database;
    KNMusicLibraryModel *m_libraryModel;
    KNMusicLibraryTab *m_songTab;
};

#endif // KNMUSICLIBRARY_H
