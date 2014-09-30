/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMAINWINDOWCATEGORYSTACKPLUGIN_H
#define KNMAINWINDOWCATEGORYSTACKPLUGIN_H

#include <QObject>

class KNMainWindowCategoryStackPlugin : public QObject
{
    Q_OBJECT
public:
    KNMainWindowCategoryStackPlugin(QObject *parent = 0):QObject(parent){}
    virtual QWidget *stackedWidget()=0;

signals:

public slots:
    virtual void addCentralWidget(QWidget *widget)=0;
};

#endif // KNMAINWINDOWCATEGORYSTACKPLUGIN_H
