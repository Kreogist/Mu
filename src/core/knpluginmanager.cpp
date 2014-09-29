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

#include "knpluginmanager.h"

#include <QDebug>

KNPluginManager *KNPluginManager::m_instance=nullptr;

KNPluginManager *KNPluginManager::instance()
{
    return m_instance==nullptr?m_instance=new KNPluginManager:m_instance;
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
    ;
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
