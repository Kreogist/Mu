/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QTranslator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QFile>

#include "knlocalemanager.h"

#include <QDebug>

KNLocaleManager *KNLocaleManager::m_instance=nullptr;

KNLocaleManager *KNLocaleManager::instance()
{
    return m_instance==nullptr?m_instance=new KNLocaleManager:m_instance;
}

QString KNLocaleManager::currentLanguageCaption() const
{
    return (m_currentIndex>-1 && m_currentIndex<m_languageList.size())?
                m_languageList.at(m_currentIndex).name:"N/A";
}

QString KNLocaleManager::currentLanguage() const
{
    return (m_currentIndex>-1 && m_currentIndex<m_languageList.size())?
                m_languageList.at(m_currentIndex).id:QString();
}

QPixmap KNLocaleManager::currentLanguageIcon() const
{
    return (m_currentIndex>-1 && m_currentIndex<m_languageList.size())?
                m_languageList.at(m_currentIndex).icon:m_noImageIcon;
}

int KNLocaleManager::languageSize() const
{
    return m_languageList.size();
}

QString KNLocaleManager::languageNameAt(const int &i)
{
    return m_languageList.at(i).name;
}

QPixmap KNLocaleManager::languageIconAt(const int &i)
{
    return m_languageList.at(i).icon;
}

void KNLocaleManager::setLanguageIndex(const int &index)
{
    if(index<-1 || index>m_languageList.size())
    {
        return;
    }
    //Backup the index.
    m_currentIndex=index;
    //Remove the old translator.
    qApp->removeTranslator(m_translator);
    //Load the language file.
    m_translator->load(m_languageList.at(m_currentIndex).filePath);
    if(!m_translator->isEmpty())
    {
        //Install translator.
        qApp->installTranslator(m_translator);
    }
    //Ask to retranslate.
    emit requireRetranslate();
}

void KNLocaleManager::setLanguage(const QString &id)
{
    //Check if the id is empty, ignore the unavailable id.
    if(id.isEmpty())
    {
        return;
    }
    //Find the language one by one.
    for(int i=0; i<m_languageList.size(); i++)
    {
        if(m_languageList.at(i).id==id)
        {
            setLanguageIndex(i);
            return;
        }
    }
    //When we came here, it means that we can't find the id, set the language to
    //0.
    setLanguageIndex(0);
}

void KNLocaleManager::loadLanguageFiles()
{
    //Add English language item, English will always be the first language.
    LanguageItem englishLanguage;
    englishLanguage.id="English";
    englishLanguage.name="English";
    englishLanguage.icon=QPixmap("://public/English.png");
    englishLanguage.filePath="";
    m_languageList.append(englishLanguage);
    //We will only find language files in "Language" folder of the app path.
    QDir languageFolder(m_languageDirPath);
    if(!languageFolder.exists()) //No folder you say a P?
    {
        return;
    }
    //Get the language translation.
    QFile languageTranslationFile(languageFolder.filePath("language.json"));
    QJsonObject languageTranslation;
    if(languageTranslationFile.open(QIODevice::ReadOnly))
    {
        QJsonDocument rawFileDocument=
                QJsonDocument::fromJson(languageTranslationFile.readAll());
        languageTranslation=rawFileDocument.object();
        languageTranslationFile.close();
    }
    //Get all the folder in the language list.
    QFileInfoList folderList=languageFolder.entryInfoList();
    for(QFileInfoList::iterator i=folderList.begin();
        i!=folderList.end();
        ++i)
    {
        //Ignore the . and .. dir.
        if(!(*i).isDir() || (*i).fileName()=="." || (*i).fileName()=="..")
        {
            continue;
        }
        //Get the language file and the icon file.
        QFileInfo languageFileInfo((*i).absoluteFilePath()+"/"+(*i).fileName()+".qm"),
                  languageIconInfo((*i).absoluteFilePath()+"/"+(*i).fileName()+".png");
        //Check is the language file available.
        if(languageFileInfo.exists())
        {
            LanguageItem currentLanguage;
            //Using filename as ID.
            currentLanguage.id=(*i).fileName();
            //Get the language name.
            currentLanguage.name=languageTranslation[(*i).fileName()].toString();
            if(currentLanguage.name.isEmpty())
            {
                currentLanguage.name=(*i).fileName();
            }
            //Add to language file list.
            currentLanguage.filePath=languageFileInfo.absoluteFilePath();
            //Check the icon file.
            if(languageIconInfo.exists())
            {
                //Try to load the icon file.
                currentLanguage.icon.load(languageIconInfo.absoluteFilePath());
                //If the file cannot be used, add a no image icon instead.
                if(currentLanguage.icon.isNull())
                {
                    currentLanguage.icon=m_noImageIcon;
                }
            }
            else
            {
                //Use no icon file icon.
                currentLanguage.icon=m_noImageIcon;
            }
            //Append language list.
            m_languageList.append(currentLanguage);
        }
    }
}

KNLocaleManager::KNLocaleManager(QObject *parent) :
    QObject(parent)
{
    //Initial the translator.
    m_translator=new QTranslator(this);
    //Initial no iamge icon.
    m_noImageIcon=QPixmap(":/plugin/configure/locale/noIcon.png");
}

QString KNLocaleManager::systemLocaleLanguageID()
{
    //Generate the default locale, get the system data.
    QLocale locale;
    //Magic!
    switch(locale.language())
    {
    case QLocale::Chinese:
        return "Simplified_Chinese";
    default:
        return "English";
    }
}
QString KNLocaleManager::languageDirPath() const
{
    return m_languageDirPath;
}

void KNLocaleManager::setLanguageDirPath(const QString &languageDirPath)
{
    m_languageDirPath = languageDirPath;
}

