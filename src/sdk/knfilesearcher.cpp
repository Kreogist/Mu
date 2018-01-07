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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QDir>
#include <QLinkedList>

#include "knfilesearcher.h"

#include <QDebug>

QStringList KNFileSearcher::m_suffixList=QStringList();

KNFileSearcher::KNFileSearcher(QObject *parent) :
    QObject(parent),
    m_queue(QStringList()),
    m_counter(0),
    m_working(false)
{
    //These signals are only used to avoid a deep calling stack.
    //Calling funcion directly may caused a deep , and that will make the stack
    //really really deep.
    //Link the analysis next item.
    connect(this, &KNFileSearcher::requireAnalysisNext,
            this, &KNFileSearcher::analysisNext,
            Qt::QueuedConnection);
}

void KNFileSearcher::analysisPaths(QStringList paths, QList<uint> monitorHash)
{
    //Check the paths is empty or not.
    if(paths.isEmpty())
    {
        //If the adding path list is an empty list, ignore the request.
        return;
    }
    //Check the queue before.
    if(m_queue.isEmpty())
    {
        //Emit the search start signal.
        emit searchStart();
        //Reset the counter.
        m_counter=0;
        //Reset the working state.
        m_working=true;
    }
    //Append the urls to the waiting list.
    m_queue.append(paths);
    //Check the monitor hash list length.
    if(monitorHash.isEmpty())
    {
        //Create the monitor hash with empty string hash result.
        uint emptyHash=qHash(QString(""));
        //Reallocate the monitor hash memory.
        monitorHash.reserve(paths.size());
        int pathsLength=paths.size();
        //Append the data.
        while(pathsLength--)
        {
            //Build the empty list.
            monitorHash.append(emptyHash);
        }
    }
    //Append the monitor hash list.
    m_hashQueue.append(monitorHash);
    //Check the queue, if the queue is not empty then ask to analysis one.
    if(!m_queue.isEmpty())
    {
        //Ask to anlaysis next file.
        emit requireAnalysisNext();
    }
}

void KNFileSearcher::analysisNext()
{
    //Check the queue first.
    if(m_queue.isEmpty())
    {
        //Clear the working state.
        m_working=false;
        //Mission complete.
        emit searchFinish(m_counter);
        //Finish.
        return;
    }
    //Get the first path in the queue, use QFileInfo to parse it.
    QFileInfo typeChecker(m_queue.takeFirst());
    //Check whether it's a dir.
    if(typeChecker.isDir())
    {
        //Analysis item as a folder.
        analysisFolder(typeChecker, m_hashQueue.takeFirst());
    }
    else if(typeChecker.isFile())
    {
        //Analysis item as a file.
        analysisFile(typeChecker, m_hashQueue.takeFirst());
    }
    //Emit the parse next signal.
    emit requireAnalysisNext();
}

inline void KNFileSearcher::analysisFolder(const QFileInfo &folderInfo,
                                           uint folderDirHash)
{
    //Construct the sub internal queue of the folder.
    QLinkedList<QString> folderQueue;
    folderQueue.append(folderInfo.absoluteFilePath());
    //Prepare the file info list.
    QFileInfoList validFileList;
    //Loop until the folder queue is empty.
    while(!folderQueue.isEmpty())
    {
        //Get the entry file info under the folder.
        QFileInfoList contents=QDir(folderQueue.takeFirst()).entryInfoList();
        //Check all the items of the folder.
        for(auto i : contents)
        {
            //Get the file name.
            QString fileName=i.fileName();
            //Ignore the dot and dotdot.
            if(fileName=="." || fileName=="..")
            {
                continue;
            }
            //Check the current item.
            if(i.isFile())
            {
                //Check whether file is valid.
                if(isFileValid(i))
                {
                    //Append the file in the list.
                    validFileList.append(i);
                    //Increase the counter.
                    ++m_counter;
                }
            }
            else if(i.isDir())
            {
                //If it's a directory, prepend it to the analysis queue.
                folderQueue.append(i.absoluteFilePath());
            }
        }
    }
    //Check the valid file list.
    if(!validFileList.isEmpty())
    {
        //Build the directory hash list.
        QList<uint> hashList;
        //Construct the hash list.
        hashList.reserve(folderDirHash);
        int fileListSize=validFileList.size();
        while(fileListSize--)
        {
            hashList.append(folderDirHash);
        }
        //Process the file in batch.
        emit findFiles(validFileList, hashList);
    }
}

inline void KNFileSearcher::analysisFile(const QFileInfo &fileInfo,
                                         uint fileDirHash)
{
    //Check whether the suffix is in the suffix list.
    if(m_suffixList.contains(fileInfo.suffix().toLower()))
    {
        //Increase the counter.
        ++m_counter;
        //If we can find the suffix, emit the find out signal.
        emit findFile(fileInfo, fileDirHash);
    }
}

inline bool KNFileSearcher::isFileValid(const QFileInfo &fileInfo)
{
    //Check whether the suffix is in the suffix list.
    return m_suffixList.contains(fileInfo.suffix().toLower());
}

QStringList KNFileSearcher::suffixList()
{
    return m_suffixList;
}

bool KNFileSearcher::isWorking() const
{
    return m_working;
}

void KNFileSearcher::setSuffixList(const QStringList &suffixList)
{
    m_suffixList = suffixList;
}
