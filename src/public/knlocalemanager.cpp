/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QDir>

#include "knlocalemanager.h"

#include <QDebug>

KNLocaleManager *KNLocaleManager::m_instance=nullptr;

KNLocaleManager *KNLocaleManager::instance()
{
    return m_instance==nullptr?m_instance=new KNLocaleManager:m_instance;
}

void KNLocaleManager::loadLanguageFiles()
{
    //We will only find language files in "Language" folder of the app path.
    QDir languageFolder(QApplication::applicationDirPath() + "/Language");
    if(!languageFolder.exists()) //No folder you say a P?
    {
        return;
    }
    QFileInfoList folderList=languageFolder.entryInfoList();
    for(QFileInfoList::iterator i=folderList.begin();
        i!=folderList.end();
        ++i)
    {
        if(!(*i).isDir() || (*i).fileName()=="." || (*i).fileName()=="..")
        {
            continue;
        }
        qDebug()<<(*i).fileName();
    }
}

KNLocaleManager::KNLocaleManager(QObject *parent) :
    QObject(parent)
{
    loadLanguageFiles();
}
