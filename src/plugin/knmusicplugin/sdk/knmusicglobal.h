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

#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QObject>

/*!
 * \def knGlobal
 * A global pointer referring to the unique music global object.
 */
#define knMusicGlobal (KNMusicGlobal::instance())

/*!
 * \brief The KNMusicGlobal class provides some public instance and function of
 * the official music category plugin.
 */
class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the singleton instance of class KNMusicGlobal.
     * \return The global instance of KNMusicGlobal.
     */
    static KNMusicGlobal *instance();

    /*!
     * \brief Initial the music global class.
     * \param parent The parent object of the music global class.
     */
    static void initial(QObject *parent = 0);

signals:

public slots:

private:
    static KNMusicGlobal *m_instance;
    explicit KNMusicGlobal(QObject *parent = 0);
};

#endif // KNMUSICGLOBAL_H
