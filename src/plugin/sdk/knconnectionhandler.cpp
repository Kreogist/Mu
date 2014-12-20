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
#include "knconnectionhandler.h"

KNConnectionHandler::KNConnectionHandler(QObject *parent) :
    QObject(parent)
{
    m_connections.clear();
}

KNConnectionHandler::~KNConnectionHandler()
{
    //Disconnect all the connections in the linked list.
    disconnectAll();
}

bool KNConnectionHandler::isEmpty() const
{
    return m_connections.isEmpty();
}

int KNConnectionHandler::size() const
{
    return m_connections.size();
}

void KNConnectionHandler::disconnectAll()
{
    //Disconnect all the connections.
    while(!m_connections.isEmpty())
    {
        disconnect(m_connections.takeLast());
    }
}

void KNConnectionHandler::addConnectionHandle(
        const QMetaObject::Connection connection)
{
    //Add the connections to the linked list.
    m_connections.append(connection);
}

KNConnectionHandler &KNConnectionHandler::operator +=(
        const QMetaObject::Connection connection)
{
    addConnectionHandle(connection);
    return *this;
}
