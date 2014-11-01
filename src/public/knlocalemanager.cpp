/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QTranslator>
#include <QDir>

#include "knlocalemanager.h"

#include <QDebug>

KNLocaleManager *KNLocaleManager::m_instance=nullptr;

KNLocaleManager *KNLocaleManager::instance()
{
    return m_instance==nullptr?m_instance=new KNLocaleManager:m_instance;
}

QString KNLocaleManager::currentLanguageName() const
{
    return (m_currentIndex>-1 && m_currentIndex<m_languageNames.size())?
                m_languageNames.at(m_currentIndex):"N/A";
}

QPixmap KNLocaleManager::currentLanguageIcon() const
{
    return (m_currentIndex>-1 && m_currentIndex<m_languageIcons.size())?
                m_languageIcons.at(m_currentIndex):m_noImageIcon;
}

void KNLocaleManager::setLanguage(const int &index)
{
    if(index<-1 || index>m_languageFiles.size())
    {
        return;
    }
    //Backup the index.
    m_currentIndex=index;
    //Remove the old translator.
    qApp->removeTranslator(m_translator);
    //Load the language file.
    m_translator->load(m_languageFiles.at(m_currentIndex));
    //Install translator.
    qApp->installTranslator(m_translator);
    //Ask to retranslate.
    emit requireRetranslate();
}

void KNLocaleManager::loadLanguageFiles()
{
    //We will only find language files in "Language" folder of the app path.
    QDir languageFolder(QApplication::applicationDirPath() + "/Language");
    if(!languageFolder.exists()) //No folder you say a P?
    {
        return;
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
            //Get the language name.
            //!FIXME: Temply use folder name.
            m_languageNames.append((*i).fileName());
            //Add to language file list.
            m_languageFiles.append(languageFileInfo.absoluteFilePath());
            //Check the icon file.
            if(languageIconInfo.exists())
            {
                //Try to load the icon file.
                QPixmap languageIcon;
                languageIcon.load(languageIconInfo.absoluteFilePath());
                //If the file cannot be used, add a no image icon instead.
                m_languageIcons.append(languageIcon.isNull()?m_noImageIcon:languageIcon);
            }
            else
            {
                //Use no icon file icon.
                m_languageIcons.append(m_noImageIcon);
            }
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
    //Load all the language.
    loadLanguageFiles();
}
