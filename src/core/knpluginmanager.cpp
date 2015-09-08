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
#include <QStyleFactory>

//Dependences.
#include "knglobal.h"
#include "knconfiguremanager.h"
#include "knmainwindow.h"
#include "knversion.h"

//Ports
#include "knmainwindowheaderbase.h"
#include "knpreferenceplugin.h"
#include "kncategoryplugin.h"
#include "knabstractmusicplugin.h"

//Plugins
#include "knmainwindowheader.h"
#include "plugin/knmusicplugin/knmusicplugin.h"
#include "plugin/knpreference/knpreference.h"

#include "knmessagebox.h"

#include "knpluginmanager.h"

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent),
    m_mainWindow(nullptr),
    m_header(nullptr)
{
    //Set the application information.
    setApplicationInformation();
    //Set fusion style to application.
    QApplication::setStyle(QStyleFactory::create("fusion"));
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
    //Check the preference pointer.
    if(plugin==nullptr)
    {
        return;
    }
    //Set the preference panel to global preference plugin.
    knGlobal->setPreference(plugin);
    //Give the preference panel to main window.
    if(m_mainWindow!=nullptr)
    {
        m_mainWindow->setPreferencePanel(plugin);
    }
}

void KNPluginManager::loadMusicPlugin(KNAbstractMusicPlugin *plugin)
{
    //Load the music categroy plugin first.
    loadCategoryPlugin(plugin);
    //Set the main player provided by the music plugin.
    m_mainWindow->setMainPlayer(plugin->mainPlayer());
}

void KNPluginManager::loadCategoryPlugin(KNCategoryPlugin *plugin)
{
    //Load the plugins of the plugin.
    plugin->loadPlugins();
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
        //Set the main window to global.
        knGlobal->setMainWindow(m_mainWindow);
    }
}

void KNPluginManager::loadPlugins()
{
    //Initial the infrastructure;
    loadHeader(new KNMainWindowHeader);
    loadPreference(new KNPreference);

    //Load the category plugin.
    loadMusicPlugin(new KNMusicPlugin);
}

void KNPluginManager::launchApplication()
{
    //Show the main window.
    m_mainWindow->show();
}
