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

#ifndef KNMUSICSTOREHOMEALBUMVIEW_H
#define KNMUSICSTOREHOMEALBUMVIEW_H

#include "knmusicstorehomeitemview.h"

/*!
 * \brief The KNMusicStoreHomeAlbumView class provides the view to display the
 * album view at the home page. It will render the item only in horizontal way.
 * Each column will contains two items.
 */
class KNMusicStoreHomeAlbumView : public KNMusicStoreHomeItemView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreHomeAlbumView widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreHomeAlbumView(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractItemView::indexAt().
     */
    QModelIndex indexAt(const QPoint &point) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::visualRect().
     */
    QRect visualRect(const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractItemView::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

protected slots:
    /*!
     * \brief Reimplemented from QAbstractItemView::updateGeometries().
     */
    void updateGeometries() Q_DECL_OVERRIDE;

private:
    QPixmap m_noAlbumArtCache, m_albumShadow, m_leftShadowCache,
            m_rightShadowCache;
};

#endif // KNMUSICSTOREHOMEALBUMVIEW_H
