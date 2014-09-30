/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMAINWINDOWHEADERPLUGIN_H
#define KNMAINWINDOWHEADERPLUGIN_H

#include <QObject>

class KNMainWindowHeaderPlugin : public QObject
{
    Q_OBJECT
public:
    KNMainWindowHeaderPlugin(QObject *parent = 0):QObject(parent){}
    virtual QWidget *headerWidget()=0;

signals:
    void requireShowCategorySwitcher();

public slots:
    virtual void addHeaderWidget(QWidget *widget)=0;
    virtual void setHeaderText(const QString &text)=0;
    virtual void setHeaderIcon(const QPixmap &icon)=0;
    virtual void restoreHeaderButton()=0;
};

#endif // KNMAINWINDOWHEADERPLUGIN_H
