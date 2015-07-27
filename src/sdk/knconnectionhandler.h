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

#ifndef KNCONNECTIONHANDLER_H
#define KNCONNECTIONHANDLER_H

#include <QMetaObject>
#include <QLinkedList>

/*!
 * \brief The KNConnectionHandler class is a special linked list. The element
 * type is QMetaObject::Connection. This list is used to managed all the
 * connections. You can use the disconnectAll() function to disconnect all the
 * connection in the handler at one time. This is really handy sometimes.
 */
class KNConnectionHandler : public QLinkedList<QMetaObject::Connection>
{
public:
    /*!
     * \brief Disconnect all the connection in the linked list.
     */
    void disconnectAll();

    /*!
     * \brief Reimplement from the QLinkedList::operator +=().
     */
    KNConnectionHandler &operator += (
            const QMetaObject::Connection &connection);

signals:

public slots:
};

#endif // KNCONNECTIONHANDLER_H
