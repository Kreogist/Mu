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
#ifndef KNPLUGINMANAGER_H
#define KNPLUGINMANAGER_H

#include <QObject>
#include <QStringList>
#include <QLinkedList>

class KNExpandMainWindow;
class KNGlobal;
class KNConfigureManager;
class KNMainWindowPlugin;
class KNMainWindowHeaderPlugin;
class KNMainWindowCategoryStackPlugin;
class KNMainWindowCategorySwitcherPlugin;
class KNPreferencePlugin;
class KNCategoryPlugin;
class KNPlatformExtras;
class KNPluginManager : public QObject
{
    Q_OBJECT
public:
    static KNPluginManager *instance();
    ~KNPluginManager();
    KNExpandMainWindow *mainWindow() const;
    void setMainWindow(KNExpandMainWindow *mainWindow);
    void loadPlugins();
    void processArguments();
    void start();

signals:
    void requireProcessArguments(QStringList arguments);

public slots:
    void onActionArgumentReceive(const QStringList &message);

private slots:
    void onActionMainWindowDestory();

private:
    inline void setApplicationInformation();
    inline void loadMainWindowPlugin(KNMainWindowPlugin *plugin);
    inline void loadMainWindowHeader(KNMainWindowHeaderPlugin *plugin);
    inline void loadMainWindowCategoryStack(KNMainWindowCategoryStackPlugin *plugin);
    inline void loadMainWindowCategorySwitcher(KNMainWindowCategorySwitcherPlugin *plugin);
    inline void loadPreference(KNPreferencePlugin *plugin);
    inline void loadPlatformExtras(KNPlatformExtras *plugin);
    inline void loadCategoryPlugin(KNCategoryPlugin *plugin);
    static KNPluginManager *m_instance;
    explicit KNPluginManager(QObject *parent = 0);
    inline void backupWindowGeometry();
    inline void recoverWindowGeometry();
    KNExpandMainWindow *m_mainWindow=nullptr;
    KNMainWindowPlugin *m_mainWindowPlugin=nullptr;
    KNPreferencePlugin *m_preferencePlugin=nullptr;
    KNPlatformExtras *m_platformExtra=nullptr;
    QLinkedList<QObject *> m_pluginList;
    KNGlobal *m_global;
    KNConfigureManager *m_configure;
};

#endif // KNPLUGINMANAGER_H
