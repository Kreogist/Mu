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
class KNConfigureManager;
class KNFontManager;
class KNLocaleManager;
class KNPreferenceItemPanel;
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    static KNGlobal *instance();
    QString byteToHigherUnit(const qint64 &fileSize);
    //Dymantic link suffix.
    QString dylibSuffix();
    void setDylibSuffix(const QString &dylibSuffix);
    KNConfigure *cacheConfigure();
    KNConfigure *systemConfigure();
    KNConfigure *userConfigure();
    static QString applicationDirPath();
    static QString userDataPath();
    static QString pluginDirPath();
    QString libraryPath();
    static QString resourceDirPath();
    static QString simplifiedPath(const QString &path);
    static QString legalFileName(QString fileName);
    static QTextCodec *localeDefaultCodec();
    static QString ensurePathAvaliable(const QString &path);
    static QStringList urlToPathList(const QList<QUrl> urls);
    static int similarity(const QString &str1, const QString &str2);
    void setLibraryPath(const QString &libraryPath);
    static void showInGraphicalShell(const QString &filePath);
    static void openLocalFile(const QString &filePath);
    static void setClipboardText(const QString &text);
    static void moveFolder(const QString &sourceDirPath,
                           const QString &destinationDirPath);
    static bool renameFile(const QString &originalPath,
                           const QString &currentPath);
    KNPreferenceItemPanel *generalPreferencePanel() const;
    void setGeneralPreferencePanel(KNPreferenceItemPanel *generalPreferencePanel);

signals:
    void requireRetranslate();
    void requireApplyPreference();
    void libraryMoved(QString originalPath, QString currentPath);

public slots:
    void retranslate();
    void updateInfrastructure();
    void loadConfigure();
    void saveConfigure();

private:
    inline void initialStorageUnit();
    inline void initialDefaultPath();
    inline void initialInfrastrcture();
#ifdef Q_OS_LINUX
    static QString substituteFileBrowserParameters(QString &pre, QString &file);
#endif
    static KNGlobal *m_instance;
    QString m_dylibSuffix;
    static QString m_userDataPath;
    static QString m_resourceDirPath;
    static QString m_pluginDirPath;
    QString m_libraryPath;
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
    KNConfigure *m_globalConfigure;
    KNPreferenceItemPanel *m_generalPreferencePanel;
    KNConfigureManager *m_configureManager;
    KNFontManager *m_fontManager;
    KNLocaleManager *m_localeManager;
};

#endif // KNGLOBAL_H
