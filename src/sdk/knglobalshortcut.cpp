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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QAbstractEventDispatcher>

#include "knglobalshortcut.h"

#include <QDebug>

#ifndef Q_OS_MACX
int KNGlobalShortcut::reference=0;
#endif
QHash<QPair<quint32, quint32>, KNGlobalShortcut*> KNGlobalShortcut::shortcuts;

KNGlobalShortcut::KNGlobalShortcut(QObject *parent) :
    QObject(parent),
    m_count(0),
    m_enable(true)
{
    //Clear the keys and modifiers.
    for(int i=0; i<4; ++i)
    {
        m_key[i]=Qt::Key(0);
        m_modifiers[i]=Qt::NoModifier;
    }
#ifndef Q_OS_MAC
    //Check the current reference count.
    if(reference==0)
    {
        //Install the global shortcut to the event dispatcher.
        QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    }
    //Increase the reference count.
    ++reference;
#endif
}

KNGlobalShortcut::~KNGlobalShortcut()
{
#ifndef Q_OS_MACX
    //Remove the key bindings from the shortcuts.
    clearAll();
    //Decrease the reference.
    --reference;
    //Check the reference count.
    if(reference==0)
    {
        //Get the dispatcher.
        QAbstractEventDispatcher *eventDispatcher=
                QAbstractEventDispatcher::instance();
        //Check the result.
        if(!eventDispatcher)
        {
            //Remove the native event filter.
            eventDispatcher->removeNativeEventFilter(this);
        }
    }
#endif
}

QKeySequence KNGlobalShortcut::shortcut() const
{
    //Give back the key sequence data.
    return QKeySequence(m_key[0] | m_modifiers[0],
                        m_key[1] | m_modifiers[1],
                        m_key[2] | m_modifiers[2],
                        m_key[3] | m_modifiers[3]);
}

bool KNGlobalShortcut::setShortcut(const QKeySequence &shortcut)
{
    //Check the current count size.
    if(m_count>0 && !clearAll())
    {
        //Failed to clear the bindings.
        return false;
    }
    //Get the all poosible modifiers.
    Qt::KeyboardModifiers allModifiers=
            Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier |
            Qt::MetaModifier;
    //Save the count.
    m_count=shortcut.count();
    //Save the key information.
    for(int i=0; i<m_count; ++i)
    {
        //Save the key and modifiers.
        m_key[i]=Qt::Key((shortcut[i] ^ allModifiers) & shortcut[i]);
        m_modifiers[i]=Qt::KeyboardModifiers(shortcut[i] & allModifiers);
    }
    //Let all the other keys and modifiers to be 0.
    for(int i=m_count; i<4; ++i)
    {
        //Clear the key and modifiers.
        m_key[i]=Qt::Key(0);
        m_modifiers[i]=Qt::KeyboardModifiers(0);
    }
    //Mission complete.
    return m_enable ? bindAll() : true;
}

bool KNGlobalShortcut::isEnabled() const
{
    //Give back the result.
    return m_enable;
}

void KNGlobalShortcut::activateShortcut(quint32 nativeKey, quint32 nativeMods)
{
    //Get the global short cut from the pairs.
    KNGlobalShortcut *shortcut=shortcuts.value(qMakePair(nativeKey,
                                                         nativeMods),
                                               nullptr);
    //Check the pointer.
    if (shortcut && shortcut->isEnabled())
    {
        //Activate the shortcut.
        emit shortcut->activated();
    }
}

void KNGlobalShortcut::setEnabled(bool enabled)
{
    //Check the enabled state.
    if(enabled==m_enable)
    {
        //Already the same.
        return;
    }
    //Save the enabled state.
    m_enable=enabled;
    //Update the state.
    updateEnableState();
}

void KNGlobalShortcut::setDisabled(bool disabled)
{
    //Check the enabled state.
    if(!disabled==m_enable)
    {
        //Already the same.
        return;
    }
    //Simply reverse the setting.
    m_enable=!disabled;
    //Update the state.
    updateEnableState();
}

inline void KNGlobalShortcut::updateEnableState()
{
    //Check the enable state.
    if(m_enable)
    {
        //Bind all the keys.
        bindAll();
    }
    else
    {
        //Unbind all the keys.
        unbindAll();
    }
}

inline bool KNGlobalShortcut::bindAll()
{
    //Register all the sequence.
    for(int i=0; i<m_count; ++i)
    {
        //Get the native key and modifiers of the current sequence.
        const quint32 nativeKey = getNativeKeycode(m_key[i]);
        const quint32 nativeModifiers = getNativeModifiers(m_modifiers[i]);
        //Register the shortcut data.
        if(!registerShortcut(nativeKey, nativeModifiers))
        {
            qWarning("KNGlobalShortcut failed to register.");
            return false;
        }
        //Insert this object to the list.
        shortcuts.insert(qMakePair(nativeKey, nativeModifiers), this);
    }
    //Mission complete.
    return true;
}

bool KNGlobalShortcut::unbindAll()
{
    //Familiar with the clear all, but do not clear all the data.
    for(int i=0; i<m_count; ++i)
    {
        //Get the native key and modifiers.
        const quint32 nativeKey = getNativeKeycode(m_key[i]);
        const quint32 nativeModifiers = getNativeModifiers(m_modifiers[i]);
        //Unbind the shortcut.
        if(!unregisterShortcut(nativeKey, nativeModifiers))
        {
            //Failed to unbind the sequence.
            qWarning("KNGlobalShortcut failed to unregister.");
            return false;
        }
        //If the result success, remove the key pair from the list.
        shortcuts.remove(qMakePair(nativeKey, nativeModifiers));
    }
    //Complete.
    return true;
}

inline bool KNGlobalShortcut::clearAll()
{
    //Unbind all.
    if(m_enable && !unbindAll())
    {
        //Failed to unbind, cannot clear.
        return false;
    }
    //Which means that currently, there're key sequence bind to the action,
    //we need to remove these first.
    for(int i=0; i<m_count; ++i)
    {
        //Clear the key and modifers.
        m_key[i]=Qt::Key(0);
        m_modifiers[i]=Qt::NoModifier;
    }
    //Reset the counter.
    m_count=0;
    //Complete.
    return true;
}
