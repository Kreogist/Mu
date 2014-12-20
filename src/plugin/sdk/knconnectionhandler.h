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
#ifndef KNCONNECTIONHANDLER_H
#define KNCONNECTIONHANDLER_H

#include <QLinkedList>

#include <QObject>

class KNConnectionHandler : public QObject
{
    Q_OBJECT
public:
    explicit KNConnectionHandler(QObject *parent = 0);
    ~KNConnectionHandler();
    bool isEmpty() const;
    int size() const;
    void disconnectAll();
    void addConnectionHandle(const QMetaObject::Connection connection);
    KNConnectionHandler &operator += (const QMetaObject::Connection connection);

private:
    QLinkedList<QMetaObject::Connection> m_connections;
};

#endif // KNCONNECTIONHANDLER_H
