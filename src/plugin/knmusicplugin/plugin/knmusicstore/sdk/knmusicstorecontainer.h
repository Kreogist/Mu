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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTORECONTAINER_H
#define KNMUSICSTORECONTAINER_H

#include "knmusicstoreutil.h"

#include <QWidget>

using namespace MusicStoreUtil;

class KNScrollArea;
class KNMusicStorePage;
/*!
 * \brief The KNMusicStoreContainer class provides the container of all the
 * widgets. It will manage all the page widgets.
 */
class KNMusicStoreContainer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreContainer widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreContainer(QWidget *parent = 0);

    /*!
     * \brief Get the music store page widget.
     * \param pageIndex The page index.
     * \return The specific page index widget pointer.
     */
    KNMusicStorePage *page(int pageIndex);

signals:
    /*!
     * \brief When the displaying page changed, this signal will be emitted.
     */
    void currentPageChanged();

    /*!
     * \brief Ask the backend to fetch the home information.
     */
    void requireShowHome();

    /*!
     * \brief Ask the backend to fetch information about one artist.
     * \param backendId The backend name which will process the metadata.
     * \param metadata The artist's metadata.
     */
    void requireShowArtist(QString backendId, QString metadata);

    /*!
     * \brief Ask the backend to fetch information about one album.
     * \param backendId The backend name which will process the metadata.
     * \param metadata The album's metadata.
     */
    void requireShowAlbum(QString backendId, QString metadata);

    /*!
     * \brief Ask the backend to fetch information about single song.
     * \param backendId The backend name which will process the metadata.
     * \param metadata The song's metadata.
     */
    void requireShowSingleSong(QString backendId, QString metadata);

    /*!
     * \brief Set the navigator item text in the header text.
     * \param navigatorIndex The navigator item index.
     * \param caption The text of the navigator item.
     */
    void requireSetNavigatorItem(int navigatorIndex,
                                 const QString &caption);

    /*!
     * \brief Require to download one song file.
     * \param url The song url.
     * \param filename The saving file name of the song.
     * \param title The title of the song, this will display at the song list.
     */
    void requireDownload(const QString &url,
                         const QString &filename,
                         const QString &title);

public slots:
    /*!
     * \brief Refresh the current page, if there is no page at the moment, it
     * will require to show the home page.
     */
    void refresh();

    /*!
     * \brief Require to show a specific page via index.
     * \param index
     */
    void showPageIndex(int index);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void showPage(KNMusicStorePage *pageWidget);
    void onShowPage();
    void onShowAlbum(const QString &metadata);
    void onShowSingleSong(const QString &metadata);
    void onUpdatePageWidth();

private:
    inline void updatePageWidth(KNMusicStorePage *pageWidget);
    inline void configurePage(KNMusicStorePage *pageWidget);
    KNMusicStorePage *m_pages[PagesCount];
    KNScrollArea *m_pageContainer;
    int m_contentWidth;
};

#endif // KNMUSICSTORECONTAINER_H
