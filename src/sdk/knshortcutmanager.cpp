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
#include "knglobalshortcut.h"

#include "knshortcutmanager.h"

#include <QDebug>

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
    //Set the identifier as the action object name.
    action->setObjectName(identifier);
    //Prepare the shortcut action structure.
    ShortcutAction currentAction;
    //Save the action pointer.
    currentAction.action=action;
    //Create a global action instance.
    KNGlobalShortcut *globalShortcut=new KNGlobalShortcut(this);
    //Update the enable state.
    globalShortcut->setEnabled(
                m_shortcutConfigure->data("Global", false).toBool());
    //Link the global shortcut actions.
    connect(globalShortcut, &KNGlobalShortcut::activated,
            action, &QAction::trigger);
    //Insert the global shortcut to the list.
    m_globalActionMap.insert(identifier, globalShortcut);
    //Insert the current action.
    insertAction(currentAction);
}

KNShortcutManager::KNShortcutManager(QObject *parent) :
    QObject(parent),
    m_shortcutConfigure(nullptr)
{
}

void KNShortcutManager::setShortcutConfigure(KNConfigure *shortcutConfigure)
{
    //Check the configure pointer first.
    if(m_shortcutConfigure)
    {
        //Igonre the incorrect request.
        return;
    }
    //Save the configure data.
    m_shortcutConfigure = shortcutConfigure;
    //Link the configure.
    connect(m_shortcutConfigure, &KNConfigure::valueChanged,
            this, &KNShortcutManager::updateFromConfigure);
    updateFromConfigure();
}

QList<int> KNShortcutManager::conflictShortcut(const QString &actionName)
{
    return m_actionMapper.value(actionName).conflictKeys;
}

void KNShortcutManager::updateFromConfigure()
{
    //Pick all the shortcut actions out.
    QList<ShortcutAction> actionList=m_actionMapper.values();
    //Ignore the request when action list is empty.
    if(actionList.isEmpty())
    {
        //Nothing need to update.
        return;
    }
    //Clear all the mapper and binding list.
    m_keyBindings.clear();
    m_actionMapper.clear();
    //Loop and set all the new actions.
    while(!actionList.isEmpty())
    {
        //Insert the current action.
        insertAction(actionList.takeFirst());
    }
    //When the configure is updated, the shortcut info updated signal will be
    //emitted.
    emit shortcutUpdate();
}

inline void KNShortcutManager::insertAction(ShortcutAction currentAction)
{
    //Get the action.
    QAction *action=currentAction.action;
    //Clear the conflict keys.
    currentAction.conflictKeys=QList<int>();
    //Find the identifier in the shortcut configure.
    QVariant shortcut=m_shortcutConfigure->data(action->objectName());
    if(!shortcut.isNull())
    {
        //Get the key sequence.
        QKeySequence sequence=shortcut.value<QKeySequence>();
        //Initial te global sequence.
        int globalSequence[4], globalSequenceSize=0;
        globalSequence[0]=Qt::Key(0) | Qt::NoModifier;
        globalSequence[1]=globalSequence[0];
        globalSequence[2]=globalSequence[0];
        globalSequence[3]=globalSequence[0];
        //Prepare the sequence list.
        QList<QKeySequence> sequenceList;
        //This is the cheat part. Change the one sequence into four sperate
        //sequence.
        for(int i=0; i<sequence.count(); ++i)
        {
            //Check the list has the key or not.
            if(m_keyBindings.contains(sequence[i]))
            {
                //Add the sequence to the conflict action.
                currentAction.conflictKeys.append(sequence[i]);
                //Pick out the original action structure.
                const QString &conflictIdentifier=
                        m_keyBindings.value(sequence[i])->objectName();
                //Get the shortcut structure.
                ShortcutAction conflictAction=
                        m_actionMapper.value(conflictIdentifier);
                conflictAction.conflictKeys.append(sequence[i]);
                //Update the mapper.
                m_actionMapper.insert(conflictIdentifier,
                                      conflictAction);
            }
            else
            {
                //No conflict happens, add to the mapper.
                m_keyBindings.insert(sequence[i], action);
                //Set the global sequence.
                globalSequence[globalSequenceSize++]=sequence[i];
            }
            //Add the key sequence to the list.
            sequenceList.append(QKeySequence(sequence[i]));
        }
        //Set the shortcut to the action.
        action->setShortcuts(sequenceList);
        //Construct the global sequence.
        QKeySequence globalActionShortcut(globalSequence[0], globalSequence[1],
                                          globalSequence[2], globalSequence[3]);
        //Set to the global action.
        KNGlobalShortcut *globalShortcut=
                m_globalActionMap.value(action->objectName());
        //Set the shortcut.
        qDebug()<<"Set global shortcut.";
        globalShortcut->setShortcut(globalActionShortcut);
        globalShortcut->setEnabled(
                    m_shortcutConfigure->data("Global", false).toBool());
    }
    //Save the action to the action map.
    m_actionMapper.insert(action->objectName(), currentAction);
}
