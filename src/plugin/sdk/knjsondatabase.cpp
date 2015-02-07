/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QFile>
#include <QDir>
#include <QJsonParseError>

#include "knjsondatabase.h"

#include <QDebug>

int KNJSONDatabase::m_majorVersion=2;
int KNJSONDatabase::m_minorVersion=0;

#define MAX_BATCH 300

KNJSONDatabase::KNJSONDatabase(QObject *parent) :
    QObject(parent)
{
}

void KNJSONDatabase::setDatabaseFile(const QString &filePath)
{
    //Set the database file info.
    m_databaseFileInfo=QFileInfo(filePath);
    //Check the directory of the database.
    checkDatabaseDir();
    //Set the file to the file pointer.
    m_databaseFile=new QFile(m_databaseFileInfo.absoluteFilePath());
}

void KNJSONDatabase::read()
{
    //Check the file existance.
    if(!m_databaseFile->exists())
    {
        return;
    }
    //Open the file and read all the data, if cannot read the file, return.
    if(!m_databaseFile->open(QIODevice::ReadOnly))
    {
        return;
    }
    //Read the data from the file.
    m_document=QJsonDocument::fromJson(m_databaseFile->readAll(),
                                       &m_lastError);
    m_databaseFile->close();
    //Check whether the document is null.
    if(m_document.isNull())
    {
        return;
    }
    //Transform the document to object.
    m_contentObject=m_document.object();
    //Clear the document.
    m_document=QJsonDocument();
    //Check the version of the database.
    if(m_contentObject.value("Major").toInt()>m_majorVersion ||
            m_contentObject.value("Minor").toInt()>m_minorVersion)
    {
        //!FIXME: This is create by a higher version.
        return;
    }
    //Get the data field.
    //*****Magic, don't touch!!*****
    //I don't know why give the datafield the raw data it will crash.
    //The reason is: when you delete one item from the QJsonArray, the size
    //will be strange. Although it contains more than 70 items, the size() will
    //give out only 3. But if the array is empty at beginning, this bug won't
    //happend.
    //So manually copy the data can solve this bug until Digia give out a fix.
    QJsonArray rawDataField=m_contentObject.value("Database").toArray();
    for(QJsonArray::iterator i=rawDataField.begin();
        i!=rawDataField.end();
        ++i)
    {
        m_dataField.append(*i);
    }
    //Clear the content object.
    m_contentObject=QJsonObject();
}

void KNJSONDatabase::write()
{
    //Check if we need to write.
    if(m_batchCount==0)
    {
        return;
    }
    //Insert the content data to object.
    m_contentObject.insert("Database", m_dataField);
    //Set the version data.
    m_contentObject.insert("Major", m_majorVersion);
    m_contentObject.insert("Minor", m_minorVersion);
    //Set the object to document.
    m_document.setObject(m_contentObject);
    //Check the dir first.
    checkDatabaseDir();
    //Write the document to file.
    if(m_databaseFile->open(QIODevice::WriteOnly))
    {
        m_databaseFile->write(m_document.toJson());
        m_databaseFile->close();
    }
    //Clear count.
    m_batchCount=0;
}

void KNJSONDatabase::append(const QJsonValue &value)
{
    m_dataField.append(value);
    //Count a operate.
    addBatchCount();
}

void KNJSONDatabase::replace(int i, const QJsonValue &value)
{
    m_dataField.replace(i, value);
    //Count a operate.
    addBatchCount();
}

void KNJSONDatabase::removeAt(int i)
{
    m_dataField.removeAt(i);
    //Count a operate.
    addBatchCount();
}

QJsonValue KNJSONDatabase::at(int i)
{
    return m_dataField.at(i);
}

QJsonArray::iterator KNJSONDatabase::begin()
{
    return m_dataField.begin();
}

QJsonArray::iterator KNJSONDatabase::end()
{
    return m_dataField.end();
}

inline void KNJSONDatabase::addBatchCount()
{
    //Count the operate.
    m_batchCount++;
    //Check the count.
    if(m_batchCount==MAX_BATCH)
    {
        //Write to disk.
        write();
    }
}

inline void KNJSONDatabase::checkDatabaseDir()
{
    //Check is the dir a dir?
    QFileInfo databaseDir(m_databaseFileInfo.absolutePath());
    if(databaseDir.isDir() && databaseDir.exists())
    {
        return;
    }
    //If it's a file, delete it.
    if(databaseDir.isFile())
    {
        //Parse it as a file.
        QFile sameNameFile(databaseDir.absoluteFilePath());
        //Remove it.
        sameNameFile.remove();
    }
    //Generate the dir.
    QDir destinationDir;
    destinationDir.mkpath(databaseDir.absoluteFilePath());
}
