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
#include <QDir>
#include <QHash>
#include <QJsonArray>
#include <QFileSystemWatcher>

#include "knmusicglobal.h"

#include "knmusiclibrarydirmonitor.h"

#include <QDebug>

KNMusicLibraryDirMonitor::KNMusicLibraryDirMonitor(QObject *parent) :
    QObject(parent),
    m_monitorMap(QHash<QString, uint>()),
    m_isWorking(false)
{
}

bool KNMusicLibraryDirMonitor::isWorking() const
{
    //Get the working state.
    return m_isWorking;
}

void KNMusicLibraryDirMonitor::checkEntireLibrary(
        QHash<QString, int> watchFileList)
{
    //Mark working state.
    m_isWorking=true;
    //Create the adding file list.
    QStringList directoryQueue=m_monitorMap.keys(), addingList;
    QList<uint> hashList;
    //Loop until all the directory in the queue list has been operated.
    while(!directoryQueue.isEmpty())
    {
        //Prepare the directory.
        QString currentMonitorDir=directoryQueue.takeFirst();
        uint dirHash=m_monitorMap.value(currentMonitorDir);
        //Prepare the directory queue.
        QStringList currentQueue;
        currentQueue.append(currentMonitorDir);
        while(!currentQueue.isEmpty())
        {
            //Get the file info list.
            QFileInfoList entireInfoList=
                    QDir(currentQueue.takeFirst()).entryInfoList();
            //Loop and check the list.
            for(auto j : entireInfoList)
            {
                //Ignore the dot and dot dot.
                if(j.fileName()=="." || j.fileName()=="..")
                {
                    continue;
                }
                //Get the file path of j.
                QString &&itemPath=j.absoluteFilePath();
                //Check whether j is a directory or a file.
                if(j.isDir())
                {
                    //Push the directory into the queue.
                    currentQueue.append(itemPath);
                    continue;
                }
                //Now j should be a file.
                //If the path j is in the watch file list.
                if(j.isFile() && watchFileList.contains(itemPath))
                {
                    //Remove the file from the list.
                    watchFileList.remove(itemPath);
                }
                else
                {
                    //The file need to be added.
                    addingList.append(itemPath);
                    hashList.append(dirHash);
                }
            }
        }
    }
    //Add the file in the adding list to the library.
    //Emit the sync signal.
    emit requireSync(addingList, hashList, watchFileList.values());
    //Mark working state.
    m_isWorking=false;
}

void KNMusicLibraryDirMonitor::setMonitorDirs(const QStringList &directories)
{
    //Prepare the dir list and reset the hash list.
    QHash<QString, uint> newDirMap;
    //Loop and check existance.
    for(auto i : directories)
    {
        //Get the directory i.
        QDir currentDir(i);
        //Check instance.
        if(!currentDir.exists())
        {
            //For non-existance directory, ignore.
            return;
        }
        //Calculate the directory hash and build the queue.
        QString currentDirPath=currentDir.absolutePath();
        //Insert to the new dir map.
        newDirMap.insert(currentDirPath, qHash(currentDirPath));
        if(m_monitorMap.contains(currentDirPath))
        {
            //Remove the path from the original map.
            m_monitorMap.remove(currentDirPath);
        }
    }
    //Remove all the directory in the old monitor map.
    QList<uint> removedHash=m_monitorMap.values();
    //Update the monitor dir list.
    m_monitorMap=newDirMap;
    //Emit the updated signal.
    emit monitorDirUpdated(removedHash);
}
