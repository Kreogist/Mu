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
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QWidget>

#include "knutil.h"

#include "knthememanager.h"

#include <QDebug>

KNThemeManager *KNThemeManager::m_instance=nullptr;

KNThemeManager *KNThemeManager::instance()
{
    return m_instance;
}

void KNThemeManager::initial(QObject *parent)
{
    //Check the instance first.
    if(nullptr==m_instance)
    {
        //Save the current instance as the global instance.
        m_instance=new KNThemeManager(parent);
    }
}

KNThemeManager::KNThemeManager(QObject *parent) :
    QObject(parent)
{
    //Initial the global color role map.
    m_colorRoleList["WindowText"]=QPalette::WindowText;
    m_colorRoleList["Button"]=QPalette::Button;
    m_colorRoleList["Light"]=QPalette::Light;
    m_colorRoleList["Midlight"]=QPalette::Midlight;
    m_colorRoleList["Dark"]=QPalette::Dark;
    m_colorRoleList["Mid"]=QPalette::Mid;
    m_colorRoleList["Text"]=QPalette::Text;
    m_colorRoleList["BrightText"]=QPalette::BrightText;
    m_colorRoleList["ButtonText"]=QPalette::ButtonText;
    m_colorRoleList["Base"]=QPalette::Base;
    m_colorRoleList["Window"]=QPalette::Window;
    m_colorRoleList["Shadow"]=QPalette::Shadow;
    m_colorRoleList["Highlight"]=QPalette::Highlight;
    m_colorRoleList["HighlightedText"]=QPalette::HighlightedText;
    m_colorRoleList["Link"]=QPalette::Link;
    m_colorRoleList["LinkVisited"]=QPalette::LinkVisited;
    m_colorRoleList["AlternateBase"]=QPalette::AlternateBase;
    m_colorRoleList["NoRole"]=QPalette::NoRole;
    m_colorRoleList["ToolTipBase"]=QPalette::ToolTipBase;
    m_colorRoleList["ToolTipText"]=QPalette::ToolTipText;
}

QPalette KNThemeManager::getPalette(const QString &name) const
{
    return m_paletteList.value(name, QApplication::palette());
}

void KNThemeManager::registerWidget(QWidget *widget)
{
    //Add the widget to widget list.
    m_widgetList.append(widget);
    //Give the registered widget its palette.
    widget->setPalette(getPalette(widget->objectName()));
}

void KNThemeManager::loadThemeFiles(const QString &themeDirPath)
{
    //Clear the previous theme list.
    m_themeList.clear();
    //Add the default theme as the first item in the list.
    addTheme("Default", "://public/default_theme.json", QPixmap());
    //The structure of a theme directory.
    /* Theme
     * |-<Theme Name>
     * | |-<Theme Name>.json
     * | |-<Theme Name>.png
     */
    //Check the folder exist. If no, return.
    QDir themeFolder(themeDirPath);
    if(!themeFolder.exists())
    {
        return;
    }
    //Get all the folder in the theme folder.
    QFileInfoList folderList=themeFolder.entryInfoList();
    for(QFileInfoList::const_iterator i=folderList.constBegin();
        i!=folderList.constEnd();
        ++i)
    {
        QString themeName=(*i).fileName();
        //Ignore if the current info is not folder, dot(.) and dot-dot(..).
        if(!(*i).isDir() || themeName=="." || themeName=="..")
        {
            continue;
        }
        //Get the theme file and the preview image.
        QFileInfo themeFile((*i).absoluteFilePath()+"/"+themeName+".json"),
                previewFile((*i).absoluteFilePath()+"/"+themeName+".png");
        if(themeFile.exists())
        {
            //Check the preview image.
            QPixmap previewImage;
            if(previewFile.exists())
            {
                previewImage.load(previewFile.absoluteFilePath());
                if(previewImage.isNull())
                {
                    //! FIXME: Set the preview to no image.
                    ;
                }
            }
            else
            {
                //! FIXME: Set the preview to no image.
                ;
            }
            //Add the new theme
            addTheme(themeName,
                     themeFile.absoluteFilePath(),
                     previewImage);
        }
    }
}

void KNThemeManager::setTheme(const int &index)
{
    if(index>-1 && index<m_themeList.size())
    {
        //Save the current item index.
        m_currentTheme=index;
        //Load the theme file.
        loadTheme(m_themeList.at(m_currentTheme).path);
    }
}

void KNThemeManager::setTheme(const QString &themeName)
{
    //Generate a fake theme item, find it in the list.
    ThemeItem theme;
    theme.name=themeName;
    //Find the item.
    int itemIndex=m_themeList.indexOf(theme);
    //If we cannot find the item, ignore the theme set request.
    if(itemIndex==-1)
    {
        return;
    }
    //Save the current item index, load the theme file.
    m_currentTheme=itemIndex;
    loadTheme(m_themeList.at(m_currentTheme).path);
}

inline void KNThemeManager::addTheme(const QString &name,
                                     const QString &path,
                                     const QPixmap &preview)
{
    ThemeItem theme;
    theme.name=name;
    theme.path=path;
    theme.preview=preview;
    m_themeList.append(theme);
}

void KNThemeManager::loadTheme(const QString &themeFilePath)
{
    QFile themeFile(themeFilePath);
    //Check the theme file exist or not. Open the file as ReadOnly mode.
    if(!themeFile.exists() || !themeFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //Read the data and close the file.
    QJsonObject themeData=QJsonDocument::fromJson(themeFile.readAll()).object();
    themeFile.close();
    //Generate the temporary palette map and name hash list.
    //We don't write it to palette list to avoid the bad file.
    QHash<QString, QPalette> paletteList;
    //Parse the theme data.
    QStringList paletteNames=themeData.keys();
    for(QStringList::iterator i=paletteNames.begin();
        i!=paletteNames.end();
        ++i)
    {
        //Check whether we have loaded this name before.
        if(paletteList.contains(*i))
        {
            continue;
        }
        //Parse the data.
        parsePalette(*i, &themeData, paletteList);
    }
    //Check the new map is available or not.
    if(!paletteList.isEmpty())
    {
        //Save the palette map.
        m_paletteList=paletteList;
        //Update all the widget's palette in the widget list.
        for(QLinkedList<QWidget *>::iterator i=m_widgetList.begin();
            i!=m_widgetList.end();
            ++i)
        {
            (*i)->setPalette(getPalette((*i)->objectName()));
        }
        //Emit the theme change signal.
        emit themeChange();
    }
}

void KNThemeManager::parsePalette(const QString &name,
                                  QJsonObject *data,
                                  QHash<QString, QPalette> &map)
{
    //Get current object.
    QJsonObject palData=data->value(name).toObject();
    QPalette pal;
    //Check data contains parent or not.
    if(palData.contains("Parent"))
    {
        //If current palette has a parent palette, check whether the parent
        //palette has been load.
        QString parentName=palData.value("Parent").toString();
        if(!map.contains(parentName))
        {
            //If the parent hasn't been loaded, load the parent palette
            //recursively.
            parsePalette(parentName, data, map);
        }
        //Get a copy from the parent palette.
        pal=map.value(parentName);
    }
    else
    {
        //Get the application palette.
        pal=QApplication::palette();
    }
    //Parse the current palette.
    QStringList properties=palData.keys();
    for(QStringList::iterator i=properties.begin();
        i!=properties.end();
        ++i)
    {
        if(m_colorRoleList.contains(*i))
        {
            pal.setColor(m_colorRoleList.value(*i),
                         KNUtil::parseColor(palData.value(*i).toString()));
        }
    }
    //Insert the palette to the map.
    map.insert(name, pal);
}
