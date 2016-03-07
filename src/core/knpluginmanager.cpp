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
#include "knplatformextras.h"

//Plugins
#include "knmainwindowheader.h"
#include "plugin/knmusicplugin/knmusicplugin.h"
#include "plugin/knpreference/knpreference.h"

//Platform Extras.
#ifdef Q_OS_WIN
#include "plugin/knwindowsextras/knwindowsextras.h"
#endif

#include "knpluginmanager.h"

#include "account/knaccount.h"

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent),
    m_mainWindow(nullptr),
    m_header(nullptr),
    m_musicPlugin(nullptr),
    m_platformExtra(nullptr)
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
    //Save account preference.
    knAccount->saveConfigure();
    //Save the configure, this must be done at last.
    knConf->saveConfigure();
}

inline void KNPluginManager::setApplicationInformation()
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
    //Check the plugin pointer first.
    if(plugin==nullptr)
    {
        return;
    }
    //Save the plugin pointer.
    m_musicPlugin=plugin;
    //Load the music categroy plugin first.
    loadCategoryPlugin(m_musicPlugin);
    //Check the main window pointer.
    if(m_mainWindow!=nullptr)
    {
        //Set the main player provided by the music plugin.
        m_mainWindow->setMusicPlugin(m_musicPlugin);
    }
    //Check the platform extra pointer.
    if(m_platformExtra!=nullptr)
    {
        //Set the platform extra to the music plugin.
        m_musicPlugin->setPlatformExtras(m_platformExtra);
    }
}

void KNPluginManager::loadCategoryPlugin(KNCategoryPlugin *plugin)
{
    //Check the plugin first.
    if(plugin==nullptr)
    {
        return;
    }
    //Load the plugins of the plugin.
    plugin->loadPlugins();
    //Check the header.
    if(m_header!=nullptr)
    {
        //Set the category plugin.
        m_header->setCategoryPlugin(plugin);
    }
    //Check the main window pointer.
    if(m_mainWindow!=nullptr)
    {
        //Set the category to be the main widget.
        m_mainWindow->setMainWidget(plugin);
    }
}

void KNPluginManager::loadPlatformExtras(KNPlatformExtras *plugin)
{
    //Check the plugin first.
    if(m_platformExtra!=nullptr)
    {
        return;
    }
    //Set the main window.
    m_platformExtra=plugin;
    //Check the platform pointer.
    if(m_platformExtra==nullptr)
    {
        //Ignore the null pointer.
        return;
    }
    //Set parent relationship.
    m_platformExtra->setParent(this);
    //Set the main window.
    m_platformExtra->setMainWindow(m_mainWindow);
    //Load preference.
    m_platformExtra->loadPreference();
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
#ifdef Q_OS_WIN
    loadPlatformExtras(new KNWindowsExtras);
#endif

    //Load the category plugin.
    loadMusicPlugin(new KNMusicPlugin);
}

void KNPluginManager::launchApplication()
{
    //Start the global manager to work.
    emit knGlobal->startWorking();
    //Check out the argument size.
    if(qApp->arguments().size() > 1)
    {
        //Process the arguments from system.
        onActionArgumentsAvaliable(qApp->arguments());
        //Start mini player.
        m_musicPlugin->showMiniPlayer();
        //Don't need to show the main window, it will start for mini player.
        return;
    }
    //Show the main window.
    m_mainWindow->show();
}

void KNPluginManager::onActionArgumentsAvaliable(QStringList arguments)
{
    //If there's only one item in arguments list, it means no arguments get
    //from system.
    if(arguments.size()<2 && (!m_musicPlugin))
    {
        return;
    }
    //Remove the first item in the arguments list.
    //That's the file path of the application execution.
    arguments.removeFirst();
    //Ask the music plugin to process the arguments.
    m_musicPlugin->onArgumentsAvailable(arguments);
}
