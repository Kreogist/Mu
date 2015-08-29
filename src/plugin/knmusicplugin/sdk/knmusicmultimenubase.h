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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICMULTIMENUBASE_H
#define KNMUSICMULTIMENUBASE_H

#include "knanimationmenu.h"

class KNMusicProxyModel;
/*!
 * \brief The KNMusicMultiMenuBase class provides you a basic interface for all
 * the multi item operation menus. You should implement this port class to
 * realized your own multi menu for KNMusicTreeView.
 */
class KNMusicMultiMenuBase : public KNAnimationMenu
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicMultiMenuBase widget.
     * \param parent The parent widget parent.
     */
    KNMusicMultiMenuBase(QWidget *parent = 0):KNAnimationMenu(parent){}

    /*!
     * \brief Set the music model and the selected rows to the multi menu.
     * \param model The model pointer.
     * \param rows The selected rows list.
     * \param clickedIndex The index which was clicked.
     */
    virtual void setMusicRows(KNMusicProxyModel *model,
                              const QList<int> &rows,
                              const QModelIndex &clickedIndex)=0;
};

#endif // KNMUSICMULTIMENUBASE_H
