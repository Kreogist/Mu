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

#include "knthememanager.h"

KNThemeManager *KNThemeManager::m_instance=nullptr;

KNThemeManager *KNThemeManager::instance()
{
    return m_instance;
}

KNThemeManager::KNThemeManager(QObject *parent) :
    QObject(parent)
{
    //Check the instance first.
    if(nullptr==m_instance)
    {
        //Save the current instance as the global instance.
        m_instance=this;
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
}

QPalette KNThemeManager::getPalette(const QString &name) const
{
    ;
}

void KNThemeManager::loadThemeFiles(const QString &themeDirPath)
{
    //Clear the previous theme list.
    m_themeList.clear();
    //Add the default theme as the first item in the list.
    ThemeItem themeDefault;
    themeDefault.name="Default";
    themeDefault.path="";
    //The structure of a theme directory.
    /* Theme
     * |-<Theme Name>
     * | |-<Theme Name>.json
     * | |-<Theme Name>.png
     */

}

