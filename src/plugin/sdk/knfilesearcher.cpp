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
    connect(this, &KNFileSearcher::requireAnalysisFile,
            this, &KNFileSearcher::analysisFile);
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
    for(auto i=urls.begin();
        i!=urls.end();
        ++i)
    {
        QFileInfo typeChecker(*i);
        //Analysis the current items.
        if(typeChecker.isDir())
        {
            emit requireAnlaysisFolder(*i);
        }
        if(typeChecker.isFile())
        {
            emit requireAnalysisFile(typeChecker.suffix(), *i);
        }
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
            emit requireAnalysisFile((*i).suffix(),
                                     (*i).absoluteFilePath());
        }
        if((*i).isDir())
        {
            emit requireAnlaysisFolder((*i).absoluteFilePath());
        }
    }
}
