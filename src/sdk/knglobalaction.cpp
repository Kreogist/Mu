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

#include "knglobalaction.h"

#include <QDebug>

KNGlobalAction::KNGlobalAction(QObject *parent) :
    QObject(parent),
    #ifdef Q_OS_WIN
    m_sequenceIds(QHash<uint, QPair<quint32, quint32>>()),
    #endif
    #ifdef Q_OS_MAC
    m_sequences(QSet<Identifier>()),
    #endif
    m_enabled(true)
{
#ifdef Q_OS_WIN
    //If this is the first global action.
    if(m_reference==0)
    {
        //Install the global shortcut to the event dispatcher.
        QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    }
    //Increase the reference.
    ++m_reference;
#endif
}

bool KNGlobalAction::isEnabled() const
{
    //Simply return the enabled state.
    return m_enabled;
}

void KNGlobalAction::setEnabled(bool enabled)
{
    //Check the enabled state.
    if(enabled==m_enabled)
    {
        //Already the same.
        return;
    }
    //Save the enabled state.
    m_enabled=enabled;
    //Update the state.
    updateEnableState();
}

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
void KNGlobalAction::updateEnableState()
{
    //Check the enable state.
    if(m_enabled)
    {
        //Register all the current state.
        registerCurrent();
    }
    else
    {
        //Unregister all the current state.
        unregisterCurrent();
    }
}
#endif
