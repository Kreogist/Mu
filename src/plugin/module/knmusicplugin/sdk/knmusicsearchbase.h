/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICSEARCHBASE_H
#define KNMUSICSEARCHBASE_H

#include <QObject>

class KNMusicSearchBase : public QObject
{
    Q_OBJECT
public:
    KNMusicSearchBase(QObject *parent = 0):QObject(parent){}
    virtual QWidget *searchBox()=0;
    virtual QString searchText()=0;

signals:
    void requireSearch(const QString &text);

public slots:
    virtual void setSearchFocus(QWidget *focusSource=nullptr)=0;
    virtual void search(const QString &text)=0;

};

#endif // KNMUSICSEARCHBASE_H
