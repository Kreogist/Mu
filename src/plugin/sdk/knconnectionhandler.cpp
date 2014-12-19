/*
 *  Copyright 2013 Kreogist Dev Team
 *
 *  This file is part of Kreogist Cuties.
 *
 *    Kreogist Cuties is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *    Kreogist Cuties is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Kreogist Cuties.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "knconnectionhandler.h"

KNConnectionHandler::KNConnectionHandler(QObject *parent) :
    QObject(parent)
{
    m_connections.clear();
}

KNConnectionHandler::~KNConnectionHandler()
{
    disConnectAll();
}

bool KNConnectionHandler::isEmpty()
{
    return m_connections.isEmpty();
}

int KNConnectionHandler::size() const
{
    return m_connections.size();
}

void KNConnectionHandler::disConnectAll()
{
    for(auto i=m_connections.begin();
        i!=m_connections.end();
        ++i)
    {
        disconnect(*i);
    }
    m_connections.clear();
}

void KNConnectionHandler::addConnectionHandle(
        const QMetaObject::Connection connection)
{
    m_connections.append(connection);
}

KNConnectionHandler &KNConnectionHandler::operator +=(
        const QMetaObject::Connection connection)
{
    addConnectionHandle(connection);
    return *this;
}
