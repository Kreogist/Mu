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
    //CHeck if there's a file named the same as the path.
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
