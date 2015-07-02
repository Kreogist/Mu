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
#include "knconfiguremanager.h"
#include "knmainwindow.h"
#include "knversion.h"

//Ports
#include "knmainwindowheaderbase.h"
#include "knpreferenceplugin.h"
#include "kncategoryplugin.h"

//Plugins
#include "knmainwindowheader.h"
#include "plugin/knmusicplugin/knmusicplugin.h"
#include "plugin/knpreference/knpreference.h"

#include "knpluginmanager.h"

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent),
    m_mainWindow(nullptr),
    m_header(nullptr),
    m_preference(nullptr)
{
    //Set the application information.
    setApplicationInformation();
    //Initial the global.
    KNGlobal::initial(this);
}

KNPluginManager::~KNPluginManager()
{
    //Save the configure.
    knConf->saveConfigure();
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
    //Configure application attributes.
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
}

void KNPluginManager::loadHeader(KNMainWindowHeaderBase *header)
{
    //You should load the main window first.
    if(m_mainWindow==nullptr)
    {
        return;
    }
    //Save the header pointer.
    m_header=header;
    //Check the header pointer is valid or not.
    if(m_header==nullptr)
    {
        return;
    }
    //Give the header to main window.
    m_mainWindow->setHeader(m_header);
}

void KNPluginManager::loadPreference(KNPreferencePlugin *plugin)
{
    //You should load the main window first.
    if(m_mainWindow==nullptr)
    {
        return;
    }
    //Save the preference
    m_preference=plugin;
    //Check the preference pointer.
    if(m_preference==nullptr)
    {
        return;
    }
    if(m_mainWindow!=nullptr)
    {
        //Give the preference panel to main window.
        m_mainWindow->setPreferencePanel(m_preference);
    }
}

void KNPluginManager::loadCategoryPlugin(KNCategoryPlugin *plugin)
{
    //Set the category plugin.
    m_header->setCategoryPlugin(plugin);
    //Set the category to be the main widget.
    m_mainWindow->setMainWidget(plugin);
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
    //Initial the infrastructure;
    loadHeader(new KNMainWindowHeader);
    loadPreference(new KNPreference);

    //Load the category plugin.
    loadCategoryPlugin(new KNMusicPlugin);
}

void KNPluginManager::launchApplication()
{
    //Show the main window.
    m_mainWindow->show();
}
