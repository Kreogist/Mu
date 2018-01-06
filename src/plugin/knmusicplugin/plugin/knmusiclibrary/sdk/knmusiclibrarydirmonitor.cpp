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

#include "knmusicglobal.h"

#include "knmusiclibrarydirmonitor.h"

#include <QDebug>

KNMusicLibraryDirMonitor::KNMusicLibraryDirMonitor(QObject *parent) :
    QObject(parent),
    m_monitorDirList(QStringList())
{
}

void KNMusicLibraryDirMonitor::checkTotal(const QStringList &filePathList)
{
    QHash<QString, int> watchFileList;
    //Check the file path from the list is in the watching list.
    for(int i=0; i<filePathList.size(); ++i)
    {
        //Check the file path at position.
        if(isFileInMonitorDir(filePathList.at(i)))
        {
            //Append the file to the watch file list.
            watchFileList.insertMulti(filePathList.at(i), i);
        }
    }
    //Create the adding file list.
    QStringList directoryQueue=m_monitorDirList, addingList;
    //Loop until all the directory in the queue list has been operated.
    while(!directoryQueue.isEmpty())
    {
        //Get the file info list.
        QFileInfoList entireInfoList=
                QDir(directoryQueue.takeFirst()).entryInfoList();
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
                directoryQueue.append(itemPath);
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
            }
        }
    }
    //Add the file in the adding list to the library.
    //Emit the sync signal.
    emit requireSync(addingList, watchFileList.values());
}

void KNMusicLibraryDirMonitor::setMonitorDirs(const QStringList &directories)
{
    //Update the monitor dir list.
    m_monitorDirList=directories;
}

bool KNMusicLibraryDirMonitor::isFileInMonitorDir(const QString &filePath)
{
    //Check the path is the prefix or not.
    for(auto i : m_monitorDirList)
    {
        //Loop and check the prefix.
        if(filePath.startsWith(i))
        {
            return true;
        }
    }
    //No file matches in the list.
    return false;
}
