/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICANALYSISER_H
#define KNMUSICANALYSISER_H

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class KNMusicAnalysiser : public QObject
{
    Q_OBJECT
public:
    KNMusicAnalysiser(QObject *parent = 0):QObject(parent){}
    virtual bool analysis(const QString &filePath,
                          KNMusicDetailInfo &detailInfo)=0;

signals:

public slots:

private:
};

#endif // KNMUSICANALYSISER_H
