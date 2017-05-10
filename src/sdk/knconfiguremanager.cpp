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

#include "knutil.h"
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

QVariant KNConfigureManager::configureValue(QStringList path,
                                            const QVariant &defaultValue)
{
    //Check the data of the path.
    QString topPath=path.takeFirst();
    //Prepare the configure data.
    KNConfigure *targetConfigure=getTopLevelConfigure(topPath);
    //Check whether the pointer is nullptr.
    if(!targetConfigure)
    {
        //No configure found, return a null variant.
        return QVariant();
    }
    //Get the value from the configure.
    return targetConfigure->getPathData(path, defaultValue);
}

void KNConfigureManager::setFolderPath(const QString &folderPath,
                                       const QString &accountFolderPath)
{
    //Check if the folder path is vaild.
    if((!folderPath.isEmpty()) && !QFileInfo::exists(folderPath))
    {
        //Build the director.
        KNUtil::ensurePathValid(folderPath);
    }
    //Check if account folder path is valid.
    if((!accountFolderPath.isEmpty()) && !QFileInfo::exists(accountFolderPath))
    {
        //Build the director.
        KNUtil::ensurePathValid(accountFolderPath);
    }
    //Save the folder path.
    m_folderPath=folderPath;
    m_accountFolderPath=accountFolderPath;
    //Reload the configure.
    reloadConfigure();
}

void KNConfigureManager::setConfigureValue(QStringList path,
                                           const QVariant &value)
{
    //Check the data of the path.
    QString topPath=path.takeFirst();
    //Prepare the configure data.
    KNConfigure *targetConfigure=getTopLevelConfigure(topPath);
    //Check whether the pointer is nullptr.
    if(targetConfigure)
    {
        //Save the value to the configure.
        targetConfigure->setPathData(path, value);
    }
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
    //Load the user configure file, save its access time.
    QString userConfigurePath=configureDir.filePath("user.json");
    loadConfigureFile(userConfigurePath, User);
    //Get the user configure file update time.
    m_userConfigureUpdateTime=QFileInfo(userConfigurePath).lastModified();
    //If the account folder is empty, then set the folder path to application
    //dir.
    QDir accountDir(m_accountFolderPath.isEmpty()?
                        qApp->applicationDirPath():
                        m_accountFolderPath);
    loadConfigureFile(accountDir.filePath("account.json"), Account);
}

void KNConfigureManager::saveConfigure()
{
    //If the folder path is empty, then set the folder path to application dir.
    QDir configureDir(m_folderPath.isEmpty()?
                          qApp->applicationDirPath():
                          m_folderPath);
    //Save the configure data.
    saveConfigureFile(configureDir.filePath("cache.json"), Cache);
    saveConfigureFile(configureDir.filePath("system.json"), System);
    saveConfigureFile(configureDir.filePath("user.json"), User);
    //If the account folder is empty, then set the folder path to application
    //dir.
    QDir accountDir(m_accountFolderPath.isEmpty()?
                        qApp->applicationDirPath():
                        m_accountFolderPath);
    saveConfigureFile(accountDir.filePath("account.json"), Account);
    //Update the user configure updated time.
    m_userConfigureUpdateTime=QDateTime::currentDateTime();
    //Emit the signal.
    emit userConfigureUpdateTimeChanged();
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
            configureObject=
                    QJsonDocument::fromJson(configureFile.readAll()).object();
            //Close the file.
            configureFile.close();
        }
        //Set the object to configure.
        m_configures[type]->setDataObject(configureObject);
    }
}

void KNConfigureManager::saveConfigureFile(const QString &filePath, int type)
{
    //Check whether the type is valid.
    if(type<ConfigureTypeCount && type>-1)
    {
        //Load the file.
        QFile configureFile(filePath);
        //Open the configure file in write only mode.
        if(configureFile.open(QIODevice::WriteOnly))
        {
            //Generate the Json document.
            QJsonDocument configureDocument=
                    QJsonDocument(m_configures[type]->dataObject());
            //Write the Json data to the configure file.
            configureFile.write(configureDocument.toJson());
            //Close file.
            configureFile.close();
        }
    }
}

inline KNConfigure *KNConfigureManager::getTopLevelConfigure(
        const QString &topPath)
{
    //Check the path.
    if(topPath=="Cache")
    {
        //Cache configure.
        return configure(Cache);
    }
    else if(topPath=="System")
    {
        //System configure.
        return configure(System);
    }
    else if(topPath=="User")
    {
        //User configure
        return configure(User);
    }
    else if(topPath=="Account")
    {
        //Account configure
        return configure(Account);
    }
    //Undefined data.
    return nullptr;
}

QDateTime KNConfigureManager::userConfigureUpdateTime() const
{
    return m_userConfigureUpdateTime;
}
