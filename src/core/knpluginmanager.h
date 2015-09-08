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
#ifndef KNPLUGINMANAGER_H
#define KNPLUGINMANAGER_H

#include <QObject>

class KNGlobal;
class KNMainWindow;
class KNMainWindowHeaderBase;
class KNAbstractMusicPlugin;
class KNPreferencePlugin;
class KNCategoryPlugin;
/*!
 * \brief The KNPluginManager class is a port-class manager. It will create and
 * load all the plugin class with the specific port class. You should tweak the
 * plugin class name here.
 */
class KNPluginManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs the KNPluginManager with parent object parent.
     * \param parent The owner of KNPluginManager. Normally, this should be
     * empty. You should construct KNPluginManager in main() function.
     */
    explicit KNPluginManager(QObject *parent = 0);
    ~KNPluginManager();

    /*!
     * \brief Get the application main window.
     * \return The main window class.
     */
    KNMainWindow *mainWindow() const;

signals:

public slots:
    /*!
     * \brief Set the application main window class.\n
     * Only the first main window object will be set. When you called this
     * function more than once, it will ignore those request except the first
     * time.
     * \param mainWindow The main window class.
     */
    void setMainWindow(KNMainWindow *mainWindow);

    /*!
     * \brief Load all the plugins.
     */
    void loadPlugins();

    /*!
     * \brief Show the main window, start to launch the application.
     */
    void launchApplication();

private:
    inline void setApplicationInformation();
    void loadHeader(KNMainWindowHeaderBase *header);
    void loadPreference(KNPreferencePlugin *plugin);
    void loadMusicPlugin(KNAbstractMusicPlugin *plugin);
    void loadCategoryPlugin(KNCategoryPlugin *plugin);
    KNMainWindow *m_mainWindow;
    KNMainWindowHeaderBase *m_header;
};

#endif // KNPLUGINMANAGER_H
