/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNCATEGORYPLUGIN_H
#define KNCATEGORYPLUGIN_H

#include <QThread>
#include <QLinkedList>

#include <QObject>

class KNPreferenceWidgetsPanel;
class KNCategoryPlugin : public QObject
{
    Q_OBJECT
public:
    KNCategoryPlugin(QObject *parent = 0):QObject(parent){}
    virtual QString caption()=0;
    virtual QPixmap icon()=0;
    virtual QPixmap headerIcon()=0;
    virtual QPixmap preferenceIcon()=0;
    virtual QWidget *centralWidget()=0;
    virtual QWidget *headerWidget()=0;
    virtual KNPreferenceWidgetsPanel *preferencePanelWidget()=0;

signals:

public slots:

};

#endif // KNCATEGORYPLUGIN_H
