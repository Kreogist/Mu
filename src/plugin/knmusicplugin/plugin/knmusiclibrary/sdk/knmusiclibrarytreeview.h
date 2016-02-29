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

#ifndef KNMUSICLIBRARYTREEVIEW_H
#define KNMUSICLIBRARYTREEVIEW_H

#include "knmusictreeviewbase.h"

/*!
 * \brief The KNMusicLibraryTreeView class provides the library optimized tree
 * view. You could set a category column for fixed on column to be only specific
 * data. This could be used for artist and genre tab.
 */
class KNMusicLibraryTreeView : public KNMusicTreeViewBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLibraryTreeView widget.
     * \param parent The parent widget pointer.
     * \param tab The tab pointer.
     */
    explicit KNMusicLibraryTreeView(QWidget *parent = 0,
                                    KNMusicTab *tab = 0);

signals:

public slots:
    /*!
     * \brief Set the category column index.
     * \param column The column index.
     */
    void setCategoryColumn(int column);

    /*!
     * \brief Set the category specific text.
     * \param text The category text. Tree view will autocamtically filter the
     * music model.
     */
    void setCategoryText(const QVariant &text);

protected:
    /*!
     * \brief Reimplemented from KNMusicTreeViewBase::resetHeaderState().
     */
    void resetHeaderState() Q_DECL_OVERRIDE;

};

#endif // KNMUSICLIBRARYTREEVIEW_H
