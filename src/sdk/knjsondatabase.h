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

#ifndef KNJSONDATABASE_H
#define KNJSONDATABASE_H

#include <QFileInfo>
#include <QScopedPointer>
#include <QFile>
#include <QJsonArray>

#include <QObject>

class QFile;
class KNJsonDatabase : public QObject
{
    Q_OBJECT
public:
    explicit KNJsonDatabase(QObject *parent = 0);
    bool link(const QString &jsonFile);
    bool read();
    bool write();
    QJsonArray::iterator begin();
    QJsonArray::iterator end();

signals:

public slots:
private:
    inline void checkDatabaseDir();
    QFileInfo m_fileInfo;
    QScopedPointer<QFile> m_file;
    QJsonArray m_dataField;
};

#endif // KNJSONDATABASE_H
