/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>

#include "knconfigure.h"
#include "knfontmanager.h"

#include "knglobal.h"

#include <QDebug>

QString KNGlobal::m_dylibSuffix="";

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

void KNGlobal::setDylibSuffix(const QString &dylibSuffix)
{
    m_dylibSuffix = dylibSuffix;
}

void KNGlobal::setSystemData(const QString &key, const QString &value)
{
    m_configure->setSystemData(key, value);
}

void KNGlobal::setSystemData(const QString &key, const double &value)
{
    m_configure->setSystemData(key, value);
}

void KNGlobal::setSystemData(const QString &key, const int &value)
{
    m_configure->setSystemData(key, value);
}

void KNGlobal::setSystemData(const QString &key, const bool &value)
{
    m_configure->setSystemData(key, value);
}

QVariant KNGlobal::systemData(const QString &key)
{
    return m_configure->systemData(key);
}

void KNGlobal::retranslate()
{
    //Update the storage unit.
    m_storageUnit[0]=tr("Byte");
}

void KNGlobal::loadConfigure()
{
    m_configure->loadConfigure();
}

void KNGlobal::saveConfigure()
{
    m_configure->saveConfigure();
}

void KNGlobal::initialStorageUnit()
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

KNGlobal::KNGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial the basic strings.
    initialStorageUnit();

    //Initial the fonts.
    m_fontManager=KNFontManager::instance();
    m_fontManager->loadCustomFontFolder(QApplication::applicationDirPath() +
                                        "/Fonts");
    m_fontManager->initialDefaultFont();

    //Initial the configure.
    m_configure=KNConfigure::instance();
    //Set the configure file path.
    m_configure->setConfigurePath(QApplication::applicationDirPath() +
                                      "/Configure");

    //Retranslate every thing.
    retranslate();
}
