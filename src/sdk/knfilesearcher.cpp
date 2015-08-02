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

#include "knfilesearcher.h"

QStringList KNFileSearcher::m_suffixList=QStringList();

KNFileSearcher::KNFileSearcher(QObject *parent) :
    QObject(parent),
    m_queue(QStringList())
{
    //These signals are only used to avoid a deep calling stack.
    //Calling funcion directly may caused a deep , and that will make the stack
    //really really deep.
    //Link the analysis next item.
    connect(this, &KNFileSearcher::requireAnalysisNext,
            this, &KNFileSearcher::analysisNext);
}

void KNFileSearcher::analysisPaths(const QStringList &paths)
{
    //Append the urls to the waiting list.
    m_queue.append(paths);
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
        return;
    }
    //Get the first path in the queue, use QFileInfo to parse it.
    QFileInfo typeChecker(m_queue.takeFirst());
    //Check whether it's a dir.
    if(typeChecker.isDir())
    {
        analysisFolder(typeChecker);
    }
    else if(typeChecker.isFile())
    {
        analysisFile(typeChecker);
    }
    //Emit the parse next signal.
    emit requireAnalysisNext();
}

void KNFileSearcher::analysisFolder(const QFileInfo &folderInfo)
{
    //Get the entry file info under the folder.
    QFileInfoList contents=QDir(folderInfo.absoluteFilePath()).entryInfoList();
    //Check all the items of the folder.
    for(auto i=contents.constBegin(); i!=contents.constEnd(); ++i)
    {
        //Get the file name.
        QString fileName=(*i).fileName();
        //Ignore the dot and dotdot.
        if(fileName=="." || fileName=="..")
        {
            continue;
        }
        //Check the current item.
        if((*i).isFile())
        {
            analysisFile(*i);
        }
        else if((*i).isDir())
        {
            //If it's a directory, prepend it to the analysis queue.
            m_queue.prepend((*i).absoluteFilePath());
        }
    }
}

inline void KNFileSearcher::analysisFile(const QFileInfo &fileInfo)
{
    //Check whether the suffix is in the suffix list.
    if(m_suffixList.contains(fileInfo.suffix().toLower()))
    {
        //If we can find the suffix, emit the find out signal.
        emit findFile(fileInfo);
    }
}
QStringList KNFileSearcher::suffixList()
{
    return m_suffixList;
}


void KNFileSearcher::setSuffixList(const QStringList &suffixList)
{
    m_suffixList = suffixList;
}
