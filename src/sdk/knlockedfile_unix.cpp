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

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "knlockedfile.h"

bool KNLockedFile::lock(LockMode mode, bool block)
{
    //Check the file open state first.
    if(!isOpen())
    {
        //When the file is not open, it cannot be locked.
        qWarning("KNLockedFile::lock(): file is not opened");
        return false;
    }
    //Check the operation type.
    if(mode==NoLock)
    {
        //For no lock, we have to unlock the file.
        return unlock();
    }
    //Check whether the current mode is the same.
    if(mode==m_lockMode)
    {
        //Already lock the file at the mode, complete.
        return true;
    }
    //If the file is locked, then we need to unlock the file first, and relock
    //the file at the specific mode.
    if(NoLock!=m_lockMode)
    {
        //Unlock the file.
        unlock();
    }
    //Construct the flock structure.
    struct flock fl;
    fl.l_whence=SEEK_SET;
    fl.l_start=0;
    fl.l_len=0;
    fl.l_type=(mode == ReadLock) ? F_RDLCK : F_WRLCK;
    int cmd=block ? F_SETLKW : F_SETLK;
    //Launch the lock operation.
    int ret=fcntl(handle(), cmd, &fl);
    //Check the result.
    if(ret==-1)
    {
        //Error occurs.
        if (errno!=EINTR && errno!=EAGAIN)
        {
            qWarning("KNLockedFile::lock(): fcntl: %s", strerror(errno));
        }
        return false;
    }
    //Save the lock mode.
    m_lockMode=mode;
    return true;
}

bool KNLockedFile::unlock()
{
    //Check the file is open or not.
    if (!isOpen())
    {
        //For unopen file, ignore.
        qWarning("KNLockedFile::unlock(): file is not opened");
        return false;
    }
    //Check the file is locked or not.
    if (!isLocked())
    {
        //Ignore for unlocked file.
        return true;
    }
    //Construct the flock structure.
    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = F_UNLCK;
    //Launch the operation.
    int ret=fcntl(handle(), F_SETLKW, &fl);
    //Check error.
    if(ret==-1)
    {
        //Display the error.
        qWarning("KNLockedFile::lock(): fcntl: %s", strerror(errno));
        return false;
    }
    //Set the lock mode to no lock.
    m_lockMode = NoLock;
    return true;
}

KNLockedFile::~KNLockedFile()
{
    //Check the file is open or not.
    if (isOpen())
    {
        //Unlock the file when the class is dead.
        unlock();
    }
}
