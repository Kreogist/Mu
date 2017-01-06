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
#include <QFileInfo>
#include <qt_windows.h>

#include "knlockedfile.h"

#define MUTEX_PREFIX "KNLockedFile mutex "
// Maximum number of concurrent read locks. Must not be greater than
// MAXIMUM_WAIT_OBJECTS
#define MAX_READERS MAXIMUM_WAIT_OBJECTS

//This patch is used to fix the QT_WA cannot be compiled on the Qt 5. This patch
//is provided from:
//  http://blog.csdn.net/kl222/article/details/21026299
//The code is originally from:
//  https://qt.gitorious.org/qt-solutions/ov3r1oads-qt-solutions/commit/
//70855469ce1fe82e1bba6b88d32a97081a70d7d1
#if QT_VERSION >= 0x050000
#define QT_WA(unicode, ansi) unicode
#endif

Qt::HANDLE KNLockedFile::getMutexHandle(int idx, bool doCreate)
{
    //Check the mutex name
    if (m_mutexName.isEmpty())
    {
        //Get the current file path, use the file path
        QFileInfo fi(*this);
        //Generate the mutex name.
        m_mutexName=MUTEX_PREFIX+fi.absoluteFilePath().toLower();
    }
    //Check the mutex name
    QString mutexName(m_mutexName);
    if (idx >= 0)
    {
        //Combine the new mutex name
        mutexName += QString::number(idx);
    }
    //Create the mutex.
    Qt::HANDLE mutex;
    //Check the create flag.
    if(doCreate)
    {
        //Use Windows API to create mutex, Wide char/ANSI.
        QT_WA({mutex=CreateMutexW(NULL, FALSE, (WCHAR*)mutexName.utf16());},
              {mutex=CreateMutexA(NULL, FALSE,
                                  mutexName.toLocal8Bit().constData());});
        //Check the mutex result.
        if(!mutex)
        {
            //Failed to create mutex.
            qErrnoWarning("KNLockedFile::lock(): CreateMutex failed");
            return 0;
        }
    }
    else
    {
        //It is not ask for create the mutex, so it means to open mutex.
        QT_WA({mutex=OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE,
                                (WCHAR*)mutexName.utf16());},
              {mutex=OpenMutexA(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE,
                                mutexName.toLocal8Bit().constData());});
        //Check the open mutex result.
        if(!mutex)
        {
            //Check the last error.
            if (GetLastError() != ERROR_FILE_NOT_FOUND)
            {
                //Cannot open the mutex lock.
                qErrnoWarning("KNLockedFile::lock(): OpenMutex failed");
            }
            return 0;
        }
    }
    //Give back the mutex information.
    return mutex;
}

bool KNLockedFile::waitMutex(Qt::HANDLE mutex, bool doBlock)
{
    Q_ASSERT(mutex);
    //Wait for the mutex.
    DWORD res=WaitForSingleObject(mutex, doBlock ? INFINITE : 0);
    //Check the result.
    switch (res)
    {
    case WAIT_OBJECT_0:
    case WAIT_ABANDONED:
        return true;
        break;
    case WAIT_TIMEOUT:
        break;
    default:
        qErrnoWarning("KNLockedFile::lock(): WaitForSingleObject failed");
    }
    return false;
}

bool KNLockedFile::lock(LockMode mode, bool block)
{
    //Check file open state.
    if (!isOpen())
    {
        //Cannot lock the file when it is not opened.
        qWarning("KNLockedFile::lock(): file is not opened");
        return false;
    }
    //Check the locking mode.
    if(mode==NoLock)
    {
        //Unlock the file.
        return unlock();
    }
    //Check the locking mode is already the same as current lock.
    if(mode==m_lockMode)
    {
        //Mission complete.
        return true;
    }
    //If we need to change the lock mode, we have to unlock the file first.
    if(m_lockMode != NoLock)
    {
        //Unlock the file, reset the lock mode to NoLock.
        unlock();
    }
    //Create the write mutex handle.
    if(!m_writeMutex && !(m_writeMutex = getMutexHandle(-1, true)))
    {
        return false;
    }
    //Wait for the mutex.
    if(!waitMutex(m_writeMutex, block))
    {
        return false;
    }
    //Check the locking mode.
    if(mode==ReadLock)
    {
        //Check the reader index.
        int idx = 0;
        for (; idx < MAX_READERS; idx++)
        {
            //Get the read mutex handle.
            m_readMutex = getMutexHandle(idx, false);
            //Check the read mutex state.
            if (!m_readMutex || waitMutex(m_readMutex, false))
            {
                break;
            }
            //Close the handle.
            CloseHandle(m_readMutex);
        }
        //Check the mutex running state.
        bool ok=true;
        if (idx >= MAX_READERS)
        {
            qWarning("QtLockedFile::lock(): too many readers");
            m_readMutex = 0;
            ok = false;
        }
        else if (!m_readMutex)
        {
            //Get the reading mutex handle.
            m_readMutex = getMutexHandle(idx, true);
            //Check the read mutex state.
            if (!m_readMutex || !waitMutex(m_readMutex, false))
            {
                ok = false;
            }
        }
        //Check the reading mutex state.
        if (!ok && m_readMutex)
        {
            //Close the read mutex.
            CloseHandle(m_readMutex);
            m_readMutex = 0;
        }
        ReleaseMutex(m_writeMutex);
        //Check the mutex state.
        if (!ok)
        {
            return false;
        }
    }
    else
    {
        Q_ASSERT(m_readMutexes.isEmpty());
        for (int i = 0; i < MAX_READERS; i++)
        {
            Qt::HANDLE mutex = getMutexHandle(i, false);
            if (mutex)
            {
                m_readMutexes.append(mutex);
            }
        }
        //Check the read mutex size.
        if (m_readMutexes.size())
        {
            //Wait for the lock.
            DWORD res=WaitForMultipleObjects(m_readMutexes.size(),
                                             m_readMutexes.constData(),
                                             TRUE, block ? INFINITE : 0);
            //Check the result.
            if (res != WAIT_OBJECT_0 && res != WAIT_ABANDONED)
            {
                //If the result could wait for the object,
                if (res != WAIT_TIMEOUT)
                {
                    qErrnoWarning("KNLockedFile::lock(): "
                                  "WaitForMultipleObjects failed");
                }
                //Save the write lock state.
                m_lockMode = WriteLock;
                // trick unlock() to clean up - semiyucky
                unlock();
                return false;
            }
        }
    }
    //Save the locking mode state.
    m_lockMode = mode;
    return true;
}

bool KNLockedFile::unlock()
{
    //Check file is opened or not.
    if (!isOpen())
    {
        //File cannot be unlocked.
        qWarning("KNLockedFile::unlock(): file is not opened");
        return false;
    }
    //Check the locked state.
    if (!isLocked())
    {
        //Already unlocked.
        return true;
    }
    //Process for different locking mode.
    if (m_lockMode == ReadLock)
    {
        //Unlock th read lock.
        ReleaseMutex(m_readMutex);
        CloseHandle(m_readMutex);
        m_readMutex = 0;
    }
    else
    {
        //Search in the lock list.
        foreach(Qt::HANDLE mutex, m_readMutexes)
        {
            //Release the mutex.
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }
        //Clear the list.
        m_readMutexes.clear();
        //Release resource.
        ReleaseMutex(m_writeMutex);
    }
    //Save the locking mode.
    m_lockMode=NoLock;
    //Complete.
    return true;
}

KNLockedFile::~KNLockedFile()
{
    //If the file is still opened,
    if(isOpen())
    {
        //Unlock the file.
        unlock();
    }
    //Check the write mutex.
    if (m_writeMutex)
    {
        //Close the writing mutex handle.
        CloseHandle(m_writeMutex);
    }
}
