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

#ifndef KNMUSICSTORELISTVIEW_H
#define KNMUSICSTORELISTVIEW_H

#include "knmusicstoreutil.h"

#include "knmousesensetreeview.h"

using namespace MusicStoreUtil;

/*!
 * \brief The KNMusicStoreListView class provides the album list view for the
 * store album songs.
 */
class KNMusicStoreListView : public KNMouseSenseTreeView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreListView widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreListView(QWidget *parent = 0);

    /*!
     * \brief Get the fixed row height.
     * \return The row height, unit is pixel.
     */
    static int fixedRowHeight();

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::wheelEvent().
     */
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
};

#endif // KNMUSICSTORELISTVIEW_H
