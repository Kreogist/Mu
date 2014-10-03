/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include "knconfigure.h"

#include <QDebug>

KNConfigure *KNConfigure::m_instance=nullptr;

KNConfigure *KNConfigure::instance()
{
    return m_instance==nullptr?m_instance=new KNConfigure:m_instance;
}

void KNConfigure::loadConfigure()
{
    //Do folder check first.
    checkConfigureFolder();
    //Generate the system and user configure file path.
    m_systemConfigurePath=m_configurePath+"/system.json";
    m_userConfigurePath=m_configurePath+"/user.json";
    //Load the configure.
    loadConfigureFromFile(m_systemConfigurePath, m_systemConfigure);
    loadConfigureFromFile(m_userConfigurePath, m_userConfigure);
}

void KNConfigure::saveConfigure()
{
    //Do folder check first.
    checkConfigureFolder();
    //Save the
    saveConfigureToFile(m_systemConfigurePath, m_systemConfigure);
    saveConfigureToFile(m_userConfigurePath, m_userConfigure);
}

void KNConfigure::setSystemData(const QString &key,
                                const QJsonValue &value)
{
    m_systemConfigure[key]=value;
}

void KNConfigure::setUserData(const QString &module,
                              const QString &key,
                              const QJsonValue &value)
{
    QJsonValue currentModuleValue=m_userConfigure[module];
    QJsonObject currentModule=
            currentModuleValue.type()==QJsonValue::Object?
                currentModuleValue.toObject():QJsonObject();
    currentModule[key]=value;
    m_userConfigure[module]=currentModule;
}

QVariant KNConfigure::systemData(const QString &key)
{
    return QVariant(m_systemConfigure[key]);
}

KNConfigure::KNConfigure(QObject *parent) :
    QObject(parent)
{
    ;
}

void KNConfigure::checkConfigureFolder()
{
    //Check is the folder exist.
    QDir configureFolder(m_configurePath);
    //Update the configure path.
    m_configurePath=configureFolder.absolutePath();
    //If not exist, means there's no file and no folder, create the folder.
    if(!configureFolder.exists())
    {
        configureFolder.mkpath(m_configurePath);
        //Double check, if still not exist, check is there is a file named this.
        if(!configureFolder.exists())
        {
            QFile sameNameCheck(m_configurePath);
            //If there's no same named file.
            if(!sameNameCheck.exists())
            {
                //I can't understand what happend.
                return;
            }
            //Remove the file.
            QFile::remove(m_configurePath);
            //Create the folder again.
            configureFolder.mkpath(m_configurePath);
            if(!configureFolder.exists())
            {
                //If still not exists, I have no idea.
                return;
            }
        }
    }
}

void KNConfigure::loadConfigureFromFile(const QString &filePath,
                                        QJsonObject &configureObject)
{
    QFile configureFile(filePath);
    //Check is the file exist.
    if(!configureFile.exists())
    {
        //Okay, load nothing.
        return;
    }
    //Open the file.
    if(configureFile.open(QIODevice::ReadOnly))
    {
        //Read the confiure.
        QJsonDocument configureDocument=
                QJsonDocument::fromJson(configureFile.readAll());
        configureFile.close();
        //Get the configure array.
        configureObject=configureDocument.object();
    }
}

void KNConfigure::saveConfigureToFile(const QString &filePath,
                                        const QJsonObject &configureObject)
{
    //Open the file for writing.
    QFile configureFile(filePath);
    if(configureFile.open(QIODevice::WriteOnly))
    {
        //Generate the document.
        QJsonDocument configureDocument;
        configureDocument.setObject(configureObject);
        //Write the document to file.
        configureFile.write(configureDocument.toJson());
        //Close the file.
        configureFile.close();
    }
}

QString KNConfigure::configurePath() const
{
    return m_configurePath;
}

void KNConfigure::setConfigurePath(const QString &configureFilePath)
{
    m_configurePath=configureFilePath;
}

