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

#ifndef KNMUSICSTOREHOMELISTVIEW_H
#define KNMUSICSTOREHOMELISTVIEW_H

#include "knmusicstorehomeitemview.h"

/*!
 * \brief The KNMusicStoreHomeListView class is used for providing view widget
 * for the normal ranking lists. It could only be used in the music main page,
 * and its model must be the music store list model.\n
 * The size of the list view will be changed according to the model size.
 * However, this model DOES NOT provide the automatic height changing function.
 * But a tweakHeight() function instead. Which means that the view will tweak
 * the height according to the item count set by this function.
 */
class KNMusicStoreHomeListView : public KNMusicStoreHomeItemView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreHomeListView widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreHomeListView(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractItemView::indexAt().
     */
    QModelIndex indexAt(const QPoint &point) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::visualRect().
     */
    QRect visualRect(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Tweak the view height for specific rendering counts.
     * \param maxRenderingCount The maximum items of the view will display.
     */
    void tweakHeight(int maxRenderingCount);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractItemView::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int m_maxRenderingCount;
};

#endif // KNMUSICSTOREHOMELISTVIEW_H
