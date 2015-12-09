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

#include "knconnectionhandler.h"

#include "knmusiclibrarybase.h"

class KNMusicLibraryModel;
class KNMusicLibraryTab;
class KNMusicCategoryModelBase;
class KNMusicLibraryCategoryTab;
/*!
 * \brief The KNMusicLibrary class is the official plugin for provide a full
 * functional music library. All the plugins for music library can be treated as
 * an ofiicial plugin example.
 */
class KNMusicLibrary : public KNMusicLibraryBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibrary object with given parent object.
     * \param parent The parent object.
     */
    explicit KNMusicLibrary(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::songTab().
     */
    KNMusicTab *songTab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::artistTab().
     */
    KNMusicTab *artistTab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::albumTab().
     */
    KNMusicTab *albumTab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::genreTab().
     */
    KNMusicTab *genreTab() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicLibraryBase::showInSongTab().
     */
    void showInSongTab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::showInArtistTab().
     */
    void showInArtistTab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::showInAlbumTab().
     */
    void showInAlbumTab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::showInGenreTab().
     */
    void showInGenreTab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryBase::setNowPlaying().
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying) Q_DECL_OVERRIDE;

private slots:
    void onActionLoadLibrary();

private:
    enum CategoryTabs
    {
        TabArtists,
        TabAlbums,
        TabGenres,
        CategoryTabsCount
    };
    inline void linkLoadRequest(KNMusicLibraryTab *libraryTab);
    KNMusicCategoryModelBase *m_categoryModel[CategoryTabsCount];
    KNMusicLibraryCategoryTab *m_libraryTabs[CategoryTabsCount];

    KNConnectionHandler m_loadHandler;
    QString m_libraryPath;
    KNMusicLibraryModel *m_libraryModel;
    KNMusicLibraryTab *m_songTab;
    KNMusicNowPlayingBase *m_nowPlaying;
};

#endif // KNMUSICLIBRARY_H
