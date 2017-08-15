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

QHash<QPair<quint32, quint32>, KNGlobalShortcut*> KNGlobalShortcut::shortcuts;

KNGlobalShortcut::KNGlobalShortcut(QObject *parent) :
    QObject(parent),
    m_modifiers(Qt::NoModifier),
    m_key(Qt::Key(0)),
    m_enable(true)
{
}

KNGlobalShortcut::~KNGlobalShortcut()
{
}

QKeySequence KNGlobalShortcut::shortcut() const
{
    return QKeySequence(m_key | m_modifiers);
}

bool KNGlobalShortcut::setShortcut(const QKeySequence &shortcut)
{
    Qt::KeyboardModifiers allMods=Qt::ShiftModifier | Qt::ControlModifier |
                                  Qt::AltModifier | Qt::MetaModifier;
    //Save the key information.
    m_key=shortcut.isEmpty() ? (Qt::Key(0)) :
                               (Qt::Key((shortcut[0] ^ allMods) & shortcut[0]));
    m_modifiers=shortcut.isEmpty() ?
                Qt::KeyboardModifiers(0) :
                Qt::KeyboardModifiers(shortcut[0] & allMods);
    //Get the native key and modifiers.
    const quint32 nativeKey = getNativeKeycode(m_key);
    const quint32 nativeModifiers = getNativeModifiers(m_modifiers);
    //Register the shortcut data.
    const bool result = registerShortcut(nativeKey, nativeModifiers);
    if (result)
    {
        //Insert this object to the list.
        shortcuts.insert(qMakePair(nativeKey, nativeModifiers), this);
    }
    else
    {
        qWarning("KNGlobalShortcut failed to register.");
    }
    return result;
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
    //Save the enabled state.
    m_enable=enabled;
}

void KNGlobalShortcut::setDisabled(bool disabled)
{
    //Simply reverse the setting.
    m_enable=!disabled;
}
