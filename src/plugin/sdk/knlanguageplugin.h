/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNLANGUAGEPLUGIN_H
#define KNLANGUAGEPLUGIN_H

#include <QObject>

class KNLanguagePlugin : public QObject
{
    Q_OBJECT
public:
    KNLanguagePlugin(QObject *parent = 0):QObject(parent){}

signals:

public slots:

};

#endif // KNLANGUAGEPLUGIN_H
