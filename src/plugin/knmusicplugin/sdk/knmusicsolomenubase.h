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

#ifndef KNMUSICSOLOMENUBASE_H
#define KNMUSICSOLOMENUBASE_H

#include "knanimationmenu.h"

class KNMusicProxyModel;
/*!
 * \brief The KNMusicSoloMenuBase class provide the basic port of a menu when
 * user is requested to operate only one file.
 */
class KNMusicSoloMenuBase : public KNAnimationMenu
{
    Q_OBJECT
public:
    KNMusicSoloMenuBase(QWidget *parent = 0):KNAnimationMenu(parent){}

    /*!
     * \brief Set the music proxy model and the specfic row to the solo menu in
     * order to locate the file.
     * \param model The proxy music model.
     * \param itemIndex The item index. This can help you to get the speicfic
     * row and the column where the mouse pressed.
     */
    virtual void setMusicRow(KNMusicProxyModel *model,
                             const QModelIndex &itemIndex)=0;

signals:
    /*!
     * \brief When the menu is asked to play the current row, this signal will
     * be emitted.
     */
    void requirePlayCurrent();

    /*!
     * \brief When the menu is asked to remove the current row, this signal will
     * be emitted.
     */
    void requireRemoveCurrent();

    /*!
     * \brief When the menu is asked to rename the current row, this signal will
     * be emitted.
     * \param preferName The prefer name generate according to the row data.
     */
    void requireRenameCurrent(QString preferName);

public slots:

};

#endif // KNMUSICSOLOMENUBASE_H
