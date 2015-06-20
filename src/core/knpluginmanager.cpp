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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QApplication>

//Dependences.
#include "knglobal.h"
#include "knmainwindow.h"
#include "knversion.h"

#include "knpluginmanager.h"

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent),
    m_mainWindow(nullptr)
{
    //Set the application information.
    setApplicationInformation();
    //Initial the global.
    new KNGlobal(this);
}

KNPluginManager::~KNPluginManager()
{
    ;
}

void KNPluginManager::setApplicationInformation()
{
    //Generate the application name.
    QString applicationName=QChar(0x03BC);
    //Set static application information.
    QApplication::setApplicationName(applicationName);
    QApplication::setApplicationVersion(APP_VERSION_STR);
    QApplication::setApplicationDisplayName(applicationName);
    QApplication::setOrganizationName("Kreogist Dev Team");
    QApplication::setOrganizationDomain("http://kreogist.github.io/");
}

KNMainWindow *KNPluginManager::mainWindow() const
{
    return m_mainWindow;
}

void KNPluginManager::setMainWindow(KNMainWindow *mainWindow)
{
    if(m_mainWindow==nullptr)
    {
        //Save the main window object.
        m_mainWindow = mainWindow;
    }
}

void KNPluginManager::loadPlugins()
{
    ;
}

void KNPluginManager::launchApplication()
{
    //Show the main window.
    m_mainWindow->show();
}
