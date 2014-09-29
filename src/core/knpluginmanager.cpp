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

#include "knpluginmanager.h"

KNPluginManager *KNPluginManager::m_instance=nullptr;

KNPluginManager *KNPluginManager::instance()
{
    return m_instance==nullptr?m_instance=new KNPluginManager:m_instance;
}

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent)
{
}

QMainWindow *KNPluginManager::mainWindow() const
{
    return m_mainWindow;
}

void KNPluginManager::setMainWindow(QMainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
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
