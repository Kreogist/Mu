/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QDir>

#include "knconfigure.h"

#include "knconfiguremanager.h"

#include <QDebug>

KNConfigureManager *KNConfigureManager::m_instance=nullptr;

KNConfigureManager *KNConfigureManager::instance()
{
    return m_instance==nullptr?m_instance=new KNConfigureManager:m_instance;
}

void KNConfigureManager::loadConfigure()
{
    //Load configure data.
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configures[i]->loadConfigure();
    }
}

void KNConfigureManager::saveConfigure()
{
    //Save the configure, the data should be set before calling this function.
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configures[i]->saveConfigure();
    }
}

KNConfigure *KNConfigureManager::getConfigure(const int &index)
{
    Q_ASSERT(index<ConfigureTypeCount);
    return m_configures[index];
}

KNConfigureManager::KNConfigureManager(QObject *parent) :
    QObject(parent)
{
    //Initial the configure classes.
    for(int i=0; i<ConfigureTypeCount; i++)
    {
        m_configures[i]=new KNConfigure(this);
    }
}

inline void KNConfigureManager::generateConfigureFolder()
{
    //Check is the folder exist.
    QDir configureFolder(m_configurePath);
    //Update the configure path.
    m_configurePath=configureFolder.absolutePath();
    //If not exist, means there's no file and no folder, create the folder.
    if(!configureFolder.exists())
    {
        //Check is there is a file named this.
        if(!configureFolder.exists())
        {
            QFileInfo sameNameCheck(m_configurePath);
            if(sameNameCheck.exists() &&
                    sameNameCheck.isFile())
            {
                //Remove the file.
                QFile::remove(m_configurePath);
            }
        }
        //Now it should be ok the create the path.
        configureFolder.mkpath(m_configurePath);
    }
}

QString KNConfigureManager::configurePath() const
{
    return m_configurePath;
}

void KNConfigureManager::setConfigurePath(const QString &configureFilePath)
{
    //Save the configure path.
    m_configurePath=configureFilePath;
    //Generate the configure folder right now.
    generateConfigureFolder();
    //Set the configure file path according to the configure path.
    m_configures[Cache]->setFilePath(m_configurePath+"/cache.json");
    m_configures[System]->setFilePath(m_configurePath+"/system.json");
    m_configures[User]->setFilePath(m_configurePath+"/user.json");
}
