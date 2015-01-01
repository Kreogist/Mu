/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICTAB_H
#define KNMUSICTAB_H

#include <QPixmap>

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class QWidget;
class KNMusicModel;
class KNMusicTab : public QObject
{
    Q_OBJECT
public:
    KNMusicTab(QObject *parent = 0):QObject(parent){}
    virtual QString caption()=0;
    virtual QPixmap icon()=0;
    virtual QWidget *widget()=0;
    virtual void showIndexInModel(KNMusicModel *model,
                                  const QModelIndex &index)=0;

signals:
    void requireShowTab();

public slots:
    virtual void onActionSearch(const QString &text)=0;

};

#endif // KNMUSICTAB_H
