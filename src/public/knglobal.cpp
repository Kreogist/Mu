/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>

#include "knconfigure.h"

#include "knglobal.h"

#include <QDebug>

QString KNGlobal::m_dylibSuffix="";

KNGlobal *KNGlobal::m_instance=nullptr;

KNGlobal *KNGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNGlobal:m_instance;
}
QString KNGlobal::dylibSuffix()
{
    return m_dylibSuffix;
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

void KNGlobal::loadConfigure()
{
    m_configure->loadConfigure();
}

void KNGlobal::saveConfigure()
{
    m_configure->saveConfigure();
}

KNGlobal::KNGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial the configure.
    m_configure=KNConfigure::instance();
    //Set the configure file path.
    m_configure->setConfigurePath(QApplication::applicationDirPath() +
                                      "/Configure");
}
