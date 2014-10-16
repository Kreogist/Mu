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
#include <QLinkedList>

class QMainWindow;
class KNGlobal;
class KNConfigure;
class KNMainWindowPlugin;
class KNMainWindowHeaderPlugin;
class KNMainWindowCategoryStackPlugin;
class KNMainWindowCategorySwitcherPlugin;
class KNPreferencePlugin;
class KNCategoryPlugin;
class KNPluginManager : public QObject
{
    Q_OBJECT
public:
    static KNPluginManager *instance();
    ~KNPluginManager();
    QMainWindow *mainWindow() const;
    void setMainWindow(QMainWindow *mainWindow);
    void loadPlugins();
    void start();

signals:

public slots:
    void onActionArgumentReceive(const QString &message);

private slots:
    void onActionMainWindowDestory();

private:
    void loadMainWindowPlugin(KNMainWindowPlugin *plugin);
    void loadMainWindowHeader(KNMainWindowHeaderPlugin *plugin);
    void loadMainWindowCategoryStack(KNMainWindowCategoryStackPlugin *plugin);
    void loadMainWindowCategorySwitcher(KNMainWindowCategorySwitcherPlugin *plugin);
    void loadPreference(KNPreferencePlugin *plugin);
    void loadCategoryPlugin(KNCategoryPlugin *plugin);
    static KNPluginManager *m_instance;
    explicit KNPluginManager(QObject *parent = 0);
    void backupWindowGeometry();
    void recoverWindowGeometry();
    QMainWindow *m_mainWindow=nullptr;
    KNMainWindowPlugin *m_mainWindowPlugin=nullptr;
    KNPreferencePlugin *m_preferencePlugin=nullptr;
    QLinkedList<QObject *> m_pluginList;
    KNGlobal *m_global;
    KNConfigure *m_configure;
};

#endif // KNPLUGINMANAGER_H
