/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNCONFIGUREMANAGER_H
#define KNCONFIGUREMANAGER_H

#include <QObject>

namespace KNConfigureManagerGlobal
{
enum ConfigureTypes
{
    Cache,
    System,
    User,
    ConfigureTypeCount
};
}

using namespace KNConfigureManagerGlobal;

class KNConfigure;
class KNConfigureManager : public QObject
{
    Q_OBJECT
public:
    static KNConfigureManager *instance();
    void loadConfigure();
    void saveConfigure();
    KNConfigure *getConfigure(const int &index);

    QString configurePath() const;
    void setConfigurePath(const QString &configurePath);

signals:

public slots:

private:
    static KNConfigureManager *m_instance;
    explicit KNConfigureManager(QObject *parent = 0);

    inline void generateConfigureFolder();

    QString m_configurePath;

    KNConfigure *m_configures[ConfigureTypeCount];
};

#endif // KNCONFIGUREMANAGER_H
