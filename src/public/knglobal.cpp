/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QClipboard>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDesktopServices>
#include <QProcess>
#include <QTextCodec>
#include <QDir>

#include "knconfiguremanager.h"
#include "knfontmanager.h"
#include "knlocalemanager.h"

#include "knglobal.h"

#include <QDebug>

QString KNGlobal::m_dylibSuffix="";
QString KNGlobal::m_libraryPath="";
QString KNGlobal::m_userDataPath="";
QString KNGlobal::m_resourceDirPath="";
QString KNGlobal::m_pluginDirPath="";
KNGlobal *KNGlobal::m_instance=nullptr;
KNGlobal *KNGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNGlobal:m_instance;
}

QString KNGlobal::byteToHigherUnit(const qint64 &fileSize)
{
    qreal dFileSize=(qreal)fileSize;
    int unitPointer=0;
    while(dFileSize>1024.0 && unitPointer<StorageUnitCount)
    {
        dFileSize/=1024.0;
        unitPointer++;
    }
    return QString::number(dFileSize, 'f', 2)+" "+m_storageUnit[unitPointer];
}

QString KNGlobal::dylibSuffix()
{
    return m_dylibSuffix;
}

QString KNGlobal::applicationDirPath()
{
    return QApplication::applicationDirPath();
}

QString KNGlobal::userDataPath()
{
    return m_userDataPath;
}

QString KNGlobal::pluginDirPath()
{
    return m_pluginDirPath;
}

QString KNGlobal::simplifiedPath(const QString &path)
{
    QFileInfo simplifiedInfo(path);
    return simplifiedInfo.absoluteFilePath();
}

void KNGlobal::legalFileName(QString &fileName)
{
    //Remove the unavailable characters in the prefer string.
    fileName.replace(QRegExp("[\\\\/:*?\"<>]"), "_");
}

QString KNGlobal::libraryPath()
{
    return m_libraryPath;
}

QString KNGlobal::resourceDirPath()
{
    return m_resourceDirPath;
}

QTextCodec *KNGlobal::localeDefaultCodec()
{
    switch(QLocale::system().country())
    {
    case QLocale::China:
        return QTextCodec::codecForName("GB18030");
    case QLocale::HongKong:
        return QTextCodec::codecForName("Big5-HKSCS");
    case QLocale::Macau:
    case QLocale::Taiwan:
        return QTextCodec::codecForName("Big5");
    case QLocale::Japan:
        return QTextCodec::codecForName("Shift-JIS");
    default:
        return QTextCodec::codecForName("ISO 8859-1");
    }
}

QString KNGlobal::ensurePathAvaliable(const QString &path)
{
    //Check if there's a file named the same as the path.
    QFileInfo detectInfo(path);
    if(detectInfo.isFile())
    {
        //Remove the file first.
        QFile detectFile(detectInfo.absoluteFilePath());
        if(!detectFile.remove())
        {
            return QString();
        }
    }
    //Check if the directory has already exist.
    if(detectInfo.isDir() && detectInfo.exists())
    {
        return detectInfo.absoluteFilePath();
    }
    //Generate the folder.
    QDir detectFolder(detectInfo.absoluteFilePath());
    return detectFolder.mkpath(detectFolder.absolutePath())?
                detectFolder.absolutePath():QString();
}

QStringList KNGlobal::urlToPathList(const QList<QUrl> urls)
{
    QStringList filePathList;
    for(auto i=urls.begin();
            i!=urls.end();
            ++i)
    {
        QString currentPath=(*i).path();
#ifdef Q_OS_WIN32
        //Remove the very beginning '/' char in Windows.
        currentPath.remove(0,1);
#endif
        filePathList.append(currentPath);
    }
    return filePathList;
}

int KNGlobal::similarity(const QString &str1, const QString &str2)
{
    //Levenshtein distance.
    //Based on http://en.wikibooks.org/wiki/Algorithm_implementation/Strings/Levenshtein_distance#C.2B.2B
    //Initial the size.
    const unsigned len1=str1.size(), len2=str2.size();
    std::vector<size_t> col(len2+1), prevCol(len2+1);
    //Fills the vector with ascending numbers, starting by 0
    //Fuck Clang, why not support itoa?!
    //Or else these thing following can be done in only one sentence:
    //std::itoa(prevCol.begin(), prevCol.end(), prevCol.size());
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
            col[j+1]=std::min(std::min(1+col[j], 1+prevCol[1+j]), prevCol[j]+(str1[i]!=str2[j]));
        }
        std::swap(col, prevCol);
    }
    return prevCol[len2];
}

void KNGlobal::setDylibSuffix(const QString &dylibSuffix)
{
    m_dylibSuffix = dylibSuffix;
}

void KNGlobal::setLibraryPath(const QString &libraryPath)
{
    m_libraryPath = ensurePathAvaliable(libraryPath);
}

void KNGlobal::showInGraphicalShell(const QString &filePath)
{
#ifdef Q_OS_WIN32
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
#ifdef Q_OS_MACX
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
               .arg(filePath);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    return;
#endif
#ifdef Q_OS_LINUX
    QFileInfo fileInfo(filePath);
    QString folder = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.filePath();
    QString app = QLatin1String("xdg-open %d");
    QString browserArgs = substituteFileBrowserParameters(app, folder);
    QProcess::startDetached(browserArgs);
#endif
}

#ifdef Q_OS_LINUX
QString KNGlobal::substituteFileBrowserParameters(QString &pre, QString &file)
{
    QString cmd;
    for (int i = 0; i < pre.size(); ++i)
    {
        QChar c = pre.at(i);
        if (c == QLatin1Char('%') && i < pre.size()-1)
        {
            c = pre.at(++i);
            QString s;
            if (c == QLatin1Char('d'))
                s = QLatin1Char('"') + QFileInfo(file).path() + QLatin1Char('"');
            else if (c == QLatin1Char('f'))
                s = QLatin1Char('"') + file + QLatin1Char('"');
            else if (c == QLatin1Char('n'))
                s = QLatin1Char('"') + QFileInfo(file).fileName() + QLatin1Char('"');
            else if (c == QLatin1Char('%'))
                s = c;
            else
            {
                s = QLatin1Char('%');
                s += c;
            }
            cmd += s;
            continue;
        }
        cmd += c;
    }
    return cmd;
}
#endif

void KNGlobal::openLocalFile(const QString &filePath)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void KNGlobal::setClipboardText(const QString &text)
{
    QApplication::clipboard()->setText(text, QClipboard::Clipboard);
}

void KNGlobal::moveFolder(const QString &sourceDirPath,
                          const QString &destinationDirPath)
{
    //Ensure that the destination dir should exist.
    QDir sourceDir(sourceDirPath);
    QFileInfoList sourceDirInfoList=sourceDir.entryInfoList(QDir::Dirs |
                                                            QDir::Files |
                                                            QDir::NoDotAndDotDot);
    for(QFileInfoList::iterator i=sourceDirInfoList.begin();
        i!=sourceDirInfoList.end();
        ++i)
    {
        QString destinationPath=destinationDirPath+"/"+(*i).fileName();
        if((*i).isDir())
        {
            //Generate the sub dir.
            sourceDir.mkpath(destinationPath);
            //Move the sub dir.
            moveFolder((*i).absoluteFilePath(), destinationPath);
        }
        else if((*i).isFile())
        {
            //Delete the destination path file if it already exist.
            QFileInfo destinationCheck(destinationPath);
            if(destinationCheck.exists())
            {
                //We can hint user to chose.
                QFile::remove(destinationCheck.absoluteFilePath());
            }
            //Rename the file to move it.
            QFile::rename((*i).absoluteFilePath(),
                          destinationPath);
        }
    }
    //Remove the source dir.
    sourceDir.rmdir(".");
}

bool KNGlobal::renameFile(const QString &originalPath, const QString &currentPath)
{
    QFile targetFile(originalPath);
    //Check the file is exist or not.
    if(!targetFile.exists())
    {
        return false;
    }
    //This is the most fucking place, the string you give in rename should be
    //the COMPLETE path, like: "D:/Tojo Nozomi - Start_DASH!.flac"
    return targetFile.rename(currentPath);
}

void KNGlobal::setSystemData(const QString &key, const QVariant &value)
{
    switch(value.type())
    {
    case QVariant::Double:
        m_configure->setSystemData(key, value.toDouble());
        break;
    case QVariant::String:
        m_configure->setSystemData(key, value.toString());
        break;
    case QVariant::Int:
        m_configure->setSystemData(key, value.toInt());
        break;
    case QVariant::Bool:
        m_configure->setSystemData(key, value.toBool());
        break;
    case QVariant::Font:
        m_configure->setSystemData(key, fontToObject(value.value<QFont>()));
        break;
    default:
        break;
    }
}

QVariant KNGlobal::systemData(const QString &key)
{
    return m_configure->systemData(key);
}

void KNGlobal::setCustomData(const QString &module,
                             const QString &key,
                             const QVariant &value)
{
    switch(value.type())
    {
    case QVariant::Double:
        m_configure->setCustomData(module, key, value.toDouble());
        break;
    case QVariant::String:
        m_configure->setCustomData(module, key, value.toString());
        break;
    case QVariant::Int:
        m_configure->setCustomData(module, key, value.toInt());
        break;
    case QVariant::Bool:
        m_configure->setCustomData(module, key, value.toBool());
        break;
    case QVariant::Font:
        m_configure->setCustomData(module, key, fontToObject(value.value<QFont>()));
    default:
        break;
    }
}

QVariant KNGlobal::customData(const QString &module,
                              const QString &key,
                              const QVariant &defaultValue)
{
    QVariant preferData=m_configure->customData(module, key);
    return preferData.isNull()?defaultValue:preferData;
}

void KNGlobal::retranslate()
{
    //Update the storage unit.
    m_storageUnit[0]=tr("Byte");
}

void KNGlobal::loadConfigure()
{
    //Load the configure first.
    m_configure->loadConfigure();
}

void KNGlobal::saveConfigure()
{
    //Set the language information.
    setSystemData("Language", m_localeManager->currentLanguageID());
    //Save the configure data.
    m_configure->saveConfigure();
}

inline void KNGlobal::initialStorageUnit()
{
    m_storageUnit[KiloByte]="KB";
    m_storageUnit[MegaByte]="MB";
    m_storageUnit[GigaByte]="GB";
    m_storageUnit[TeraByte]="TB";
    m_storageUnit[PetaByte]="PB";
    m_storageUnit[ExaByte] ="EB";
    m_storageUnit[ZetaByte]="ZB";
    m_storageUnit[YottaByte]="YB";
    m_storageUnit[NonaByte]="NB";
    m_storageUnit[DoggaByte]="DB";
}

inline void KNGlobal::initialDefaultPath()
{
#ifdef Q_OS_WIN32
    m_userDataPath=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
                   "/Kreogist/Mu";
    m_resourceDirPath=applicationDirPath();
#endif
#ifdef Q_OS_MACX
    m_userDataPath=QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
                   "/Mu";
    m_resourceDirPath=simplifiedPath(applicationDirPath()+"/../Resources");
#endif
#ifdef Q_OS_LINUX
    m_userDataPath=QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+
                   "/.kreogist/mu";
    m_resourceDirPath=m_userDataPath;
#endif
    m_libraryPath=userDataPath()+"/Library";
    m_pluginDirPath=userDataPath()+"/Plugins";
}

inline void KNGlobal::initialInfrastrcture()
{
    //Configure set the custom font folder, load fonts from the folder.
    m_fontManager->loadCustomFontFolder(m_resourceDirPath+"/Fonts");
    //Set the language dir path.
    m_localeManager->setLanguageDirPath(m_resourceDirPath+"/Language");
    //Load all the language.
    m_localeManager->loadLanguageFiles();
}

void KNGlobal::updateInfrastructure()
{
    //Update the data path.
    QString originalLibraryPath=m_libraryPath;
    m_libraryPath=customData("General", "LibraryPath", m_libraryPath).toString();
    //Give out the library path udpate signal.
    emit libraryMoved(originalLibraryPath, m_libraryPath);
}

QJsonObject KNGlobal::fontToObject(const QFont &font)
{
    QJsonObject fontObject;
    fontObject.insert("KNObjectType", "Font");
    fontObject.insert("Family", font.family());
    fontObject.insert("PixelSize", font.pixelSize());
    fontObject.insert("Bold", font.bold());
    fontObject.insert("Italic", font.italic());
    fontObject.insert("Underline", font.underline());
    fontObject.insert("Strikeout", font.strikeOut());
    fontObject.insert("Kerning", font.kerning());
    return fontObject;
}

KNGlobal::KNGlobal(QObject *parent) :
    QObject(parent)
{
    //Set dymanic link library suffix.
#ifdef Q_OS_WIN32
    setDylibSuffix("dll");
#endif
#ifdef Q_OS_MACX
    setDylibSuffix("dylib");
#endif
#ifdef Q_OS_LINUX
    setDylibSuffix("so");
#endif
    //Initial the basic strings.
    initialStorageUnit();
    //Set the default library path.
    initialDefaultPath();

    //Initial the configure manager.
    m_configure=KNConfigureManager::instance();
    //Set the configure file path.
    //-- Why set configure path here?
    //A: Because we won't change the configure path.
    m_configure->setConfigurePath(userDataPath()+"/Configure");
    //Load the configure.
    loadConfigure();

    //Initial the font manager.
    m_fontManager=KNFontManager::instance();
    //Initial the locale.
    m_localeManager=KNLocaleManager::instance();
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNGlobal::requireRetranslate);

    //Initial the infrastructure.
    initialInfrastrcture();
    //Update the infrastructure.
    updateInfrastructure();

    //Load the default settings.
    m_fontManager->initialDefaultFont();
    //Load the default language according to the system locale settings.
    m_localeManager->setLanguageFromID(m_localeManager->systemLocaleLanguageID());
    //Set the language by the id from the configure.
    m_localeManager->setLanguageFromID(systemData("Language").toString());

    //Connect retranslate signal.
    connect(this, &KNGlobal::requireRetranslate,
            this, &KNGlobal::retranslate);
    //Retranslate every thing.
    retranslate();
}
