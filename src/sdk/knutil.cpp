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
#include <QProcess>

#include "knutil.h"

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
    //Siderite Zackwehdex's Sift-4 string distance algorithm.
    //Transfer to the lower case.
    string1=string1.toLower(); string2=string2.toLower();
    //Compare to Levenshtein distance algorithm, it's much swift and low memory
    //cost.
    int l1=string1.length(), l2=string2.length();
    //Check the special states for speed up.
    //Check out the distance of l1.
    if(l1==0)
    {
        //The distance will be the length of string 2.
        return l2;
    }
    //Check out the distance of l2.
    if(l2==0)
    {
        //The distance will be the length of string 1.
        return l1;
    }

    int c1 = 0,  //cursor for string 1
        c2 = 0,  //cursor for string 2
        lcss = 0,  //largest common subsequence
        local_cs = 0; //local common substring
    //Make sure the cursor is not be out-of-range.
    while ((c1 < l1) && (c2 < l2))
    {
        //Compare the cursor character.
        if (string1.at(c1)==string2.at(c2))
        {
            //Increase the common substring length.
            local_cs++;
        }
        else
        {
            //Add local common substring length to largest common subsequence.
            lcss+=local_cs;
            //Reset the local common substring length.
            local_cs=0;
            //Check the pointer position.
            if(c1!=c2)
            {
                //Reset the pointer to be the later one.
                c1=c2=qMax(c1, c2);
            }
            //If matching tokens are found, remove 1 from both cursors (they get
            //incremented at the end of the loop).
            //So that we can have only one code block handling matches.
            //The original Sift-4 has maximum offset, we will ignore the offset
            //and check out the entire string.
            for(int i=0; (c1+i<l1) && (c2+i<l2); ++i)
            {
                //Check pointer of string 1.
                if((c1+i<l1) && string1.at(c1+i)==string2.at(c2))
                {
                    c1+=i-1;
                    --c2;
                    break;
                }
                //Check pointer of string 2.
                if((c2 + i < l2) && string1.at(c1)==string2.at(c2+i))
                {
                    --c1;
                    c2+=i-1;
                    break;
                }
            }
        }
        //Move the cursor.
        ++c1;
        ++c2;
    }
    //Add local common substring length to largest common subsequence.
    lcss+=local_cs;
    //Calculate the length.
    return (l1-lcss)+(l2-lcss);
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
