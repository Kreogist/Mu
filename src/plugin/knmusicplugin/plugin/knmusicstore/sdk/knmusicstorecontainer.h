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
class KNMusicStoreHeader;
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

public slots:
    /*!
     * \brief Set the header navigator label text.
     * \param itemIndex The item index.
     * \param text The navigator text.
     */
    void setNavigatorText(int itemIndex, const QString &text);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void showPage(KNMusicStorePage *pageWidget);
    void onShowPage();
    void onShowPageIndex(int index);
    void onShowAlbum(const QString &metadata);
    void onShowSingleSong(const QString &metadata);

private:
    inline void updatePageWidth();
    inline void configurePage(KNMusicStorePage *pageWidget);
    KNMusicStorePage *m_pages[PagesCount];
    KNScrollArea *m_pageContainer;
    QWidget *m_headerContainer;
    KNMusicStoreHeader *m_header;
};

#endif // KNMUSICSTORECONTAINER_H
