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
#include <QStyleFactory>
#include <QStandardPaths>

#include "knglobal.h"
#include "knversion.h"
#include "knconfiguremanager.h"

#include "knexpandmainwindow.h"

//Ports
#include "knmainwindowplugin.h"
#include "knmainwindowheaderplugin.h"
#include "knmainwindowcategorystackplugin.h"
#include "knmainwindowcategoryswitcherplugin.h"
#include "knpreferenceplugin.h"
#include "kncategoryplugin.h"
#include "knplatformextras.h"

//Basic Plugins
#include "plugin/base/knmainwindow/knmainwindow.h"
#include "plugin/base/knmainwindowheader/knmainwindowheader.h"
#include "plugin/base/knmainwindowcategoryswitcher/knmainwindowcategoryswitcher.h"
#include "plugin/base/knmainwindowcategorystack/knmainwindowcategorystack.h"
#include "plugin/base/knpreference/knpreference.h"

//Category Plugins
#include "plugin/module/knmusicplugin/knmusicplugin.h"

//Platform Plugins
#ifdef Q_OS_WIN32
#include "plugin/module/knwindowsextras/knwindowsextras.h"
#endif

#include "knpluginmanager.h"

#include <QDebug>

KNPluginManager *KNPluginManager::m_instance=nullptr;

KNPluginManager *KNPluginManager::instance()
{
    return m_instance==nullptr?m_instance=new KNPluginManager:m_instance;
}

KNPluginManager::~KNPluginManager()
{
    //Delete all the plugins.
    while(!m_pluginList.isEmpty())
    {
        //We only need to recover the object which don't have parent.
        //Why take last:
        //Because we build the plugin list from the first to the last, and the
        //later loaded plugin must based on the plugin loaded earlier. So delete
        //it later.
        QObject *currentObject=m_pluginList.takeLast();
        if(currentObject->parent()==nullptr)
        {
            delete currentObject;
        }
    }
    //Save the configure right now.
    m_global->saveConfigure();
    //Delete the global object.
    delete m_global;
}

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent)
{
    //---Initial infrastructure.---
    //Initial the application information.
    setApplicationInformation();
    //Initial global style.
    QApplication::setStyle(QStyleFactory::create("fusion"));
    //Initial global.
    m_global=KNGlobal::instance();
}

KNExpandMainWindow *KNPluginManager::mainWindow() const
{
    return m_mainWindow;
}

void KNPluginManager::setMainWindow(KNExpandMainWindow *mainWindow)
{
    //Ensure the main window is null.
    if(m_mainWindow==nullptr)
    {
        //Set the main window. It can't be changed.
        m_mainWindow=mainWindow;
        //Set the basic property.
        m_mainWindow->setWindowIcon(QIcon("://icon/mu.png"));
#ifdef Q_OS_MACX
        //Mac OS X title hack.
        m_mainWindow->setWindowTitle(QApplication::applicationDisplayName());
#endif
        //Set the configure.
        m_mainWindow->setCacheConfigure(m_global->cacheConfigure());
        //Set the basic palette.
        QPalette pal=m_mainWindow->palette();
        pal.setColor(QPalette::WindowText, QColor(255,255,255));
        m_mainWindow->setPalette(pal);
        //Connect destory signal.
        connect(m_mainWindow, &KNExpandMainWindow::windowAboutToClose,
                this, &KNPluginManager::onActionMainWindowDestory);
        //Recover the geometry.
        m_mainWindow->recoverGeometry();
    }
}

void KNPluginManager::loadPlugins()
{
    //Initial main window infrastructure.
    //And the order must be:
    //header -> category stack -> category switcher -> preference.
    loadMainWindowPlugin(new KNMainWindow);
    loadHeader(new KNMainWindowHeader);
    loadCategoryStack(new KNMainWindowCategoryStack);
    loadCategorySwitcher(new KNMainWindowCategorySwitcher);
    loadPreference(new KNPreference);

    //Load the platform extras if possible.
#ifdef Q_OS_WIN32
    loadPlatformExtras(new KNWindowsExtras);
#endif

    //Initial category modules.
    loadCategoryPlugin(new KNMusicPlugin);

}

void KNPluginManager::processArguments()
{
    //If there's only one item in arguments list, it means no arguments get
    //from system.
    QStringList args=QApplication::arguments();
    if(args.size()==1)
    {
        return;
    }
    //Ask to process the arguments.
    args.removeFirst();
    emit requireProcessArguments(args);
}

void KNPluginManager::start()
{
    m_mainWindow->show();
}

void KNPluginManager::onActionMainWindowDestory()
{
    ;
}

inline void KNPluginManager::setApplicationInformation()
{
    //Generate application name.
    QString applicationName=QChar(0x03BC);
    //Set application name and other info.
    QApplication::setApplicationName(applicationName);
    QApplication::setApplicationVersion(APP_VERSION_STR);
    QApplication::setApplicationDisplayName(applicationName);
    QApplication::setOrganizationName("Kreogist Dev Team");
    QApplication::setOrganizationDomain("http://kreogist.github.io/");
}

inline void KNPluginManager::loadMainWindowPlugin(KNMainWindowPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Save the plugin, and set the main window.
    m_mainWindowPlugin=plugin;
    m_mainWindowPlugin->setMainWindow(m_mainWindow);
}

inline void KNPluginManager::loadHeader(KNMainWindowHeaderPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the header to main window plugin.
    m_mainWindowPlugin->setHeader(plugin);
}

inline void KNPluginManager::loadCategoryStack(KNMainWindowCategoryStackPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setCategoryStack(plugin);
}

inline void KNPluginManager::loadCategorySwitcher(KNMainWindowCategorySwitcherPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setCategorySwitcher(plugin);
}

inline void KNPluginManager::loadPreference(KNPreferencePlugin *plugin)
{
    //Ensure the preference is null.
    if(m_preferencePlugin==nullptr)
    {
        //Save the preference pointer.
        m_preferencePlugin=plugin;
        //Add this to the plugin list.
        m_pluginList.append(plugin);
        //Set the plugin.
        m_mainWindowPlugin->setPreferencePanel(m_preferencePlugin);
        //Link the preference to global apply preference.
        connect(m_preferencePlugin, &KNPreferencePlugin::requireApplyPreference,
                m_global, &KNGlobal::requireApplyPreference);
    }
}

inline void KNPluginManager::loadCategoryPlugin(KNCategoryPlugin *plugin)
{
    //Add this to the plugin list.
    m_pluginList.append(plugin);
    //Set platform extra for this category.
    if(m_platformExtra!=nullptr)
    {
        plugin->setPlatformExtras(m_platformExtra);
    }
    //Connect arguments process.
    connect(this, &KNPluginManager::requireProcessArguments,
            plugin, &KNCategoryPlugin::onArgumentsAvailable);
    //Add this plugin to main window.
    m_mainWindowPlugin->addCategoryPlugin(plugin);
}

inline void KNPluginManager::loadPlatformExtras(KNPlatformExtras *plugin)
{
    //Add this to the plugin list.
    m_pluginList.append(plugin);
    //Save the platform extras.
    m_platformExtra=plugin;
    //Set main window.
     m_platformExtra->setMainWindow(m_mainWindow);
}
