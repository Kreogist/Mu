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
#include <QAction>

#include "knconfigure.h"

#include "knshortcutmanager.h"

KNShortcutManager *KNShortcutManager::m_instance=nullptr;

KNShortcutManager *KNShortcutManager::instance()
{
    return m_instance;
}

void KNShortcutManager::initial(QObject *parent)
{
    //Set the current instance as the singleton instance.
    if(m_instance==nullptr)
    {
        m_instance=new KNShortcutManager(parent);
    }
}

void KNShortcutManager::append(const QString &identifier, QAction *action)
{
    //Save the action to the action map.
    m_actionMapper.insert(qHash(identifier), action);
    //Find the identifier in the shortcut configure.
    QVariant shortcut=m_shortcutConfigure->data(identifier);
    if(shortcut.isNull())
    {
        //No user configure shortcut from the identifier.
        return;
    }
    //Set the shortcut to the action.
    action->setShortcut(shortcut.value<QKeySequence>());
}

KNShortcutManager::KNShortcutManager(QObject *parent) :
    QObject(parent),
    m_shortcutConfigure(nullptr)
{
}

void KNShortcutManager::setShortcutConfigure(KNConfigure *shortcutConfigure)
{
    //Check the configure pointer first.
    if(m_shortcutConfigure==nullptr)
    {
        //Igonre the incorrect request.
        return;
    }
    //Save the configure data.
    m_shortcutConfigure = shortcutConfigure;
    //Link the configure.
    connect(m_shortcutConfigure, &KNConfigure::valueChanged,
            this, &KNShortcutManager::onConfigureChanged);
}

void KNShortcutManager::onConfigureChanged()
{
    //Check the shortcut configure object.
    ;
}
