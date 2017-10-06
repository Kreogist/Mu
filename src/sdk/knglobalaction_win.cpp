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
#include <QWidget>

#include "Windows.h"

#include "knglobal.h"
#include "knglobalaction.h"

#include <QDebug>

//Get the all poosible modifiers.
const Qt::KeyboardModifiers allModifiers=
        Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier |
        Qt::MetaModifier;

QHash<uint, KNGlobalAction *> KNGlobalAction::m_registeredKeys=
        QHash<uint, KNGlobalAction *>();

int KNGlobalAction::m_reference=0;

quint32 getNativeModifiers(Qt::KeyboardModifiers modifiers)
{
    // MOD_ALT, MOD_CONTROL, (MOD_KEYUP), MOD_SHIFT, MOD_WIN
    quint32 native = 0;
    //Check for the Shift, Ctrl and Alt key.
    if (modifiers & Qt::ShiftModifier)
    {
        native |= MOD_SHIFT;
    }
    if (modifiers & Qt::ControlModifier)
    {
        native |= MOD_CONTROL;
    }
    if (modifiers & Qt::AltModifier)
    {
        native |= MOD_ALT;
    }
    //For the Windows logo key.
    if (modifiers & Qt::MetaModifier)
    {
        native |= MOD_WIN;
    }
    //Give back the native modifiers.
    return native;
}

quint32 getNativeKeys(Qt::Key key)
{
    //Detect for specific keys.
    switch (key)
    {
    case Qt::Key_Escape:
        return VK_ESCAPE;
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        return VK_TAB;
    case Qt::Key_Backspace:
        return VK_BACK;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return VK_RETURN;
    case Qt::Key_Insert:
        return VK_INSERT;
    case Qt::Key_Delete:
        return VK_DELETE;
    case Qt::Key_Pause:
        return VK_PAUSE;
    case Qt::Key_Print:
        return VK_PRINT;
    case Qt::Key_Clear:
        return VK_CLEAR;
    case Qt::Key_Home:
        return VK_HOME;
    case Qt::Key_End:
        return VK_END;
    case Qt::Key_Left:
        return VK_LEFT;
    case Qt::Key_Up:
        return VK_UP;
    case Qt::Key_Right:
        return VK_RIGHT;
    case Qt::Key_Down:
        return VK_DOWN;
    case Qt::Key_PageUp:
        return VK_PRIOR;
    case Qt::Key_PageDown:
        return VK_NEXT;
    case Qt::Key_F1:
        return VK_F1;
    case Qt::Key_F2:
        return VK_F2;
    case Qt::Key_F3:
        return VK_F3;
    case Qt::Key_F4:
        return VK_F4;
    case Qt::Key_F5:
        return VK_F5;
    case Qt::Key_F6:
        return VK_F6;
    case Qt::Key_F7:
        return VK_F7;
    case Qt::Key_F8:
        return VK_F8;
    case Qt::Key_F9:
        return VK_F9;
    case Qt::Key_F10:
        return VK_F10;
    case Qt::Key_F11:
        return VK_F11;
    case Qt::Key_F12:
        return VK_F12;
    case Qt::Key_F13:
        return VK_F13;
    case Qt::Key_F14:
        return VK_F14;
    case Qt::Key_F15:
        return VK_F15;
    case Qt::Key_F16:
        return VK_F16;
    case Qt::Key_F17:
        return VK_F17;
    case Qt::Key_F18:
        return VK_F18;
    case Qt::Key_F19:
        return VK_F19;
    case Qt::Key_F20:
        return VK_F20;
    case Qt::Key_F21:
        return VK_F21;
    case Qt::Key_F22:
        return VK_F22;
    case Qt::Key_F23:
        return VK_F23;
    case Qt::Key_F24:
        return VK_F24;
    case Qt::Key_Space:
        return VK_SPACE;
    case Qt::Key_Asterisk:
        return VK_MULTIPLY;
    case Qt::Key_Plus:
        return VK_ADD;
    case Qt::Key_Comma:
        return VK_SEPARATOR;
    case Qt::Key_Minus:
        return VK_SUBTRACT;
    case Qt::Key_Slash:
        return VK_DIVIDE;
    case Qt::Key_MediaTogglePlayPause:
        return VK_MEDIA_PLAY_PAUSE;
    case Qt::Key_MediaNext:
        return VK_MEDIA_NEXT_TRACK;
    case Qt::Key_MediaPrevious:
        return VK_MEDIA_PREV_TRACK;
    case Qt::Key_MediaStop:
        return VK_MEDIA_STOP;
    case Qt::Key_VolumeDown:
        return VK_VOLUME_DOWN;
    case Qt::Key_VolumeUp:
        return VK_VOLUME_UP;
    case Qt::Key_VolumeMute:
        return VK_VOLUME_MUTE;
    // Numbers
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
    // Letters
    case Qt::Key_A:
    case Qt::Key_B:
    case Qt::Key_C:
    case Qt::Key_D:
    case Qt::Key_E:
    case Qt::Key_F:
    case Qt::Key_G:
    case Qt::Key_H:
    case Qt::Key_I:
    case Qt::Key_J:
    case Qt::Key_K:
    case Qt::Key_L:
    case Qt::Key_M:
    case Qt::Key_N:
    case Qt::Key_O:
    case Qt::Key_P:
    case Qt::Key_Q:
    case Qt::Key_R:
    case Qt::Key_S:
    case Qt::Key_T:
    case Qt::Key_U:
    case Qt::Key_V:
    case Qt::Key_W:
    case Qt::Key_X:
    case Qt::Key_Y:
    case Qt::Key_Z:
        return key;
    //Ignore all the other keys.
    default:
        return 0;
    }
}

void getNative(int keySequence, quint32 &nativeModifiers, quint32 &nativeKeys)
{
    //Get the native modifiers.
    nativeModifiers=getNativeModifiers(
                (Qt::KeyboardModifiers)keySequence & allModifiers);
    //Get the native keys.
    nativeKeys=getNativeKeys(
                (Qt::Key)((keySequence ^ allModifiers) & keySequence));
}

uint getSequenceId(const quint32 &nativeModifiers, const quint32 &nativeKeys)
{
    return qHash(qMakePair(nativeModifiers, nativeKeys));
}

bool KNGlobalAction::nativeEventFilter(const QByteArray &eventType,
                                       void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    //Recast the message.
    MSG *nativeMessage=static_cast<MSG*>(message);
    //Check the message type.
    if(nativeMessage->message==WM_HOTKEY)
    {
        //Get the keycode and modifiers.
        const quint32 nativeMods=HIWORD(nativeMessage->lParam),
                      nativeKey=LOWORD(nativeMessage->lParam);
        //Get the global short cut from the pairs.
        KNGlobalAction *shortcut=m_registeredKeys.value(
                    getSequenceId(nativeKey, nativeMods), nullptr);
        //Check the pointer.
        if(shortcut && shortcut->isEnabled())
        {
            //Activate the shortcut.
            emit shortcut->activated();
        }
    }
    //Filter done.
    return false;
}

KNGlobalAction::~KNGlobalAction()
{
    //Unbinded the native keys.
    unregisterCurrent();
    //Decrease the reference.
    --m_reference;
    //Check the reference count.
    if(m_reference==0)
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
}

void KNGlobalAction::registerCurrent()
{
    //For all the key sequence in the list.
    for(auto i : m_sequenceIds.keys())
    {
        //Search in the registered keys.
        if(m_registeredKeys.find(i)==m_registeredKeys.end())
        {
            //Cannot find the key from the global binding, register this key
            //binding.
            QPair<quint32, quint32> keySequence=m_sequenceIds.value(i);
            //Use the Windows API to register the key sequence.
            RegisterHotKey(NULL, i, keySequence.first, keySequence.second);
            //Insert to registered keys.
            m_registeredKeys.insert(i, this);
        }
    }
}

void KNGlobalAction::unregisterCurrent()
{
    //Check whether the key sequence is enabled or not.
    for(auto i : m_sequenceIds.keys())
    {
        //Search in the registered keys.
        if(m_registeredKeys.find(i)==m_registeredKeys.end())
        {
            //Cannot find the key from the global binding, ignore.
            continue;
        }
        //Use the Windows API to remove the key sequence.
        UnregisterHotKey(NULL, i);
        //Remove the if from the list.
        m_registeredKeys.remove(i);
    }
}

void KNGlobalAction::setKeySequence(const QKeySequence &keySequence)
{
    //Reset the sequence id, modifiers and keys.
    unregisterCurrent();
    //Reset the sequence ids.
    m_sequenceIds=QHash<uint, QPair<quint32, quint32>>();
    //Parse the new key sequence.
    quint32 sequenceId, nativeModifiers, nativeKeys;
    for(int i=0; i<keySequence.count(); ++i)
    {
        //Calculate the native modifiers and native keys.
        getNative(keySequence[i], nativeModifiers, nativeKeys);
        //Calculate the Id.
        sequenceId=getSequenceId(nativeModifiers, nativeKeys);
        //Insert the id and native to the hash list.
        m_sequenceIds.insert(sequenceId,
                             qMakePair(nativeModifiers, nativeKeys));
    }
    //If the global action is enabled, register the key.
    if(m_enabled)
    {
        //Register the keys.
        registerCurrent();
    }
}

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
