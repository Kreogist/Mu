/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QStringList>
#include <QUrl>
#include <QJsonObject>
#include <QFont>
#include <QObject>

class KNConfigure;
class KNFontManager;
class KNLocaleManager;
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    static KNGlobal *instance();
    QString byteToHigherUnit(const qint64 &fileSize);
    static QString dylibSuffix();
    static QString applicationDirPath();
    static QString userDataPath();
    static QString pluginDirPath();
    static QString libraryPath();
    static QTextCodec *localeDefaultCodec();
    static QString ensurePathAvaliable(const QString &path);
    static QStringList urlToPathList(const QList<QUrl> urls);
    static void setDylibSuffix(const QString &dylibSuffix);
    static void setLibraryPath(const QString &libraryPath);
    static void showInGraphicalShell(const QString &filePath);
    static void openLocalFile(const QString &filePath);
    static void setClipboardText(const QString &text);
    static void moveFolder(const QString &sourceDirPath,
                           const QString &destinationDirPath);
    static bool renameFile(const QString &originalPath,
                           const QString &currentPath);
    void setSystemData(const QString &key, const QVariant &value);
    QVariant systemData(const QString &key);
    void setCustomData(const QString &module,
                       const QString &key,
                       const QVariant &value);
    QVariant customData(const QString &module,
                        const QString &key,
                        const QVariant &defaultValue);

signals:
    void languageChanged();
    void libraryMoved(QString originalPath, QString currentPath);

public slots:
    void retranslate();
    void updateInfrastructure();
    void loadConfigure();
    void saveConfigure();

private:
    inline void initialStorageUnit();
    inline void initialDefaultPath();
    inline QJsonObject fontToObject(const QFont &font);
#ifdef Q_OS_LINUX
    static QString substituteFileBrowserParameters(QString &pre, QString &file);
#endif
    static KNGlobal *m_instance;
    static QString m_dylibSuffix;
    static QString m_userDataPath;
    static QString m_pluginDirPath;
    static QString m_libraryPath;
    enum StorageUnit
    {
        Byte,
        KiloByte,
        MegaByte,
        GigaByte,
        TeraByte,
        PetaByte,
        ExaByte,
        ZetaByte,
        YottaByte,
        NonaByte,
        DoggaByte,
        StorageUnitCount
    };
    QString m_storageUnit[StorageUnitCount];
    explicit KNGlobal(QObject *parent = 0);
    KNConfigure *m_configure;
    KNFontManager *m_fontManager;
    KNLocaleManager *m_localeManager;
};

#endif // KNGLOBAL_H
