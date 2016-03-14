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

#ifndef KNMUSICSTOREGLOBAL_H
#define KNMUSICSTOREGLOBAL_H

#include <QObject>

/*!
 * \def knMusicStoreGlobal
 * A global pointer referring to the unique music store global object.
 */
#define knMusicStoreGlobal (KNMusicStoreGlobal::instance())

/*!
 * \brief The KNMusicStoreGlobal class provides the global items for music
 * widget store.
 */
class KNMusicStoreGlobal : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the music store global instance pointer.
     * \return The global instance pointer.
     */
    static KNMusicStoreGlobal *instance();

    /*!
     * \brief Initial the music store global object.
     * \param parent The parent object pointer.
     */
    static KNMusicStoreGlobal *initial(QObject *parent=0);

    /*!
     * \brief Get the store widget content width.
     * \return The store widget content width.
     */
    int storeContentWidth() const;

signals:

public slots:

private:
    explicit KNMusicStoreGlobal(QObject *parent = 0);
    static KNMusicStoreGlobal *m_instance;
};

#endif // KNMUSICSTOREGLOBAL_H
