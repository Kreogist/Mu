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
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QApplication>
#include <QLinkedList>
#include <QProcess>

#include "knutil.h"

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

QColor KNUtil::parseColor(const QString &data)
{
    switch(data.length())
    {
    case 3:
        //Format should be RGB.
        return QColor(data.mid(0,1).toInt(nullptr, 16)*16-1,
                      data.mid(1,1).toInt(nullptr, 16)*16-1,
                      data.mid(2,1).toInt(nullptr, 16)*16-1);
    case 4:
        //Format should be #RGB.
        return QColor(data.mid(1,1).toInt(nullptr, 16)*16-1,
                      data.mid(2,1).toInt(nullptr, 16)*16-1,
                      data.mid(3,1).toInt(nullptr, 16)*16-1);
    case 6:
        //Format should be RRGGBB.
        return QColor(data.mid(0,2).toInt(nullptr, 16),
                      data.mid(2,2).toInt(nullptr, 16),
                      data.mid(4,2).toInt(nullptr, 16));
    case 7:
        //Format should be #RRGGBB
        return QColor(data.mid(1,2).toInt(nullptr, 16),
                      data.mid(3,2).toInt(nullptr, 16),
                      data.mid(5,2).toInt(nullptr, 16));
    case 8:
        //Format should be RRGGBBAA
        return QColor(data.mid(0,2).toInt(nullptr, 16),
                      data.mid(2,2).toInt(nullptr, 16),
                      data.mid(4,2).toInt(nullptr, 16),
                      data.mid(6,2).toInt(nullptr, 16));
    case 9:
        //Format should be #RRGGBBAA
        return QColor(data.mid(1,2).toInt(nullptr, 16),
                      data.mid(3,2).toInt(nullptr, 16),
                      data.mid(5,2).toInt(nullptr, 16),
                      data.mid(7,2).toInt(nullptr, 16));
    default:
        //Or else return a default color;
        return QColor();
    }
}

QString KNUtil::simplifiedPath(const QString &path)
{
    return QFileInfo(path).absoluteFilePath();
}

QString KNUtil::ensurePathValid(const QString &path)
{
    //Gernerate the file info about the given path.
    QFileInfo detectInfo(path);
    //Check if the directory has alread exist.
    if(detectInfo.isDir() && detectInfo.exists())
    {
        return detectInfo.absoluteFilePath();
    }
    //Check if there's a file named the same as the path.
    if(detectInfo.isFile())
    {
        //Remove the file first.
        if(!QFile::remove(detectInfo.absoluteFilePath()))
        {
            return QString();
        }
    }
    //Generate the folder.
    return QDir().mkpath(detectInfo.absoluteFilePath())?
                detectInfo.absoluteFilePath():QString();
}

void KNUtil::openLocalFile(const QString &filePath)
{
    //Use the openUrl to open a local file.
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void KNUtil::showInGraphicalShell(const QString &filePath)
{
    //For Windows, explorer with /select parameter can open a new explorer
    //window and select that file.
#ifdef Q_OS_WIN
    QStringList args;
    QFileInfo pathInfo(filePath);
    if(!pathInfo.isDir())
    {
        args<<"/select,";
    }
    args<<QDir::toNativeSeparators(filePath);
    QProcess::startDetached("explorer.exe", args);
    return;
#endif
    //For Mac OS X, use osacript.
#ifdef Q_OS_MACX
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal "
                                      "POSIX file \"%1\"")
               .arg(filePath);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    return;
#endif
    //For Linux, it's magic. Don't touch.
#ifdef Q_OS_LINUX
    QFileInfo fileInfo(filePath);
    QString folder = fileInfo.isDir() ?
                fileInfo.absoluteFilePath() :
                fileInfo.filePath();
    QString app = QLatin1String("xdg-open %d");
    QString browserArgs;
    for (int i = 0; i < app.size(); ++i)
    {
        QChar c = app.at(i);
        if (c == QLatin1Char('%') && i < app.size()-1)
        {
            c = app.at(++i);
            QString s;
            if (c == QLatin1Char('d'))
            {
                s = QLatin1Char('"') +
                        QFileInfo(folder).path() +
                        QLatin1Char('"');
            }
            else if (c == QLatin1Char('f'))
            {
                s = QLatin1Char('"') + folder + QLatin1Char('"');
            }
            else if (c == QLatin1Char('n'))
            {
                s = QLatin1Char('"') +
                        QFileInfo(folder).fileName() +
                        QLatin1Char('"');
            }
            else if (c == QLatin1Char('%'))
            {
                s = c;
            }
            else
            {
                s = QLatin1Char('%');
                s += c;
            }
            browserArgs += s;
            continue;
        }
        browserArgs += c;
    }
    QProcess::startDetached(browserArgs);
#endif
}

void KNUtil::setClipboardText(const QString &text)
{
    //Get the clipboard from the QApplication instance.
    QClipboard *clipboard=qApp->clipboard();
    //Check the clipboard first.
    if(clipboard==nullptr)
    {
        return;
    }
    //Set the text to the clipboard.
    clipboard->setText(text, QClipboard::Clipboard);
}

QStringList KNUtil::urlListToPathList(const QList<QUrl> &urlList)
{
    //Generate a file path list first.
    QStringList filePathList;
    //Translate all the url to file path.
    for(auto i=urlList.constBegin(); i!=urlList.constEnd(); ++i)
    {
        //Get the path of the file list.
        QString path=(*i).path();
        //Check whether the path is empty.
        if(!path.isEmpty())
        {
#ifdef Q_OS_WIN
            //Remove the very beginning '/' char in Windows.
            path.remove(0,1);
#endif
            //Simply add the path to the path list.
            filePathList.append(path);
        }
    }
    //Give back the file path list.
    return filePathList;
}

QString KNUtil::legalFileName(QString fileName)
{
    //Remove the unavailable characters in the prefer string.
    fileName.replace(QRegExp("[\\\\/:*?\"<>]"), "_");
    return fileName;
}

int KNUtil::similarity(QString string1, QString string2)
{
    //Wagner–Fischer algorithm Levenshtein distance.
    //Based on
    /* http://en.wikibooks.org/wiki/Algorithm_implementation/Strings/Levenshtein
_distance#C.2B.2B*/
    //And
    /*http://www.codeproject.com/Articles/13525/Fast-memory-efficient-Levenshtei
n-algorithm*/
    //The original Levenshtein Algorithm will used a matrix whose size is
    //length(string1) * length(string2), this is a common fast, memory efficient
    //version. It can reduce the memory usage to length(string1) +
    //length(string2). This won't reduce the time complexity.
    //Thanks for QString and QChar, a chinese character will be description as
    //a char.
    //Initial the size.
    const unsigned len1=string1.size(), len2=string2.size();
    std::vector<int> col(len2+1), prevCol(len2+1);
    //Fills the vector with ascending numbers, starting by 0
    //Because of the FUCK clang, we can NOT use itoa, or else these thing
    //following can be done in only one sentence:
    for(unsigned i=0; i<prevCol.size(); i++)
    {
        prevCol[i]=i;
    }
    //Use double std::min instead of std::min({,,}).
    for(unsigned i=0; i<len1; i++)
    {
        col[0]=i+1;
        for(unsigned j=0; j<len2; j++)
        {
            col[j+1]=std::min(std::min(1+col[j],
                                       1+prevCol[1+j]),
                              prevCol[j]+(string1[i]!=string2[j]));
        }
        std::swap(col, prevCol);
    }
    //Force cast the size_t to int
    return prevCol[len2];
}

bool KNUtil::saveTextToFile(const QString &filePath, const QString &content)
{
    //Generate the target file.
    QFile targetFile(filePath);
    //Ensure the path is valid.
    //Open as write only mode.
    if(ensurePathValid(QFileInfo(targetFile).absolutePath()).isEmpty() ||
            !targetFile.open(QIODevice::WriteOnly))
    {
        //If there's no path to this file, how can we write the file?
        return false;
    }
    //Generate the text stream.
    QTextStream textStream(&targetFile);
    //Set the UTF-8 codec.
    textStream.setCodec(QTextCodec::codecForName("UTF-8"));
    //Save the content to the file.
    textStream << content << flush;
    //Close the file.
    targetFile.close();
    //Mission complete.
    return true;
}

bool KNUtil::renameFile(const QString &filePath, const QString &newFileName)
{
    return QFile::rename(filePath, newFileName);
}

bool KNUtil::copyDirectory(const QString &fromDir, const QString &toDir,
                           bool overwrite)
{
    //Prepare the copy stack.
    QLinkedList<QString> copyStack, destStack;
    //Check directory existance.
    if(!QDir(fromDir).exists())
    {
        //Failed to find the original dir.
        return false;
    }
    //Insert the current directory to stack.
    copyStack.append(fromDir);
    destStack.append(toDir);
    //Loop until copy stack is empty.
    while(!copyStack.isEmpty())
    {
        //Pick out the string.
        QString srcDirPath=copyStack.takeLast(),
                destDirPath=destStack.takeLast();
        //Get the directory.
        QDir currentDir(srcDirPath), destDir(destDirPath);
        //Check destdir state.
        if(!destDir.exists() &&
                KNUtil::ensurePathValid(destDir.absolutePath()).isEmpty())
        {
            //Build the directory failed.
            return false;
        }
        //Loop and check all the file.
        for(auto fileInfo : currentDir.entryInfoList())
        {
            //Ignore the . and ..
            if(fileInfo.fileName()=="." || fileInfo.fileName()=="..")
            {
                //Do not operate these two directories.
                continue;
            }
            //Check it is file or directory.
            if(fileInfo.isDir())
            {
                //Push the file info to the stack.
                copyStack.append(fileInfo.absolutePath());
                destStack.append(destDir.absoluteFilePath(fileInfo.fileName()));
                continue;
            }
            if(fileInfo.isFile())
            {
                //Get the destination path.
                QString destFilePath(destDir.filePath(fileInfo.fileName()));
                //Check existance.
                if(QFile::exists(destFilePath))
                {
                    //If not overwrite the file, ignore this file.
                    if(!overwrite)
                    {
                        continue;
                    }
                    //When overwrite enabled, remove and copy the file.
                    if(!QFile::remove(destFilePath))
                    {
                        //Failed to remove the target file.
                        return false;
                    }
                }
                //Copy the file.
                if(!QFile::copy(fileInfo.absoluteFilePath(), destFilePath))
                {
                    //Failed to copy the file.
                    return false;
                }
            }
        }
    }
    return true;
}

bool KNUtil::removeDirectory(const QString &dirPath)
{
    QDir targetDir(dirPath);
    //Check the orignal dir existance.
    if(!targetDir.exists())
    {
        //No need to remove directory.
        return true;
    }
    //Remove the content and directory.
    return targetDir.removeRecursively() && targetDir.rmdir(dirPath);
}

bool KNUtil::moveDirectory(const QString &fromDir, const QString &toDir)
{
    //Copy the data from original directory to the new directory.
    if(!copyDirectory(fromDir, toDir, true))
    {
        //Failed to copy the data.
        return false;
    }
    //Delete the orignal directory.
    return removeDirectory(fromDir);
}
