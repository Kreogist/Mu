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

#ifndef KNMUSICSTOREPAGE_H
#define KNMUSICSTOREPAGE_H

#include <QWidget>

/*!
 * \brief The KNMusicStorePage class provides a page used for the abstract ports
 * of all the pages for the music store. These pages include home page, search
 * results, artist & album and single song.
 */
class KNMusicStorePage : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStorePage widget.
     * \param parent The parent widget.
     */
    KNMusicStorePage(QWidget *parent = 0) : QWidget(parent) {}

signals:

public slots:
};

#endif // KNMUSICSTOREPAGE_H
