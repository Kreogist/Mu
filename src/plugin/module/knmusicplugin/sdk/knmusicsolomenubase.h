/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICSOLOMENUBASE_H
#define KNMUSICSOLOMENUBASE_H

#include <QModelIndex>

#include "knanimationmenu.h"

class KNMusicDetailDialogBase;
class KNMusicProxyModel;
class KNMusicSoloMenuBase : public KNAnimationMenu
{
    Q_OBJECT
public:
    KNMusicSoloMenuBase(QWidget *parent = 0):KNAnimationMenu(parent){}
    virtual QString currentFilePath() const=0;
    virtual void setProxyModel(KNMusicProxyModel *model)=0;
    virtual void setCurrentIndex(const QModelIndex &itemIndex)=0;
    virtual void setDetailDialog(KNMusicDetailDialogBase *dialog)=0;

signals:
    void requirePlayCurrent();
    void requireRemoveCurrent();

public slots:
    virtual void addMusicActions(QList<QAction *> actions)=0;

};

#endif // KNMUSICSOLOMENUBASE_H
