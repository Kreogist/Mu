/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QDir>
#include <QFileInfo>

#include "knfilesearcher.h"

#include <QDebug>

KNFileSearcher::KNFileSearcher(QObject *parent) :
    QObject(parent)
{
    //Connect analysis signal and slots. Using Signal-Slost instead of calling
    //funcion directly, can avoid a deep calling stack.
    connect(this, &KNFileSearcher::requireAnalysisNext,
            this, &KNFileSearcher::analysisNext);
    connect(this, &KNFileSearcher::requireAnlaysisFolder,
            this, &KNFileSearcher::analysisFolder);
}

bool KNFileSearcher::isFilePathAccept(const QString &filePath)
{
    //This function is using for a quick check of a file path.
    QFileInfo typeChecker(filePath);
    return isSuffixAccept(typeChecker.suffix());
}

void KNFileSearcher::analysisUrls(QStringList urls)
{
    //Append the urls to the waiting list.
    m_fileSearchQueue.append(urls);
    //Emit analysis signal.
    if(!m_fileSearchQueue.isEmpty())
    {
        emit requireAnalysisNext();
    }
}

void KNFileSearcher::analysisFile(const QString &suffix,
                                  const QString &filePath)
{
    if(isSuffixAccept(suffix))
    {
        emit fileFound(filePath);
    }
}

void KNFileSearcher::analysisFolder(const QString &folderPath)
{
    QDir folderInfo(folderPath);
    //Get the entry file info under the folder.
    QFileInfoList contents=folderInfo.entryInfoList();
    for(auto i=contents.begin();
        i!=contents.end();
        ++i)
    {
        QString currentName=(*i).fileName();
        //Ignore dot and dotdot.
        if(currentName=="." || currentName=="..")
        {
            continue;
        }
        //Analysis item.
        if((*i).isFile())
        {
            analysisFile((*i).suffix(), (*i).absoluteFilePath());
        }
        if((*i).isDir())
        {
            emit requireAnlaysisFolder((*i).absoluteFilePath());
        }
    }
}

void KNFileSearcher::analysisNext()
{
    QFileInfo typeChecker(m_fileSearchQueue.takeFirst());
    //Analysis the current items.
    if(typeChecker.isDir())
    {
        emit requireAnlaysisFolder(typeChecker.absoluteFilePath());
    }
    if(typeChecker.isFile())
    {
        analysisFile(typeChecker.suffix(), typeChecker.absoluteFilePath());
    }
    //Emit analysis signal.
    if(!m_fileSearchQueue.isEmpty())
    {
        emit requireAnalysisNext();
    }
}
