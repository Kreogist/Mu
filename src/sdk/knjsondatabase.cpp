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

#define MajorVersion 3
#define MinorVersion 0
#define MaxOperateCount 400

KNJsonDatabase::KNJsonDatabase(QObject *parent) :
    QObject(parent),
    m_fileInfo(QFileInfo()),
    m_dataField(QJsonArray()),
    m_operateCount(0)
{
}

bool KNJsonDatabase::link(const QString &jsonFile)
{
    //Check if there's a previous data base before.
    if(m_fileInfo!=QFileInfo())
    {
        //Write the previous data to the database file first.
        if(!write())
        {
            //We cannot write the previous data to database file. We cannot link
            //to a new one.
            return false;
        }
    }
    //Save the database file information.
    m_fileInfo=QFileInfo(jsonFile);
    //Check the directory of the database.
    checkDatabaseDir();
    //Reset the database file.
    m_file.reset(new QFile(m_fileInfo.absoluteFilePath()));
    //Clear the data field.
    m_dataField=QJsonArray();
    //Reset the operate count.
    m_operateCount=0;
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
    if(contentObject.value("Major").toInt()!=MajorVersion ||
            contentObject.value("Minor").toInt()>MinorVersion)
    {
        //This is crate by a higher version.
        return false;
    }
    //Get the data field.
    m_dataField=
            QJsonArray::fromVariantList(
                contentObject.value("Database").toArray().toVariantList());
    //Read complete.
    return true;
}

bool KNJsonDatabase::write()
{
    //Check whether we really need to write. If there's no operate after the
    //change, then ignore the write request.
    if(m_operateCount==0)
    {
        return true;
    }
    //Write the data to database file.
    bool writeResult=writeToFile();
    //Check the result. if we can write it successfully, clear the batch count.
    if(writeResult)
    {
        //Clear the count.
        m_operateCount=0;
    }
    //Give back the write result.
    return writeResult;
}

void KNJsonDatabase::append(const QJsonValue &value)
{
    //Append the value to the data field.
    m_dataField.append(value);
    //Count a operate.
    count();
}

void KNJsonDatabase::replace(int i, const QJsonValue &value)
{
    //Check the position before replace.
    Q_ASSERT(i>-1 && i<m_dataField.size());
    //Replace the value in the data field.
    m_dataField.replace(i, value);
    //Count a operate.
    count();
}

void KNJsonDatabase::insert(int i, const QJsonValue &value)
{
    //Check the position before replace.
    Q_ASSERT(i>-1 && i<m_dataField.size());
    //Insert the value in the data field.
    m_dataField.insert(i, value);
    //Count a operate.
    count();
}

void KNJsonDatabase::removeAt(int i)
{
    //Check the size before remove it.
    Q_ASSERT(i>-1 && i<m_dataField.size());
    //Remove the indexed value in the data field.
    m_dataField.removeAt(i);
    //Count a operate.
    count();
}

void KNJsonDatabase::clear()
{
    //Reset the data field.
    m_dataField=QJsonArray();
}

QJsonValue KNJsonDatabase::at(int i)
{
    //Don't need to check, the at() will be checked for us.
    return m_dataField.at(i);
}

int KNJsonDatabase::size() const
{
    return m_dataField.size();
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

inline bool KNJsonDatabase::writeToFile()
{
    //Check the directory first.
    checkDatabaseDir();
    //Generate the database object.
    QJsonObject contentObject;
    //Insert the data field.
    contentObject.insert("Database", m_dataField);
    //Insert the database version information to the object.
    contentObject.insert("Major", MajorVersion);
    contentObject.insert("Minor", MinorVersion);
    //Open the file as write only mode.
    if(m_file->open(QIODevice::WriteOnly))
    {
        //Write the binary data to the file.
        m_file->write(QJsonDocument(contentObject).toBinaryData());
        //Close the file.
        m_file->close();
        //Write success.
        return true;
    }
    //Or else, write failed.
    return false;
}

inline void KNJsonDatabase::count()
{
    //Increase the counter.
    m_operateCount++;
    //Check the operate count is under the maximum write counting.
    if(m_operateCount==MaxOperateCount)
    {
        //Do a write, to save those datas. If the program crashed, the data
        //won't be lose a lot.
        write();
    }
}

