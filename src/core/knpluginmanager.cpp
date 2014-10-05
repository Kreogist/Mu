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
#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>

#include "knglobal.h"
#include "knconfigure.h"

//Ports
#include "knmainwindowplugin.h"
#include "knmainwindowheaderplugin.h"
#include "knmainwindowcategorystackplugin.h"
#include "knmainwindowcategoryswitcherplugin.h"
#include "knpreferenceplugin.h"
#include "kncategoryplugin.h"

//Basic Plugins
#include "plugin/base/knmainwindow/knmainwindow.h"
#include "plugin/base/knmainwindowheader/knmainwindowheader.h"
#include "plugin/base/knmainwindowcategoryswitcher/knmainwindowcategoryswitcher.h"
#include "plugin/base/knmainwindowcategorystack/knmainwindowcategorystack.h"
#include "plugin/base/knpreference/knpreference.h"

//Category Plugins
#include "plugin/module/knmusicplugin/knmusicplugin.h"

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
}

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent)
{
    //---Initial infrastructure.---
    //Initial global.
    m_global=KNGlobal::instance();
    //Set dymanic link library suffix.
#ifdef Q_OS_WIN32
    KNGlobal::setDylibSuffix("dll");
#endif
#ifdef Q_OS_MACX
    KNGlobal::setDylibSuffix("dylib");
#endif
    //Load the configure.
    m_global->loadConfigure();
}

void KNPluginManager::backupWindowGeometry()
{
    //Backup the window state.
    m_global->setSystemData("windowState", m_mainWindow->windowState());
    //Window position.
    m_global->setSystemData("windowX", m_mainWindow->geometry().x());
    m_global->setSystemData("windowY", m_mainWindow->geometry().y());
    m_global->setSystemData("windowWidth", m_mainWindow->geometry().width());
    m_global->setSystemData("windowHeight", m_mainWindow->geometry().height());
    //Desktop size.
    m_global->setSystemData("desktopWidth", QApplication::desktop()->width());
    m_global->setSystemData("desktopHeight", QApplication::desktop()->height());
}

void KNPluginManager::recoverWindowGeometry()
{
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

QMainWindow *KNPluginManager::mainWindow() const
{
    return m_mainWindow;
}

void KNPluginManager::setMainWindow(QMainWindow *mainWindow)
{
    //Ensure the main window is null.
    if(m_mainWindow==nullptr)
    {
        //Set the main window. It can't be changed.
        m_mainWindow=mainWindow;
        //Set the basic property.
        m_mainWindow->setMinimumSize(730, 432);
        //Connect destory signal.
        connect(m_mainWindow, SIGNAL(destroyed()),
                this, SLOT(onActionMainWindowDestory()));
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

void KNPluginManager::start()
{
    m_mainWindow->show();
}

void KNPluginManager::onActionArgumentReceive(const QString &message)
{
    ;
}

void KNPluginManager::onActionMainWindowDestory()
{
    //Backup geometry.
    backupWindowGeometry();
    //Save configure.
    m_global->saveConfigure();
}

void KNPluginManager::loadMainWindowPlugin(KNMainWindowPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Save the plugin.
    m_mainWindowPlugin=plugin;
    m_mainWindowPlugin->setMainWindow(m_mainWindow);
}

void KNPluginManager::loadMainWindowHeader(KNMainWindowHeaderPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setHeader(plugin);
}

void KNPluginManager::loadMainWindowCategoryStack(KNMainWindowCategoryStackPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setCategoryStack(plugin);
}

void KNPluginManager::loadMainWindowCategorySwitcher(KNMainWindowCategorySwitcherPlugin *plugin)
{
    //Add this plugin to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setCategorySwitcher(plugin);
}

void KNPluginManager::loadPreference(KNPreferencePlugin *plugin)
{
    //Add this to the plugin list.
    m_pluginList.append(plugin);
    //Set the plugin.
    m_mainWindowPlugin->setPreferencePanel(plugin);
}

void KNPluginManager::loadCategoryPlugin(KNCategoryPlugin *plugin)
{
    //Add this to the plugin list.
    m_pluginList.append(plugin);
    //Add the plugin data to the main window.
    /*! FIXME: Here we need to add the icon to the category list.
     *         But now we just need one category, so we don't need to do it now.
     *         Fix this ASAP.
     */
    m_mainWindowPlugin->setHeaderIcon(plugin->icon());
    m_mainWindowPlugin->setHeaderText(plugin->caption());
    //Add header widget and central widget.
    m_mainWindowPlugin->addHeaderWidget(plugin->headerWidget());
    m_mainWindowPlugin->addCentralWidget(plugin->centralWidget());
}
