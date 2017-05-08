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

#include "knlockedfile.h"

KNLockedFile::KNLockedFile(QObject *parent) :
    QFile(parent)
{
    //Initialized parameters.
    initialParameters();
}

KNLockedFile::KNLockedFile(const QString &name, QObject *parent) :
    QFile(name, parent)
{
    //Initialized parameters.
    initialParameters();
}

bool KNLockedFile::open(QIODevice::OpenMode mode)
{
    //Check the file mode, if the mode contains truncate.
    if (mode & QIODevice::Truncate)
    {
        //Cannot open the file as truncate mode.
        qWarning("KNLockedFile::open(): Truncate mode not allowed.");
        return false;
    }
    //Open the file.
    return QFile::open(mode);
}

bool KNLockedFile::isLocked() const
{
    //Simply check the lock mode.
    return m_lockMode != NoLock;
}

KNLockedFile::LockMode KNLockedFile::lockMode() const
{
    return m_lockMode;
}

inline void KNLockedFile::initialParameters()
{
    //According to the original code, all the initialized codes are the same,
    //so we put the code here, only do the QFile initial in the construction
    //function.
#ifdef Q_OS_WIN
    m_writeMutex = 0;
    m_readMutex = 0;
#endif
    m_lockMode = NoLock;
}
