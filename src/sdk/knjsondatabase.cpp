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
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "knjsondatabase.h"

#include <QDebug>

#define MajorVersion 2
#define MinorVersion 1

KNJsonDatabase::KNJsonDatabase(QObject *parent) :
    QObject(parent),
    m_fileInfo(QFileInfo())
{
}

bool KNJsonDatabase::link(const QString &jsonFile)
{
    //Save the database file information.
    m_fileInfo=QFileInfo(jsonFile);
    //Check the directory of the database.
    checkDatabaseDir();
    //Reset the database file.
    m_file.reset(new QFile(m_fileInfo.absoluteFilePath()));
    //Link established.
    return true;
}

bool KNJsonDatabase::read()
{
    //Check the following things:
    //  1.Is m_file null, if the file has been linked, it won't be null.
    //  2.Check the existance of the file. The file cannot be load if it doesn't
    //exist.
    //  3.Try to open the database file. If it cannot be opened as read only
    //mode, it cannot be loaded.
    if(m_file.isNull() ||
            (!m_file->exists()) ||
            (!m_file->open(QIODevice::ReadOnly)))
    {
        return false;
    }
    //Read the database binary data from the file, get the object from the data.
    QJsonObject contentObject=
            QJsonDocument::fromBinaryData(m_file->readAll()).object();
    //Close the database file.
    m_file->close();
    //Check is the document valid.
    if(contentObject.isEmpty())
    {
        return false;
    }
    //Check the version of the database.
    if(contentObject.value("Major").toInt()>MajorVersion ||
            contentObject.value("Minor").toInt()>MinorVersion)
    {
        //This is crate by a higher version.
        return false;
    }
    //Get the data field.
    m_dataField=contentObject.value("Database").toArray();
    //Read complete.
    return true;
}

bool KNJsonDatabase::write()
{
    return false;
}

QJsonArray::iterator KNJsonDatabase::begin()
{
    return m_dataField.begin();
}

QJsonArray::iterator KNJsonDatabase::end()
{
    return m_dataField.end();
}

inline void KNJsonDatabase::checkDatabaseDir()
{
    //Check is the destination path is a real dir, and it's exist.
    QFileInfo databaseDir(m_fileInfo.absolutePath());
    if(databaseDir.isDir() && databaseDir.exists())
    {
        return;
    }
    //If it's a file, delete it.
    if(databaseDir.isFile())
    {
        //Remove it.
        QFile::remove(databaseDir.absoluteFilePath());
    }
    //Generate the dir.
    QDir().mkpath(databaseDir.absoluteFilePath());
}

