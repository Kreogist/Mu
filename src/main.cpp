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
#include "knmainwindow.h" //Main window.
#include "knsingletonapplication.h" //Singleton application
#include "knversion.h"  // For public settings.

#include "core/knpluginmanager.h" //Plugin Manager.

int main(int argc, char *argv[])
{
#if QT_VERSION_MINOR >= 6
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
    //Create a singleton pattern application.
    KNSingletonApplication app(argc, argv, APP_HANDLER);
    //Check the running status of the instance.
    if(!app.isInstanceRunning())
    {
        //Current instance is not running, means it finds another running
        //instance. Exit the current instance.
        return EXIT_SUCCESS;
    }
    //Initial the plugin manager.
    KNPluginManager pluginManager;
    //Initial the main window.
    KNMainWindow mainWindow;
    //Set the main window to plugin manager.
    pluginManager.setMainWindow(&mainWindow);
    //Load plugins.
    pluginManager.loadPlugins();
    //Link the application instance to the plugin manager.
    QObject::connect(&app, &KNSingletonApplication::messageAvailable,
                     &pluginManager,
                     &KNPluginManager::onActionArgumentsAvaliable,
                     Qt::QueuedConnection);
    QObject::connect(&app, &KNSingletonApplication::applicationQuit,
                     &pluginManager, &KNPluginManager::saveConfigure);
    //Launch the application.
    pluginManager.launchApplication();
    //Spread message loop.
    return app.exec();
}
