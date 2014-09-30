/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMAINWINDOWPLUGIN_H
#define KNMAINWINDOWPLUGIN_H

#include "knmainwindowheaderplugin.h"
#include "knmainwindowcategorystackplugin.h"
#include "knmainwindowcategoryswitcherplugin.h"
#include "knpreferenceplugin.h"

#include <QObject>

class QMainWindow;
class KNMainWindowPlugin : public QObject
{
    Q_OBJECT
public:
    KNMainWindowPlugin(QObject *parent = 0):QObject(parent){}

signals:

public slots:
    virtual void setMainWindow(QMainWindow *mainWindow)=0;
    virtual void setHeader(KNMainWindowHeaderPlugin *plugin)=0;
    virtual void setCategoryStack(KNMainWindowCategoryStackPlugin *widget)=0;
    virtual void setCategorySwitcher(KNMainWindowCategorySwitcherPlugin *widget)=0;
    virtual void setPreferencePanel(KNPreferencePlugin *plugin)=0;
    virtual void showPreference()=0;
    virtual void hidePreference()=0;
    virtual void setHeaderIcon(const QPixmap &icon)=0;
    virtual void setHeaderText(const QString &text)=0;
    virtual void addHeaderWidget(QWidget *widget)=0;
    virtual void addCentralWidget(QWidget *widget)=0;
};

#endif // KNMAINWINDOWPLUGIN_H
