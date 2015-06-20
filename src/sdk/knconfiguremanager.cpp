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
#include <QFile>
#include <QApplication>
#include <QFileInfo>
#include <QJsonDocument>

#include "knconfigure.h"

#include "knconfiguremanager.h"

KNConfigureManager *KNConfigureManager::m_instance=nullptr;

KNConfigureManager *KNConfigureManager::instance()
{
    return m_instance;
}

void KNConfigureManager::initial(QObject *parent)
{
    //Set the current instance as the singleton instance.
    if(m_instance==nullptr)
    {
        m_instance=new KNConfigureManager(parent);
    }
}

KNConfigureManager::KNConfigureManager(QObject *parent) :
    QObject(parent),
    m_folderPath(QString())
{
    //Initial the configures.
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configures[i]=new KNConfigure(this);
    }
}

QString KNConfigureManager::folderPath() const
{
    return m_folderPath;
}

KNConfigure *KNConfigureManager::configure(int index)
{
    Q_ASSERT(index>-1 && index<ConfigureTypeCount);
    return m_configures[index];
}

void KNConfigureManager::setFolderPath(const QString &folderPath)
{
    //Check if the folder path is vaild.
    if(!(folderPath.isEmpty() || QFileInfo::exists(folderPath)))
    {
        return;
    }
    //Save the folder path.
    m_folderPath=folderPath;
    //Reload the configure.
    reloadConfigure();
}

void KNConfigureManager::reloadConfigure()
{
    //If the folder path is empty, then set the folder path to application dir.
    QDir configureDir(m_folderPath.isEmpty()?
                          qApp->applicationDirPath():
                          m_folderPath);
    //Load the configure file.
    loadConfigureFile(configureDir.filePath("cache.json"), Cache);
    loadConfigureFile(configureDir.filePath("system.json"), System);
    loadConfigureFile(configureDir.filePath("user.json"), User);
}

void KNConfigureManager::loadConfigureFile(const QString &filePath, int type)
{
    //Check whether the type is valid.
    if(type<ConfigureTypeCount && type>-1)
    {
        QJsonObject configureObject=QJsonObject();
        //Load the file.
        QFile configureFile(filePath);
        //Load the Json object only when the file is exist and it can be opened
        //in read only mode.
        if(configureFile.exists() &&
                configureFile.open(QIODevice::ReadOnly))
        {
            //Parse the json object.
            configureObject=QJsonDocument::fromJson(configureFile.readAll()).object();
            //Close the file.
            configureFile.close();
        }
        //Set the object to configure.
        m_configures[type]->setDataObject(configureObject);
    }
}
