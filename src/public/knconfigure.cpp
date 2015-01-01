/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QDir>
#include <QFont>
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
    generateConfigureFolder();
    //Generate the system and user configure file path.
    m_systemConfigurePath=m_configurePath+"/system.json";
    m_userConfigurePath=m_configurePath+"/user.json";
    //Load the configure.
    loadConfigureFromFile(m_systemConfigurePath, m_systemConfigure);
    loadConfigureFromFile(m_userConfigurePath, m_customConfigure);
}

void KNConfigure::saveConfigure()
{
    //Do folder check first.
    generateConfigureFolder();
    //Save the configure.
    saveConfigureToFile(m_systemConfigurePath, m_systemConfigure);
    saveConfigureToFile(m_userConfigurePath, m_customConfigure);
}

void KNConfigure::setSystemData(const QString &key,
                                const QJsonValue &value)
{
    m_systemConfigure[key]=value;
}

void KNConfigure::setCustomData(const QString &module,
                                const QString &key,
                                const QJsonValue &value)
{
    QJsonValue currentModuleValue=m_customConfigure[module];
    QJsonObject currentModule=
            currentModuleValue.type()==QJsonValue::Object?
                currentModuleValue.toObject():QJsonObject();
    currentModule[key]=value;
    m_customConfigure[module]=currentModule;
}

QVariant KNConfigure::systemData(const QString &key)
{
    return parseJsonValue(m_systemConfigure[key]);
}

QVariant KNConfigure::customData(const QString &module, const QString &key)
{
    QJsonValue currentModuleValue=m_customConfigure[module];
    return currentModuleValue.type()==QJsonValue::Object?
              parseJsonValue(currentModuleValue.toObject()[key]):QVariant();
}

KNConfigure::KNConfigure(QObject *parent) :
    QObject(parent)
{
    //Initial the type hash.
    m_objectType.insert("Font", Font);
}

inline QVariant KNConfigure::parseJsonValue(const QJsonValue &value)
{
    if(value.type()==QJsonValue::Object)
    {
        QJsonObject valueObject=value.toObject();
        //This is an available configure file.
        if(valueObject.contains("KNObjectType"))
        {
            int objectType=m_objectType.value(
                        valueObject.value("KNObjectType").toString(),
                        -1);
            switch(objectType)
            {
            case Font:
            {
                QFont valueFont=QApplication::font();
                valueFont.setFamily(valueObject.value("Family").toString());
                valueFont.setPixelSize(valueObject.value("PixelSize").toInt());
                valueFont.setBold(valueObject.value("Bold").toBool());
                valueFont.setItalic(valueObject.value("Italic").toBool());
                valueFont.setUnderline(valueObject.value("Underline").toBool());
                valueFont.setStrikeOut(valueObject.value("Strikeout").toBool());
                valueFont.setKerning(valueObject.value("Kerning").toBool());
                return QVariant::fromValue(valueFont);
                break;
            }
            }
        }
        return QVariant();
    }
    return QVariant(value);
}

inline void KNConfigure::generateConfigureFolder()
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

inline void KNConfigure::loadConfigureFromFile(const QString &filePath,
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

inline void KNConfigure::saveConfigureToFile(const QString &filePath,
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

