/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFontDatabase>

#include "knfontmanager.h"

#include <QDebug>

KNFontManager *KNFontManager::m_instance=nullptr;

KNFontManager *KNFontManager::instance()
{
    return m_instance==nullptr?m_instance=new KNFontManager:m_instance;
}

void KNFontManager::loadCustomFont(const QString &fontPath)
{
    loadCustomFont(QFileInfo(fontPath));
}

void KNFontManager::loadCustomFont(const QFileInfo &fontInfo)
{
    QFile fontFile(fontInfo.absoluteFilePath());
    //Read all the file, try to load the font.
    if(fontFile.open(QIODevice::ReadOnly))
    {
        QFontDatabase::addApplicationFontFromData(fontFile.readAll());
        fontFile.close();
    }
}

void KNFontManager::loadCustomFontFolder(const QString &folderPath)
{
    QDir m_customFontDir(folderPath);
    //Check the folder is exsist or not, if not exsist, create one.
    if(m_customFontDir.exists())
    {
        //Get all the files, load all the fonts.
        QFileInfoList fontFiles=m_customFontDir.entryInfoList();
        int fontCount=fontFiles.size();
        while(fontCount--)
        {
            QFileInfo fontFileInfo=fontFiles.takeFirst();
            //Ignore the dot and dot-dot.
            if(fontFileInfo.fileName()=="." || fontFileInfo.fileName()=="..")
            {
                continue;
            }
            //If the current item is a file.
            if(fontFileInfo.isFile())
            {
                loadCustomFont(fontFileInfo);
            }
        }
    }
    else
    {
        m_customFontDir.mkpath(folderPath);
    }
}

void KNFontManager::setGlobalFont(const QString &fontName, const int &pixelSize)
{
    if(fontName.isNull() || fontName.isEmpty())
    {
        return;
    }
    //Generate the font.
    QFont globalFont=QApplication::font();
    globalFont.setFamily(fontName);
    globalFont.setPixelSize(pixelSize);
    //Set the font.
    QApplication::setFont(globalFont);
}

void KNFontManager::initialDefaultFont()
{
    setGlobalFont("Source Han Sans");
}

KNFontManager::KNFontManager(QObject *parent) :
    QObject(parent)
{
}
