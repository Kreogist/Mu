/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICSEARCHPLUGIN_H
#define KNMUSICSEARCHPLUGIN_H

#include <QObject>

class KNMusicSearchPlugin : public QObject
{
    Q_OBJECT
public:
    KNMusicSearchPlugin(QObject *parent = 0):QObject(parent){}
    virtual QWidget *searchBox()=0;

signals:
    void requireSearch(const QString &text);

public slots:

};

#endif // KNMUSICSEARCHPLUGIN_H
