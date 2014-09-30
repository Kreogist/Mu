/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMAINWINDOWCATEGORYSWITCHERPLUGIN_H
#define KNMAINWINDOWCATEGORYSWITCHERPLUGIN_H

#include <QObject>

class KNMainWindowCategorySwitcherPlugin: public QObject
{
    Q_OBJECT
public:
    KNMainWindowCategorySwitcherPlugin(QObject *parent = 0):QObject(parent){}
    virtual QWidget *switcherWidget()=0;

signals:
    void requireResetHeaderButton();
    void requireShowPreference();


public slots:
    virtual void showSwitcher()=0;
};

#endif // KNMAINWINDOWCATEGORYSWITCHERBASE_H
