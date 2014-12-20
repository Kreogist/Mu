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
#include <QDesktopWidget>

#include "knglobal.h"
#include "knconfigure.h"

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
//#include "plugin/module/knwindowsextras/knwindowsextras.h"
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
        delete m_pluginList.takeFirst();
    }
    //Save the configure.
    m_global->saveConfigure();
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
    //Set dymanic link library suffix.
#ifdef Q_OS_WIN32
    KNGlobal::setDylibSuffix("dll");
#endif
#ifdef Q_OS_MACX
    KNGlobal::setDylibSuffix("dylib");
#endif
#ifdef Q_OS_LINUX
    KNGlobal::setDylibSuffix("so");
#endif
    //Load the configure.
    m_global->loadConfigure();
}

inline void KNPluginManager::backupWindowGeometry()
{
    //Backup the window state.
    m_global->setSystemData("windowState", (int)m_mainWindow->windowState());
    //Window position.
    m_global->setSystemData("windowX", m_mainWindow->geometry().x());
    m_global->setSystemData("windowY", m_mainWindow->geometry().y());
    m_global->setSystemData("windowWidth", m_mainWindow->geometry().width());
    m_global->setSystemData("windowHeight", m_mainWindow->geometry().height());
    //Desktop size.
    m_global->setSystemData("desktopWidth", QApplication::desktop()->width());
    m_global->setSystemData("desktopHeight", QApplication::desktop()->height());
}

inline void KNPluginManager::recoverWindowGeometry()
{
    //Check is the value avaliable.
    //!FIXME: We need a better way to check is the last record is complete.
    if(m_global->systemData("windowWidth").isNull())
    {
        return;
    }
    //Recover the window state.
    int windowState=m_global->systemData("windowState").toInt();
    switch(windowState)
    {
    case Qt::WindowMaximized:
        m_mainWindow->setWindowState(Qt::WindowMaximized);
        return;
    case Qt::WindowFullScreen:
        m_mainWindow->setWindowState(Qt::WindowFullScreen);
        return;
    default:
        m_mainWindow->setWindowState(Qt::WindowNoState);
    }
    //Check is the resolution is the same as the last time.
    int lastResolutionWidth=m_global->systemData("desktopWidth").toInt(),
        lastResolutionHeight=m_global->systemData("desktopHeight").toInt(),
        lastX=m_global->systemData("windowX").toInt(),
        lastY=m_global->systemData("windowY").toInt(),
        lastWidth=m_global->systemData("windowWidth").toInt(),
        lastHeight=m_global->systemData("windowHeight").toInt();
    if(QApplication::desktop()->width()==lastResolutionWidth &&
            QApplication::desktop()->height()==lastResolutionHeight)
    {
        //In the same resolution.
        m_mainWindow->setGeometry(lastX, lastY, lastWidth, lastHeight);
    }
    else
    {
        //Calculate the new geometry in the current resolution.
        m_mainWindow->setGeometry((double)lastX/(double)lastResolutionWidth*(double)QApplication::desktop()->width(),
                                  (double)lastY/(double)lastResolutionHeight*(double)QApplication::desktop()->height(),
                                  (double)lastWidth/(double)lastResolutionWidth*(double)QApplication::desktop()->width(),
                                  (double)lastHeight/(double)lastResolutionHeight*(double)QApplication::desktop()->height());
    }
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
#ifdef Q_OS_MACX
        m_mainWindow->setWindowTitle(QApplication::applicationDisplayName());
#endif
        m_mainWindow->setMinimumSize(730, 432);
        //Set the basic palette.
        QPalette pal=m_mainWindow->palette();
        pal.setColor(QPalette::WindowText, QColor(255,255,255));
        m_mainWindow->setPalette(pal);
        //Connect destory signal.
        connect(m_mainWindow, &KNExpandMainWindow::windowAboutToClose,
                this, &KNPluginManager::onActionMainWindowDestory);
        //Recover the geometry.
        recoverWindowGeometry();
    }
}

void KNPluginManager::loadPlugins()
{
    //Initial main window infrastructure.
    loadMainWindowPlugin(new KNMainWindow);
    loadMainWindowHeader(new KNMainWindowHeader);
    loadMainWindowCategoryStack(new KNMainWindowCategoryStack);
    loadMainWindowCategorySwitcher(new KNMainWindowCategorySwitcher);
    loadPreference(new KNPreference);

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

void KNPluginManager::onActionArgumentReceive(const QStringList &message)
{
    emit requireProcessArguments(message);
}

void KNPluginManager::onActionMainWindowDestory()
{
    //Backup geometry.
    backupWindowGeometry();
}

inline void KNPluginManager::setApplicationInformation()
{
    //Generate application name.
    QString applicationName=QChar(0x03BC);
    //Set application name and other info.
    QApplication::setApplicationName(applicationName);
    QApplication::setApplicationVersion("0.3");
    QApplication::setApplicationDisplayName(applicationName);
    QApplication::setOrganizationName("Kreogist Dev Team");
    QApplication::setOrganizationDomain("http://kreogist.github.io/");
}

inline void KNPluginManager::loadMainWindowPlugin(KNMainWindowPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Save the plugin.
    m_mainWindowPlugin=plugin;
    m_mainWindowPlugin->setMainWindow(m_mainWindow);
}

inline void KNPluginManager::loadMainWindowHeader(KNMainWindowHeaderPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setHeader(plugin);
}

inline void KNPluginManager::loadMainWindowCategoryStack(KNMainWindowCategoryStackPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setCategoryStack(plugin);
}

inline void KNPluginManager::loadMainWindowCategorySwitcher(KNMainWindowCategorySwitcherPlugin *plugin)
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
