/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNABSTRACTMUSICPLUGIN_H
#define KNABSTRACTMUSICPLUGIN_H

#include "kncategoryplugin.h"

class KNAbstractMusicPlugin : public KNCategoryPlugin
{
    Q_OBJECT
public:
    KNAbstractMusicPlugin(QObject *parent = 0):KNCategoryPlugin(parent){}

signals:

public slots:

protected slots:
    virtual int addMusicCategory(const QPixmap &icon,
                                 const QString &caption,
                                 QWidget *widget)=0;

};

#endif // KNABSTRACTMUSICPLUGIN_H
