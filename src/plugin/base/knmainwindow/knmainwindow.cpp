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

#include "knglobal.h"
#include "knmainwindowcontainer.h"
#include "knmainwindowheaderplugin.h"

#include "knmainwindow.h"

KNMainWindow::KNMainWindow(QObject *parent) :
    KNMainWindowPlugin(parent)
{
    m_container=new KNMainWindowContainer;
}

void KNMainWindow::setMainWindow(QMainWindow *mainWindow)
{
    m_mainWindow=mainWindow;
    //Set properties.
    m_mainWindow->setAutoFillBackground(true);
    m_mainWindow->setContentsMargins(0,0,0,0);
    //Set palette.
    QPalette pal=m_mainWindow->palette();
    pal.setColor(QPalette::Window, QColor(0,0,0));
    m_mainWindow->setPalette(pal);
    //Change parent relationship.
    m_container->setParent(m_mainWindow);
    //Set the container to the central widget.
    m_mainWindow->setCentralWidget(m_container);
}

void KNMainWindow::setHeader(KNMainWindowHeaderPlugin *plugin)
{
    m_headerPlugin=plugin;
    m_container->setHeaderWidget(m_headerPlugin->headerWidget());
}

void KNMainWindow::setCategoryStack(KNMainWindowCategoryStackPlugin *plugin)
{
    m_categoryStackPlugin=plugin;
    m_container->setCategoryStack(m_categoryStackPlugin->stackedWidget());
}

void KNMainWindow::setCategorySwitcher(KNMainWindowCategorySwitcherPlugin *plugin)
{
    m_categorySwitcherPlugin=plugin;
    m_container->setCategorySwitcher(m_categorySwitcherPlugin->switcherWidget());
}

void KNMainWindow::setPreferencePanel(KNPreferencePlugin *plugin)
{
    m_preferencePlugin=plugin;
    m_container->setPreferencePanel(m_preferencePlugin->preferencePanel());
}

void KNMainWindow::showPreference()
{
    m_container->showPreference();
}

void KNMainWindow::hidePreference()
{
    m_container->hidePreference();
}
