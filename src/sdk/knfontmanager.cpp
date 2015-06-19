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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFontDatabase>

#include "knfontmanager.h"

KNFontManager *KNFontManager::m_instance=nullptr;

KNFontManager *KNFontManager::instance()
{
    return m_instance;
}

KNFontManager::KNFontManager(QObject *parent) :
    QObject(parent)
{
    //Check the singleton instance first.
    if(m_instance==nullptr)
    {
        m_instance=this;
    }
}

void KNFontManager::loadCustomFontFolder(const QString &folderPath)
{
    QDir fontDir(folderPath);
    //Check the folder is exsist or not, if not exsist, create one.
    if(fontDir.exists())
    {
        //Get all the files, load all the fonts.
        QFileInfoList fontFiles=fontDir.entryInfoList();
        for(QFileInfoList::const_iterator i=fontFiles.constBegin();
            i!=fontFiles.constEnd();
            ++i)
        {
            //Ignore the dot(.) and dot-dot(..).
            if((*i).fileName()=="." || (*i).fileName()=="..")
            {
                continue;
            }
            //If the type of current file is File, then try to load the font.
            if((*i).isFile())
            {
                loadCustomFont(*i);
            }
        }
    }
    else
    {
        fontDir.mkpath(folderPath);
    }
}

void KNFontManager::loadCustomFont(const QString &filePath)
{
    QFontDatabase::addApplicationFont(filePath);
}

void KNFontManager::setGlobalFont(const QString &fontName,
                                  const qreal &pointSize)
{
    //Ignore the invalid request.
    if(fontName.isEmpty())
    {
        return;
    }
    //Generate the font from the application font.
    QFont globalFont=qApp->font();
    globalFont.setFamily(fontName);
    globalFont.setPointSizeF(pointSize);
    //Set the font.
    qApp->setFont(globalFont);
}
