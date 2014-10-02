/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QObject>

class KNConfigure;
class KNFontManager;
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    static KNGlobal *instance();
    static QString dylibSuffix();
    static void setDylibSuffix(const QString &dylibSuffix);
    void setSystemData(const QString &key, const QString &value);
    void setSystemData(const QString &key, const double &value);
    void setSystemData(const QString &key, const int &value);
    void setSystemData(const QString &key, const bool &value);
    QVariant systemData(const QString &key);

signals:

public slots:
    void loadConfigure();
    void saveConfigure();

private:
    static KNGlobal *m_instance;
    static QString m_dylibSuffix;
    explicit KNGlobal(QObject *parent = 0);
    KNConfigure *m_configure;
    KNFontManager *m_fontManager;
};

#endif // KNGLOBAL_H
