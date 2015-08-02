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
#include <QObject>

#include "knconnectionhandler.h"

KNConnectionHandler::~KNConnectionHandler()
{
    //If the connection handler is going to be destoryed, disconnect all the
    //things.
    disconnectAll();
}

void KNConnectionHandler::disconnectAll()
{
    //If the linked list is not empty, then disconnect all of them.
    while(!isEmpty())
    {
        //Disconnect the last taken connection.
        QObject::disconnect(takeLast());
    }
}

KNConnectionHandler &KNConnectionHandler::operator +=(
        const QMetaObject::Connection &connection)
{
    //Add this connection to the linked list.
    append(connection);
    //And for this kind of operator, just return this.
    return *this;
}
