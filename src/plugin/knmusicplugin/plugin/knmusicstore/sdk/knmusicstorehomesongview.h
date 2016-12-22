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

#ifndef KNMUSICSTOREHOMESONGVIEW_H
#define KNMUSICSTOREHOMESONGVIEW_H

#include "knmusicstorehomeitemview.h"

/*!
 * \brief The KNMusicStoreHomeSongView class provides the widge to display the
 * home item as song item.\n
 * This view is implemented as a music store view. It will manage only the music
 * store model.
 */
class KNMusicStoreHomeSongView : public KNMusicStoreHomeItemView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreHomeSongView widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreHomeSongView(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicStoreHomeItemView::indexAt().
     */
    QModelIndex indexAt(const QPoint &point) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreHomeItemView::visualRect().
     */
    QRect visualRect(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreHomeItemView::setModel().
     */
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMusicStoreHomeItemView::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

protected slots:
    /*!
     * \brief Reimplemented from KNMusicStoreHomeItemView::updateGeometries().
     */
    void updateGeometries() Q_DECL_OVERRIDE;

private:
    QPixmap m_noAlbumArtCache;
};

#endif // KNMUSICSTOREHOMESONGVIEW_H
