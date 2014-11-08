/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICDETAILTOOLTIPBASE_H
#define KNMUSICDETAILTOOLTIPBASE_H

#include <QPersistentModelIndex>

#include <QWidget>

class KNMusicBackend;
class KNMusicModel;
class KNMusicDetailTooltipBase : public QWidget
{
    Q_OBJECT
public:
    KNMusicDetailTooltipBase(QWidget *parent = 0):QWidget(parent){}
    virtual void setBackend(KNMusicBackend *backend)=0;

signals:

public slots:
    virtual void showTooltip()=0;
    virtual void setPreviewIndex(KNMusicModel *musicModel,
                                 const QPersistentModelIndex &index,
                                 const QPoint &position)=0;
};

#endif // KNMUSICDETAILTOOLTIPBASE_H
