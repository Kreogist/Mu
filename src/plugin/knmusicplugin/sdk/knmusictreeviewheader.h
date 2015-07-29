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

#ifndef KNMUSICTREEVIEWHEADER_H
#define KNMUSICTREEVIEWHEADER_H

#include "knmousesenseheader.h"

class KNMusicTreeViewHeaderMenu;
/*!
 * \brief The KNMusicTreeViewHeader class provides a specific music tree view
 * header for music tree view. It will provides a menu to control the visibility
 * of the music header.
 */
class KNMusicTreeViewHeader : public KNMouseSenseHeader
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTreeViewHeader widget with given tree view
     * widget parent.
     * \param parent A music tree view widget.
     */
    explicit KNMusicTreeViewHeader(QWidget *parent = 0);

signals:
    /*!
     * \brief When one column is being asked to resize it to fit the content,
     * this signal will be emitted.
     * \param column The column index.
     */
    void requireResizeColumnToContents(int column);

public slots:

private slots:
    void resizeAllColumns();
    void showCustomContextMenu(const QPoint &mousePoint);

private:
    KNMusicTreeViewHeaderMenu *m_headerMenu;
};

#endif // KNMUSICTREEVIEWHEADER_H
