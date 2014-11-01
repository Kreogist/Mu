/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNLOCALEMANAGER_H
#define KNLOCALEMANAGER_H

#include <QObject>

class KNLocaleManager : public QObject
{
    Q_OBJECT
public:
    static KNLocaleManager *instance();

signals:
    void languageChange();

public slots:

private:
    void loadLanguageFiles();
    static KNLocaleManager *m_instance;
    explicit KNLocaleManager(QObject *parent = 0);
};

#endif // KNLOCALEMANAGER_H
