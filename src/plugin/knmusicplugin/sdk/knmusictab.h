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

#ifndef KNMUSICTAB_H
#define KNMUSICTAB_H

#include <QWidget>

class QAbstractButton;
/*!
 * \brief The KNMusicTab class is a port class for all the tabs.\n
 * The music plugin will manage those tab widgets themselves. The tab widget
 * will be added to switcher.
 */
class KNMusicTab : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicTab widget.
     * \param parent The parent widget. It should be the music plugin.
     */
    KNMusicTab(QWidget *parent = 0): QWidget(parent){}

    /*!
     * \brief The tab of the current content.
     * \return The tab widget pointer.
     */
    virtual QAbstractButton *tab()=0;

signals:

public slots:
};

#endif // KNMUSICTAB_H
