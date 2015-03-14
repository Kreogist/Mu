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
#include "core/knsingleapplication.h"
#include "core/knpluginmanager.h"
#include "knexpandmainwindow.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    //Create a singleton pattern application.
    KNSingleApplication app(argc, argv, "org.kreogist.mu");
    //Check is there an another pattern,
    if(app.isPatternRunning())
    {
        //If so, send all the arguments to that pattern.
        QStringList args=KNSingleApplication::arguments();
        //Ignore the first pattern, that's the application path.
        args.removeFirst();
        app.sendMessage(args);
        //Exit.
        return EXIT_SUCCESS;
    }
    //Create the main window.
    KNExpandMainWindow mainWindow;
    //Initial the plugin manager.
    KNPluginManager *pluginManager=KNPluginManager::instance();
    //Connect message process slot.
    QObject::connect(&app, &KNSingleApplication::messageAvailable,
                     pluginManager, &KNPluginManager::requireProcessArguments);
    pluginManager->setMainWindow(&mainWindow);
    //Load plugins.
    pluginManager->loadPlugins();
    //Start application.
    pluginManager->start();
    //Process the arguments.
    pluginManager->processArguments();
    //Spread message loop.
    int result=app.exec();
    //Remove the plugin manager and recover the memory.
    delete pluginManager;
    return result;
}
