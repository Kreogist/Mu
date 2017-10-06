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
#include <Carbon/Carbon.h>

#include "knglobalaction.h"

#include <QDebug>

//Get the all poosible modifiers.
const Qt::KeyboardModifiers allModifiers=
        Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier |
        Qt::MetaModifier;

static QMap<quint32, QPair<EventHotKeyRef, KNGlobalAction *>> keyRefs;
static QHash<Identifier, quint32> keyIds;
static quint32 hotKeySerial=0;
static bool macHandlerInstalled=false;

quint32 getNativeModifiers(Qt::KeyboardModifiers modifiers)
{
    //Prepare the native key modifiers.
    quint32 nativeVersion = 0;
    //For Shift, Ctrl, Alt and the Meta key(Win logo key)
    if(modifiers & Qt::ShiftModifier)
    {
        nativeVersion |= shiftKey;
    }
    if(modifiers & Qt::ControlModifier)
    {
        nativeVersion |= cmdKey;
    }
    if(modifiers & Qt::AltModifier)
    {
        nativeVersion |= optionKey;
    }
    if(modifiers & Qt::MetaModifier)
    {
        nativeVersion |= controlKey;
    }
    //Num lock key. (Wow!)
    if(modifiers & Qt::KeypadModifier)
    {
        nativeVersion |= kEventKeyModifierNumLockMask;
    }
    //Return the native data.
    return nativeVersion;
}

quint32 getNativeKeys(Qt::Key key)
{
    // Constants found in NSEvent.h from AppKit.framework
    switch (key)
    {
    case Qt::Key_Return:
        return kVK_Return;
    case Qt::Key_Enter:
        return kVK_ANSI_KeypadEnter;
    case Qt::Key_Tab:
        return kVK_Tab;
    case Qt::Key_Space:
        return kVK_Space;
    case Qt::Key_Backspace:
        return kVK_Delete;
    case Qt::Key_Control:
        return kVK_Command;
    case Qt::Key_Shift:
        return kVK_Shift;
    case Qt::Key_CapsLock:
        return kVK_CapsLock;
    case Qt::Key_Option:
        return kVK_Option;
    case Qt::Key_Meta:
        return kVK_Control;
    case Qt::Key_F17:
        return kVK_F17;
    case Qt::Key_VolumeUp:
        return kVK_VolumeUp;
    case Qt::Key_VolumeDown:
        return kVK_VolumeDown;
    case Qt::Key_F18:
        return kVK_F18;
    case Qt::Key_F19:
        return kVK_F19;
    case Qt::Key_F20:
        return kVK_F20;
    case Qt::Key_F5:
        return kVK_F5;
    case Qt::Key_F6:
        return kVK_F6;
    case Qt::Key_F7:
        return kVK_F7;
    case Qt::Key_F3:
        return kVK_F3;
    case Qt::Key_F8:
        return kVK_F8;
    case Qt::Key_F9:
        return kVK_F9;
    case Qt::Key_F11:
        return kVK_F11;
    case Qt::Key_F13:
        return kVK_F13;
    case Qt::Key_F16:
        return kVK_F16;
    case Qt::Key_F14:
        return kVK_F14;
    case Qt::Key_F10:
        return kVK_F10;
    case Qt::Key_F12:
        return kVK_F12;
    case Qt::Key_F15:
        return kVK_F15;
    case Qt::Key_Help:
        return kVK_Help;
    case Qt::Key_Home:
        return kVK_Home;
    case Qt::Key_PageUp:
        return kVK_PageUp;
    case Qt::Key_Delete:
        return kVK_ForwardDelete;
    case Qt::Key_F4:
        return kVK_F4;
    case Qt::Key_End:
        return kVK_End;
    case Qt::Key_F2:
        return kVK_F2;
    case Qt::Key_PageDown:
        return kVK_PageDown;
    case Qt::Key_F1:
        return kVK_F1;
    case Qt::Key_Left:
        return kVK_LeftArrow;
    case Qt::Key_Right:
        return kVK_RightArrow;
    case Qt::Key_Down:
        return kVK_DownArrow;
    case Qt::Key_Up:
        return kVK_UpArrow;
    default:
        break;
    }
    //Check the key id.
    UTF16Char keyChar;
    //The key ASCII code would be the key char for common invisible keys.
    if(key==Qt::Key_Escape)
    {
        keyChar = 27;
    }
    else if(key==Qt::Key_Return)
    {
        keyChar = 13;
    }
    else if(key==Qt::Key_Enter)
    {
        keyChar = 3;
    }
    else if(key==Qt::Key_Tab)
    {
        keyChar = 9;
    }
    else
    {
        //For others, simply assign as the origina key id from Qt.
        keyChar = key;
    }
    //Get the keyboard layout information.
    CFDataRef currentLayoutData;
    TISInputSourceRef currentKeyboard = TISCopyCurrentKeyboardInputSource();
    //Check the pointer.
    if(currentKeyboard==NULL)
    {
        //Ignore when no keyboard layout is returned.
        return 0;
    }
    //Fetch the layout data.
    currentLayoutData = (CFDataRef)TISGetInputSourceProperty(
                currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    CFRelease(currentKeyboard);
    //Check the layout data.
    if(currentLayoutData==NULL)
    {
        return 0;
    }
    //Check the header data of the layout.
    UCKeyboardLayout *header=(UCKeyboardLayout*)CFDataGetBytePtr(
                currentLayoutData);
    UCKeyboardTypeHeader *table=header->keyboardTypeList;
    //Get the data.
    uint8_t *data=(uint8_t *)header;
    //God, would a little documentation for this shit kill you...
    /* - Saki:
     * You may think you could understand what the following code does.
     * But you dont. Trust me.
     * Fiddle with it, and youll spend many a sleepless night cursing the moment
     * you thought youd be clever enough to "understand" the code below.
     * Now close this file and enjoy your ada course.
     */
    for (quint32 i=0; i<header->keyboardTypeCount; i++)
    {
        UCKeyStateRecordsIndex *stateRecords = 0;
        if(table[i].keyStateRecordsIndexOffset != 0)
        {
            stateRecords=reinterpret_cast<UCKeyStateRecordsIndex*>(
                        data+table[i].keyStateRecordsIndexOffset);
            if(stateRecords->keyStateRecordsIndexFormat
                    !=kUCKeyStateRecordsIndexFormat)
            {
                stateRecords = 0;
            }
        }
        UCKeyToCharTableIndex *charTable=
                reinterpret_cast<UCKeyToCharTableIndex*>(
                    data+table[i].keyToCharTableIndexOffset);
        if(charTable->keyToCharTableIndexFormat != kUCKeyToCharTableIndexFormat)
        {
            continue;
        }
        for (quint32 j=0; j<charTable->keyToCharTableCount; j++)
        {
            UCKeyOutput *keyToChar=
                    reinterpret_cast<UCKeyOutput*>(
                        data+charTable->keyToCharTableOffsets[j]);
            for (quint32 k=0; k < charTable->keyToCharTableSize; k++)
            {
                if(keyToChar[k] & kUCKeyOutputTestForIndexMask)
                {
                    long idx = keyToChar[k] & kUCKeyOutputGetIndexMask;
                    if(stateRecords && idx < stateRecords->keyStateRecordCount)
                    {
                        UCKeyStateRecord *record=
                                reinterpret_cast<UCKeyStateRecord*>(
                                    data +
                                    stateRecords->keyStateRecordOffsets[idx]);
                        if(record->stateZeroCharData==keyChar)
                        {
                            return k;
                        }
                    }
                }
                else if(!(keyToChar[k] & kUCKeyOutputSequenceIndexMask) &&
                        keyToChar[k] < 0xFFFE)
                {
                    if(keyToChar[k]==keyChar)
                    {
                        return k;
                    }
                }
            } // for k
        } // for j
    } // for i
    //For those key cannot be transferred, return 0 for failed.
    return 0;
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

OSStatus macHandleHotKey(EventHandlerCallRef nextHandler,
                         EventRef event,
                         void *data)
{
    Q_UNUSED(nextHandler);
    Q_UNUSED(data);
    //Filter out the keyboard events.
    if(GetEventClass(event)==kEventClassKeyboard &&
            GetEventKind(event)==kEventHotKeyPressed)
    {
        //Check the hot key id.
        EventHotKeyID keyId;
        //Get the parameter of the event.
        GetEventParameter(event,
                          kEventParamDirectObject,
                          typeEventHotKeyID, NULL, sizeof(keyId), NULL, &keyId);
        //Get the id of the key and find the shortcut pointer.
        KNGlobalAction *shortcut=
                keyRefs.value(keyId.id, qMakePair(nullptr, nullptr)).second;
        //Check the pointer.
        if(shortcut && shortcut->isEnabled())
        {
            //Activate the shortcut.
            emit shortcut->activated();
        }
    }
    //Mission complete.
    return noErr;
}

KNGlobalAction::~KNGlobalAction()
{
}

void KNGlobalAction::registerCurrent()
{
    //Loop and register al the key sequences.
    for(auto id : m_sequences)
    {
        //Check the id in the key ids.
        if(keyIds.contains(id))
        {
            //Already assign to another, ignore the request.
            continue;
        }
        //Prepare the hot key id.
        quint32 keySerialId=(++hotKeySerial);
        //Prepare the hot key id structure.
        EventHotKeyID keyId;
        keyId.signature='krmu';
        keyId.id=keySerialId;
        //Register the hot key.
        EventHotKeyRef keyReference = 0;
        if(!RegisterEventHotKey(id.second/*nativeKey*/, id.first/*nativeMods*/,
                                keyId, GetApplicationEventTarget(), 0,
                                &keyReference))
        {
            //Successfully register the event hot key.
            //Add the key id to the map.
            keyRefs.insert(keySerialId, qMakePair(keyReference, this));
            keyIds.insert(id, keySerialId);
        }
    }
}

void KNGlobalAction::unregisterCurrent()
{
    //Loop and remove all the ids.
    for(auto id : m_sequences)
    {
        //Check in the key reference map list.
        if(keyIds.contains(id))
        {
            //Remove the reference from the map list.
            EventHotKeyRef keyReference=keyRefs.take(keyIds.value(id)).first;
            //Use the Carbon API to do this.
            UnregisterEventHotKey(keyReference);
            //Remove from key ids.
            keyIds.remove(id);
        }
    }
}

void KNGlobalAction::setKeySequence(const QKeySequence &keySequence)
{
    //Check whether the handler has been installed.
    if(!macHandlerInstalled)
    {
        //Prepare the specific of the event type.
        EventTypeSpec typeSpecific;
        typeSpecific.eventClass = kEventClassKeyboard;
        typeSpecific.eventKind = kEventHotKeyPressed;
        //Install the event handler.
        InstallApplicationEventHandler(&macHandleHotKey,
                                       1, &typeSpecific, NULL, NULL);
        //Set the handler installed flag to true.
        macHandlerInstalled=true;
    }
    //Unregister for all the previous key bindings.
    unregisterCurrent();
    //Clear the sequences.
    m_sequences=QSet<Identifier>();
    //Parse the key sequence for all the Mac key bindings.
    quint32 nativeModifiers, nativeKeys;
    for(int i=0; i<keySequence.count(); ++i)
    {
        //Calculate the native modifiers and native keys.
        getNative(keySequence[i], nativeModifiers, nativeKeys);
        //Check the result.
        if(nativeKeys==0 && nativeModifiers==0)
        {
            //Ignore the unused keys.
            continue;
        }
        //Insert the result to key sequences.
        m_sequences.insert(qMakePair(nativeModifiers, nativeKeys));
    }
    //If the global action is enabled, register the key.
    if(m_enabled)
    {
        //Register the keys.
        registerCurrent();
    }
}
