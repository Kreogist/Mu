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

#ifndef KNLOCKEDFILE_H
#define KNLOCKEDFILE_H

#include <QFile>

#ifdef Q_OS_WIN
#include <QVector>
#endif

/*!
 * \brief The KNLockedFile class is a supported class for the singleton
 * application. It modified from the QtLockedFile from the Clementine player.\n
 * This extends QFile with advisory locking functions.\n
 * A file may be locked in read or write mode. Multiple instances of
 * \e KNLockedFile, created in multiple processes running on the same machine,
 * may have a file locked in read mode. Exactly one instance may have it locked
 * in write mode. A read and a write lock cannot exist simultaneously on the
 * same file.\n
 * The file locks are advisory. This means that nothing prevents another process
 * from manipulating a locked file using QFile or file system functions offered
 * by the OS. Serialization is only guaranteed if all processes that access the
 * file use QLockedFile. Also, while holding a lock on a file, a process must
 * not open the same file again (through any API), or locks can be unexpectedly
 * lost.\n
 * The lock provided by an instance of \e KNLockedFile is released whenever the
 * program terminates. This is true even when the program crashes and no
 * destructors are called.
 */
class KNLockedFile : public QFile
{
    Q_OBJECT
public:
    enum LockMode { NoLock = 0, ReadLock, WriteLock };

    /*!
     * \brief Construct an unlocked KNLockedFile object.
     * \param parent The parent object.
     */
    KNLockedFile(QObject *parent = 0);

    /*!
     * \brief Construct an unlocked KNLockedFile object.
     * \param name The file name.
     * \param parent The parent object.
     */
    KNLockedFile(const QString &name, QObject *parent = 0);
    ~KNLockedFile();

    /*!
     * \brief Reimplemented from QFile::open().
     */
    bool open(OpenMode mode) Q_DECL_OVERRIDE;

    /*!
     * \brief Obtains a lock of type \a mode. The file must be opened before it
     * can be locked.\n
     * If \a block is true, this function will block until the lock is acquired.
     * If \a block is false, this function returns \e false immediately if the
     * lock cannot be acquired.\n
     * If this object already has a lock of type \a mode, this function returns
     * \e true immediately. If this object has a lock of a different type than
     * \a mode, the lock is first released and then a new lock is obtained.
     * \param mode The file lock type.
     * \param block Whether the file should be blocked.
     * \return This function returns \e true if, after it executes, the file is
     * locked by this object, and \e false otherwise.
     */
    bool lock(LockMode mode, bool block = true);

    /*!
     * \brief Releases a lock.\n
     * If the object has no lock, this function returns immediately.
     * \return This function returns \e true if, after it executes, the file is
     * not locked by this object, and \e false otherwise.
     */
    bool unlock();

    /*!
     * \brief Get whether the file is locked.
     * \return Returns \e true if this object has a in read or write lock;
     * otherwise returns \e false.
     */
    bool isLocked() const;

    /*!
     * \brief Get the current file lock mode.
     * \return Returns the type of lock currently held by this object, or \e
     * KNLockedFile::NoLock.
     */
    LockMode lockMode() const;

signals:

public slots:

private:
    inline void initialParameters();
#ifdef Q_OS_WIN
    Qt::HANDLE getMutexHandle(int idx, bool doCreate);
    bool waitMutex(Qt::HANDLE mutex, bool doBlock);
    Qt::HANDLE m_writeMutex;
    Qt::HANDLE m_readMutex;
    QVector<Qt::HANDLE> m_readMutexes;
    QString m_mutexName;
#endif
    LockMode m_lockMode;
};

#endif // KNLOCKEDFILE_H
